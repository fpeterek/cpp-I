#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <algorithm>
#include <cstddef>
#include <utility>


namespace fp {

    template<size_t idx, size_t buff_size, typename... Ts>
    struct variant_impl;


    template<size_t idx, size_t buff_size>
    struct variant_impl<idx, buff_size> {

        size_t active;
        std::byte buffer[buff_size];

        std::byte* get_buff() { return buffer; }

    };

    template<size_t idx, size_t buff_size, typename T, typename... Ts>
    struct variant_impl<idx, buff_size, T, Ts...> : variant_impl<idx+1, buff_size, Ts...> {

        using parent = variant_impl<idx+1, buff_size, Ts...>;
        using base = variant_impl<idx+sizeof...(Ts)+1, buff_size>;


        variant_impl()
            : variant_impl<idx+1, buff_size, Ts...>() {

            if constexpr (idx == 0) {
                new(get_buff()) T{};
            }

        }

        template<typename Arg, std::enable_if_t<std::is_same_v<Arg, T>, int> = 1>
        variant_impl(Arg arg)
            : variant_impl<idx+1, buff_size, Ts...>() {

            new(get_buff()) T{std::move(arg)};

        }

        template<typename Arg, std::enable_if_t<not std::is_same_v<Arg, T>, int> = 1>
        variant_impl(Arg arg)
            : variant_impl<idx+1, buff_size, Ts...>(std::move(arg)) { }


        ~variant_impl() {
            if (is_active()) {
                get().~T();
            }
        }


        parent& get_parent() { return *this; }
        auto* get_buff() { return get_parent().get_buff(); }

        base& get_base() { return *this; }

        void clear_current() {
            if (is_active()) {
                get().~T();
                return;
            }

            if constexpr (not std::is_same_v<parent, base>) {
                get_parent().clear_current();
            }
        }

        T& get() {
            return *reinterpret_cast<T*>(get_buff());
        }

        bool is_active() {
            return get_base().active == idx;
        }

        void assign_inactive(auto&& val) {
            clear_current();

            auto& base = get_base();
            new(base.get_buff()) T{std::move(val)};
            base.active = idx;
        }

        void assign_active(auto&& val) {
            get() = std::move(val);
        }

        void assign(auto&& val) {
            if (is_active()) {
                assign_active(val);
            } else {
                assign_inactive(val);
            }
        }

        void operator=(const T& val) {
            assign(val);
        }

        void operator=(T&& val) {
            assign(val);
        }

        void operator=(const parent& other) {
            if (other.is_active) {
                assign(other.get());
            } else {
                get_parent() = other.get_parent();
            }
        }

        void operator=(parent&& other) {
            if (other.is_active) {
                assign(std::move(other.get()));
            } else {
                get_parent() = std::move(other.get_parent());
            }
        }
    };

    template<typename... Ts>
    struct max_size_t;

    template<>
    struct max_size_t<> {
        constexpr static inline size_t size = 0;
    };

    template<typename T, typename... Ts>
    struct max_size_t<T, Ts...> {
        constexpr static inline size_t size = std::max(sizeof(T), max_size_t<Ts...>::size);
    };

    template<typename... Ts>
    constexpr size_t max_size = max_size_t<Ts...>::size;


    template<typename... Ts>
    class variant : variant_impl<0, max_size<Ts...>, Ts...> {

        template<typename T, size_t idx, typename... Rest>
        static auto& get_var_impl_helper(variant_impl<idx, max_size<Ts...>, T, Rest...>& vi) {
            return vi;
        }

        template<typename T>
        auto& get_var_impl() {
            return get_var_impl_helper<T>(*this);
        }

        using parent = variant_impl<0, max_size<Ts...>, Ts...>;

        parent&       get_parent()       { return *this; }
        parent const& get_parent() const { return *this; }

    public:


        template<typename Needle, typename... Haystack>
        friend Needle& get(fp::variant<Haystack...>& var);

        template<typename Needle, typename... Haystack>
        friend bool has(fp::variant<Haystack...>& var);

        variant() : variant_impl<0, max_size<Ts...>, Ts...>() { }

        template<typename Arg>
        variant(Arg arg) : variant_impl<0, max_size<Ts...>, Ts...>(std::move(arg)) { }

        template<typename Arg>
        variant& operator=(Arg value) {
            auto& var_impl = get_var_impl<Arg>();
            var_impl = std::move(value);

            return *this;
        }

        variant& operator=(const variant<Ts...>& other) {
            get_parent() = other.get_parent();

            return *this;
        }

        variant& operator=(variant<Ts...>&& other) {
            get_parent() = std::move(other.get_parent());

            return *this;
        }

    };

    template<typename T, size_t idx, size_t buff_size, typename... Ts>
    T& get_impl(fp::variant_impl<idx, buff_size, T, Ts...>& var) {
        return var.get();
    }

    template<typename T, typename... Ts>
    T& get(fp::variant<Ts...>& var) {
        return fp::get_impl<T>(var);
    }

    template<typename T, size_t idx, size_t buff_size, typename... Ts>
    T& has_impl(fp::variant_impl<idx, buff_size, T, Ts...>& var) {
        return var.is_active();
    }

    template<typename T, typename... Ts>
    bool& has(::fp::variant<Ts...>& var) {
        return fp::has_impl<T>(var);
    }
} // namespace fp


namespace std {
    template<typename T, typename... Ts>
    T& get(::fp::variant<Ts...>& variant) {
        return ::fp::get<T>(variant);
    }

    template<typename T, typename... Ts>
    bool& has(::fp::variant<Ts...>& variant) {
        return ::fp::has<T>(variant);
    }
}

#endif // TUPLE_HPP
