// Copyright © 2022 UOX Development. All rights reserved.

#include "main.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "argument.hpp"
#include "configuration.hpp"
#include "tileinfo.hpp"
using namespace std::string_literals;

//=================================================================================
auto main(int argc, char **argv) -> int {
    auto exit_code = EXIT_SUCCESS;
    try {
        auto arguments = argument_t(argc, argv);
        auto config = config_t(arguments.cfgpath);
        // Lets get the tile information
        std::clog << "Loading tile information" << std::endl;
        auto info = tileinfo_t(
            config.location(relativepath_t::none, datalocation_t::uodata));
        info.loadUpdates(
            config.location(relativepath_t::none, datalocation_t::server) /
            std::filesystem::path("tileinfo.cfg"));
        info.loadUpdates(
            config.location(relativepath_t::none, datalocation_t::server) /
            std::filesystem::path("shard.cfg"));
        info.processUpdates();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit_code = EXIT_FAILURE;
    }
    return exit_code;
}
