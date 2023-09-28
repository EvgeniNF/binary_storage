#pragma once

#include <variant>
#include <fstream>
#include <string>
#include <shared_mutex>
#include <unordered_map>
#include <filesystem>
#include <map>

#include "serde/traits.hpp"
#include "ValueStorage.hpp"
#include "Parameters.hpp"

namespace binary_storage::storage {

template<class T>
class Storage {
   public:
    using ValueType = T;
    using Container = std::unordered_map<std::string, Value<ValueType>>; 

   public:
    Storage(BaseParameters params) :
        m_paramters {std::move(params)} {
        loadFiles();
    }
    
    Storage(Storage const&) = delete;
    Storage(Storage &&) noexcept = delete;
    Storage& operator=(Storage const&) = delete;
    Storage& operator=(Storage &&) noexcept = delete; 
    
    ~Storage() noexcept {
        if (m_paramters.saveAllOnDestruct) {
            for (auto& node: m_container) {
                storeValue(node.second, m_paramters.path + node.first + m_paramters.extension);
            }
        }
    }

   public:
    void store(std::string const& key, ValueType&& value) {
        std::unique_lock lock(m_mutex);
        m_container[key] = createFromData(std::forward<ValueType>(value));
    }

    ValueType& load(std::string const& key) {
        std::shared_lock lock(m_mutex);
        auto const iter = m_container.find(key);
        if (iter == m_container.end()) {
            throw std::logic_error("Key not found: " + key);
        }

        return getData(iter->second);
    }

    void fitSize() {
        std::unique_lock lock(m_mutex);
        std::map<std::chrono::system_clock::time_point, Container::const_iterator> accessMap;
        
        for (auto it = m_container.cbegin(); it != m_container.cend(); ++it) {
            if (isCashed(it->second)) {
                accessMap[it->second.lastAccess] = it;
            }
        }

        auto const targetSize = accessMap.size() > m_paramters.cashSize 
            ? accessMap.size() 
            : static_cast<size_t>(accessMap.size() / m_paramters.resizeCoeff);

        while (targetSize != 0 and accessMap.size() > targetSize) {
            auto const node = accessMap.extract(std::prev(accessMap.cend()));
            auto path = m_paramters.path + node.mapped()->first + m_paramters.extension;
            storeValue(node.mapped()->second, std::move(path));
        }
    } 

    void clear() {
        std::unique_lock lock(m_mutex);
        while (not m_container.empty()) {
            eraseImpl(m_container.begin()->first);
        }
    }

    size_t size() const noexcept {
        std::shared_lock lock(m_mutex);
        return m_container.size();
    }

    void erase(std::string const& key)  {
        std::unique_lock lock(m_mutex);
        eraseImpl(key);
    }

   private:
    mutable std::shared_mutex m_mutex;
    BaseParameters m_paramters;
    Container m_container; 

   private: 
    void eraseImpl(std::string const& key) {
        auto const node = m_container.extract(key);
        std::filesystem::directory_iterator file(m_paramters.path + node.key());
        if (file->exists()) {
            std::filesystem::remove(file->path());
        }
    } 

    void loadFiles() {
        namespace fs = std::filesystem;
        if (m_paramters.path.empty()) {
            throw std::logic_error("Path is empty");
        }

        if (m_paramters.path.back() != fs::path::preferred_separator) {
            m_paramters.path.push_back(fs::path::preferred_separator);
        }

        if (not fs::exists(m_paramters.path)) {
            if (not fs::create_directories(m_paramters.path)) {
                throw std::logic_error("Can't create directories: " + m_paramters.path);
            }
        }

        if (not m_paramters.extension.empty()) {
            if (m_paramters.extension.front() != '.') {
                m_paramters.extension = std::move('.' + m_paramters.extension);
            }
        }

        if (not m_paramters.loadAllOnCreate) {
            return;
        }

        for (auto const& entry: fs::directory_iterator(m_paramters.path)) {
            if (not entry.is_regular_file()) {
                continue;
            }

            if (entry.path().extension() != m_paramters.extension) {
                continue;
            }

            m_container.insert({entry.path().filename(), createFormFile<ValueType>(std::move(entry.path()))});
        }
    }
};

} // namespace binary_storage::storage
