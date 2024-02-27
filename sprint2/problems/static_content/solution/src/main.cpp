#include "sdk.h"
//
// boost.beast будет использовать std::string_view вместо boost::string_view
//#define BOOST_BEAST_USE_STD_STRING_VIEW

#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <iostream>
#include <thread>

#include "json_loader.h"
#include "request_handler.h"


namespace beast = boost::beast;
namespace http = beast::http;
using namespace std::literals;
namespace net = boost::asio;
namespace fs = std::filesystem;

namespace {

// Запускает функцию fun на cnt потоках, включая текущий
template <typename Fun>
void RunWorkers(unsigned cntThread, const Fun& fun) {
    cntThread = std::max(1u, cntThread);
    std::vector<std::jthread> workers;
    workers.reserve(cntThread - 1);
    // Запускаем cnt-1 рабочих потоков, выполняющих функцию fun
    while (--cntThread) {
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
        //std::cout << "argsComandLine.configMap..."sv << argsComandLine.configMap << std::endl;
        model::Game game = json_loader::LoadGame(argsComandLine.configMap);

        //std::cout << "argsComandLine.wwwRootPath..."sv << argsComandLine.wwwRootPath << std::endl;
        fs::path wwwRootPath{argsComandLine.wwwRootPath};
        //std::cout << json::serialize(json_parsing::GetJsonData) << std::endl; // {"name": "Harry Potter"}

        // 2. Инициализируем io_context
        const unsigned num_threads = std::thread::hardware_concurrency();
        net::io_context ioc(num_threads);

        // 3. Добавляем асинхронный обработчик сигналов SIGINT и SIGTERM

        // 4. Создаём обработчик HTTP-запросов и связываем его с моделью игры
        http_handler::RequestHandler handler{game, wwwRootPath};

        // 5. Запустить обработчик HTTP-запросов, делегируя их обработчику запросов

        const auto address = net::ip::make_address("0.0.0.0");
        constexpr net::ip::port_type port = 8080;
        http_server::ServeHttp(ioc, {address, port}, [&handler](auto&& req, auto&& send) {
            handler(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
        });


        // Эта надпись сообщает тестам о том, что сервер запущен и готов обрабатывать запросы
        std::cout << "Server has started..."sv << std::endl;

        // 6. Запускаем обработку асинхронных операций
        RunWorkers(std::max(1u, num_threads), [&ioc] {
            ioc.run();
        });
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
