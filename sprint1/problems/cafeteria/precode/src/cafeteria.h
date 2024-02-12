#pragma once
#ifdef _WIN32
#include <sdkddkver.h>
#endif

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <memory>

#include "hotdog.h"
#include "result.h"

#include <syncstream>

namespace net = boost::asio;

// Функция-обработчик операции приготовления хот-дога
using HotDogHandler = std::function<void(Result<HotDog> hot_dog)>;

class MyOrder: public std::enable_shared_from_this <MyOrder> {
    MyOrder(net::io_context& io, int id, std::shared_ptr<GasCooker> cooker):
        io_(io),
        id_(id),
        cooker_{std::move(cooker)} {
    }

    // Запускает асинхронное выполнение заказа
    void Execute() {
        BakingBread();

        FryingSausage();

    }

private:
    void BakingBread() {
        //std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    void FryingSausage() {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
         /*sausage->StartFry(*cooker,  [] {  // Эта функция будет вызвана асинхронно в контексте io2
             osync(std::cout) << "StartFry"sv;
        net::dispatch(io_, [&strand] {  // (3)
                osync(std::cout) << "StartFry"sv;
            });
            osync(std::cout) << 'B';
        });*/

    }

    net::io_context& io_;
    int id_;
private:
    std::shared_ptr<GasCooker> cooker_;
    //Timer roast_timer_{io_, 1s};
    //Timer marinade_timer_{io_, 2s};

};

// Класс "Кафетерий". Готовит хот-доги
class Cafeteria {
public:
    explicit Cafeteria(net::io_context& io)
        : io_{io} {
    }

    // Асинхронно готовит хот-дог и вызывает handler, как только хот-дог будет готов.
    // Этот метод может быть вызван из произвольного потока
    void OrderHotDog(HotDogHandler handler) {
        auto strand = net::make_strand(io_);
        // TODO: Реализуйте метод самостоятельно
        // При необходимости реализуйте дополнительные классы
        using osync = std::osyncstream;
        //std::cout << "RUN OrderHotDog :" << std::endl;

        auto sausage = store_.GetSausage();
        auto bread = store_.GetBread();

        net::post(io_, [strand, sausage ] {
            //if (!sausage->IsCooked())
            {

            }

        });
        if (!sausage->IsCooked()) {
            sausage->StartFry(*gas_cooker_,  [] {  // Эта функция будет вызвана асинхронно в контексте io2
                osync(std::cout) << 'A';
            });
            sausage->StopFry();

        }
        if (!bread->IsCooked()) {
            bread->StartBake(*gas_cooker_,  [] {  // Эта функция будет вызвана асинхронно в контексте io2
                osync(std::cout) << 'B';
            });
            bread->StopBaking();

        }
            //std::cout << " SRUN OrderHotDog : " << std::endl;

    }

private:
    net::io_context& io_;
    // Используется для создания ингредиентов хот-дога
    Store store_;
    // Газовая плита. По условию задачи в кафетерии есть только одна газовая плита на 8 горелок
    // Используйте её для приготовления ингредиентов хот-дога.
    // Плита создаётся с помощью make_shared, так как GasCooker унаследован от
    // enable_shared_from_this.
    std::shared_ptr<GasCooker> gas_cooker_ = std::make_shared<GasCooker>(io_);
    HotDogHandler& handler_;
};
