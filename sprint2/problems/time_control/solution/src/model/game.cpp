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

const Maps &Game::GetMaps() const noexcept {
    return maps_;
}

const Map* Game::FindMap(const Map::Id &id) const noexcept {
    if (auto it = map_id_to_index_.find(id); it != map_id_to_index_.end()) {
        return &maps_.at(it->second);
    }
    return nullptr;
}

void Game::AddGameSession(std::shared_ptr<GameSession> session)
{
    const size_t index = sessions_.size();
    if (auto [it, inserted] = map_id_to_session_index_.emplace(session->GetMapId(), index); !inserted) {
        throw std::invalid_argument("Game session with map id "s + *(session->GetMapId()) + " already exists"s);
    } else {
        try {
            sessions_.emplace_back(std::move(session));
        } catch (...) {
            map_id_to_session_index_.erase(it);
            throw;
        }
    }
}

std::shared_ptr<GameSession> Game::FindGameSessionBy(const Map::Id &id)
{
    const Map* map = FindMap(id);
    if (!map) {
        throw std::invalid_argument("Map"s + *id + "id not exist"s);
    }

    for (auto& it : sessions_) {
        if (it->GetMapId() == id) {
            return it;
        }
    }

    auto session = std::make_shared<GameSession>(id);
    AddGameSession(session);
    return session;
}

void Game::SetDefaultDogSpeed(const double defaultDogSpeed)
{
    defaultDogSpeed_ = defaultDogSpeed;
}

const double &Game::GetDefaultDogSpeed() const
{
    return defaultDogSpeed_;
}

void Game::UpdateTime(const double& timeDelta)
{
    for (auto& it : sessions_) {
        MovingDog(it->GetDogs(), it->GetMapId(), timeDelta);
    }

}

void Game::MovingDog(std::vector<std::shared_ptr<Dog> > dogs, const Map::Id &mapId, const double& timeDelta)
{
    const Map* map = FindMap(mapId);

    for (auto it = dogs.begin(); it != dogs.end(); ++it) {
        auto dog = *it;
        if (dog == nullptr)
            continue;

        auto dir = dog->GetDirection();

        auto pos = dog->GetPosition();
        auto speed = dog->GetSpeed();

        auto posNew = pos + (speed * timeDelta);

        auto minPos{pos};
        auto maxPos{pos};
        bool checkPos = false;
        for( auto road = map->GetRoads().cbegin(); road != map->GetRoads().cend(); ++road) {
            minPos = road->GetMinPos();
            maxPos = road->GetMaxPos();
            if(road->CheckingPos(pos)) {
                checkPos = true;
                continue;
            }
        }

      if (!checkPos)
          continue;

      if (posNew.y < minPos.y) {
          posNew.y = minPos.y;
          dog->SetSpeed({0.0, 0.0});
      } else if (posNew.y > maxPos.y) {
          posNew.y = maxPos.y;
          dog->SetSpeed({0.0, 0.0});
      } else if (posNew.x < minPos.x) {
          posNew.x = minPos.x;
          dog->SetSpeed({0.0, 0.0});
      } else if (posNew.x > maxPos.x) {
          posNew.x = maxPos.x;
          dog->SetSpeed({0.0, 0.0});
      }

      dog->SetPosition(posNew);

    }
}


}

  // namespace model
