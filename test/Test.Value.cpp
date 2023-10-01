#include <gtest/gtest.h>

#include <storage/ValueStorage.hpp>
#include <filesystem>
#include <shared_mutex>
#include <thread>
#include <array>

static constexpr std::string_view path {"/tmp/test.bin"};

using namespace binary_storage::storage;

struct TestValue {
    uint64_t a {10};
    std::string b {"Hello test"};
    double c {0.15};
};

REFL_AUTO(
    type(TestValue),
    field(a),
    field(b),
    field(c)
)

TEST(Value, create) {
    uint16_t const data1 = 55;
    TestValue data2;

    ASSERT_NO_THROW(
        auto const value1 = createFromData(data1);
        auto const value2 = createFromData(std::move(data2));
    );
}

TEST(Value, get) {
    uint16_t data1 = 55;
    TestValue data2;

    auto value1 = createFromData(data1);
    auto value2 = createFromData(data2);

    auto result1 = getData(value1);
    auto result2 = getData(value2);

    ASSERT_EQ(data1, result1);
    
    ASSERT_EQ(data2.a, result2.a);
    ASSERT_EQ(data2.b, result2.b);
    ASSERT_DOUBLE_EQ(data2.c, result2.c);
}

TEST(Value, getStoredData) {
    TestValue data;
    std::ofstream stream(path.data());

    binary_storage::serde::serialize(stream, data);
    stream.close();

    
    auto value = createFormFile<TestValue>(std::string(path.data()));
    auto result = getData(value);

    ASSERT_EQ(result.a, data.a);
    ASSERT_EQ(result.b, data.b);
    ASSERT_DOUBLE_EQ(result.c, data.c);
}

TEST(Value, update) {
    TestValue data;
    
    auto value = createFromData(data);
    data.a = 315;
    data.b = "Test2";
    data.c = 643.112351;
    updateData(data, value);

    ASSERT_EQ(getData(value).a, data.a);
    ASSERT_EQ(getData(value).b, data.b);
    ASSERT_DOUBLE_EQ(getData(value).c, data.c);
}

TEST(Value, store) {
    TestValue data;

    auto value = createFromData(data);
    storeValue(value, std::string(path));

    ASSERT_EQ(getData(value).a, data.a);
    ASSERT_EQ(getData(value).b, data.b);
    ASSERT_DOUBLE_EQ(getData(value).c, data.c);
}
