//Copyright © 2022 Charles Kerr. All rights reserved.

#include "radarcolor.hpp"

#include <iostream>
#include <fstream>

//=========================================================
radarcolor::radarcolor(const std::filesystem::path &uodir ) {
	if (!uodir.empty()){
		load(uodir) ;
	}
}
//=========================================================
auto radarcolor::load(const std::filesystem::path &uodir)->bool {
	auto rvalue = false ;
	_colors.clear();
	auto path = uodir / std::filesystem::path("radarcol.mul");
	if(std::filesystem::exists(path)) {
		auto size = std::filesystem::file_size(path)/2 ;
		if (size >0){
			auto input = std::ifstream(path.string(),std::ios::binary);
			if (input.is_open()){
				rvalue = true ;
				_colors.resize(size,0);
				input.read(reinterpret_cast<char*>(_colors.data()),size*2);
			}
		}
	}
	return true ;
}
//=========================================================
auto radarcolor::color(tileid_t id, tiletype_t type) const ->const colorid_t& {
	auto offset = (tiletype_t::terrain == type?id:id+0x4000);
	return _colors[offset];
}
//=========================================================
auto radarcolor::color(tileid_t id, tiletype_t type)  -> colorid_t& {
	auto offset = (tiletype_t::terrain == type?id:id+0x4000);
	return _colors[offset];
}

