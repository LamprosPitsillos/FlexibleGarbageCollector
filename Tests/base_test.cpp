#include "../include/GCPtr.hpp"
#include "../include/Logger.hpp"

#include <iostream>
#include <unistd.h>
#include <malloc.h>
#include <list>
#include <map>

using namespace GC;

struct Foo : public Collected {
    GCPtr<Foo> lfoo, rfoo;
};

void empty(){return;}
void make_garbage(void) {
    for(int i = 0; i < 20; ++i) {
        new Foo();
    }
    // Foo* foo = new Foo();
    // auto x = new int;
    // delete x;
}

// void make_garbage_list(void) {
//     Foo* foo = new Foo();
//     for(int i = 1; i < 1'000'000; ++i) {
//         foo->foo = new Foo();
//         foo = foo->foo; 
//     }
// }

void make_tree(GCPtr<Foo>& foo, int depth) {
    foo = new Foo();
    if(!depth) return;
    // foo->lfoo = new Foo();
    // foo->rfoo = new Foo();
    make_tree(foo->lfoo, depth-1);
    make_tree(foo->rfoo, depth-1);
    return;
}

GCPtr<Foo> make_garbage_tree2(int depth) {
    GCPtr<Foo> foo;
    make_tree(foo, depth);
    return foo;
}

void bar2(void) {
    make_garbage();
    GarbageCollector::gc();
}

class Bar {};

void foobar(void) {
    GCPtr<Foo> f1 = new Foo();
    GCPtr<Foo> f2 = new Foo();
    GCPtr<Foo> f3 = new Foo();
    GCPtr<Foo> f4 = f1;
    GCPtr<Foo> f5 = f4;
    GCPtr<Foo> f6 = f2;
}
void test (){
    GCPtr< Foo > foo = new Foo;
    GCPtr< Foo > aoo = new Foo;
    GCPtr< Foo > joo = new Foo;
    // GCPtr< Foo > jsoo = new Foo;
}
void test1 (){
    GCPtr<Foo> foo = new Foo;
    foo->lfoo = new Foo;
    foo->lfoo->lfoo = new Foo;
    foo->lfoo->lfoo->lfoo = foo->lfoo;
}

 int main(int argc, char* argv[]) {
     GarbageCollector::init(argv[1]);
     Logger::GetInstance().init(argv[1]);
     // GCPtr< Foo > joo = new Foo;
     Logger::GetInstance().start();
     test();
     test1();
     make_garbage_tree2(2);
     make_garbage_tree2(3);

     GCPtr<Foo> foo = new Foo;
     GCPtr<Foo> aoo = new Foo;
     GCPtr<Foo> joo = new Foo;

     GarbageCollector::gc();

     // TEST(GarbageCollector::used_memory==3*sizeof(GCPtr<Foo>), "Simple Test")

    // std::cout << " 3*sizeof(GCPtr<Foo>)" << 3*sizeof(GCPtr<Foo>) << std::endl;
     GarbageCollector::gc();
     Logger::GetInstance().stop();
    

     return 0;
 }
