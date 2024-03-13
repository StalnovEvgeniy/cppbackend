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

    Player(const std::string& name)
        : name_(name)
        , id_(Id{Player::idCount++}) { }

    //Token GetToken() const;
    const Id &GetId() const noexcept;

    std::string GetName() const;

    void SetGameSession(std::shared_ptr<GameSession> session);
    std::shared_ptr<GameSession> GetGameSession();

    void SetDog(std::shared_ptr<Dog> dog);

private:
    //Token token_;
    std::string name_;
    std::shared_ptr<GameSession> session_;
    std::shared_ptr<Dog> dog_;
    Id id_;
    static int idCount;
};

}

#endif // PLAYER_H
