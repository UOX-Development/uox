// Copyright © 2022 UOX Development. All rights reserved.

#include "serialgen.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace std::string_literals;

//=================================================================================
serialgen_t::serialgen_t(serial_t current, serial_t low, serial_t high)
    : ceiling(high), floor(low), nextserial(current) {}
//=================================================================================
auto serialgen_t::acquire() -> serial_t {
    std::lock_guard lock(serial_lock);
    auto serial = nextserial++;
    if (serial > ceiling) {
        throw std::runtime_error("Request for serial exceeded ceiling: "s +
                                 std::to_string(ceiling));
    }
    return serial;
}
//=================================================================================
auto serialgen_t::free(serial_t serial) -> void {
    std::lock_guard lock(serial_lock);
    if (serial == nextserial - 1) {
        nextserial--;
    }
}
//=================================================================================
auto serialgen_t::insert(serial_t serial) -> void {
    std::lock_guard lock(serial_lock);
    if (serial > nextserial) {
        nextserial = serial;
    }
}
