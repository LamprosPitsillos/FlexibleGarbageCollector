#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"
#include <iostream>

// sizeof(Object) == 16
struct Object : public GC::Collected {
    GC::GCPtr<Object> ptr1, ptr2;
};

void MakeGarbageSimple(void) {
    GC::GCPtr<Object> x = new Object;
    GC::GCPtr<Object> y = new Object;
    GC::GCPtr<Object> z = new Object;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void MakeGarbageComplex(void) {
    GC::GCPtr<Object> x = new Object;
    GC::GCPtr<Object> y = new Object;
    GC::GCPtr<Object> z = new Object;
    x->ptr1 = y;
    x->ptr2 = z;
    y->ptr1 = z;
    y->ptr2 = z;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void MakeGarbageCycle(void) {
    GC::GCPtr<Object> x = new Object;
    GC::GCPtr<Object> y = new Object;
    GC::GCPtr<Object> z = new Object;
    z->ptr1 = y;
    y->ptr1 = x;
    x->ptr1 = y;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main(int argc, char *argv[]) {
    GC::GarbageCollector::init(argv[1]);
    GC::Logger::GetInstance().init(argv[1]);
    GC::Logger::GetInstance().start();
    MakeGarbageSimple();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    GC::GarbageCollector::gc();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    MakeGarbageComplex();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    GC::GarbageCollector::gc();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    MakeGarbageCycle();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    GC::GarbageCollector::gc();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    GC::Logger::GetInstance().stop();
}
