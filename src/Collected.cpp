#include "../include/Collected.hpp"
#include "../include/GarbageCollector.hpp"

namespace GC {

Header* Collected::get_header(void) {
    return GarbageCollector::objects_info.at(this);
}

Collected::Allocator Collected::allocator;
Collected::Destructor Collected::destructor;
Collected::Assigner Collected::assigner;

void* Collected::operator new(std::size_t size) {
    void* const object = ::operator new(size);
    GarbageCollector::used_memory += size;
    allocator(object, size);
    return object;
}

Collected::PtrVector Collected::get_pointers(void) {
    using std::uint8_t, std::uintptr_t;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(this);
    assert(ptr);
    uint8_t* const end = ptr + this->get_header()->size - sizeof(Collected*) + 1;
    PtrVector pointers;
    while(ptr < end) {
        Collected* const address = reinterpret_cast<Collected*>(*(uintptr_t*) ptr);
        if(GarbageCollector::objects_info.count(address)) {
            pointers.push_back(address);
            ptr += sizeof(Collected*);
        } else {
            ++ptr;
        }
    }
    return pointers;
}

void Collected::Delete(Collected* __this, std::size_t size) {
    delete __this;
    GarbageCollector::used_memory -= size;
}

}