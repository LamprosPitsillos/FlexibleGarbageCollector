#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"

// sizeof(Object) == 16
struct Object : public GC::Collected {
    GC::GCPtr<Object> ptr1, ptr2;
};

void MakeGarbageIterative(unsigned const garbage_object_no) {
    GC::GCPtr<Object> arr[garbage_object_no];
    for(unsigned i = 0; i < garbage_object_no; ++i) {
        arr[i] = new Object;
    }
}

int main(int argc, char *argv[]) {
    GC::GarbageCollector::init(argv[1]);
    GC::Logger::GetInstance().init(argv[1]);
    GC::Logger::GetInstance().start();
    MakeGarbageIterative(1000);
    GC::GarbageCollector::gc();
    MakeGarbageIterative(1000);
    MakeGarbageIterative(1000);
    GC::GarbageCollector::gc();
    MakeGarbageIterative(2000);
    MakeGarbageIterative(1000);
    MakeGarbageIterative(1000);
    GC::GarbageCollector::gc();
    GC::Logger::GetInstance().stop();
}
