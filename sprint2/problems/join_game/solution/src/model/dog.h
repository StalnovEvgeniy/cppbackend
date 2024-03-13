#ifndef DOG_H
#define DOG_H

#pragma once

#include <vector>
#include <string>

#include "util/tagged.h"

namespace model {

class Dog
{
public:
    using Id = util::Tagged<int, Dog>;

    Dog(std::string name)
        : id_(Id{Dog::idCount++})
        , name_(name)
    { }

    const Id &GetId() const;
    const std::string &GetName() const;

private:
    Id id_;
    std::string name_;

    static int idCount;

};

}

#endif // DOG_H
