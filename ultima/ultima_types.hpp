//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef ultima_types_hpp
#define ultima_types_hpp

#include <cstdint>
#include <limits>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
//=========================================================
// Constants that we use
static constexpr auto invalid_id = std::numeric_limits<std::uint16_t>::max();
static constexpr auto invalid_gump = std::numeric_limits<std::int32_t>::max();
//===============================================================
// Why dont we use bitset for this?
// Because it doesn't lend itself to testing multiple specific
// flags at the same time.  We dont want to pollute the entire namepace
// with these names, so we wrapper it in a UO namespace (we could have done that
// for all of the uo stuff), something to consider?
//=======================================================================
namespace ultima {
	//===============================================================
	//flag constants
	static constexpr std::uint64_t none 	= 0x0000000000000000;
	static constexpr std::uint64_t background = 0x0000000000000001;
	static constexpr std::uint64_t weapon 	= 0x0000000000000002;
	static constexpr std::uint64_t holdable 	= 0x0000000000000002;	// Alternative for weapon
	static constexpr std::uint64_t transparent= 0x0000000000000004;
	// The tile is rendered with partial alpha-transparency.
	static constexpr std::uint64_t translucent= 0x0000000000000008;
	// The tile is a wall.
	static constexpr std::uint64_t wall 	= 0x0000000000000010;
	// The tile can cause damage when moved over.
	static constexpr std::uint64_t damaging 	= 0x0000000000000020;
	// The tile may not be moved over or through.
	static constexpr std::uint64_t impassable = 0x0000000000000040;
	static constexpr std::uint64_t wet 		= 0x0000000000000080;
	static constexpr std::uint64_t unknownBit9= 0x0000000000000100;
	// The tile is a surface. It may be moved over, but not through.
	static constexpr std::uint64_t surface 	= 0x0000000000000200;
	// The tile is a stair, ramp, or ladder.
	static constexpr std::uint64_t bridge 	= 0x0000000000000400;
	static constexpr std::uint64_t climable 	= 0x0000000000000400;  // alternative for bridge
	// The tile is stackable
	static constexpr std::uint64_t generic 	= 0x0000000000000800;
	static constexpr std::uint64_t stackable 	= 0x0000000000000800;  // alternative for generic
	// The tile is a window. Like NoShoot, tiles with this flag block line of sight.
	static constexpr std::uint64_t window 	= 0x0000000000001000;
	// The tile blocks line of sight.
	static constexpr std::uint64_t noShoot 	= 0x0000000000002000;
	// For single-amount tiles, the string "a " should be prepended to the tile name.
	static constexpr std::uint64_t articleA 	= 0x0000000000004000;
	// For single-amount tiles, the string "a " should be prepended to the tile name.
	static constexpr std::uint64_t articleAn 	= 0x0000000000008000;
	// Probably article The prepended to the tile name.
	static constexpr std::uint64_t articleThe = 0x0000000000010000;
	static constexpr std::uint64_t description= 0x0000000000010000;  // alternative for articThe
	// The tile becomes translucent when walked behind. Boat masts also have this flag.
	static constexpr std::uint64_t foliage 	= 0x0000000000020000;
	// Only gray pixels will be hued
	static constexpr std::uint64_t partialHue = 0x0000000000040000;
	// NoHouse or Unknown. Needs further research.
	static constexpr std::uint64_t noHouse 	= 0x0000000000080000;
	// The tile is a map--in the cartography sense. Unknown usage.
	static constexpr std::uint64_t map 		= 0x0000000000100000;
	// The tile is a container.
	static constexpr std::uint64_t container 	= 0x0000000000200000;
	// The tile may be equipped.
	static constexpr std::uint64_t wearable 	= 0x0000000000400000;
	// The tile gives off light.
	static constexpr std::uint64_t lightSource= 0x0000000000800000;
	// The tile is animated.
	static constexpr std::uint64_t animated 	= 0x0000000001000000;
	// Gargoyles can fly over or NoDiagonal
	static constexpr std::uint64_t hoverOver 	= 0x0000000002000000;
	// NoDiagonal (Unknown3).
	static constexpr std::uint64_t noDiagnol 	= 0x0000000004000000;
	static constexpr std::uint64_t armor 	= 0x0000000008000000;
	static constexpr std::uint64_t roof 	= 0x0000000010000000;
	static constexpr std::uint64_t door 	= 0x0000000020000000;
	static constexpr std::uint64_t stairBack 	= 0x0000000040000000;
	static constexpr std::uint64_t stairRight = 0x0000000080000000;
	
	// HS tile flags
	// Blend Alphas, tile blending.
	static constexpr std::uint64_t alphaBlend = 0x0000000100000000;
	// Uses new art style? Something related to the nodraw tile?
	static constexpr std::uint64_t useNewArt 	= 0x0000000200000000;
	// Has art being used?
	static constexpr std::uint64_t artUsed 	= 0x0000000400000000;
	// Disallow shadow on this tile, lightsource? lava?
	static constexpr std::uint64_t noShadow 	= 0x0000001000000000;
	// Let pixels bleed in to other tiles? Is this Disabling Texture Clamp?
	static constexpr std::uint64_t pixelBleed	= 0x0000002000000000;
	// Play tile animation once.
	static constexpr std::uint64_t animatedOnce = 0x0000004000000000;
	
	// Movable multi? Cool ships and vehicles etc? Something related to the masts ???
	static constexpr std::uint64_t multiMove 	= 0x0000100000000000;
	
}

// Types that are used
using texid_t = std::uint16_t ;
using tileid_t = std::uint16_t ;
using animid_t = std::uint16_t ;
using colorid_t = std::uint16_t ;
using flag_t = std::uint64_t ;
using gumpid_t = std::int32_t ;
enum class tiletype_t {terrain,art};
struct tileinfo_t {
protected:
	static const std::map<std::string, std::uint64_t> _flag_names ;
public:
	// Common attributes
	std::string name ;
	flag_t flags ;
	tileinfo_t() ;
	auto setFlag(const std::string &flagname,bool value) ->void;
};
struct terraininfo_t : public tileinfo_t {
	// Terrain only attribute
	texid_t texture ; // terrain only has this
	terraininfo_t() ;
};
struct artinfo_t : public tileinfo_t {
	// Art only attributes
	int weight ;  // std::uint8_t in data
	int quality ; // std::uint8_t in data
	int miscData ; // std::uint16_t? in data
	int unk2 ;	// std::uint8_t in data
	int quantity ;	// std::uint8_t in data
	animid_t animid ;
	int unk3 ;	// std::uint8_t in data
	int hue ;	// std::uint8_t in data
	int stackingOffset ;  // unk4
	int value ; // unk5 , std::uint8_t in data
	int  height ; // This is a uint8_t in the data
	artinfo_t() ;
};

struct tile_t {
	tileid_t tileid ;
	tiletype_t type ;
	union{
		const artinfo_t *artInfo ;
		const terraininfo_t *terrainInfo;
	};
	int altitude ;
	std::uint16_t static_hue ;
	tile_t(tiletype_t type= tiletype_t::terrain) ;
};

struct multi_item {
	tileid_t tileid ;
	int altitude ;
	int offsetx ;
	int offsety ;
	const artinfo_t *info ;
	std::uint16_t flag ;
	multi_item() ;
};

struct collection_item {
	int min_y;
	int max_y;
	int min_alt ;
	int max_alt;
	int min_x ;
	int max_x ;
	std::vector<multi_item> items ;
	collection_item();
	static const std::unordered_map<int,std::string> collection_names ;
};
#endif /* ultima_types_hpp */
