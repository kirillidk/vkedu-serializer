#include <cassert>

#include <serializer.hpp>

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    vkedu::Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }
};

int main() {
    Data x {1, true, 2};

    std::stringstream stream;

    vkedu::Serializer serializer(stream);
    serializer.save(x);

    Data y {0, false, 0};

    vkedu::Deserializer deserializer(stream);
    const vkedu::Error err = deserializer.load(y);

    assert(err == vkedu::Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
}