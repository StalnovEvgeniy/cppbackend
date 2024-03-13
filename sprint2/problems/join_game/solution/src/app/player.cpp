
#include "player.h"

namespace app {

int Player::idCount = 0;

/*Token Player::GetToken() const
{
    return token_;
}*/

const Player::Id &Player::GetId() const noexcept
{
    return id_;
}

std::string Player::GetName() const
{
    return name_;
}

void Player::SetGameSession(std::shared_ptr<GameSession> session)
{
    session_ = session;
}

std::shared_ptr<GameSession> Player::GetGameSession()
{
    return session_;
}

void Player::SetDog(std::shared_ptr<Dog> dog)
{
    dog_ = dog;
}



}
