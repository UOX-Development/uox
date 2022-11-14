//Copyright © 2022 UOX Development. All rights reserved.

#include "tileinfo.hpp"
#include "defines.h"
#include "strutil.hpp"

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <stdexcept>

using namespace std::string_literals;

//=================================================================================

//=================================================================================
// Define flagnames
//=================================================================================
static const std::unordered_map<std::string,flag_t> flagnames{
    {"background"s,uo::background},{"holdable"s,uo::holdable},
    {"transparent"s,uo::transparent},{"translucent"s,uo::translucent},
    {"wall"s,uo::wall},{"damaging"s,uo::damaging},
    {"impassable"s,uo::impassable},{"wet"s,uo::wet},
    {"bit9"s,uo::bit9},{"surface"s,uo::surface},
    {"bridge"s,uo::bridge},{"generic"s,uo::generic},
    {"window"s,uo::window},{"noshoot"s,uo::noshoot},
    {"articlea"s,uo::articlea},{"articlean"s,uo::articlean},
    {"articlethe"s,uo::articlethe},{"foilage"s,uo::foilage},
    {"partialhue"s,uo::partialhue},{"nohouse"s,uo::nohouse},
    {"map"s,uo::map},{"container"s,uo::container},
    {"wearable"s,uo::wearable},{"lightsource"s,uo::lightsource},
    {"animation"s,uo::animation},{"hoverover"s,uo::hoverover},
    {"nodiagonal"s,uo::nodiagonal},{"armor"s,uo::armor},
    {"roof"s,uo::roof},{"door"s,uo::door},
    {"stairback"s,uo::stairback},{"stairright"s,uo::stairright},
    {"alphablend"s,uo::alphablend},{"usenewart"s,uo::usenewart},
    {"artused"s,uo::artused},{"bit36"s,uo::bit36},
    {"noshadow"s,uo::noshadow},{"pixelbleed"s,uo::pixelbleed},
    {"playanimonce"s,uo::playanimonce},{"bit40"s,uo::bit40},
    {"multimovable"s,uo::multimovable},{"bit42"s,uo::bit42},
    {"bit45"s,uo::bit45},{"bit46"s,uo::bit46},
    {"bit47"s,uo::bit47},{"bit48"s,uo::bit48},
    {"bit49"s,uo::bit49},{"bit50"s,uo::bit50},
    {"bit51"s,uo::bit51},{"bit52"s,uo::bit52},
    {"bit53"s,uo::bit53},{"bit54"s,uo::bit54},
    {"bit55"s,uo::bit55},{"bit56"s,uo::bit56},
    {"bit57"s,uo::bit57},{"bit58"s,uo::bit58},
    {"bit59"s,uo::bit59},{"bit60"s,uo::bit60},
    {"bit61"s,uo::bit61},{"bit62"s,uo::bit62},
    {"bit63"s,uo::bit63},{"bit64"s,uo::bit64}
};

//==============================================================
// uoflag_t -     Structure to hold the uo flag, and allow some convience
//                methods on it.
//==============================================================

//==============================================================
uoflag_t::uoflag_t(flag_t value):flag(value) {
    
}
    
//==============================================================
auto uoflag_t::any(flag_t value) const ->bool {
    return static_cast<bool>(flag & value) ;
}
//==============================================================
auto uoflag_t::all(flag_t value) const ->bool {
    return !static_cast<bool>((flag&value) ^ value) ;
}
//==============================================================
auto uoflag_t::only(flag_t value) const ->bool {
    return !static_cast<bool>((~value) & flag);
 }

//==============================================================
auto uoflag_t::set(flag_t value) ->void {
    flag |= value ;
}
//==============================================================
auto uoflag_t::clear(flag_t value) ->void {
    flag &=(~value);
}


//==============================================================
// info_t -    Base class for tile information
//==============================================================
auto info_t::loadName(std::istream &input)->void {
    std::array<char,21> buffer ;
    buffer[20]=0 ;
    input.read(buffer.data(),20);
    name = buffer.data() ;
}

//==============================================================
// terrain_info_t-
//==============================================================
//===============================================================
auto terrain_info_t::load(std::istream &input) -> void {
    // Read in the flag
    input.read(reinterpret_cast<char*>(&tileflag.flag),sizeof(tileflag.flag)) ;
    // read in the texture
    input.read(reinterpret_cast<char*>(&texture),sizeof(texture));
    loadName(input);
}
//==============================================================
// art_info_t-
//==============================================================
//===============================================================
auto art_info_t::load(std::istream &input) -> void {
    // Read in the flag
    input.read(reinterpret_cast<char*>(&tileflag.flag),sizeof(tileflag.flag)) ;
    // read in the data
    input.read(reinterpret_cast<char*>(&weight),sizeof(weight));
    input.read(reinterpret_cast<char*>(&quality),sizeof(quality));
    input.read(reinterpret_cast<char*>(&miscdata),sizeof(miscdata));
    input.read(reinterpret_cast<char*>(&unknown2),sizeof(unknown2));
    input.read(reinterpret_cast<char*>(&quantity),sizeof(quantity));
    input.read(reinterpret_cast<char*>(&animid),sizeof(animid));
    input.read(reinterpret_cast<char*>(&unknown3),sizeof(unknown3));
    input.read(reinterpret_cast<char*>(&hue),sizeof(hue));
    input.read(reinterpret_cast<char*>(&stackingoffset),sizeof(stackingoffset));
    input.read(reinterpret_cast<char*>(&height),sizeof(height));
    loadName(input);
}

