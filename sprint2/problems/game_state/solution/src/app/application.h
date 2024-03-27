#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once

//#include <vector>
#include "model/game.h"
#include "model/map.h"
#include "player.h"
#include "util/tagged.h"

#include "app/players.h"
#include "playertokens.h"

namespace app {

using namespace model;

//using PairPlayerToken = std::pair<Player&, Token>;
using PairPlayerToken = std::pair<std::shared_ptr<Player>, Token>;

class Application
{
public:
    Application(Game& game);

    const Maps &ListMap() const noexcept;
    const Map *FindMap(const Map::Id &id) const noexcept;
    //Token
    PairPlayerToken JoinGame(const Map::Id &mapId, std::string &userName);

    //Players &ListPlayersUse();

    //std::vector<Player> &GetPlayers();
    std::shared_ptr<Player> FindByToken(Token &token);
    const Token *FindToken(const Player &player) const noexcept;

    const PlayerTokens::TokenToPlayer& GetTokenToPlayer() const;

    void SetTimeDelta(const double timeDelta);

private:
    PlayerTokens playerTokens_;


    model::Game& game_;

    double timeDelta_;


};

}

#endif // APPLICATION_H
