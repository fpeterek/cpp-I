#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <initializer_list>
#include <memory>


namespace fp {

template<typename Type>
void destroy(Type* ptr) {
    ptr->~Type();
}

template<typename Type>
void destroy(Type* begin, Type* end) {
    for (Type* ptr = begin; ptr != end; ++ptr) {
        ptr->~Type();
    }
}


template<typename Type>
struct allocator {

    Type* allocate(size_t elems) {
        const size_t bytes = elems * sizeof(Type);

        return static_cast<Type*>(operator new(bytes));
    }

    void deallocate(Type* ptr, [[maybe_unused]] size_t elems) {
        operator delete(ptr);
    }

};


template<typename Type, typename Alloc = fp::allocator<Type>>
class vector {

    Type* begin_ptr = nullptr;
    Type* end_ptr = nullptr;

    size_t reserved = 0;

    Alloc allocator;

    void alloc_with_size_on_cons() {
        begin_ptr = allocator.allocate(reserved);
        end_ptr = begin_ptr;
    }

    template<typename... Args>
    void emplace_back_unsafe(Args&&... args) {
        new(end_ptr) Type { std::forward<Args>(args)... };
        ++end_ptr;
    }

    void grow_if_needed() {
        const size_t oldsize = size();

        if (oldsize < reserved) {
            return;
        }

        reserved = std::max(size_t{32}, reserved * 2);

        Type* mem = allocator.allocate(reserved);

        std::uninitialized_move(begin_ptr, end_ptr, mem);

        std::swap(begin_ptr, mem);
        end_ptr = begin_ptr + oldsize;

        allocator.deallocate(mem, oldsize);
    }

    void destroy_all() {
        fp::destroy(begin_ptr, end_ptr);
    }

public:

    using iterator = Type*;
    using const_iterator = const Type*;

    vector(Alloc allocator = Alloc{}) :
        allocator { std::move(allocator) } { }


    vector(vector<Type>&& other) noexcept :
        begin_ptr { other.begin_ptr },
        end_ptr { other.end_ptr },
        reserved { other.reserved },
        allocator { std::move(other.allocator) } {

        other.begin_ptr = nullptr;
        other.end_ptr = nullptr;
        other.reserved = 0;
    }

    vector(const size_t reserved, Alloc allocator = Alloc{}) :
        reserved { reserved },
        allocator { std::move(allocator) } {

        alloc_with_size_on_cons();
    }

    vector(const vector<Type>& other) :
        vector { other.reserved, other.allocator } {

        try {
            for (const auto& it : other) {
                emplace_back_unsafe(it);
            }
        } // Catch-all just to call destructors and rethrow
        catch (...) {
            destroy_all();
            throw;
        }
    }


    vector(const size_t size, const Type& value, Alloc allocator = Alloc{}) :
        vector { size, std::move(allocator) } {

        try {
            for (size_t i = 0; i < size; ++i) {
                emplace_back_unsafe(value);
            }
        } // Catch-all just to call destructors and rethrow
        catch (...) {
            destroy_all();
            throw;
        }
    }

    vector(std::initializer_list<Type> initializer_list, Alloc allocator = Alloc{}) :
        vector { initializer_list.size(), std::move(allocator) } {

        try {
            for (const auto& it : initializer_list) {
                emplace_back_unsafe(it);
            }
        } // Catch-all just to call destructors and rethrow
        catch (...) {
            destroy_all();
            throw;
        }
    }

    ~vector() {
        fp::destroy(begin_ptr, end_ptr);

        allocator.deallocate(begin_ptr, reserved);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        grow_if_needed();

        emplace_back_unsafe(std::forward<Args>(args)...);
    }
    
    size_t size() const noexcept {
        return static_cast<size_t>(end_ptr - begin_ptr);
    }

    Type& front(this auto& self) noexcept {
        return *(self.begin_ptr);
    }

    Type& back(this auto& self) noexcept {
        return *(self.end_ptr);
    }

    Type& operator[](this auto& self, size_t idx) noexcept {
        return self.begin_ptr[idx];
    }

    iterator begin() noexcept {
        return begin_ptr;
    }

    iterator end() noexcept {
        return end_ptr;
    }

    const_iterator begin() const noexcept {
        return begin_ptr;
    }

    const_iterator end() const noexcept {
        return end_ptr;
    }

};


} // namespace fp


#endif  // VECTOR_HPP
