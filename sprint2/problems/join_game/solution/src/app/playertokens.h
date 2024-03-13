#ifndef PLAYERTOKENS_H
#define PLAYERTOKENS_H

#pragma once

#include <random>
#include "util/tagged.h"
#include "app/player.h"
#include <iomanip>
#include <cassert>
#include <memory>

#include <unordered_map>

namespace app {

namespace detail {
struct TokenTag {};
}  // namespace detail


using Token = util::Tagged<std::string, detail::TokenTag>;
using TokenHasher = util::TaggedHasher<Token>;


class PlayerTokens {
    using TokenToPlayer = std::unordered_map<Token, std::shared_ptr<Player>, TokenHasher >;
public:
    Token CreateToken();

    std::shared_ptr<Player> FindPlayerByToken(const Token token);

    Token AddPlayer(std::shared_ptr<Player> player);


    PlayerTokens::TokenToPlayer &GetTokenToPlayer();
    std::vector<std::shared_ptr<Player> > &GetPlayers();
private:
    std::random_device random_device_;
    std::mt19937_64 generator1_{[this] {
        std::uniform_int_distribution<std::mt19937_64::result_type> dist;
        return dist(random_device_);
    }()};
    std::mt19937_64 generator2_{[this] {
        std::uniform_int_distribution<std::mt19937_64::result_type> dist;
        return dist(random_device_);
    }()};
    // Чтобы сгенерировать токен, получите из generator1_ и generator2_
    // два 64-разрядных числа и, переведя их в hex-строки, склейте в одну.
    // Вы можете поэкспериментировать с алгоритмом генерирования токенов,
    // чтобы сделать их подбор ещё более затруднительным

    //std::unordered_map< Token, std::weak_ptr<app::Player>, TokenHasher > tokenToPalyer_;

    std::vector<std::shared_ptr<Player>> players_;
    TokenToPlayer tokenToPlayer_;
    const size_t numOfDigits_ = 16;
    //using PlayerIdToIndex = std::unordered_map<Token, size_t, TokenHasher>;
};

}

#endif // PLAYERTOKENS_H
