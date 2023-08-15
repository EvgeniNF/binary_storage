cmake_minimum_required(VERSION 3.10)
project(binary_storage)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

set(HEADERS
    ${INCLUDE_DIR}/binary_storage.hpp
    
    ${INCLUDE_DIR}/serde/traits.hpp
    ${INCLUDE_DIR}/serde/macros.hpp
    ${INCLUDE_DIR}/serde/serde.hpp)

set(SOURCES
    src/binary_storage.cpp)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_link_directories(${PROJECT_NAME}
        INTERFACE
        ${INCLUDE_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
