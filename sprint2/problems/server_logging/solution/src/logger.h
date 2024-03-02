#pragma once

#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <optional>
#include <mutex>
#include <thread>

#include <shared_mutex>
#include <algorithm>

using namespace std::literals;
namespace logger_utils {


}


#define LOG(...) Logger::GetInstance().Log(__VA_ARGS__)

class Logger {
    auto GetTime() const;

    auto GetTimeStamp() const;

    // Для имени файла возьмите дату с форматом "%Y_%m_%d"
    std::string GetFileTimeStamp() const;

    Logger() = default;
    Logger(const Logger&) = delete;

public:
    ~Logger(){
        logFile_.close();
    }
    static Logger& GetInstance();

    // Выведите в поток все аргументы.
    template<class... Ts>
    void Log(const Ts&... args)
    {
        std::lock_guard guard(mutex_);

        //logFile_ << GetTimeStamp() << ": ";
        ((logFile_ << args), ...);
        logFile_ << std::endl;

    }

    // Установите manual_ts_. Учтите, что эта операция может выполняться
    // параллельно с выводом в поток, вам нужно предусмотреть
    // синхронизацию.
    void SetTimestamp(std::chrono::system_clock::time_point ts);

private:
    std::optional<std::chrono::system_clock::time_point> manual_ts_;

    const std::string defNameLogFile = "sample_log_";
    const std::string folderLogFile = "/var/log/";
    //const std::string folderLogFile = "./log/";
    const std::string fileExp = ".log";
    std::ofstream logFile_;

    //mutable std::shared_mutex mutex_;
    mutable std::mutex mutex_;
};
