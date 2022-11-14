// Copyright © 2022 UOX Development. All rights reserved.

#include "configuration.hpp"
#include "defines.h"
#include "strutil.hpp"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std::string_literals;

//===============================================================================================
// Obtain the location of where the UO Data files used by the server are located
//==============================================================================================
// Set up a default location, if not specified
#if !defined(UODATA)
#define _UODATA_ "UODATA"
#endif
const std::string uoenviroment = _UODATA_;

#if defined(_WIN32)
const std::string uodefaultdir =
    "C:\\Program Files (x86)\\Electronc Arts\\Ultima Online Classic";
#else
const std::string uodefaultdir = "~/uodata";
#endif
//===============================================================================================
auto obtainUODataLocation(const std::string &value) -> std::filesystem::path {
    auto path = std::filesystem::path(value);
    if (value.empty()) {
        // Nothing was specified, see if an enviroment variable is set
        if (const char *env_p = std::getenv(uoenviroment.c_str())) {
            path = std::filesystem::path(env_p);
        } else {
            path = std::filesystem::path(uodefaultdir);
        }
    }
    return path;
}

//=================================================================================
// Constants used
//=================================================================================
const std::unordered_map<relativepath_t, std::filesystem::path>
    config_t::relativepath = {
        {relativepath_t::dictionary, std::filesystem::path("dictionary"s)},
        {relativepath_t::log, std::filesystem::path("log"s)},
        {relativepath_t::book, std::filesystem::path("book"s)},
        {relativepath_t::msgboard, std::filesystem::path("msgboard"s)},
        {relativepath_t::world, std::filesystem::path("world"s)},
        {relativepath_t::definition, std::filesystem::path("definition"s)},
        {relativepath_t::none, std::filesystem::path("."s)}};

//=================================================================================
// config_t
//=================================================================================

//=================================================================================
auto config_t::clear() -> void {
    port = 2593;
    public_ip = "localhost";
    datalocation.clear();
    filenames.clear();
    datalocation[datalocation_t::server] =
        std::filesystem::current_path() / std::filesystem::path("server_data");
    datalocation[datalocation_t::shard] =
        std::filesystem::current_path() / std::filesystem::path("shard_data");
    datalocation[datalocation_t::uodata] = obtainUODataLocation("");
    filenames[filepath_t::account] = std::filesystem::path("account.act");
    filenames[filepath_t::firewall] = std::filesystem::path("firewall.cfg");
    filenames[filepath_t::setup] = std::filesystem::path("setup.cfg");
}
//=================================================================================
auto config_t::process(const std::string &key, const std::string &value,
                       int linenumber) -> void {
    auto lkey = strutil::lower(key);
    if (lkey == "port") {
        port = strutil::ston<std::uint16_t>(value);
    } else if (lkey == "public_ip") {
        public_ip = value;
    } else if (lkey == "server_directory") {
        datalocation[datalocation_t::server] = std::filesystem::path(value);
    } else if (lkey == "shard_directory") {
        datalocation[datalocation_t::shard] = std::filesystem::path(value);
    } else if (lkey == "uodata_directory") {
        datalocation[datalocation_t::uodata] = std::filesystem::path(value);
    } else if (lkey == "setup_file") {
        filenames[filepath_t::setup] = std::filesystem::path(value).filename();
    } else if (lkey == "account_file") {
        filenames[filepath_t::account] =
            std::filesystem::path(value).filename();
    } else if (lkey == "firewall_file") {
        filenames[filepath_t::firewall] =
            std::filesystem::path(value).filename();
    } else {
        std::clog << "In configuration, line: " << linenumber
                  << " uknown key: '" << key << "' and value: '" << value << "'"
                  << std::endl;
    }
}
//=================================================================================
config_t::config_t(const std::filesystem::path &configpath)
    : port(2593), public_ip("localhost") {
    clear();
    if (!configpath.empty()) {
        load(configpath);
    }
}

//=================================================================================
auto config_t::load(const std::filesystem::path &configpath) -> void {
    clear();
    auto input = std::ifstream(configpath);
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open: "s + configpath.string());
    }
    // This config file has no "headers", just keyvalues
    auto buffer = std::array<char, _LINE_BUFFER>();
    auto linenumber = 0;
    while (input.good() && !input.eof()) {
        linenumber++;
        input.getline(buffer.data(), _LINE_BUFFER - 1);
        buffer[input.gcount()] = 0;
        if (input.gcount() > 0) {
            if (buffer.at(input.gcount() - 1) == '\n') {
                buffer[input.gcount() - 1] = 0;
            }
            auto line = buffer.data();
            auto curline = strutil::trim(strutil::strip(line, "//"));
            if (!curline.empty()) {
                auto [key, value] = strutil::split(curline, "=");
                process(key, value, linenumber);
            }
        }
    }
}

//=================================================================================
// Returns a directory file path for the specified location
auto config_t::location(relativepath_t path, datalocation_t location) const
    -> std::filesystem::path {
    auto loc = datalocation.at(location);
    if (relativepath_t::none != path) {
        loc = datalocation.at(location) / relativepath.at(path);
    }
    return loc;
}
//=================================================================================
// Returns a complete file path for the specified file
auto config_t::filepath(filepath_t path, datalocation_t location) const
    -> std::filesystem::path {
    return datalocation.at(location) / filenames.at(path);
}
