#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include "Collected.hpp"
#include "json.hpp" 
#include <fstream>

namespace GC {

using json = nlohmann::json;
extern std::map<std::string, std::function<void(nlohmann::json const&)>> const init_dispatcher;

// #define __READ_RSP(reg) reg =(uintptr_t*) __builtin_frame_address(0)

class GarbageCollector {
public:
    using InitFunction = std::function<void(json const&)>;
    using CollectedMapType = std::map<Collected*, Header*>;
    static CollectedMapType objects_info;
    static std::size_t used_memory;
    static std::function<void(void)> gc_functor;
    static uintptr_t *__rbp;
    static uintptr_t *__rsp;
    static inline __attribute__((always_inline)) void ReadRBP(void) { asm ("movq %%rbp, %0" : "=r"(__rbp)); }
    static inline __attribute__((always_inline)) void ReadRSP(void) { asm ("movq %%rsp, %0" : "=r"(__rsp)); }
    static void gc(void) { INVOKE(gc_functor); }
    static inline __attribute__((always_inline)) void init(std::string const& config_path) {
        ReadRBP();
        std::ifstream config_file(config_path);
        if(!config_file.is_open()) {
            ASSERT_V(false,"UNABLE TO OPEN FILE AT PATH: %s ",config_path.c_str())
        }
        json config_json = json::parse(config_file);

        init_dispatcher.at(config_json["collection_type"])(config_json);
    }
};

}
