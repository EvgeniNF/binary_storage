#pragma once

#include <future>

#include "Storage.hpp"

namespace binary_storage::storage {

template<class T>
class AutoStorage {
   public:
    using ValueType = T;
    using StorageType = Storage<ValueType>;

   public:
    void store(std::string const& key, T&& value) {
        m_storage.store(key, std::forward<T>(value));
    } 

    T& load(std::string const& key) {
        return m_storage.load(key);
    }

    size_t size() const noexcept {
        return m_storage.size();
    }

    void clear() {
        m_storage.clear();
    }

    void erase(std::string const& key) {
        m_storage.erase(key);
    }

   private:
    StorageType m_storage;
    std::shared_future<void> m_fitSizeFuture;
    std::atomic_bool m_stoped {false};

   private:
    void fitSizeTask() noexcept {
        
    }
};

} // namespace binary_storage::storage
