#include "playertokens.h"
#include <sstream>

namespace app {

const Token PlayerTokens::CreateToken(){
    std::stringstream strStream;
    strStream << std::setw(numOfDigits_) << std::setfill('0') << std::hex << generator1_();
    strStream << std::setw(numOfDigits_) << std::setfill('0') << std::hex << generator2_();

    return Token(strStream.str());
}

std::shared_ptr<Player> PlayerTokens::FindPlayerByToken(const Token token) {
    if(tokenToPlayer_.contains(token)){
        return tokenToPlayer_.at(token);
    }
    return std::shared_ptr<Player> ();
}

Token PlayerTokens::AddPlayer(std::shared_ptr<Player> player)
{
    Token token{CreateToken()};

    tokenToPlayer_[token] = player;

    return token;

}

const Token *PlayerTokens::FindToken(const Player &player) const noexcept
{
    auto id = player.GetId();
    for (auto it = tokenToPlayer_.cbegin(); it != tokenToPlayer_.cend(); ++it) {
        auto findPlayer = it->second;
        if (findPlayer->GetGameSession() == player.GetGameSession())
        if (id == (it->second)->GetId())
            return &it->first;
    }

    return nullptr;
}

const PlayerTokens::TokenToPlayer &PlayerTokens::GetTokenToPlayer() const noexcept
{
    return tokenToPlayer_;
}

std::vector<std::shared_ptr<Player> > &PlayerTokens::GetPlayers()
{
    return players_;
}


}

