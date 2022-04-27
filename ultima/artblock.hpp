//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef artblock_hpp
#define artblock_hpp

#include <cstdint>
#include <array>
#include <vector>
#include <istream>

#include "ultima_types.hpp"
//=========================================================
class tileinfo ;
//=========================================================
class artblock {
	std::array<std::array<std::vector<tile_t>,8>,8> _tiles ;
	
public:
	artblock(int length=0,std::uint8_t *data = nullptr,  const tileinfo *tileInfo=nullptr);
	auto load(int length,std::uint8_t *data, const tileinfo *tileInfo) ->void ;
	auto load(int length,std::istream &input, const tileinfo *tileInfo) ->void ;
	auto tile(int x, int y) ->std::vector<tile_t>& ;
	auto tile(int x, int y) const ->const std::vector<tile_t>& ;
	auto updateInfo(const tileinfo &tileInfo)->void ;
	auto clear() ->void ;
};
#endif /* artblock_hpp */
