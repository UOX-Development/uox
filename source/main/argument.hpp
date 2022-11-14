//Copyright © 2022 UOX Development. All rights reserved.

#ifndef argument_hpp
#define argument_hpp

#include <cstdint>
#include <string>
#include <filesystem>
//=================================================================================
// argument_t
// This class is consolidates our argument_t handling.  As we might change what arguments
// from the command line on startup we will need. This will read them, and organize it to
// how we want.  This may be simple and just be a vector of strings, or a series of
// of string variables.  Or perhaps parse it into "flags" and "varables".  Right now
// we take one one parameter, and one flag:
//		uox cfgfile --daemon
// Where cfgfile is the filepath for iniital starting settings, and the --daemon says it should be
// daemon mode
//=================================================================================

struct argument_t {
    bool daemon ;
    std::filesystem::path cfgpath ;
    std::filesystem::path executable ;
    argument_t(int argc, char** argv) ;
};
#endif /* argument_hpp */
