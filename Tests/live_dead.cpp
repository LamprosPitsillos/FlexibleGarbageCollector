#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"

// sizeof(Object) == 16
struct Object : public GC::Collected {
    GC::GCPtr<Object> ptr1, ptr2;
};

GC::GCPtr<Object> MakeGarbageList(unsigned const list_size) {
    GC::GCPtr<Object> const head = new Object;
    GC::GCPtr<Object> curr = head;
    for(unsigned i = 1; i < list_size; ++i) {
        curr->ptr1 = new Object;
        curr = curr->ptr1;
    }
    return head;
}


int main(int argc, char *argv[]) {
    GC::GarbageCollector::init(argv[1]);
    GC::Logger::GetInstance().init(argv[1]);
    GC::Logger::GetInstance().start();
    MakeGarbageList(1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    GC::GCPtr<Object> list = MakeGarbageList(1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    GC::GarbageCollector::gc();
    GC::Logger::GetInstance().stop();
    // std::cout << "memory used: " << GC::GarbageCollector::used_memory << std::endl;
}
