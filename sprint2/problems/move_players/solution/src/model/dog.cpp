#include "dog.h"

namespace model {

int Dog::idCount = 0;



Dog::Dog(std::string name)
    : id_(Id{Dog::idCount++})
    , name_{name}
    , direction_(Direction::NORTH)
    , position_(0.f, 0.f)
    , speed_(0.f, 0.f)

{ }

const Dog::Id &Dog::GetId() const noexcept
{
    return id_;
}

const std::string &Dog::GetName() const
{
    return name_;
}

const PosDouble &Dog::GetPosition() const
{
    return position_;
}

void Dog::SetPosition(const PosDouble &position)
{
    position_.x = position.x;
    position_.y = position.y;
}

void Dog::SetDefaultDogSpeed(const float speed)
{
    defaultDogSpeed = speed;
}

const SpeedDouble &Dog::GetSpeed() const
{
    return speed_;
}

void Dog::SetSpeed(const SpeedDouble &speed)
{
    speed_.x = speed.x;
    speed_.y = speed.y;
}

const std::string &Dog::GetDirection() const
{
    return DirToStr.at(direction_);
}

void Dog::SetDirection(const std::string str)
{
    if (str ==DirStr::Up)
        direction_ = Direction::NORTH;
    else if (str ==DirStr::Down)
        direction_ = Direction::SOUTH;
    else if (str ==DirStr::Left)
        direction_ = Direction::WEST;
    else if (str ==DirStr::Right)
        direction_ = Direction::EAST;
}



}
