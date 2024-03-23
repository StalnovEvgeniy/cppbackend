#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include <memory>

#include "model/gamesession.h"
#include "model/dog.h"

namespace app {

using namespace model;

class Player {

public:
    using Id = util::Tagged<int, Player>;
    using PlayerHasher = util::TaggedHasher<Player>;


    Player(const std::string& name);
    Player(std::shared_ptr<model::Dog> dog, std::shared_ptr<model::GameSession> session);

    const Id &GetId() const noexcept;

    void SetName(const std::string name);
    const std::string &GetName() const;

    void SetGameSession(std::shared_ptr<GameSession> session);
    const std::shared_ptr<GameSession> GetGameSession() const;

    void SetDog(std::shared_ptr<Dog> dog);
    const std::shared_ptr<Dog> GetDog() const;

private:
    std::string name_{};
    Id id_;
    std::shared_ptr<Dog> dog_;
    std::shared_ptr<GameSession> session_;

    static int idCount;
};

}

#endif // PLAYER_H
