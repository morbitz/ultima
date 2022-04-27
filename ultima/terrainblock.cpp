//Copyright © 2022 Charles Kerr. All rights reserved.

#include "terrainblock.hpp"
#include "tileinfo.hpp"

#include <iostream>
#include <algorithm>
//=========================================================
terrainblock::terrainblock(std::uint8_t *data,const tileinfo *tileInfo ){
	if (data != nullptr) {
		load(data,tileInfo) ;
	}
}
//=========================================================
auto terrainblock::load(std::uint8_t *data,const tileinfo *tileInfo) ->void {
	if (data !=nullptr){
		data +=4 ;
		for (auto y= 0 ; y < 8 ;++y){
			for (auto x= 0 ; x < 8 ;++x){
				tileid_t tileid ;
				// we copy here, because of potential alignment issues
				std::copy(data,data+2,reinterpret_cast<std::uint8_t*>(&(tileid)));
				_tiles[x][y].type = tiletype_t::terrain ;
				_tiles[x][y].tileid = tileid ;
				_tiles[x][y].altitude = static_cast<int>(*(reinterpret_cast<std::int8_t*>(data+2)));
				if (tileInfo != nullptr){
					_tiles[x][y].terrainInfo = &(tileInfo->terrain(tileid)) ;
				}
				else {
					_tiles[x][y].terrainInfo = nullptr ;
				}
				data += 3 ; // advance data three bytes ;
			}
		}
	}
}

//=========================================================
auto terrainblock::tile(int x, int y) ->tile_t& {
	return _tiles.at(x).at(y) ;
}
//=========================================================
auto terrainblock::tile(int x, int y) const ->const tile_t& {
	return _tiles.at(x).at(y) ;
}
//=========================================================
auto terrainblock::updateInfo(const tileinfo &tileInfo)->void {
	for (auto &row : _tiles){
		for (auto &tile: row){
			tile.terrainInfo = &(tileInfo.terrain(tile.tileid));
		}
	}
}
