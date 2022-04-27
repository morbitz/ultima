//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef tileinfo_hpp
#define tileinfo_hpp

#include <cstdint>
#include <vector>
#include <filesystem>
#include <fstream>

#include "ultima_types.hpp"
//=========================================================
class tileinfo {
	std::vector<terraininfo_t> _terrain ;
	std::vector<artinfo_t> _art ;

	auto processTerrain(std::ifstream &input) ->void ;
	auto processArt(std::ifstream &input) ->void ;
	bool _isHS ;
public:
	tileinfo(const std::filesystem::path &uodir = std::filesystem::path());
	auto load(const std::filesystem::path &uodir) ->bool;
	
	auto terrain(int id) const -> const terraininfo_t& ;
	auto terrain(int id) -> terraininfo_t& ;
	auto art(int id) const -> const artinfo_t& ;
	auto art(int id) -> artinfo_t& ;
	auto size(tiletype_t type) const ->size_t ;
	
	auto terrains() const -> const std::vector<terraininfo_t>& ;
	auto terrains()  ->  std::vector<terraininfo_t>& ;
	auto arts() const -> const std::vector<artinfo_t>&;
	auto arts() ->std::vector<artinfo_t>& ;

};
#endif /* tileinfo_hpp */
