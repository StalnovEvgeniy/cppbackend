#include <iostream>
#include <thread>
#include <string_view>

#include <boost/log/trivial.hpp>     // для BOOST_LOG_TRIVIAL
#include <boost/log/core.hpp>        // для logging::core
#include <boost/log/expressions.hpp> // для выражения, задающего фильтр

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/date_time.hpp>

using namespace std::literals;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

void MyFormatter(logging::record_view const& rec, logging::formatting_ostream& strm) {
    // Выводить LineID стало проще.
    strm << rec[line_id] << ": ";

    // Момент времени приходится вручную конвертировать в строку.
    // Для получения истинного значения атрибута нужно добавить
    // разыменование.
    auto ts = *rec[timestamp];
    strm << to_iso_extended_string(ts) << ": ";

    // Выводим уровень, заключая его в угловые скобки.
    strm << "<" << rec[logging::trivial::severity] << "> ";

    // Выводим само сообщение.
    strm << rec[logging::expressions::smessage];
}


void InitBoostLogFilter() {

    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );
}

void MyFormatter_1(logging::record_view const& rec, logging::formatting_ostream& strm) {
    // выводим атрибут LineID
    strm << logging::extract<unsigned int>("LineID", rec) << ": ";

    // выводим уровень, заключая его в угловые скобки
    strm << "<" << rec[logging::trivial::severity] << "> ";

    // выводим само сообщение
    strm << rec[logging::expressions::smessage];
}

int main() {
    logging::add_common_attributes();
    logging::add_file_log(
                keywords::file_name = "sample_%N.log",
                keywords::format = "[%TimeStamp%]: %Message%",
                keywords::open_mode = std::ios_base::app | std::ios_base::out,
                keywords::rotation_size = 10 * 1024 * 1024,
                // ротируем ежедневно в полдень
                keywords::time_based_rotation = sinks::file::rotation_at_time_point(12, 0, 0)
            );

    logging::add_file_log(
        keywords::file_name = "sample.log",
        keywords::format = &MyFormatter
    );

    logging::add_console_log(
        std::clog,
        keywords::format = "[%TimeStamp%]: %Message%",
        keywords::auto_flush = true
    );

    InitBoostLogFilter();

    int log_num = 1;

    BOOST_LOG_TRIVIAL(trace) << "Сообщение уровня trace, "sv << log_num++;
    BOOST_LOG_TRIVIAL(debug) << "Сообщение уровня debug, "sv << log_num++;
    BOOST_LOG_TRIVIAL(info) << "Сообщение уровня info, "sv << log_num++;
    BOOST_LOG_TRIVIAL(warning) << "Сообщение уровня warning, "sv << log_num++;
    BOOST_LOG_TRIVIAL(error) << "Сообщение уровня error, "sv << log_num++;
    BOOST_LOG_TRIVIAL(fatal) << "Сообщение уровня fatal, "sv << log_num++;

    BOOST_LOG_TRIVIAL(trace) << "Сообщение уровня trace"sv;
    BOOST_LOG_TRIVIAL(debug) << "Сообщение уровня debug"sv;
    BOOST_LOG_TRIVIAL(info) << "Сообщение уровня info"sv;
    BOOST_LOG_TRIVIAL(warning) << "Сообщение уровня warning"sv;
    BOOST_LOG_TRIVIAL(error) << "Сообщение уровня error"sv;
    BOOST_LOG_TRIVIAL(fatal) << "Сообщение уровня fatal"sv;

    std::cout << std::hex << std::this_thread::get_id() << std::endl;

    BOOST_LOG_TRIVIAL(trace) << "Сообщение уровня trace"sv;
    BOOST_LOG_TRIVIAL(debug) << "Сообщение уровня debug"sv;
    BOOST_LOG_TRIVIAL(info) << "Сообщение уровня info"sv;
    BOOST_LOG_TRIVIAL(warning) << "Сообщение уровня warning"sv;
    BOOST_LOG_TRIVIAL(error) << "Сообщение уровня error"sv;
    BOOST_LOG_TRIVIAL(fatal) << "Сообщение уровня fatal"sv;
}
