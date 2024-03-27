#include "gamesession.h"

#include <memory>

namespace model {

GameSession::GameSession(const Map::Id id)
    : mapId_(id)
{

}

const Map::Id &GameSession::GetMapId() const noexcept
{
    return mapId_;
}

std::shared_ptr<Dog> GameSession::AddDog(const PosDouble position, const std::string &name)
{
    auto dog = std::make_shared<Dog>(name);
    dog->SetPosition(position);
    dogs_.emplace_back(dog);
    return dog;
}


std::vector<std::shared_ptr<Dog>> GameSession::GetDogs() const
{
    return dogs_;
}


}
