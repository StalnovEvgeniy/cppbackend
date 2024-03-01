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


#define LOG(...) Logger::GetInstance().Log(__VA_ARGS__)

class Logger {
    auto GetTime() const {
        if (manual_ts_) {
            return *manual_ts_;
        }

        return std::chrono::system_clock::now();
    }

    auto GetTimeStamp() const {
        const auto now = GetTime();
        const auto t_c = std::chrono::system_clock::to_time_t(now);
        return std::put_time(std::localtime(&t_c), "%F %T");
    }

    // Для имени файла возьмите дату с форматом "%Y_%m_%d"
    std::string GetFileTimeStamp() const
    {
       std::stringstream strStream;
       const auto now = GetTime();
       const auto t_c = std::chrono::system_clock::to_time_t(now);
       strStream << std::put_time(std::localtime(&t_c), "%F");
       std::string str = strStream.str();
       //std::string str{ctime(&timeT)};

       std::replace(str.begin(), str.end(), '-', '_');

       return str;
    }

    Logger() = default;
    Logger(const Logger&) = delete;

public:
    ~Logger(){
        logFile_.close();
    }
    static Logger& GetInstance() {
        static Logger obj;
        return obj;
    }

    // Выведите в поток все аргументы.
    template<class... Ts>
    void Log(const Ts&... args)
    {
        std::lock_guard guard(mutex_);

        logFile_ << GetTimeStamp() << ": "sv;
        ((logFile_ << args), ...);
        logFile_ << std::endl;

    }

    // Установите manual_ts_. Учтите, что эта операция может выполняться
    // параллельно с выводом в поток, вам нужно предусмотреть 
    // синхронизацию.
    void SetTimestamp(std::chrono::system_clock::time_point ts)
    {
        std::lock_guard guard(mutex_);

        manual_ts_ = ts;

        logFile_.close();

        std::string pathFile{folderLogFile + defNameLogFile + GetFileTimeStamp() + fileExp};

        logFile_.open(pathFile, std::ios::app);
        //logFile_.open(pathFile, std::ios::in);
    }

private:
    std::optional<std::chrono::system_clock::time_point> manual_ts_;

    const std::string defNameLogFile = "sample_log_"s;
    const std::string folderLogFile = "/var/log/"s;
    //const std::string folderLogFile = "./log/"s;
    const std::string fileExp = ".log"s;
    std::ofstream logFile_;

    //mutable std::shared_mutex mutex_;
    mutable std::mutex mutex_;
};
