#include "players.h"

#include <vector>

using namespace app;
using namespace model;

Players::Players()
{

}

Player &Players::AddPlayer(std::shared_ptr<Dog> dog, std::shared_ptr<GameSession> session)
{
    return playerList_.emplace_back(Player(dog, session));

}

std::vector<Player> &Players::GetPlayers()
{
    return playerList_;
}
