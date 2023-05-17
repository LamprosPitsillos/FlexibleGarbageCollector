#include "../include/tracing_utils.hpp"
#include <csetjmp>
#include <iostream>
#include <stack>

namespace GC {
namespace Tracing {

std::function<void(void)> mark;

void allocator(void *object, std::size_t size) {
  LOG_V(WARN,"TRACING :: New item created of size %lu", size);
  TraceHeader *const header = new TraceHeader(size, false);
  GarbageCollector::objects_info.insert(
      std::make_pair((Collected *)object, header));
}

RootVector get_roots(void) {
  RootVector result;
  jmp_buf jb;
  setjmp(jb);
  uint8_t *rsp = (uint8_t *)GarbageCollector::__rsp;
  // LOG_V(ERROR, "STACK PTR %p \n   STACK PTR* %p", rsp, __rsp)
  uint8_t *top = (uint8_t *)GarbageCollector::__rbp;
  unsigned int cnt = 0;
  ASSERT_S(rsp < top);
  while (rsp < top) {
    ++cnt;
    auto address = (Collected *)*(uintptr_t *)rsp;
    if (GarbageCollector::objects_info.count(address)) {
      result.emplace_back(address);
      rsp += sizeof(Collected *);
    } else {
      ++rsp;
    }
  }

        LOG_V(WARN,"Loop cheched %d bytes from %p to %p",cnt,GarbageCollector::__rsp,top);
  return result;
}

static void __IDFSmark(Collected *ptr) {
  std::stack<Collected *> stack;
  stack.push(ptr);
  while (!stack.empty()) {
    Collected *const top = stack.top();
    stack.pop();
    TraceHeader *const header = (TraceHeader *)top->get_header();
    if (!header->marked) {
      header->marked = true;
      for (auto &neighbor_ptr : top->get_pointers()) {
        stack.push(neighbor_ptr);
      }
    }
  }
}

void IDFSmark(void) {
  RootVector root_vector = get_roots();
LOG_V(WARN,"IDFS :: Number of roots at mark %lu",root_vector.size())
  for (auto &root : root_vector)
    __IDFSmark(root);
}

static void __RDFSmark(Collected *ptr) {
  TraceHeader *const header = (TraceHeader *)ptr->get_header();
  if (!header->marked) {
    header->marked = true;
    for (auto &neighbor_ptr : ptr->get_pointers()) {
      __RDFSmark(neighbor_ptr);
    }
  }
}

void RDFSmark(void) {
  RootVector root_vector = get_roots();
LOG_V(WARN,"RDFS :: Number of roots at mark %lu",root_vector.size())
  for (auto &root : root_vector)
    __RDFSmark(root);
}

void BFSmark(void) {
  RootVector worklist = get_roots();
    LOG_V(WARN,"BFS :: Number of roots at mark %lu",worklist.size())
  while (!worklist.empty()) {
    Collected *traceable_ptr = worklist.back();
    worklist.pop_back();
    TraceHeader *header = (TraceHeader *)traceable_ptr->get_header();
    if (!header->marked) {
      header->marked = true;
      for (const auto &ptr : traceable_ptr->get_pointers()) {
        worklist.push_back(ptr);
      }
    }
  }
}

void sweep(void) {
  int ctr = 0;
  for (auto it = GarbageCollector::objects_info.cbegin();
       it != GarbageCollector::objects_info.cend();) {
    auto const &[traceable_ptr, header] = *it;
    TraceHeader *trace_header = (TraceHeader *)header;
    if (trace_header->marked) {
      trace_header->marked = false;
      ++it;
    } else {
      it = GarbageCollector::objects_info.erase(it);
      ctr++;
      Collected::Delete(traceable_ptr, trace_header->size);
    }
  }
    LOG_V(WARN,"Deleted %d items",ctr);
}

void gc(void) {

    LOG_V(WARN,"FRAME ADDRESS %p of GC callee",__builtin_frame_address(0));
    GarbageCollector::__rsp = (uintptr_t *)__builtin_frame_address(0);
  // Tracing::__stackBegin = Tracing::__rbp;

  LOG_V(ERROR,"The stack frame to MARK is \n   %p ==> %p ,size %lu ",
        GarbageCollector::__rbp,GarbageCollector::__rsp,
        GarbageCollector::__rbp - GarbageCollector::__rsp);
  mark();
    LOG_V(PASS,"Live objects BEFORE sweep %lu",GarbageCollector::objects_info.size());
  sweep();
    LOG_V(PASS,"Live objects AFTER sweep %lu",GarbageCollector::objects_info.size());
};

void init(GC::json const &config_json) {
  const std::map<std::string, std::function<void(void)>> mark_dispatch = {
      {"BFS", BFSmark}, {"RDFS", RDFSmark}, {"IDFS", IDFSmark}};
  Collected::allocator = allocator;
  Collected::destructor = [](Collected *) {};
  mark = mark_dispatch.at(config_json["graph_traversal"]);
  GarbageCollector::gc_functor = gc;
}

}
}
