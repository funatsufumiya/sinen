#ifndef SINEN_REGISTER_SCRIPT_HPP
#define SINEN_REGISTER_SCRIPT_HPP
#include <sol/sol.hpp>
namespace sinen {
void register_keycode(sol::state &lua);
void register_table(sol::state &lua);
void register_drawable(sol::state &lua);
} // namespace sinen
#endif