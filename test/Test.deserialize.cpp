#include <gtest/gtest.h>

#include <serde/serde.hpp>
#include <fstream>

TEST(Deserialize, charTypes) {
    using namespace binary_storage::serde;
    
    {
        uint8_t const value = 100;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<uint8_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        int8_t const value = -50;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<int8_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        char const value = 'S';
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<char>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }
}

TEST(Deserialize, numericTypes) {
    using namespace binary_storage::serde;
    
    {
        uint16_t const value = 1500;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<uint16_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        int16_t const value = -1000;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<int16_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        uint32_t const value = 50105;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<uint32_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        int32_t const value = -30301;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<int32_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        uint64_t const value = 90105;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<uint64_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        int64_t const value = -50219;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<int64_t>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }
}

TEST(Deserialize, vectorTypes) {
    using namespace binary_storage::serde;
    
    {
        using Type = int8_t;
        std::vector<Type> values;
        std::stringstream stream;
        serialize(stream, values);
        auto const result = deserialize<std::vector<Type>>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(result.value(), values);
    }

    {
        using Type = int8_t;
        std::vector<Type> values;
        auto const minValue = std::numeric_limits<Type>::min();
        auto const maxValue = std::numeric_limits<Type>::max();

        values.resize(static_cast<size_t>(maxValue) - static_cast<size_t>(minValue) + 1);
        std::fill(values.begin(), values.end(), minValue);

        std::stringstream stream;
        serialize(stream, values);
        auto const result = deserialize<std::vector<Type>>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(result.value(), values);
    }

    {
        using Type = uint8_t;
        std::vector<Type> values;
        auto const minValue = std::numeric_limits<Type>::min();
        auto const maxValue = std::numeric_limits<Type>::max();

        values.resize(static_cast<size_t>(maxValue) - static_cast<size_t>(minValue) + 1);
        std::fill(values.begin(), values.end(), minValue);

        std::stringstream stream;
        serialize(stream, values);
        auto const result = deserialize<std::vector<Type>>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(result.value(), values);
    }

    {
        using Type = int16_t;
        std::vector<Type> values;
        auto const minValue = std::numeric_limits<Type>::min();
        auto const maxValue = std::numeric_limits<Type>::max();

        values.resize(static_cast<size_t>(maxValue) - static_cast<size_t>(minValue) + 1);
        std::fill(values.begin(), values.end(), minValue);

        std::stringstream stream;
        serialize(stream, values);
        auto const result = deserialize<std::vector<Type>>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(result.value(), values);
    }

    {
        using Type = uint16_t;
        std::vector<Type> values;
        auto const minValue = std::numeric_limits<Type>::min();
        auto const maxValue = std::numeric_limits<Type>::max();

        values.resize(static_cast<size_t>(maxValue) - static_cast<size_t>(minValue) + 1);
        std::fill(values.begin(), values.end(), minValue);

        std::stringstream stream;
        serialize(stream, values);
        auto const result = deserialize<std::vector<Type>>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(result.value(), values);
    }
}

TEST(Deserialize, stringType) {
    using namespace binary_storage::serde;
    
    {
        std::string const value = "test string";
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<std::string>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }

    {
        std::string const value = "";
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<std::string>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(*result, value);
    }
}

TEST(Deserialize, floatPointType) {
    using namespace binary_storage::serde;
    
    {
        double const value = 3.524;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<double>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_DOUBLE_EQ(*result, value);
    }

    {
        double const value = 15.533214124552124;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<double>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_DOUBLE_EQ(*result, value);
    }

    {
        float const value = 3.524;
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<float>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_DOUBLE_EQ(*result, value);
    }
}

struct Point {
    double x;
    double y;
};

REFL_AUTO(
    type(Point),
    field(x),
    field(y)
)

struct TestDeserialization {
    int a;
    uint8_t b;
    std::string c; 
    double d;
    float e;
    std::vector<uint16_t> f;
    Point point;   
};

REFL_AUTO(
    type(TestDeserialization),
    field(a),
    field(b),
    field(c),
    field(d),
    field(e), 
    field(f),
    field(point) 
)

TEST(Deserialize, customType) {
    using namespace binary_storage::serde;

    {
        TestDeserialization value {10, 113, "hello world", 0.3124, 22.315f, {1, 2, 3, 4, 5, 100, 2319}, {14.0123, -22.4159}};
        std::stringstream stream;
        serialize(stream, value);
        auto const result = deserialize<TestDeserialization>(stream);
        ASSERT_EQ(result.has_value(), true);
        ASSERT_EQ(result.value().a, value.a);
        ASSERT_EQ(result.value().b, value.b);
        ASSERT_EQ(result.value().c, value.c);
        ASSERT_DOUBLE_EQ(result.value().d, value.d);
        ASSERT_DOUBLE_EQ(result.value().e, value.e);
        ASSERT_EQ(result.value().f, value.f);
        ASSERT_DOUBLE_EQ(result.value().point.x, value.point.x);
        ASSERT_DOUBLE_EQ(result.value().point.y, value.point.y);
    }
}
