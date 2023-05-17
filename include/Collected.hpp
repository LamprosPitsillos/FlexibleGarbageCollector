#pragma once
#include <functional>
#include "Header.hpp"
#include "./Utils/dev_utils.hpp"
#include <cassert>

namespace GC {

class Collected {
public:
    using PtrVector = std::vector<Collected*>;
    using Allocator = std::function<void(void*, std::size_t)>;
    using Destructor = std::function<void(Collected*)>;
    using Assigner = std::function<void(Collected*, Collected*)>;
    static Allocator allocator;
    static Destructor destructor;
    static Assigner assigner;
    Header* get_header(void);
    PtrVector get_pointers(void);
    static void* operator new(size_t);
    static void Delete(Collected*, size_t);
};

}
