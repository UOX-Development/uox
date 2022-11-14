//Copyright © 2022 UOX Development. All rights reserved.

#include "objectfactory.hpp"

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

using namespace std::string_literals;

//=================================================================================
static std::unordered_map< objecttype_t,std::string> objectnames{
    {objecttype_t::base,"base"s},{objecttype_t::item,"item"s},
    {objecttype_t::creature,"creature"s},{objecttype_t::character,"character"s},
    {objecttype_t::player,"player"s},{objecttype_t::boat,"boat"s},
    {objecttype_t::spawner,"spawner"s},{objecttype_t::building,"building"s},
    {objecttype_t::mobile,"mobile"s}
};
//=================================================================================
auto nameForType(objecttype_t type) ->const std::string&{
    return objectnames.at(type) ;
}
//=================================================================================
auto typeForName(const std::string &name) ->objecttype_t {
    auto iter = std::find_if(objectnames.begin(),objectnames.end(),[&name](const std::pair<objecttype_t,std::string> &value){
        return name == std::get<1>(value) ;
    });
    if (iter != objectnames.end()){
        return iter->first ;
    }
    throw std::out_of_range("Object type not found: "s+name);
}
