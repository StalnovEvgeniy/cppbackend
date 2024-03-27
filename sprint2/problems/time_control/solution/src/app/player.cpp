
#include "player.h"

namespace app {

int Player::idCount = 0;

Player::Player(const std::string &name)
    : name_(name)
    , id_(Id{Player::idCount++}) { }

Player::Player(std::shared_ptr<Dog> dog, std::shared_ptr<GameSession> session)
    : id_{Player::Id{idCount++}}
    , dog_{dog}
    , session_{session}
{

}

const Player::Id &Player::GetId() const noexcept
{
    return id_;
}

void Player::SetName(const std::string name)
{
    if (name.empty())
        throw "no name !!!";
    name_ = name;
}

const std::string &Player::GetName() const
{
    return name_;
}

void Player::SetGameSession(std::shared_ptr<GameSession> session)
{
    if (session == std::shared_ptr<GameSession>())
        throw "no GameSession !!!";
    session_ = std::move(session);
}

const std::shared_ptr<GameSession> Player::GetGameSession() const
{
    return session_;
}

void Player::SetDog(std::shared_ptr<Dog> dog)
{
    if (dog == std::shared_ptr<Dog>())
        throw "no DOG !!!";

    dog_ = std::move(dog);
}

const std::shared_ptr<Dog> Player::GetDog() const
{
    return dog_;
}



}
