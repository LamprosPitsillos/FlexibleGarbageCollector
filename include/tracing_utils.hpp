#pragma once
#include "GarbageCollector.hpp"
#include <map>
#include <vector>
#include "./Utils/dev_utils.hpp"

namespace GC {
namespace Tracing {

using RootVector = std::vector<Collected*>;

void init(GC::json const& config_json);
}
}
