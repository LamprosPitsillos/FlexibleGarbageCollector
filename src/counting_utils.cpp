#include "../include/counting_utils.hpp"
#include <cassert>
#include <queue>
#include <stack>

namespace GC {
namespace Count {

namespace Instant {

void allocator(void *object, std::size_t size) {
  CounterHeader *const header = new CounterHeader(size, 0);
  GarbageCollector::objects_info.insert(
      std::make_pair((Collected *)object, header));
}

void RDFS_destructor(Collected *ptr) {
  CounterHeader *const header = (CounterHeader *)ptr->get_header();
  if (--header->counter > 0) {
    return;
  }
  for (auto &neighbor_ptr : ptr->get_pointers()) {
    RDFS_destructor(neighbor_ptr);
  }
  GarbageCollector::objects_info.erase(ptr);
  Collected::Delete(ptr, header->size);
}

void IDFS_destructor(Collected *ptr) {
  std::stack<Collected*> worklist;
  worklist.push(ptr);
  while(!worklist.empty()) {
    Collected* const curr = worklist.top();
    worklist.pop();
    CounterHeader *const header = (CounterHeader *)curr->get_header();
    if (!--header->counter) {
      for (auto &neighbor_ptr : curr->get_pointers()) {
        worklist.push(neighbor_ptr);
      }
      GarbageCollector::objects_info.erase(curr);
      Collected::Delete(curr, header->size);
    }
  }
}

void BFS_destructor(Collected* ptr) {
  std::queue<Collected*> worklist;
  worklist.push(ptr);
  while(!worklist.empty()) {
    Collected* const curr = worklist.front();
    worklist.pop();
    CounterHeader *const header = (CounterHeader *)curr->get_header();
    if (!--header->counter) {
      for (auto &neighbor_ptr : curr->get_pointers()) {
        worklist.push(neighbor_ptr);
      }
      GarbageCollector::objects_info.erase(curr);
      Collected::Delete(curr, header->size);
    }
  }
}

void assigner(Collected* lvalue, Collected* rvalue) {
  if(lvalue) {
    Collected::destructor(lvalue);
  }
  if(rvalue) {
    CounterHeader* const rvalue_header = (CounterHeader*)GarbageCollector::objects_info.at(rvalue);
    ++rvalue_header->counter;
  }
}

std::map<std::string, std::function<void(Collected*)>> instant_destructor_dispatcher = {
    {"RDFS", RDFS_destructor}, {"IDFS", IDFS_destructor}, {"BFS", BFS_destructor}
};

void init(std::string const& graph_traversal) {
  Collected::allocator = allocator;
  Collected::destructor = instant_destructor_dispatcher.at(graph_traversal);
  Collected::assigner = assigner;
}

} // namespace Instant

namespace Late {

void allocator(void *object, std::size_t size) {
  CounterHeader *const header = new CounterHeader(size, 0);
  GarbageCollector::objects_info.insert(
      std::make_pair((Collected *)object, header));
}

void RDFS_destructor(Collected *ptr) {
  CounterHeader *const header = (CounterHeader *)ptr->get_header();
  if (--header->counter != 0)
    return;
  for (auto &neighbor_ptr : ptr->get_pointers()) {
    RDFS_destructor(neighbor_ptr);
  }
}

void IDFS_destructor(Collected* ptr) {
  std::stack<Collected*> worklist;
  worklist.push(ptr);
  while(!worklist.empty()) {
    Collected* const curr = worklist.top();
    worklist.pop();
    CounterHeader *const header = (CounterHeader *)curr->get_header();
    if (!--header->counter) {
      for (auto &neighbor_ptr : curr->get_pointers()) {
        worklist.push(neighbor_ptr);
      }
    }
  }
}

void BFS_destructor(Collected* ptr) {
  std::queue<Collected*> worklist;
  worklist.push(ptr);
  while(!worklist.empty()) {
    Collected* const curr = worklist.front();
    worklist.pop();
    CounterHeader *const header = (CounterHeader *)curr->get_header();
    if (!--header->counter) {
      for (auto &neighbor_ptr : curr->get_pointers()) {
        worklist.push(neighbor_ptr);
      }
    }
  }
}

void assigner(Collected* lvalue, Collected* rvalue) {
  if(lvalue) {
    Collected::destructor(lvalue);
  }
  if(rvalue) {
    CounterHeader* const rvalue_header = (CounterHeader*)GarbageCollector::objects_info.at(rvalue);
    ++rvalue_header->counter;
  }
}

    // CounterHeader *const neighbor_header =
    //     (CounterHeader *)neighbor_ptr->get_header();
    // assert(neighbor_header->counter > 0);
    // --neighbor_header->counter;
//
void gc(void) {
  auto it = GarbageCollector::objects_info.begin();
  while (it != GarbageCollector::objects_info.end()) {
    const auto &[object, header] = *it;
    CounterHeader *const counter_header = (CounterHeader *)header;
    if (counter_header->counter <= 0) {
      it = GarbageCollector::objects_info.erase(it);
      Collected::Delete(object, header->size);
    } else
      ++it;
  }
}

std::map<std::string, std::function<void(Collected*)>> late_destructor_dispatcher = {
    {"RDFS", RDFS_destructor}, {"IDFS", IDFS_destructor}, {"BFS", BFS_destructor}
};

void init(std::string const& graph_traversal) {
  Collected::allocator = allocator;
  Collected::destructor = late_destructor_dispatcher.at(graph_traversal);
  Collected::assigner = assigner;
  GarbageCollector::gc_functor = gc;
}
} // namespace Late

std::map<std::string, std::function<void(std::string const&)>> count_init_dispatcher = {
    {"Instant", Instant::init}, {"Late", Late::init}};

void init(GC::json const &config_json) {
  count_init_dispatcher.at(config_json["destruction"])(config_json["graph_traversal"]);
}

} // namespace Count
} // namespace GC
