#include "../include/GarbageCollector.hpp"
#include "../include/tracing_utils.hpp"
#include "../include/counting_utils.hpp"
#include "../include/Utils/dev_utils.hpp"


namespace GC {

std::map<std::string, std::function<void(nlohmann::json const&)>> const init_dispatcher = {
  {"Tracing", Tracing::init}, {"Counting", Count::init}
};


uintptr_t* GarbageCollector::__rbp;
uintptr_t* GarbageCollector::__rsp;
std::function<void(void)> GarbageCollector::gc_functor;
std::size_t GarbageCollector::used_memory = 0;
GarbageCollector::CollectedMapType GarbageCollector::objects_info;

}