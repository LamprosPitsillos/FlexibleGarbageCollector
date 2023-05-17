#pragma once
#include <cstddef>

namespace GC {

struct Header {
    std::size_t size;
    Header(std::size_t size) : size(size) {}
    virtual ~Header(void) = default;
};

struct TraceHeader : public Header {
    bool marked;
    TraceHeader(std::size_t size, bool marked) : Header(size), marked(marked) {}
};

struct CounterHeader : public Header {
    int counter;
    bool visited;
    CounterHeader(std::size_t size, int counter) : Header(size), counter(counter), visited(false) {}
};

}