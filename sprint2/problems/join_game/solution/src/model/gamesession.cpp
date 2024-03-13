#include "gamesession.h"

namespace model {



GameSession::GameSession(std::shared_ptr<Map> map)
    : map_(map)
    , id_(*(map->GetId()))
{

}

const GameSession::Id &GameSession::GetId() const noexcept
{
    return id_;
}

void GameSession::AddDog(Dog dog)
{
    dogs_.push_back(std::move(dog));
}

const std::vector<Dog> &GameSession::GetDogs() const
{
    return dogs_;
}

std::shared_ptr<Map> GameSession::GetMap()
{
    return map_;
}

}
