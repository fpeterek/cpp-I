#include <cstddef>
#include <cstdint>
#include <new>

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        // 
//   Open the source code in Compiler Explorer and see what the compiler generates        //
//   There is no point trying to compile the code, because there are many unimplemented   //
//   functions, and the main function is not defined.                                     // 
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        // 
//   For the best output use the latest Clang and the latest supported C++ standard:      // 
//                                                                                        // 
//       +-----------------------------------------------+                                // 
//       |  Compiler  |  x86-64 clang (trunk)            |                                //
//       +-----------------------------------------------+                                // 
//       |  Flags     |  -O3 -std=c++23 -fno-exceptions  |                                //
//       +-----------------------------------------------+                                // 
//                                                                                        // 
//   Clang gives us prettier assembly                                                     //
//   Optimizations remove unnecessary instructions                                        //
//   No exceptions to exclude stack unwinding logic                                       //
//                                                                                        // 
//   Feel free to use the latest standard. If a newer standard than C++23 is already      // 
//   supported, you can enable the latest standard, though your results may vary.         //
//                                                                                        // 
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        // 
//   For the most compatible output, use precisely this:                                  // 
//                                                                                        // 
//       +-----------------------------------------------+                                // 
//       |  Compiler  |  x86-64 clang (22.1.0)           |                                //
//       +-----------------------------------------------+                                // 
//       |  Flags     |  -O3 -std=c++23 -fno-exceptions  |                                //
//       +-----------------------------------------------+                                // 
//                                                                                        //
//   This might give you worse results than the latest compilers and standards,           //
//   but the output of your compiler should match the explanations in this file           //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////



// A "complex" class that has constructors and a destructor,
// and thus is not trivially constructible/copyable/moveable/destructible,
// so the compiler cannot return using a register and is forced to apply RVO
class ComplexClass {

    // We could also inflate the class with more member variables, but only using
    // two ints will allow us to fit into RAX if we choose to explore how the compiler
    // behaves
    std::int32_t x;
    std::int32_t y;

public:

    // No inline implementation so we avoid all compiler trickery - no inlining
    // no two 32b integers in one 64b register, ...
    // Our initial case is focused on RVO. Though the implementations are provided
    // at the end of the class definitions, so you can explore better optimizations
    // as well
    ComplexClass(std::int32_t x, std::int32_t y);

    ComplexClass(const ComplexClass& other);

    ComplexClass(ComplexClass&& other);

    // -fno-exceptions
    //
    // Disabling exceptions for this example is preferable,
    // because it gets rid of stack unwinding logic.
    // Stack unwinding is not really necessary here, we're focused on RVO,
    // not error handling, and the generated assembly quickly becomes polluted
    // by it. 
    // But feel free to enable exceptions later and explore how they're implemented
    // in C++
    ~ComplexClass();

    // Provide the implementations to experiment with compiler optimizations
    //
    // ComplexClass(std::int32_t x, std::int32_t y) : x{x}, y{y} { }
    // ComplexClass(const ComplexClass& other) : x{other.x}, y{other.y} { }
    // ComplexClass(ComplexClass&& other) : x{other.x}, y{other.y} { }
    // ~ComplexClass() { }
};


// A trivial struct
struct POD {
    std::int32_t x;
    std::int32_t y;
};


// No implementation so the compiler doesn't make assumptions and doesn't optimize
// Pretend the functions will just be linked from elsewhere, and their implementations
// are unknown at compile time
namespace linked {
    ComplexClass rvo();
    POD reg();
    void consume(ComplexClass&);
    void consume(POD&);
    void consume(ComplexClass&, POD&);
}


ComplexClass noRvo(bool flag) {
    ComplexClass cc1 { 1899, 1877 };

    ComplexClass cc2 { 1947, 1987 };

    // Do something with the objects, doesn't matter what
    // We just want to force the compiler to pass the objects somewhere,
    // as if they were actually being used
    linked::consume(cc1);
    linked::consume(cc2);

    // We make RVO impossible by returning one of two values based on a condition
    // But the compiler still finds ways to optimize
    // cmov is generated to avoid branching
    // Move constructor is called
    if (flag) {
        return cc1;
    }

    return cc2;    
}

ComplexClass rvo_impl() {
    // First, move the address of the returned value to the rbx register
    // mov     rbx, rdi
    // 
    // Then,  move two integers to the second and third arg registers
    // mov     esi, 1899
    // mov     edx, 1877
    // 
    // After that, the constructor is called. No destructor called generated
    ComplexClass instance { 1899, 1877 };

    return instance;
}


POD register_impl() {
    // Just two integers, the compiler uses one 64b register
    POD instance { .x = 1899, .y = 1877 };

    return instance;
}


// Test just rvo
void fn_simple_rvo() {
    // rbx is a protected register, and thus is used to store
    // the address for the return value
    
    // push rbx 
    // ...
    // pop rbx
    // 
    // saves the original address on the stack, so the function can use it
    // for it's own calls, then restores it to the original value for the callee 
    
    // Grow stack by 16 bytes (sub rsb)
    // Then compute return value address and store in rbx (lea trickery)
    // Finally, pass the return value address as argument (rdi register = first function arg)
    // sub     rsp, 16
    // lea     rbx, [rsp + 8]
    // mov     rdi, rbx
    ComplexClass rvo_object = linked::rvo();
}


ComplexClass* fn_placement_new_rvo(std::byte* uninit) {
    // mov rdi, rbx  -- Pass the return value address as argument to rvo()
    auto* object = new(uninit) ComplexClass { linked::rvo() };
    // mov rax, rbx -- Move the address to the return register

    return object;
}


// Test rvo, but with more compiler optimizations
void fn_simple_rvo_optimized() {
    // Function call is inlined, only constructor/destructor calls
    // are generated. Implement those two and the function will be simplified further
    ComplexClass rvo_object = rvo_impl();
}


// Test simple return through register
void fn_simple_pod() {
    POD pod_object = linked::reg();

    linked::consume(pod_object);
}


// Test both
void fn_both() {
    ComplexClass rvo_object = linked::rvo();
    POD pod_object = linked::reg();

    linked::consume(rvo_object, pod_object);
}
