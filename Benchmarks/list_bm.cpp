#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"

// sizeof(Object) == 16
struct Object : public GC::Collected {
    GC::GCPtr<Object> ptr1, ptr2;
};

GC::GCPtr<Object> MakeList(std::size_t const garbage_no) {
    GC::GCPtr<Object> head = new Object;
    GC::GCPtr<Object> curr = head;
    for(std::size_t i = 0; i < garbage_no - 1; ++i) {
        curr->ptr1 = new Object;
        curr = curr->ptr1;
    }
    return head;
}

int main(int argc, char *argv[]) {
    ASSERT_S(argc==3);
    GC::GarbageCollector::init(argv[1]);
    GC::Logger::GetInstance().init(argv[1]);
    GC::Logger::GetInstance().start();
    std::size_t const garbage_no = std::stoul(argv[2]);
    MakeList(garbage_no);
    GC::GCPtr<Object> list = MakeList(garbage_no);

    GC::GarbageCollector::gc();
    GC::Logger::GetInstance().stop();
    return 0;
}
