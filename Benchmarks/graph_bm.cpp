#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"
#include <vector>

// sizeof(Object) == 16
struct Object : public GC::Collected {
    GC::GCPtr<Object> ptr1, ptr2;
};

GC::GCPtr<Object> MakeGraph(std::size_t const garbage_no) {
    std::vector<GC::GCPtr<Object>> objects(garbage_no);
    objects.at(garbage_no-1) = new Object;
    for(std::size_t i = 0; i < garbage_no - 1; ++i) {
        objects.at(i) = new Object;
        objects.at(i)->ptr1 = objects.at(i+1);
        objects.at(i)->ptr2 = objects.at(i + (i % 2 || i == garbage_no-2 ? 1 : 2));
    }

    return objects.front();
}

int main(int argc, char *argv[]) {
    ASSERT_S(argc==3);
    GC::GarbageCollector::init(argv[1]);
    GC::Logger::GetInstance().init(argv[1]);
    GC::Logger::GetInstance().start();
    std::size_t const garbage_no = std::stoul(argv[2]);
    MakeGraph(garbage_no);
    GC::GCPtr<Object> graph = MakeGraph(garbage_no);
    GC::GarbageCollector::gc();
    GC::Logger::GetInstance().stop();
    return 0;
}
