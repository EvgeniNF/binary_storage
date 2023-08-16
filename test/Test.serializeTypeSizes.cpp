#include <gtest/gtest.h>
#include <refl.hpp>

#include <sstream>
#include <serde/for_each_macros.hpp>

#include <serde/serde.hpp>


TEST(Serialize, IntTypesSizes) {
    using namespace binary_storage::serde;

    {
        std::stringstream stream;
        int8_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 1);
        stream.clear();
    }

    {
        std::stringstream stream;
        int16_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 2);
        stream.clear();
    }

    {
        std::stringstream stream;
        int32_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 4);
        stream.clear();
    }

    {
        std::stringstream stream;
        int64_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 8);
        stream.clear();
    }

    {
        std::stringstream stream;
        uint8_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 1);
        stream.clear();
    }

    {
        std::stringstream stream;
        uint16_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 2);
        stream.clear();
    }

    {
        std::stringstream stream;
        uint32_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 4);
        stream.clear();
    }

    {
        std::stringstream stream;
        uint64_t value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), 8);
        stream.clear();
    }
}

TEST(Serialize, FloatingPointTypeSizes) {
    using namespace binary_storage::serde;

    {
        std::stringstream stream;
        float value = 0.0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), sizeof(float));
        stream.clear();
    }

    {
        std::stringstream stream;
        double value = 0;
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), sizeof(double));
        stream.clear();
    }
}

TEST(Serialize, vectorTypeSizes) {
    using namespace binary_storage::serde;

    {
        std::stringstream stream;
        std::vector<uint8_t> value = {0, 0, 0, 0};
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), sizeof(std::vector<uint8_t>::size_type) + (sizeof(uint8_t) * 4));
        stream.clear();
    }

    {
        std::stringstream stream;
        std::vector<double> value = {0, 0, 0, 0};
        serialize(stream, value);
        ASSERT_EQ(stream.str().size(), sizeof(std::vector<double>::size_type) + (sizeof(double) * 4));
        stream.clear();
    }
}

struct TestStruct {
    int a {10};
    char b {'c'};
    double c {0.16};
    float d {22.31f};
};

REFL_AUTO(
    type(TestStruct),
    field(a),
    field(b),
    field(c),
    field(d)
)

TEST(Serialize, customTypeSize) {
    using namespace binary_storage::serde;
    {
        std::stringstream stream;
        TestStruct custom;
        serialize(stream, custom);
        auto const size = sizeof(int) + sizeof(char) + sizeof(double) + sizeof(float); 
        ASSERT_EQ(stream.str().size(), size);
    }
}

TEST(Serialize, stringTypeSize) {
    using namespace binary_storage::serde;
    {
        std::stringstream stream;
        std::string value {"hello world"};
        serialize(stream, value);
        auto const size = sizeof(std::string::size_type) + (sizeof(std::string::value_type) * value.size()); 
        ASSERT_EQ(stream.str().size(), size);
    }
}

