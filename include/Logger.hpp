#include "./GarbageCollector.hpp"
#include "./Utils/dev_utils.hpp"

#include <fstream>
#include <iostream>
#include <stack>
#include <string_view>
#include <chrono>
#include <thread>

namespace GC {

class Logger {

private:
    using point_in_time = std::chrono::system_clock::time_point;
    // https://en.cppreference.com/w/cpp/thread/thread
    std::jthread logWorker;
    std::string path;

    static Logger *Singleton;
    Logger(void);
    ~Logger();

private:
    // std::stack<point_in_time> begin;
    // std::stack<point_in_time> end;
    point_in_time begin;
    point_in_time end;
    std::ofstream log_file;
    void log_work() ;
    void log();

public:
    static Logger &GetInstance();
    void start(bool append=false);
    void stop();
    void init(std::string const &config_path);
};

} // namespace GC
