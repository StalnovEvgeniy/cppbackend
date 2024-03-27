#ifndef PLAYERS_H
#define PLAYERS_H

#pragma once

#include <string>
#include <vector>

#include "model/dog.h"
#include "model/map.h"

#include "player.h"



namespace app {


class Players
{

public:
    Players();

    Player &AddPlayer(std::shared_ptr<Dog> dog, std::shared_ptr<GameSession> session);

    std::vector<Player > &GetPlayers();

private:
    std::vector<Player > playerList_;


};

}

#endif // PLAYERS_H
