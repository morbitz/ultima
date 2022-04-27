//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef artwork_hpp
#define artwork_hpp

#include <cstdint>
#include <map>
#include <filesystem>
#include <istream>

#include "ultima_types.hpp"
#include "uopfile.hpp"
#include "image.hpp"
//=========================================================
class artwork : public uopfile{
	std::map<tileid_t,image_t> _terrain ;
	std::map<tileid_t,image_t> _art ;
	
	auto processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool final;
	auto convertToTerrain(std::vector<std::uint8_t> &data) ->image_t ;
	auto convertToArt(std::vector<std::uint8_t> &data) ->image_t ;
public:
	artwork(const std::filesystem::path &uodir= std::filesystem::path()) ;
	auto load(const std::filesystem::path &uodir) ->bool ;
	auto size(tiletype_t type) const ->size_t ;
	
	auto image(tileid_t tileid, tiletype_t type) ->image_t& ;
	auto image(tileid_t tileid, tiletype_t type) const -> const image_t& ;
	
	auto images(tiletype_t type) const -> const std::map<tileid_t,image_t> & ;

};
#endif /* artwork_hpp */