//==============================================================
// tileinfo_t
//==============================================================
//==============================================================
auto tileinfo_t::updateFlagAttribute(bool isart,std::uint16_t id, const std::string &key, bool shouldset)->void{
    try {
        auto mask = flagnames.at(key) ;
        if (shouldset){
            if (isart){
                art.at(id).tileflag.set(mask);
            }
            else {
                terrain.at(id).tileflag.set(mask) ;
            }
        }
        else{
            if (isart){
                art.at(id).tileflag.clear(mask);
            }
            else {
                terrain.at(id).tileflag.clear(mask) ;
            }
        }
    }
    catch(...){
        auto name = (isart?"Art"s:"Terrain"s);
        throw std::runtime_error(strutil::format("For %s tile %i, unable to set/clear flag(s): %s",name.c_str(),id,key.c_str()));
    }
}
//==============================================================
auto tileinfo_t::updateAttribute(bool isart,std::uint16_t id, const std::string &key, const std::string &value)->void {
    try{
        if (!isart){
            if (key == "name") {
                terrain.at(id).name = value ;
             }
            else if (key =="flag"){
                terrain.at(id).tileflag.flag = std::stoull(value,nullptr,0);
            }
            else if (key == "flagset"){
                auto flags = strutil::parse(value, ",");
                for (const auto &flag:flags){
                    updateFlagAttribute(isart, id, flag, true);
                }
            }
            else if (key == "flagclear"){
                auto flags = strutil::parse(value, ",");
                for (const auto &flag:flags){
                    updateFlagAttribute(isart, id, flag, false);
                }
            }
            else if (key == "texture"){
                terrain.at(id).texture = static_cast<std::uint16_t>(std::stoul(value,nullptr,0));
           }
        }
        else {
            if (key == "name") {
                art.at(id).name = value ;
            }
            else if (key =="flag"){
                art.at(id).tileflag.flag = std::stoull(value,nullptr,0);
            }
            else if (key == "flagset"){
                auto flags = strutil::parse(value, ",");
                for (const auto &flag:flags){
                    updateFlagAttribute(isart, id, flag, true);
                }
            }
            else if (key == "flagclear"){
                auto flags = strutil::parse(value, ",");
                for (const auto &flag:flags){
                    updateFlagAttribute(isart, id, flag, false);
                }
            }
            else if (key == "weight"){
                art.at(id).weight = static_cast<std::uint8_t>(std::stoul(value,nullptr,0));
            }
            else if (key == "quality"){
                art.at(id).quality = static_cast<std::uint8_t>(std::stoul(value,nullptr,0));
            }
            else if (key == "miscdata"){
                art.at(id).miscdata = static_cast<std::uint16_t>(std::stoul(value,nullptr,0));
            }
            else if (key == "unknown2"){
                art.at(id).unknown2 = static_cast<std::uint8_t>(std::stoul(value,nullptr,0));
            }
            else if (key == "animid"){
                art.at(id).animid = static_cast<std::uint16_t>(std::stoul(value,nullptr,0));
            }
            else if (key == "unknown3"){
                art.at(id).unknown3 = static_cast<std::uint8_t>(std::stoul(value,nullptr,0));

            }
            else if (key == "hue"){
                art.at(id).hue = static_cast<std::uint8_t>(std::stoul(value,nullptr,0));
            }
            else if (key == "stackingoffset"){
                art.at(id).stackingoffset = static_cast<std::uint16_t>(std::stoul(value,nullptr,0));
            }
            else if (key == "height"){
                art.at(id).height = static_cast<std::uint8_t>(std::stoul(value,nullptr,0));
            }
        }
    }
    catch(...){
        auto name = (isart?"Art"s:"Terrain"s);
        throw std::runtime_error(strutil::format("%s tile modification failed, Key: %s Value: %s",name.c_str(),key.c_str(),value.c_str()));
    }
}
//==============================================================
tileinfo_t::tileinfo_t(const std::filesystem::path &uopath) {
    if (!uopath.empty()){
        load(uopath);
    }
}

