#ifndef GAMESESSION_H
#define GAMESESSION_H

#pragma once

#include <vector>
#include <memory>

#include "model/map.h"
#include "model/dog.h"

namespace model {

using Dogs = std::vector<std::shared_ptr<Dog> >;

class GameSession
{
public:
    GameSession(const Map::Id id);
    ~GameSession() = default;

    const Map::Id &GetMapId() const noexcept;

    std::shared_ptr<Dog> AddDog(const PosDouble position, std::string &name);
    const std::vector<std::shared_ptr<Dog> > &GetDogs() const;

private:

    Dogs dogs_;
    const Map::Id mapId_;
};

}

#endif // GAMESESSION_H
