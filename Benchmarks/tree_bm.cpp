#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"

// sizeof(Object) == 16
struct Object : public GC::Collected {
    GC::GCPtr<Object> ptr1, ptr2;
};

void __MakeTree(GC::GCPtr<Object>& node, short const depth) {
    if(!depth) return;
    node = new Object;
    __MakeTree(node->ptr1, depth-1);
    __MakeTree(node->ptr2, depth-1);
}

GC::GCPtr<Object> MakeTree(short const depth) {
    GC::GCPtr<Object> root = new Object;
    __MakeTree(root->ptr1, depth-1);
    __MakeTree(root->ptr2, depth-1);
    return root;
}

int main(int argc, char *argv[]) {
    ASSERT_S(argc==3);
    GC::GarbageCollector::init(argv[1]);
    GC::Logger::GetInstance().init(argv[1]);
    GC::Logger::GetInstance().start();
    std::size_t items = std::stoul(argv[2]);
    double depth = std::floor(std::log2(items));
    MakeTree(depth);
    GC::GCPtr<Object> tree = MakeTree(depth);
    GC::GarbageCollector::gc();
    GC::Logger::GetInstance().stop();
    return 0;
}
