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
#include <iostream>

namespace net = boost::asio;
using Timer = net::steady_timer;

using osync = std::osyncstream;


// Функция-обработчик операции приготовления хот-дога
using HotDogHandler = std::function<void(Result<HotDog> hot_dog)>;

class Order: public std::enable_shared_from_this <Order> {
public:
    Order(net::io_context& io, int id,
          std::shared_ptr<Sausage> sausage, std::shared_ptr<Bread> bread,
          std::shared_ptr<GasCooker> cooker, HotDogHandler handler):
        io_(io),
        id_(id),
        sausage_{std::move(sausage)},
        bread_{std::move(bread)},
        cooker_{std::move(cooker)},
        handler_{std::move(handler)},
        hotDog_{id_, sausage_, bread_}{

    }

    // Запускает асинхронное выполнение заказа
    void Execute() {
        osync{std::cout} << "Finish #" << id_ << std::endl;

        /*auto strand = net::make_strand(io_);
        net::dispatch(io_, [&strand, self = shared_from_this()] {
            net:post(strand, [&self] {
                self->BakingBread();
                self->FrytingSausage();
            });
            net::post(strand, [&self] {
                self->Deliver();
            });
        });*/
    }

private:
    void BakingBread() {
        auto strand = net::make_strand(io_);
        boost::asio::steady_timer timer_(io_);

        // Set an expiry time relative to now.
        timer_.expires_after(Milliseconds(1000));
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        bread_->StartBake(*cooker_, [&timer_, self = shared_from_this()]() {

            // Wait for the timer to expire.
            timer_.wait();
        });
        OnBaked();
    }
    void OnBaked(){
        bread_->StopBaking();

        // Timer expired.
        sausage_frited_ = true;

         //std::lock_guard lk{mutex_};
        //ThreadChecker checker{counter_};

        //hotDog = result_.GetValue();
    }

    void FrytingSausage() {


        //std::this_thread::sleep_for(std::chrono::seconds(1));
        sausage_->StartFry(*cooker_, [self = shared_from_this()]() {
            //boost::asio::steady_timer timer_(self->io_);

            // Set an expiry time relative to now.
            //timer_.expires_after(Milliseconds(1000));

            // Wait for the timer to expire.
            //timer_.wait();
            self->OnSausageFrited();
        });
    }

    void OnSausageFrited() {
        //std::lock_guard lk{mutex_};
        //ThreadChecker checker{counter_};
        sausage_->StopFry();

        sausage_frited_ = true;


        CheckReadiness();
    }

    void CheckReadiness() {
        if (delivered_) {
             // Выходим, если заказ уже доставлен либо клиента уведомили об ошибке
             return;
         }

        // В случае ошибки уведомляем клиента о невозможности выполнить заказ
        return Deliver();
    }


    void Deliver() {
        // Защита заказа от повторной доставки
        //delivered_ = true;
        // Доставляем   хот дог в случае успеха либо nullptr, если возникла ошибка
        //handler_(result_);
    }

    net::io_context& io_;
    int id_{0};

    //Store&  store_;
    std::shared_ptr<Sausage> sausage_;
    std::shared_ptr<Bread> bread_;
    std::shared_ptr<GasCooker> cooker_;
    HotDogHandler handler_;

    HotDog hotDog_;

    Result<HotDog> result_{hotDog_};


    bool sausage_frited_ = false;
    bool delivered_ = false; // Заказ доставлен?

    //boost::asio::steady_timer baking_timer_(io_);
    //Timer frying_timer_{io_, 1s};
    //Timer baking_timer_{io_, 1.5s};

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
        std::shared_ptr<Bread> bread = store_.GetBread();
        std::shared_ptr<Sausage> sausage = store_.GetSausage();

        // TODO: Реализуйте метод самостоятельно
        // При необходимости реализуйте дополнительные классы


        const int order_id = ++next_order_id_;
        osync{std::cout} << "Start #" << order_id << std::endl;
        net::dispatch(strand, [&]{
            std::make_shared<Order>(io_, order_id,
                                      std::move(sausage), std::move(bread),
                                      std::move(gas_cooker_), std::move(handler))->Execute();
            osync{std::cout} << "End #" << order_id << std::endl;
        });
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
    int next_order_id_ = 0;
    //HotDog hotDog;

};
