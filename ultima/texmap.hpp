//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef texmap_hpp
#define texmap_hpp

#include <cstdint>
#include <map>
#include <filesystem>

#include "ultima_types.hpp"
#include "image.hpp"
//=========================================================
class texmap {
	
	std::map<texid_t,image_t> _textures ;
	
public:
	texmap(const std::filesystem::path &uodir = std::filesystem::path());
	auto load(const std::filesystem::path &uodir) ->bool ;
	auto size() const ->size_t ;
	auto texture(texid_t texid) const ->const image_t& ;
	auto texture(texid_t texid) ->image_t& ;
	auto textures() const ->const std::map<texid_t,image_t> & ;
};
#endif /* texmap_hpp */
