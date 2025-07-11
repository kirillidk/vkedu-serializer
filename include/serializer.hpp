#pragma once

#include <sstream>
#include <string>
#include <cstdint>

namespace vkedu {

enum class Error { NoError, CorruptedArchive };

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }
private:
    std::ostream& out_;

    template <class T>
    Error process(T&& arg) {
        if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
            return process_bool(std::forward<T>(arg));
        } else if constexpr (std::is_same_v<std::decay_t<T>, uint64_t>) {
            return process_uint64(std::forward<T>(arg));
        } else {
            static_assert(
                std::is_same_v<std::decay_t<T>, bool> ||
                    std::is_same_v<std::decay_t<T>, uint64_t>,
                "Unsupported type for serialization"
            );
        }
    }

    template <class T, class... ArgsT>
    Error process(T&& arg, ArgsT&&... args) {
        Error err = process(std::forward<T>(arg));
        if (err != Error::NoError) {
            return err;
        }
        if constexpr (sizeof...(args) > 0) {
            out_ << Separator;
            return process(std::forward<ArgsT>(args)...);
        }
        return Error::NoError;
    }

    Error process_bool(bool arg) {
        out_ << (arg ? "true" : "false");
        return Error::NoError;
    }

    Error process_uint64(uint64_t arg) {
        out_ << arg;
        return Error::NoError;
    }
};

class Deserializer {
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }
private:
    std::istream& in_;

    template <class T>
    Error process(T& arg) {
        if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
            return load_bool(arg);
        } else if constexpr (std::is_same_v<std::decay_t<T>, uint64_t>) {
            return load_uint64(arg);
        } else {
            static_assert(
                std::is_same_v<std::decay_t<T>, bool> ||
                    std::is_same_v<std::decay_t<T>, uint64_t>,
                "Unsupported type for deserialization"
            );
        }
    }

    template <class T, class... ArgsT>
    Error process(T& arg, ArgsT&... args) {
        Error err = process(arg);
        if (err != Error::NoError) {
            return err;
        }
        if constexpr (sizeof...(args) > 0) {
            return process(args...);
        }
        return Error::NoError;
    }

    Error load_bool(bool& value) {
        std::string text;
        in_ >> text;
        if (text == "true") {
            value = true;
        } else if (text == "false") {
            value = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error load_uint64(uint64_t& value) {
        in_ >> value;
        if (in_.fail()) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};

}  // namespace vkedu