//==============================================================
auto tileinfo_t::load(const std::filesystem::path &uopath) ->tileinfo_t& {
    auto infopath = uopath / std::filesystem::path("tiledata.mul") ;
    auto input = std::ifstream(infopath.string(),std::ios::binary) ;
    if (!input.is_open()){
        throw std::runtime_error("Unable to open: "s+infopath.string());
    }
    
    auto dummy = std::uint32_t(0) ;
    for (auto i = 0 ; i< 0x4000; i++){
        if (!input.good() || input.eof()){
            throw std::runtime_error("Error processing: "s +infopath.string());
        }
        if ( (((i & 0x1F)==0) && (i>0)) || (i == 1)){
            input.read(reinterpret_cast<char*>(&dummy),4); // read off the group header
        }
        if (!input.good() || input.eof()){
            throw std::runtime_error("Error processing: "s +infopath.string());
        }
       terrain.at(i).load(input) ;
    }
    for (auto i = 0 ; i< 0xFFFF; i++){
        if (!input.good() || input.eof()){
            throw std::runtime_error("Error processing: "s +infopath.string());
        }
        if ( (i & 0x1f) == 0) {
            input.read(reinterpret_cast<char*>(&dummy),4); // read off the group header
        }
        if (!input.good() || input.eof()){
            throw std::runtime_error("Error processing: "s +infopath.string());
        }
       art.at(i).load(input) ;
    }
    return *this ;
}

//==============================================================
auto tileinfo_t::loadUpdates(const std::filesystem::path &infofile) ->bool {
    auto input = std::ifstream(infofile.string()) ;
    if (!input.is_open()){
        return false ;
    }
    std::array<char,_LINE_BUFFER> buffer ;
    enum class search_t {header,start,data};
    auto search = search_t::header ;
    auto linenumber =0 ;
    auto tileid = std::uint16_t(0) ;
    auto isart = false ;
    auto data = std::vector<std::pair<std::string,std::string>>() ;
    while (input.good() && !input.eof()){
        linenumber++;
        input.getline(buffer.data(),_LINE_BUFFER -1);
        buffer[input.gcount()] = 0 ;
        if (input.gcount()>0){
            if (buffer.at(input.gcount()-1) == '\n') {
                buffer[input.gcount()-1] = 0 ;
            }
            auto line = buffer.data();
            auto curline = strutil::trim(strutil::strip(line,"//"));
            if (!curline.empty()){
                switch(search) {
                    case search_t::header: {
                        if (curline.size() >2){
                            if ((curline[0]=='[') && (curline[curline.size()-1]==']')){
                                auto [id,type] = strutil::split(curline.substr(1,curline.size()-2),":") ;
                                try {
                                    tileid = std::stoi(id,nullptr,0) ;
                                }
                                catch(...){
                                    // The id isn't something we recoginize as a valid number
                                    throw std::runtime_error(strutil::format("Line: %i Invalid header: %s File: %s",linenumber,curline.c_str(),infofile.string().c_str()));
                                 }
                                auto ltype = strutil::lower(type) ;
                                if (ltype=="art" ) {
                                    isart = true ;
                                }
                                else if (ltype == "terrain"){
                                    isart = false ;
                                }
                                else {
                                    throw std::runtime_error(strutil::format("Line: %i Invalid header: %s File: %s",linenumber,curline.c_str(),infofile.string().c_str()));

                                }
                                // We get to here, we can start searching for section start
                                search = search_t::start ;
                            }
                        }
                        break;
                    }
                    case search_t::start: {
                        if (curline != "{"){
                            throw std::runtime_error(strutil::format("Line: %i Expected start of section: %s File: %s",linenumber,curline.c_str(),infofile.string().c_str()));
                        }
                        // We get to here, we can start searching for section start
                        search = search_t::data ;
                        break;
                    }
                    case search_t::data:{
                        if ((curline[0] == '[') || (curline[0]=='{')) {
                            throw std::runtime_error(strutil::format("Line: %i Unexpected value(looking for data): %s File: %s",linenumber,curline.c_str(),infofile.string().c_str()));
                        }
                        if (curline != "}"){
                            auto [key,value] = strutil::split(curline,"=");
                            data.push_back(std::make_pair(strutil::lower(key), value));
                        }
                        else {
                            if (isart){
                                art_modifications.insert_or_assign(tileid, data);
                            }
                            else {
                                terrain_modifications.insert_or_assign(tileid,data);
                            }
                            data.clear() ;
                            search = search_t::header;
                       }
                        break;
                        
                    }
                }
            }
       }
    }
    // Now, we should be searching for header, if everyting was right
    if (search != search_t::header){
        throw std::runtime_error(strutil::format("Line: %i Incomplete section in file: %s",linenumber,infofile.string().c_str()));
    }
    return true ;
}
//==============================================================
auto tileinfo_t::processUpdates() ->void {
    for (const auto &[id,values] : terrain_modifications){
        if (id < 0x4000){
            for (const auto &[key,value]:values){
                updateAttribute(false, id, key, value);
            }
        }
    }
    for (const auto &[id,values] : art_modifications){
        if (id < 0xFFFF){
            for (const auto &[key,value]:values){
                updateAttribute(true, id, key, value);
            }
        }
    }

    
}
//==============================================================
auto tileinfo_t::clear() ->void {
    art_modifications.clear();
    terrain_modifications.clear() ;
}

