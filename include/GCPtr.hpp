#pragma once
#include "Collected.hpp"
#include <concepts>
#include "GarbageCollector.hpp"

namespace GC {

template <typename Wrappee>
class GCPtr {
    Wrappee* ptr;
public:
    GCPtr(void) : ptr(nullptr) {}
    GCPtr(void* ptr) {
        this->ptr = nullptr;
        *this = ptr;
    }
    GCPtr(GCPtr const& other) {
        this->ptr = nullptr;
        *this = other;
    }

    Wrappee* get_raw_ptr(void) const { return this->ptr; }

    GCPtr& operator =(GCPtr const& other) {
        return *this = other.get_raw_ptr();
    }
    GCPtr& operator =(void* ptr) {
        INVOKE(Collected::assigner, this->ptr, (Wrappee*)ptr);
        this->ptr = (Wrappee*)ptr;
        return *this;
    }
    Wrappee& operator *(void) {
        return *this->ptr;
    }
    Wrappee* operator ->(void) {
        return this->ptr;
    }
    operator Wrappee*(void) const {
        return this->ptr;
    }
    ~GCPtr(void) {
        INVOKE(Collected::destructor, this->ptr);
        static_assert(std::is_base_of<Collected, Wrappee>::value);
    }
};

}
