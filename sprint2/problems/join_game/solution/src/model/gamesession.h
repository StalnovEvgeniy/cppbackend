#ifndef GAMESESSION_H
#define GAMESESSION_H

#pragma once

#include <vector>
#include <memory>

#include "model/map.h"
#include "model/dog.h"

namespace model {

class GameSession
{
public:
    using Id = util::Tagged<std::string, GameSession>;
    using Dogs = std::vector<Dog>;

    GameSession(std::shared_ptr<Map> map);
    ~GameSession() = default;

    const Id &GetId() const noexcept;

    void AddDog(Dog dog);
    const std::vector<Dog> &GetDogs() const;

    std::shared_ptr<Map> GetMap();

   private:
    std::shared_ptr<Map> map_;
    Dogs dogs_;
    Id id_;
};

}

#endif // GAMESESSION_H
