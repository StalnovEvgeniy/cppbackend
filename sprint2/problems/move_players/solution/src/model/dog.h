#ifndef DOG_H
#define DOG_H

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <map>

#include "util/tagged.h"
#include "types.h"

namespace model {

using namespace std::literals;

class Dog
{
public:
    using Id = util::Tagged<int, Dog>;

    Dog(std::string name);

    const Id &GetId() const noexcept;
    const std::string &GetName() const;

    const PosDouble &GetPosition() const;
    void SetPosition(const PosDouble &position);

    void SetDogSpeed(const float dogSpeed);

    const SpeedDouble &GetSpeed() const;
    void SetSpeed(const SpeedDouble& speed);

    const std::string &GetDirection() const;
    void SetDirection(const std::string str);

private:
    Id id_;
    std::string name_;
    Direction direction_;
    PosDouble position_;
    SpeedDouble speed_;

    float dogSpeed_;
    static int idCount;

};

}

#endif // DOG_H
