//Copyright © 2022 UOX Development. All rights reserved.

#ifndef objectfactory_hpp
#define objectfactory_hpp

#include <cstdint>
#include <string>

#include <mutex>

//=================================================================================
// This creates and tracks in game objects
//=================================================================================

// Object types
enum class objecttype_t {
    base,item,mobile,creature,character,player,boat,building,spawner
};
//=================================================================================
auto nameForType(objecttype_t type) ->const std::string& ;
//=================================================================================
auto typeForName(const std::string &name) ->objecttype_t ;

#endif /* objectfactory_hpp */
