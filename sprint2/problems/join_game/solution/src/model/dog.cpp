#include "dog.h"

namespace model {

int Dog::idCount = 0;



const Dog::Id &Dog::GetId() const
{
    return id_;
}

const std::string &Dog::GetName() const
{
    return name_;
}


}
