cmake_minimum_required(VERSION 3.10)
project(binary_storage)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

add_subdirectory(third_party/refl-cpp)
add_subdirectory(third_party/googletest)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

set(HEADERS
    ${INCLUDE_DIR}/binary_storage.hpp
    
    ${INCLUDE_DIR}/serde/traits.hpp
    ${INCLUDE_DIR}/serde/macros.hpp
    ${INCLUDE_DIR}/serde/serde.hpp
    ${INCLUDE_DIR}/serde/for_each_macros.hpp)

set(SOURCES
    src/binary_storage.cpp)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${INCLUDE_DIR}
        refl-cpp::refl-cpp
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        refl-cpp::refl-cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (TRUE) 
    add_subdirectory(test)
endif()
