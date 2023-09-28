#pragma once

#include <string>
#include <cstdint>

namespace binary_storage::storage {
    
struct BaseParameters {
    std::string path {""};
    std::string extension {".bin"};
    size_t cashSize {20};
    double resizeCoeff {1.5};
    bool saveAllOnDestruct {false};
    bool loadAllOnCreate {false};
};

} // namespace binary_storage::storage
