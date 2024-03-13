#include "playertokens.h"
#include <sstream>

namespace app {

Token PlayerTokens::CreateToken() {
    std::stringstream strStream;
    strStream << std::setw(numOfDigits_) << std::setfill('0') << std::hex << generator1_();
    strStream << std::setw(numOfDigits_) << std::setfill('0') << std::hex << generator2_();

    return Token(strStream.str());
}

std::shared_ptr<Player> PlayerTokens::FindPlayerByToken(const Token token) {
    if(!tokenToPlayer_.contains(token)){
        return std::shared_ptr<Player>();
    }
    return tokenToPlayer_.at(token);
}

Token PlayerTokens::AddPlayer(std::shared_ptr<Player> player)
{
    Token token{CreateToken()};
    players_.emplace_back(player);
    tokenToPlayer_[token] = player;
    //tokenToPlayer_[token] = std::make_shared(std::move(player));
    return token;

}

PlayerTokens::TokenToPlayer &PlayerTokens::GetTokenToPlayer()
{
    return tokenToPlayer_;
}

std::vector<std::shared_ptr<Player> > &PlayerTokens::GetPlayers()
{
    return players_;
}

}

