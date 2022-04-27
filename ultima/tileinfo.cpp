//Copyright © 2022 Charles Kerr. All rights reserved.

#include "tileinfo.hpp"

#include <iostream>
#include <array>
#include <algorithm>

//=========================================================
tileinfo::tileinfo(const std::filesystem::path &uodir ):_isHS(false){
	if (!uodir.empty()){
		load(uodir);
	}
}
//=========================================================
auto tileinfo::load(const std::filesystem::path &uodir) ->bool {
	auto rvalue = false ;
	_art.clear();
	_terrain.clear();
	auto path = uodir / std::filesystem::path("tiledata.mul");
	if (std::filesystem::exists(path)){
		_isHS = (std::filesystem::file_size(path)>=3188736?true:false);
		auto input = std::ifstream(path.string(),std::ios::binary);
		if (input.is_open()){
			rvalue = true ;
			processTerrain(input);
			processArt(input);
		}
	}
	return rvalue ;
}
//=========================================================
auto tileinfo::terrain(int id) const -> const terraininfo_t& {
	return _terrain[id] ;
}
//=========================================================
auto tileinfo::terrain(int id) -> terraininfo_t& {
	return _terrain[id] ;
}
//=========================================================
auto tileinfo::art(int id) const -> const artinfo_t& {
	return _art[id] ;
}
//=========================================================
auto tileinfo::art(int id) -> artinfo_t& {
	return _art[id] ;
}

//=========================================================
auto tileinfo::size(tiletype_t type) const ->size_t {
	if (type == tiletype_t::terrain){
		return _terrain.size() ;
	}
	return _art.size() ;

}

//=========================================================
auto tileinfo::processTerrain(std::ifstream &input) ->void {
	_terrain.reserve(0x4000);
	std::uint32_t value32 = 0 ;
	std::uint64_t value64 =  0;
	std::array<char,20> string_buffer ;
	string_buffer.fill(0);
	for (auto i=0 ; i< 0x4000;i++){
		// We have to get rid of the header on blocks of information
		// HS has the firt entry messed up
		if (_isHS) {
			if ( (((i & 0x1F)==0) && (i>0)) || (i == 1)){
				input.read(reinterpret_cast<char*>(&value32),4); // read off the group header
			}
			
		}
		else {
			if ( (i & 0x1f) == 0) {
				input.read(reinterpret_cast<char*>(&value32),4); // read off the group header
			}
		}
		// now create the info_t we will use
		auto info = terraininfo_t();
		// read off the flags, 32 bit on pre-HS, 64 bit after
		if (_isHS) {
			input.read(reinterpret_cast<char*>(&value64),8);
			info.flags = value64 ;
			
		}
		
		else{
			input.read(reinterpret_cast<char*>(&value32),4);
			info.flags = static_cast<std::uint64_t>(value32) ;
		}
		// only other thing for terrain is the texture
		// and name
		input.read(reinterpret_cast<char*>(&info.texture),2);
		input.read(string_buffer.data(),20);
		if (input.gcount()==20){
			// We need to find the "0", if any
			auto iter = std::find(string_buffer.begin(),string_buffer.end(),0);
			info.name = std::string(string_buffer.begin(),iter);
			_terrain.push_back(std::move(info));

		}
		else {
			break;
		}
	}

}
//=========================================================
auto tileinfo::processArt(std::ifstream &input) ->void {
	_art.reserve(0xFFFF);
	std::uint32_t value32 = 0 ;
	std::uint64_t value64 =  0;
	std::uint8_t value8 = 0 ;
	std::uint16_t value16 = 0;
	std::array<char,20> string_buffer ;
	string_buffer.fill(0);
	if ( !(input.eof() || input.bad())){
		auto loopcount = 0 ;
		do {
			if ( (loopcount & 0x1f) == 0) {
				input.read(reinterpret_cast<char*>(&value32),4); // read off the group header
			}
			auto info = artinfo_t();
			if (_isHS) {
				input.read(reinterpret_cast<char*>(&value64),8);
				info.flags = value64;
				
			}
			else {
				input.read(reinterpret_cast<char*>(&value32),4);
				info.flags = static_cast<std::uint64_t>(value32);
				
			}
			input.read(reinterpret_cast<char*>(&value8),1);
			info.weight = static_cast<int>(value8);
			input.read(reinterpret_cast<char*>(&value8),1);
			info.quality = static_cast<int>(value8);
			//misc data
			input.read(reinterpret_cast<char*>(&value16),2);
			info.miscData = static_cast<int>(value16) ;
			//Second unkown
			input.read(reinterpret_cast<char*>(&value8),1);
			info.unk2 = static_cast<int>(value8) ;
			// Quantity
			input.read(reinterpret_cast<char*>(&value8),1);
			info.quantity = static_cast<int>(value8);
			// Animation
			input.read(reinterpret_cast<char*>(&value16),2);
			info.animid = value16 ;
			
			//Third a byte
			input.read(reinterpret_cast<char*>(&value8),1);
			info.unk3 = static_cast<int>(value8 );
			
			// Hue/Value
			input.read(reinterpret_cast<char*>(&value8),1);
			info.hue = static_cast<int>(value8);
			//stacking offset
			input.read(reinterpret_cast<char*>(&value8),1);
			info.stackingOffset = static_cast<int>(value8);
			// value
			input.read(reinterpret_cast<char*>(&value8),1);
			info.value = static_cast<int>(value8);
			// Height
			input.read(reinterpret_cast<char*>(&value8),1);
			info.height = static_cast<int>(value8) ;
			// and name
			input.read(string_buffer.data(),20);
			if (input.gcount()==20){
				// We need to find the "0", if any
				auto iter = std::find(string_buffer.begin(),string_buffer.end(),0);
				info.name = std::string(string_buffer.begin(),iter);
				if (!input.good()){
					break ;
				}
				_art.push_back(std::move(info));
			}
			loopcount++;
			
		}while( (!input.eof()) && input.good());

	}

}
//=============================================================
auto tileinfo::terrains() const -> const std::vector<terraininfo_t>& {
	return _terrain;
}
//=============================================================
auto tileinfo::terrains()  ->  std::vector<terraininfo_t>& {
	return _terrain;
}
//=============================================================
auto tileinfo::arts() const -> const std::vector<artinfo_t>&{
	return _art;
}
//=============================================================
auto tileinfo::arts() ->std::vector<artinfo_t>& {
	return _art;
}
