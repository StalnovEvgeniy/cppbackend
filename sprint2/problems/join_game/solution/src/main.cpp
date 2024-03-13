#include "sdk.h"
//
// boost.beast будет использовать std::string_view вместо boost::string_view
//#define BOOST_BEAST_USE_STD_STRING_VIEW

#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <boost/log/trivial.hpp>     // для BOOST_LOG_TRIVIAL
#include <boost/log/core.hpp>        // для logging::core
#include <boost/log/expressions.hpp> // для выражения, задающего фильтр

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/date_time.hpp>

#include <iostream>
#include <thread>

#include "json/loader.h"
#include "request/handler.h"
#include "log_request.h"

//#include "logger.h"
//#include "logger/utils.h"

namespace beast = boost::beast;
namespace http = beast::http;
using namespace std::literals;
namespace net = boost::asio;
namespace fs = std::filesystem;
namespace logging = boost::log;
namespace json = boost::json;

using namespace logger_utils;

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)


namespace {

// Запускает функцию fun на cnt потоках, включая текущий
template <typename Fun>
void RunWorkers(unsigned numThread, const Fun& fun) {
    numThread = std::max(1u, numThread);
    std::vector<std::jthread> workers;
    workers.reserve(numThread - 1);
    // Запускаем cnt-1 рабочих потоков, выполняющих функцию fun
    while (--numThread) {
        workers.emplace_back(fun);
    }
    fun();
}

struct ArgsComandLine {
    bool flagComplit;
    std::string configMap;
    std::string wwwRootPath;
};

ArgsComandLine ParsingCommandLine(int argc, const char* argv[]) {
    ArgsComandLine argsComandLine{};
    argsComandLine.flagComplit = false;
    if (argc  < 2) {
        std::cerr << "Not path config file Maps "sv << std::endl;
    }
    if (argc < 3) {
        std::cerr << "Not path www root folder "sv << std::endl;
    } else {
        argsComandLine.flagComplit = true;
    }
    if (argsComandLine.flagComplit) {
        argsComandLine.configMap = argv[1];
        argsComandLine.wwwRootPath = argv[2];
    }

    return argsComandLine;
}

}  // namespace

int main(int argc, const char* argv[]) {

    InitLogger();
    using namespace http;
    using namespace std::literals;

    ArgsComandLine  argsComandLine = ParsingCommandLine(argc, argv);
    if (!argsComandLine.flagComplit) {
        std::cerr << "Error read command line"sv << std::endl;
        return EXIT_FAILURE;
    } else
        if (argc != 3) {
            std::cerr << "Usage: game_server <game-config-json>"sv << std::endl;
            return EXIT_FAILURE;
        }


    try {
        // 1. Загружаем карту из файла и построить модель игры
        model::Game game = json_loader::LoadGame(argsComandLine.configMap);
        fs::path wwwRootPath{argsComandLine.wwwRootPath};

        // 2. Инициализируем io_context
        const unsigned num_threads = std::thread::hardware_concurrency();
        net::io_context ioc(num_threads);

        // 3. Добавляем асинхронный обработчик сигналов SIGINT и SIGTERM
        // strand для выполнения запросов к API
        auto api_strand = net::make_strand(ioc);

        // 4. Создаём обработчик HTTP-запросов и связываем его с моделью игры
        //http_handler::RequestHandler handler{wwwRootPath, game};
        // Создаём обработчик запросов в куче, управляемый shared_ptr
        auto handler = std::make_shared<http_handler::RequestHandler>(wwwRootPath, api_strand, game);

        // Оборачиваем его в логирующий декоратор
        server_logging::LoggingRequestHandler<http_handler::RequestHandler> loggingHandler{*handler};
        /*server_logging::LoggingRequestHandler logging_handler{
            [handler](auto&& endpoint, auto&& req, auto&& send) {
                // Обрабатываем запрос
                (*handler)(std::forward<decltype(endpoint)>(endpoint),
                    std::forward<decltype(req)>(req),
                    std::forward<decltype(send)>(send));
                    }};*/
        // 5. Запустить обработчик HTTP-запросов, делегируя их обработчику запросов

        const auto address = net::ip::make_address("0.0.0.0");
        constexpr net::ip::port_type port = 8080;

        http_server::ServeHttp(ioc, {address, port}, [&loggingHandler](auto&& endp, auto&& req, auto&& send) {
          loggingHandler(std::forward<decltype(endp)>(endp), std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
        });
        //http_server::ServeHttp(ioc, {address, port}, logging_handler);


        // Эта надпись сообщает тестам о том, что сервер запущен и готов обрабатывать запросы
        //std::cout << "Server has started..."sv << std::endl;

        // 6. Запускаем обработку асинхронных операций
        LogStartServer(address.to_string(), port);

        RunWorkers(std::max(1u, num_threads), [&ioc] {
            ioc.run();
        });

        LogStopServer(EXIT_SUCCESS);
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        //std::cerr << ex.what() << std::endl;
        LogFaultStopServer(EXIT_FAILURE, ex.what());
        return EXIT_FAILURE;

    }
}
