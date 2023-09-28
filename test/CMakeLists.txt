cmake_minimum_required(VERSION 3.14)

project(binary_storage_test CXX)

enable_testing()

set(HEADERS
    )

set(SOURCES
    Test.main.cpp
    Test.serializeTypeSizes.cpp
    Test.deserialize.cpp
    Test.Value.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    binary_storage
    gtest
    refl-cpp)

target_include_directories(${PROJECT_NAME} 
    PUBLIC
    binary_storage
    gtest
    refl-cpp)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
