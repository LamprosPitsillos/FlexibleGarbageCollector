#pragma once
#include "GarbageCollector.hpp"

namespace GC {
namespace Count {

void init(GC::json const&);

namespace Instant {
void init(std::string const&);
}

namespace Late {
void init(std::string const&);
}

}
}
