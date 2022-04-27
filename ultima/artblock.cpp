//Copyright © 2022 Charles Kerr. All rights reserved.

#include "artblock.hpp"
#include "tileinfo.hpp"

#include <iostream>
#include <algorithm>
//=========================================================
artblock::artblock(int length,std::uint8_t *data,const tileinfo *tileInfo ){
	if (data != nullptr) {
		load(length,data,tileInfo) ;
	}
}

//=========================================================
auto artblock::tile(int x, int y) ->std::vector<tile_t>& {
	return _tiles.at(x).at(y) ;
}
//=========================================================
auto artblock::tile(int x, int y) const ->const std::vector<tile_t>& {
	return _tiles.at(x).at(y) ;
}
//=========================================================
auto artblock::updateInfo(const tileinfo &tileInfo)->void {
	for (auto &row : _tiles){
		for (auto &tile: row){
			for (auto &entry : tile) {
				entry.artInfo = &tileInfo.art(entry.tileid);
			}
		}
	}
}
//=========================================================
auto artblock::clear() ->void {
	for (auto &row : _tiles){
		for (auto &tile: row){
			tile.clear();
		}
	}
}
//==========================================================
auto artblock::load(int length, std::istream &input, const tileinfo *tileInfo) ->void{
	this->clear() ;
	auto count = length/7 ;
	auto x = std::uint8_t(0);
	auto y = std::uint8_t(0) ;
	auto alt = std::int8_t(0) ;
	for (auto i=0; i<count ;++i){
		auto arttile = tile_t() ;
		arttile.type = tiletype_t::art ;
		arttile.artInfo = nullptr ;
		input.read(reinterpret_cast<char*>(&arttile.tileid),2) ;
		input.read(reinterpret_cast<char*>(&x),1);
		input.read(reinterpret_cast<char*>(&y),1);
		input.read(reinterpret_cast<char*>(&alt),1);
		arttile.altitude = static_cast<int>(alt) ;
		input.read(reinterpret_cast<char*>(&arttile.static_hue),2);
		if (tileInfo!= nullptr){
			arttile.artInfo = &(tileInfo->art(arttile.tileid));
		}
		tile(x,y).push_back(std::move(arttile));

	}
}
//==========================================================
auto artblock::load(int length,std::uint8_t *data, const tileinfo *tileInfo) ->void{
	this->clear() ;
	
	auto count = length/7 ;
	auto x = std::uint8_t(0);
	auto y = std::uint8_t(0) ;
	for (auto i=0; i<count ;++i){
		auto arttile = tile_t() ;
		arttile.type = tiletype_t::art ;
		arttile.artInfo = nullptr ;
		std::copy(data,data+2,reinterpret_cast<char*>(&arttile.tileid)) ;
		x = *(data+2) ;
		y = *(data+3);
		arttile.altitude = static_cast<int>(*(reinterpret_cast<std::int8_t*>(data+4)));
		std::copy(data+5,data+7,reinterpret_cast<char*>(&arttile.static_hue)) ;
		if (tileInfo!= nullptr){
			arttile.artInfo = &(tileInfo->art(arttile.tileid));
		}
		tile(x,y).push_back(std::move(arttile));
		data +=7 ;
		
	}
}
