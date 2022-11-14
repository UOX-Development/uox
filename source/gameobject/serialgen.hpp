// Copyright © 2022 UOX Development. All rights reserved.

#ifndef serialgen_hpp
#define serialgen_hpp

#include <cstdint>
#include <istream>
#include <mutex>
#include <ostream>
#include <stack>
#include <string>
#include <vector>
//=================================================================================
// serialgen_t generates the next serial number available in a range
// one can specify the range it should stay within.
//=================================================================================

using serial_t = std::uint32_t;

//=================================================================================
class serialgen_t {

    // What is the range for this generator?
    serial_t floor;
    serial_t ceiling;

    // Our lock when accessing us, we declear it mutable in case used in const
    // methods
    mutable std::mutex serial_lock;

    serial_t nextserial;

  public:
    // Some ceilings that no generator can every exceed
    static constexpr serial_t abs_floor = 0;
    static constexpr serial_t abs_ceiling = 0xFFFFFFFF;

    serialgen_t(serial_t current, serial_t low = abs_floor,
                serial_t high = abs_ceiling);

    auto acquire() -> serial_t;
    auto free(serial_t serial) -> void;
    auto insert(serial_t serial) -> void;
};

#endif /* serialgen_hpp */
