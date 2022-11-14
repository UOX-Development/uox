// Copyright © 2022 UOX Development. All rights reserved.

#include "argument.hpp"

#include <cstdlib>
#include <iostream>

using namespace std::string_literals;
const std::string uoxcfgvar = "UOXCFG";
//=================================================================================
argument_t::argument_t(int argc, char **argv)
    : daemon(false), cfgpath(std::filesystem::path("uox.cfg")), executable() {
    // First look at the enviroment varaibles, and if they are set, setup the
    // defaults
    if (const char *env_p = std::getenv(uoxcfgvar.c_str())) {
        cfgpath = std::filesystem::path(env_p);
    }

    if (argc > 0) {
        executable = std::filesystem::path(argv[0]);
        for (auto index = 1; index < argc; ++index) {
            auto value = std::string(argv[index]);

            if (value.find("--") == 0) {
                // This is a flag
                if (value.substr(2) == "daemon") {
                    daemon = true;
                } else if (value.substr(2) == "no-daemon") {
                    daemon = false;
                }
            } else {
                cfgpath = std::filesystem::path(value);
            }
        }
    }
}
