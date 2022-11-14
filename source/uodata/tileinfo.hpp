// Copyright © 2022 UOX Development. All rights reserved.

#ifndef tileinfo_hpp
#define tileinfo_hpp

#include <array>
#include <cstdint>
#include <filesystem>
#include <istream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using flag_t = std::uint64_t;

//=================================================================================
// tile structures and contants used
namespace uo {
constexpr auto background = flag_t(0x0000000000000001);
constexpr auto holdable = flag_t(0x0000000000000002);
constexpr auto transparent = flag_t(0x0000000000000004);
constexpr auto translucent = flag_t(0x0000000000000008);

constexpr auto wall = flag_t(0x0000000000000010);
constexpr auto damaging = flag_t(0x0000000000000020);
constexpr auto impassable = flag_t(0x0000000000000040);
constexpr auto wet = flag_t(0x0000000000000080);

constexpr auto bit9 = flag_t(0x0000000000000100);
constexpr auto surface = flag_t(0x0000000000000200);
constexpr auto bridge = flag_t(0x0000000000000400);
constexpr auto generic = flag_t(0x0000000000000800);

constexpr auto window = flag_t(0x0000000000001000);
constexpr auto noshoot = flag_t(0x0000000000002000);
constexpr auto articlea = flag_t(0x0000000000004000);
constexpr auto articlean = flag_t(0x0000000000008000);

constexpr auto articlethe = flag_t(0x0000000000010000);
constexpr auto foilage = flag_t(0x0000000000020000);
constexpr auto partialhue = flag_t(0x0000000000040000);
constexpr auto nohouse = flag_t(0x0000000000080000);

constexpr auto map = flag_t(0x0000000000010000);
constexpr auto container = flag_t(0x0000000000020000);
constexpr auto wearable = flag_t(0x0000000000040000);
constexpr auto lightsource = flag_t(0x0000000000080000);

constexpr auto animation = flag_t(0x0000000000100000);
constexpr auto hoverover = flag_t(0x0000000000200000);
constexpr auto nodiagonal = flag_t(0x0000000000400000);
constexpr auto armor = flag_t(0x0000000000800000);

constexpr auto roof = flag_t(0x0000000001000000);
constexpr auto door = flag_t(0x0000000002000000);
constexpr auto stairback = flag_t(0x0000000004000000);
constexpr auto stairright = flag_t(0x0000000008000000);

constexpr auto alphablend = flag_t(0x0000000100000000);
constexpr auto usenewart = flag_t(0x0000000200000000);
constexpr auto artused = flag_t(0x0000000400000000);
constexpr auto bit36 = flag_t(0x0000000800000000);

constexpr auto noshadow = flag_t(0x0000001000000000);
constexpr auto pixelbleed = flag_t(0x0000002000000000);
constexpr auto playanimonce = flag_t(0x0000004000000000);
constexpr auto bit40 = flag_t(0x0000008000000000);

constexpr auto multimovable = flag_t(0x0000010000000000);
constexpr auto bit42 = flag_t(0x0000020000000000);
constexpr auto bit43 = flag_t(0x0000040000000000);
constexpr auto bit44 = flag_t(0x0000080000000000);

constexpr auto bit45 = flag_t(0x0001000000000000);
constexpr auto bit46 = flag_t(0x0002000000000000);
constexpr auto bit47 = flag_t(0x0004000000000000);
constexpr auto bit48 = flag_t(0x0008000000000000);

constexpr auto bit49 = flag_t(0x0010000000000000);
constexpr auto bit50 = flag_t(0x0020000000000000);
constexpr auto bit51 = flag_t(0x0040000000000000);
constexpr auto bit52 = flag_t(0x0080000000000000);

constexpr auto bit53 = flag_t(0x0010000000000000);
constexpr auto bit54 = flag_t(0x0020000000000000);
constexpr auto bit55 = flag_t(0x0040000000000000);
constexpr auto bit56 = flag_t(0x0080000000000000);

constexpr auto bit57 = flag_t(0x0100000000000000);
constexpr auto bit58 = flag_t(0x0200000000000000);
constexpr auto bit59 = flag_t(0x0400000000000000);
constexpr auto bit60 = flag_t(0x0800000000000000);

constexpr auto bit61 = flag_t(0x1000000000000000);
constexpr auto bit62 = flag_t(0x2000000000000000);
constexpr auto bit63 = flag_t(0x4000000000000000);
constexpr auto bit64 = flag_t(0x8000000000000000);
} // namespace uo

//==============================================================
// uoflag_t - 	Structure to hold the uo flag, and allow some convience
//				methods on it.
//==============================================================
struct uoflag_t {
    flag_t flag;
    uoflag_t(flag_t value = 0);

    auto any(flag_t value) const -> bool;
    auto all(flag_t value) const -> bool;
    auto only(flag_t value) const -> bool;

    auto set(flag_t value) -> void;
    auto clear(flag_t value) -> void;
};

enum class infotype_t { base, terrain, art };
//==============================================================
// info_t -	Base class for tile information
//==============================================================
struct info_t {
  protected:
    auto loadName(std::istream &input) -> void;

  public:
    infotype_t type;
    uoflag_t tileflag;
    std::string name;
    info_t(infotype_t itype = infotype_t::base) : type(itype) {}
    virtual auto load(std::istream &input) -> void = 0;
};

//==============================================================
// terraininfo_t
//==============================================================
struct terrain_info_t : public info_t {
    std::uint16_t texture;
    terrain_info_t() : info_t(infotype_t::terrain), texture(0) {}
    auto load(std::istream &input) -> void final;
};

//==============================================================
// artinfo_t
//==============================================================
struct art_info_t : public info_t {
    std::uint8_t weight;
    std::uint8_t quality;
    std::uint16_t miscdata;
    std::uint8_t unknown2;
    std::uint8_t quantity;
    std::uint16_t animid;
    std::uint8_t unknown3;
    std::uint8_t hue;
    std::uint16_t stackingoffset;
    std::uint8_t height;

    art_info_t()
        : info_t(infotype_t::art), weight(0), quality(0), miscdata(0),
          unknown2(0), quantity(0), animid(0xFFFF), unknown3(0), hue(0),
          stackingoffset(0), height(0) {}
    auto load(std::istream &input) -> void final;
};

//==============================================================
// tileinfo_t
//==============================================================

struct tileinfo_t {
  private:
    std::map<std::uint16_t, std::vector<std::pair<std::string, std::string>>>
        terrain_modifications;
    std::map<std::uint16_t, std::vector<std::pair<std::string, std::string>>>
        art_modifications;
    auto updateAttribute(bool isart, std::uint16_t id, const std::string &key,
                         const std::string &value) -> void;
    auto updateFlagAttribute(bool isart, std::uint16_t id,
                             const std::string &key, bool shouldset) -> void;

  public:
    std::array<terrain_info_t, 0x4000> terrain;
    std::array<art_info_t, 0xFFFF> art;
    auto load(const std::filesystem::path &uopath) -> tileinfo_t &;
    auto loadUpdates(const std::filesystem::path &infofile) -> bool;
    auto processUpdates() -> void;
    auto clear() -> void;
    tileinfo_t(const std::filesystem::path &uopath = std::filesystem::path());
};

#endif /* tileinfo_hpp */
