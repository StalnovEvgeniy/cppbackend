#include "audio.h"
#include <boost/asio.hpp>
#include <iostream>

namespace net = boost::asio;
using net::ip::udp;

using namespace std::literals;

struct initFormatRadio_t {
    initFormatRadio_t(ma_format format, int channels):
        format(format),
        channels(channels){};
    ma_format format;
   int channels;
};
static const struct initFormatRadio_t initFormat(ma_format_u8, 1);
const size_t max_buffer_size = 65000;

// Запуск сервера UDP
void StartServer(uint16_t port){
    Player player(initFormat.format, initFormat.channels);

    try {
        boost::asio::io_context io_context;

        udp::socket socket(io_context, udp::endpoint(udp::v4(), port));

        Recorder::RecordingResult rec_result = {};
        // Запускаем сервер в цикле, чтобы можно было работать со многими клиентами
        for (;;) {
            // Создаём буфер достаточного размера, чтобы вместить датаграмму.
            std::array<char, max_buffer_size> recv_buf;
            udp::endpoint remote_endpoint;

            // Получаем не только данные, но и endpoint клиента
            auto size = socket.receive_from(boost::asio::buffer(rec_result.data), remote_endpoint);

            std::cout << "Client said "sv << std::string_view(rec_result.data.data(), size) << std::endl;

            auto frameSize = player.GetFrameSize();
            rec_result.frames = rec_result.data.size() / frameSize;
            player.PlayBuffer(rec_result.data.data(), rec_result.frames, 1.5s);
            std::cout << "Playing done ..." << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }



}

//Запуск клиента UDP
void StartClient(uint16_t port, const char * strIP){
    Recorder recorder(initFormat.format, initFormat.channels);
    std::string str;
    std::cout << "Press Enter to record message..." << std::endl;
    std::getline(std::cin, str);

    auto rec_result = recorder.Record(max_buffer_size, 1.5s);
    std::cout << "Recording done ..." << std::endl;
    auto frameSize = recorder.GetFrameSize();
    std::cout << "Count record frame = "sv << frameSize<< std::endl;
    std::cout << "Size record frame = "sv << rec_result.data.size() << std::endl;

    //rec_result.data;

    try {
        net::io_context io_context;

        // Перед отправкой данных нужно открыть сокет.
        // При открытии указываем протокол (IPv4 или IPv6) вместо endpoint.
        udp::socket socket(io_context, udp::v4());

        boost::system::error_code ec;
        auto endpoint = udp::endpoint(net::ip::make_address(strIP, ec), port);
        socket.send_to(net::buffer(rec_result.data), endpoint);

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}

int main(int argc, char** argv) {
    static const int port = 3333;


    if (argc != 2) {
        std::cout << "Usage: "sv << argv[0] << " <server IP>"sv << std::endl;
        return 1;
    }

    while (true) {
        std::string str;

        std::cout << "Select mode : server/client" << std::endl;
        std::getline(std::cin, str);

        if (str == "server"){
            StartServer(port);
        } else if (str == "client") {
            StartClient(port, argv[1]);
        } else {
            break;
        }
    }

    return 0;
}
