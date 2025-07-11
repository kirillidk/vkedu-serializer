#include <gtest/gtest.h>
#include <sstream>
#include <serializer.hpp>

struct TestData {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    vkedu::Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    bool operator==(const TestData& other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

TEST(SerializerTest, BasicSerialization) {
    TestData original {1, true, 2};
    std::stringstream stream;

    vkedu::Serializer serializer(stream);
    EXPECT_EQ(serializer.save(original), vkedu::Error::NoError);
    EXPECT_EQ(stream.str(), "1 true 2");
}

TEST(SerializerTest, BasicDeserialization) {
    std::stringstream stream("1 true 2");
    TestData data {0, false, 0};

    vkedu::Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(data), vkedu::Error::NoError);
    EXPECT_EQ(data, (TestData {1, true, 2}));
}

TEST(SerializerTest, SerializationDeserialization) {
    TestData original {42, false, 999};
    std::stringstream stream;

    vkedu::Serializer serializer(stream);
    EXPECT_EQ(serializer.save(original), vkedu::Error::NoError);

    TestData restored {0, true, 0};
    vkedu::Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(restored), vkedu::Error::NoError);

    EXPECT_EQ(original, restored);
}

TEST(SerializerTest, CorruptedArchive) {
    std::stringstream stream("1 invalid_bool 2");
    TestData data {0, false, 0};

    vkedu::Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(data), vkedu::Error::CorruptedArchive);
}