#include "../include/Logger.hpp"
#include <iostream>
#include <unistd.h>
#include <utility>

namespace GC {

Logger *Logger::Singleton = nullptr;

void Logger::init(std::string const &config_path){
        std::ifstream config_file(config_path);
        if(!config_file.is_open()) {
            ASSERT_V(false,"UNABLE TO OPEN FILE AT PATH: %s ",config_path.c_str())
        }
        json config = json::parse(config_file);

    
    // LOG_V(PASS,"path %s ",config_path.c_str())

    path = config["log_path"].get<std::string>();
    // LOG_V(PASS,"path %s ",path.c_str())
}
Logger &Logger::GetInstance() {
    if (Singleton == nullptr) {
        LOG(PASS, "LOGGER initialized");
        Singleton = new Logger();
        ASSERT(Singleton, "Logger Instance not initialized");
    }
    return *Singleton;
}
Logger::Logger() {}

void Logger::log_work(){
    while (true) {
        // Check if the thread should be paused
        log();
        if (logWorker.get_stop_token().stop_requested()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        // Call log every 50 microseconds
    }
}

void Logger::start(bool append) {

    // log_file.open(path, append ? std::ofstream::app : std::ofstream::trunc);
    // ASSERT_V(log_file, "File %s failed to open.", path.c_str());
    // LOG_V(PASS, "File %s opened for logging.", path.c_str());

    if (!append) std::cout << "HeapSize (items),Memory Used (bytes),Dt (μs)\n";

    //START TIMER///////////////////////////////////////////////////////////////////
    auto now = std::chrono::system_clock::now();
    begin= now;
    logWorker = std::jthread(&Logger::log_work, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    log();
}

void Logger::log(){
    auto now = std::chrono::system_clock::now();
    end=now;
    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    const auto &gc_data = GC::GarbageCollector::objects_info;
    std::cout << gc_data.size() << ',' << GC::GarbageCollector::used_memory << ',' << delta << "\n";
    std::swap(end,begin);
}

void Logger::stop() {

    // Request that the logging thread stop
    logWorker.request_stop();
    // Wait for the thread to finish before returning
    logWorker.join();
    // log();
    std::cout.flush();

}

// void Logger::log(const std::string &path) {
//   log_file.open(path, log_file.out);
//
//   ASSERT_V(log_file.is_open(), "File %s failed to open.", path.c_str());
//
//   LOG_V(PASS, "File %s opened for logging.", path.c_str());
//
//   log_file << "HeapSize" << ',' << "foo" << ',' << "Dt"
//            << "\n";
//
//   std::chrono::duration<double> delta = end.top() - begin.top();
//
//   const auto &gc_data = GC::GarbageCollector::objects_info;
//
//   log_file << gc_data.size() << ',' << "b" << ',' << delta.count() << "\n";
// }

} // namespace GC
