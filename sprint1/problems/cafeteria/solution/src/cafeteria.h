#pragma once
#ifdef _WIN32
#include <sdkddkver.h>
#endif

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <memory>

#include <boost/asio/bind_executor.hpp>
#include <chrono>

#include <thread>
#include <atomic>

#include "hotdog.h"
#include "result.h"

#include <syncstream>
#include <iostream>

namespace net = boost::asio;
namespace sys = boost::system;

using namespace std::chrono;
using namespace std::literals;

class ThreadChecker {
public:
    explicit ThreadChecker(std::atomic_int& counter)
        : counter_{counter} {
    }

    ThreadChecker(const ThreadChecker&) = delete;
    ThreadChecker& operator=(const ThreadChecker&) = delete;

    ~ThreadChecker() {
        // assert выстрелит, если между вызовом конструктора и деструктора
        // значение expected_counter_ изменится
        assert(expected_counter_ == counter_);
    }

private:
    std::atomic_int& counter_;
    int expected_counter_ = ++counter_;
};

class Logger {
public:
    explicit Logger(std::string id)
        : id_(std::move(id)) {
    }

    void LogMessage(std::string_view message) const {
        std::osyncstream os{std::cout};
        os << id_ << "> ["sv << duration<double>(steady_clock::now() - start_time_).count()
           << "s] "sv << message << std::endl;
    }

private:
    std::string id_;
    steady_clock::time_point start_time_{steady_clock::now()};
};

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
        handler_{std::move(handler)} {

    }

    // Запускает асинхронное выполнение заказа
    void Execute() {
        BakingBread();
        FrytingSausage();

    }

private:
    void BakingBread() {
        bread_->StartBake(*cooker_, [self = shared_from_this()]() {
            self->timerBread_.expires_from_now(Milliseconds{1000});
            self->timerBread_.async_wait(
                        net::bind_executor(self->strand_, [self = std::move(self)](sys::error_code ec) {
                self->OnBaked(ec);
            }));
        });
    }
    void OnBaked(sys::error_code ec) {
        ThreadChecker checker{counter_};
        bread_->StopBaking();
        if (ec) {
            logger_.LogMessage("Baked error : "s + ec.what());
        } else {

            bread_baked_ = true;
        }

        CheckReadiness();
    }

    void FrytingSausage() {
        sausage_->StartFry(*cooker_, [self = shared_from_this()]() {
            self->timerSausage_.expires_from_now(Milliseconds{1500});
            self->timerSausage_.async_wait(
                        net::bind_executor(self->strand_, [self = std::move(self)](sys::error_code ec) {
                self->OnSausageFried(ec);
            }));
        });
    }

    void OnSausageFried(sys::error_code ec) {
        ThreadChecker checker{counter_};
        sausage_->StopFry();
        if (ec) {
            logger_.LogMessage("Sausage Fried error : "s + ec.what());
        } else {

            sausage_baked_ = true;
        }

        CheckReadiness();
    }

    void CheckReadiness() {
        if (delivered_) {
             // Выходим, если заказ уже доставлен либо клиента уведомили об ошибке
             return;
         }

        if (IsReadyToBaked())
        // В случае ошибки уведомляем клиента о невозможности выполнить заказ
        return Deliver();
    }

    //[[nodiscard]]
    bool IsReadyToBaked() const {
        return bread_baked_ && bread_->IsCooked() &&
                sausage_baked_ && sausage_->IsCooked();
    }

    void Deliver() {
        delivered_ = true;
        handler_(Result{HotDog{id_, sausage_, bread_}});
    }

    net::io_context& io_;
    net::strand<net::io_context::executor_type> strand_{net::make_strand(io_)};
    int id_{0};

    std::shared_ptr<Sausage> sausage_;
    std::shared_ptr<Bread> bread_;
    std::shared_ptr<GasCooker> cooker_;
    HotDogHandler handler_;



    Logger logger_{std::to_string(id_)};

    bool sausage_baked_ = false;
    bool bread_baked_ = false;
    bool delivered_ = false;

    std::atomic_int counter_{0};

    net::steady_timer timerBread_{io_, Milliseconds{1000}};
    net::steady_timer timerSausage_{io_, Milliseconds{1500}};

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
        // TODO: Реализуйте метод самостоятельно
        // При необходимости реализуйте дополнительные классы


        const int order_id = ++next_order_id_;
         std::make_shared<Order>(io_, order_id,
                                 store_.GetSausage(), store_.GetBread(),
                                 gas_cooker_, std::move(handler)
                                 )->Execute();
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
    //int
    std::atomic_int next_order_id_{0};

};
