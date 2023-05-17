#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"
#include <array>

// sizeof(Object) == 16
struct Object : public GC::Collected {
    GC::GCPtr<Object> ptr1, ptr2;
};

std::vector<GC::GCPtr<Object>> MakeGarbageArray(std::size_t size) {
    std::vector<GC::GCPtr<Object>> arr{size};
    for(std::size_t i = 0; i < size ; ++i) {
        arr.at(i) = new Object;
    }
    return arr;
}

int main(int argc, char *argv[]) {
    
    ASSERT_S(argc==3);
    GC::GarbageCollector::init(argv[1]);
    GC::Logger::GetInstance().init(argv[1]);
    GC::Logger::GetInstance().start();
    std::size_t items = std::stoul(argv[2]);
    std::vector<GC::GCPtr<Object>> arr;
    MakeGarbageArray(items);
    arr = MakeGarbageArray(items);
    GC::GarbageCollector::gc();
    GC::Logger::GetInstance().stop();
    return 0;
}


