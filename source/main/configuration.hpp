//Copyright © 2022 UOX Development. All rights reserved.

#ifndef configuration_hpp
#define configuration_hpp

#include <cstdint>
#include <string>
#include <filesystem>
#include <istream>
#include <ostream>
#include <unordered_map>
//=================================================================================
// This contains the basic information that the server needs to obtain all other information
// Any change to these data items, requires a restart of the server
//=================================================================================
enum class datalocation_t {
    server,shard,uodata
};
enum relativepath_t {
    none,dictionary,log,book,msgboard,world,definition
};
enum class filepath_t {
    account,firewall,setup
};

//=================================================================================
// Static type of configuration data
//=================================================================================
class config_t {
private:
    static const std::unordered_map<relativepath_t, std::filesystem::path> relativepath ;
    std::unordered_map<datalocation_t,std::filesystem::path> datalocation ;
    std::unordered_map<filepath_t, std::filesystem::path> filenames ;
    
    auto clear() ->void ;
    // Process the keys in the configuration file
    auto process(const std::string &key, const std::string &value,int linenumber) ->void ;
public:
    std::string public_ip ;			// Our public ip we want to use for server connects
    std::uint16_t port ;			// The port we want to use
    config_t(const std::filesystem::path &configpath = std::filesystem::path()) ;
    auto load(const std::filesystem::path &configpath) ->void;
    // Returns a directory file path for the specified location
    auto location(relativepath_t path,datalocation_t location) const ->std::filesystem::path ;
    // Returns a complete file path for the specified file
   	auto filepath(filepath_t path,datalocation_t location) const ->std::filesystem::path;
};



#endif /* configuration_hpp */
