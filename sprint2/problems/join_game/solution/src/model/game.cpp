#include "game.h"

#include <string>
#include <stdexcept>

#include "map.h"

namespace model {

using namespace std::literals;

void Game::AddMap(Map map) {
    const size_t index = maps_.size();
    if (auto [it, inserted] = map_id_to_index_.emplace(map.GetId(), index); !inserted) {
        throw std::invalid_argument("Map with id "s + *map.GetId() + " already exists"s);
    } else {
        try {
            maps_.emplace_back(std::move(map));
        } catch (...) {
            map_id_to_index_.erase(it);
            throw;
        }
    }
}

const Game::Maps &Game::GetMaps() const noexcept {
    return maps_;
}

const Map *Game::FindMap(const Map::Id &id) const noexcept {
    if (auto it = map_id_to_index_.find(id); it != map_id_to_index_.end()) {
        return &maps_.at(it->second);
    }
    return nullptr;
}

void Game::AddGameSession(GameSession session)
{
    //sessions_.emplace_back(std::move(session));
    const size_t index = sessions_.size();
    if (auto [it, inserted] = map_id_to_session_index_.emplace(session.GetMap()->GetId(), index); !inserted) {
        throw std::invalid_argument("Game session with map id "s + *(session.GetMap()->GetId()) + " already exists"s);
    } else {
        try {
            sessions_.emplace_back(std::move(session));
            //sessions_.push_back(session);
        } catch (...) {
            map_id_to_session_index_.erase(it);
            throw;
        }
    }
}

const GameSession *Game::FindGameSessionBy(const Map::Id &id) const noexcept
{
    if (auto it = map_id_to_index_.find(id); it != map_id_to_index_.end()) {
        return &sessions_.at(it->second);
    }
    return nullptr;
}


}  // namespace model