#include "logger.h"

using namespace std::literals;

auto Logger::GetTime() const {
    if (manual_ts_) {
        return *manual_ts_;
    }

    return std::chrono::system_clock::now();
}

auto Logger::GetTimeStamp() const {
    const auto now = GetTime();
    const auto t_c = std::chrono::system_clock::to_time_t(now);
    return std::put_time(std::localtime(&t_c), "%F %T");
}

std::string Logger::GetFileTimeStamp() const
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

Logger &Logger::GetInstance() {
    static Logger obj;
    return obj;
}

void Logger::SetTimestamp(std::chrono::system_clock::time_point ts)
{
    std::lock_guard guard(mutex_);

    manual_ts_ = ts;

    logFile_.close();

    std::string pathFile{folderLogFile + defNameLogFile + GetFileTimeStamp() + fileExp};

    logFile_.open(pathFile, std::ios::app);
    //logFile_.open(pathFile, std::ios::in);
}
