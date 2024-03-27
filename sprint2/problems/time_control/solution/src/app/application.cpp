#include "application.h"
#include "app/player.h"

#include "model/randompointdog.h"

using namespace app;
using namespace model;



Application::Application(Game &game)
    :game_{game}
{

}

const Maps &Application::ListMap() const noexcept
{
    return game_.GetMaps();
}

const Map* Application::FindMap(const Map::Id &id) const noexcept
{
    return game_.FindMap(id);
}

PairPlayerToken Application::JoinGame(const Map::Id &mapId, const std::string &userName)
{
    std::shared_ptr<GameSession> session = game_.FindGameSessionBy(mapId);
    const Map* map = game_.FindMap(mapId);

    auto randPoint = RandomPointDog(map);
    //auto spawn_point = randPoint();
    auto spawn_point = PosDouble{0., 0.};

    std::shared_ptr<Dog> dog = session->AddDog(spawn_point, userName);

    if (session == nullptr)
        throw std::invalid_argument("GameSession no Find!!"s);

    auto player = std::make_shared<Player> (dog, session);

    Token token = playerTokens_.AddPlayer(player);

    PairPlayerToken pair{player, token};

    return pair;

}

std::shared_ptr<Player> Application::FindByToken(Token &token)
{
    return playerTokens_.FindPlayerByToken(token);
}

const Token *Application::FindToken(const Player &player) const noexcept
{
    return playerTokens_.FindToken(player);
}

const PlayerTokens::TokenToPlayer &Application::GetTokenToPlayer() const
{
    return playerTokens_.GetTokenToPlayer();
}

void Application::SetTimeDelta(const double timeDelta)
{
    timeDelta_ = timeDelta;
    UpdateTime();
}

void Application::UpdateTime()
{
    game_.UpdateTime(timeDelta_);
}
