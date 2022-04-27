//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef terrainblock_hpp
#define terrainblock_hpp

#include <cstdint>
#include <array>

#include "ultima_types.hpp"
class tileinfo ;
//=========================================================
class terrainblock {
	std::array<std::array<tile_t,8>,8> _tiles ;
	
public:
	terrainblock(std::uint8_t *data = nullptr, const tileinfo *tileInfo=nullptr);
	auto load(std::uint8_t *data,const tileinfo *tileInfo) ->void ;
	auto tile(int x, int y) ->tile_t& ;
	auto tile(int x, int y) const ->const tile_t& ;
	auto updateInfo(const tileinfo &tileInfo)->void ;
};
#endif /* terrainblock_hpp */
