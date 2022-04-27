//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef radarcolor_hpp
#define radarcolor_hpp

#include <cstdint>
#include <vector>
#include <filesystem>

#include "ultima_types.hpp"
//=========================================================
class radarcolor {
private:
	std::vector<colorid_t> _colors ;
public:
	radarcolor(const std::filesystem::path &uodir = std::filesystem::path()) ;
	auto load(const std::filesystem::path &uodir)->bool ;
	auto color(tileid_t id, tiletype_t type) const ->const colorid_t& ;
	auto color(tileid_t id, tiletype_t type)  -> colorid_t& ;

};
#endif /* radarcolor_hpp */
