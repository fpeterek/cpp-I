#include <format>
#include <print>


class EditorKey {

    //////////////////////////////////////
    //                                  // 
    //          static members          //
    //                                  // 
    //////////////////////////////////////

public:

    enum class Key : int {
        A = 0,
        B = 1,
        C = 10,
        D = 11,

        Unknown = -1,
    };

    static Key keyOrUnknown(Key key) noexcept {
        // No default case so compiler throws a warning if we add a new enumerator
        // and forget to update the switch
        //
        // We can just use the LSP to print out all the valid cases for us,
        // so we check for any of the valid cases using a switch. In case of
        // an invalid key, we return Unknown.
        //
        // -Werror=switch is very useful for such cases, so the warning is not
        // missed by accident
        switch (key) {
            case Key::A:
            case Key::B:
            case Key::C:
            case Key::D:
            case Key::Unknown:
                return key;
        }
        return Key::Unknown;
    }

    static Key keyOrUnknown(int key) noexcept {
        return keyOrUnknown(Key { key });
    }

    //////////////////////////////////////
    //                                  // 
    //        non-static members        //
    //                                  // 
    //////////////////////////////////////

private:
    Key _key { Key::Unknown };

public:

    EditorKey() noexcept = default;

    EditorKey(const Key key) noexcept :
        _key { keyOrUnknown(key) } { }

    EditorKey(const int key) noexcept :
        _key { keyOrUnknown(key) } { }

    EditorKey(const EditorKey& other) noexcept :
        _key { keyOrUnknown(other._key) } { }


    bool operator==(EditorKey other) {
        return _key == other._key;
    }

    bool operator!=(this auto& self, EditorKey other) {
        return not (self == other);
    }

    Key key() const noexcept {
        return _key;
    }

    int asInt() const noexcept {
        return static_cast<int>(_key);
    }

    bool known() const noexcept {
        return _key != Key::Unknown;
    }

    bool unknown() const noexcept {
        return _key == Key::Unknown;
    }

    // Force set to any value, even an invalid one
    // For testing purposes only
    void forceSet(Key k) {
        _key = k;
    }

};

namespace std {

    // We can reuse existing formatters to format our own types
    // Not particularly smart, but it's easy to write and easy to use
    // We can improve it later if needed
    template<>
    class formatter<EditorKey::Key, char> {

        std::formatter<std::string_view, char> svFormatter{};

        template<typename FmtCtx>
        FmtCtx::iterator fmt(std::string_view sv, FmtCtx& ctx) const {
            return svFormatter.format(sv, ctx);
        }

    public:

        template<typename ParseCtx>
        constexpr ParseCtx::iterator parse(ParseCtx& ctx) {
            return svFormatter.parse(ctx);
        }

        template<typename FmtCtx>
        FmtCtx::iterator format(EditorKey::Key key, FmtCtx& ctx) const {

            // No default case so compiler throws a warning if we add a new enumerator
            // and forget to update the switch
            switch (key) {
                case EditorKey::Key::A:
                    return fmt("<A>", ctx);
                case EditorKey::Key::B:
                    return fmt("<B>", ctx);
                case EditorKey::Key::C:
                    return fmt("<C>", ctx);
                case EditorKey::Key::D:
                    return fmt("<D>", ctx);
                case EditorKey::Key::Unknown:
                    return fmt("<Unknown>", ctx);
            }

            return fmt("<Invalid>", ctx);
        }

    };

    template<>
    struct formatter<EditorKey, char> {

        std::formatter<EditorKey::Key, char> keyFormatter{};

        template<typename ParseCtx>
        constexpr ParseCtx::iterator parse(ParseCtx& ctx) {
            return keyFormatter.parse(ctx);
        }

        template<typename FmtCtx>
        // const EditorKey& because if we create a copy, the copy constructor
        // will overwrite the invalid value to Unknown, which we do not want here
        FmtCtx::iterator format(const EditorKey& key, FmtCtx& ctx) const {
            return keyFormatter.format(key.key(), ctx);
        }
    };
}


int main() {

    std::array keys {
        EditorKey { 0 },
        EditorKey { 1 },
        EditorKey { 2 },
        EditorKey { 3 },
        EditorKey { static_cast<EditorKey::Key>(4) },
        EditorKey { static_cast<EditorKey::Key>(10) },
        EditorKey { EditorKey::Key { 11 } },
        EditorKey { EditorKey::Key { 23 } },

        // For now, the constructor will set the key to Unknown
        // We will force-set the last key to an invalid value later
        EditorKey { EditorKey::Key { 66 } },
    };

    for (const auto& key: keys) {
        std::println(
            "{:>9} = Key {{ key={:>9}, asInt={:>5}, known={:>5} }}",
            key,
            key.key(),
            key.asInt(),
            key.known()
        );
    }


    std::println("");
    std::println("{:->60}", "");
    std::println("");

    // Will be <Invalid>, because the value 1899 does not correspond to
    // an existing enumerator
    keys.back().forceSet(EditorKey::Key { 1899 });

    for (const auto& key: keys) {
        std::println(
            "{:>9} = Key {{ key={:>9}, asInt={:>5}, known={:>5} }}",
            key,
            key.key(),
            key.asInt(),
            key.known()
        );
    }

}
