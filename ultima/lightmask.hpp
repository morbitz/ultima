//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef lightmask_hpp
#define lightmask_hpp

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "image.hpp"
#include "ultima_types.hpp"
//=========================================================
class lightmask {
	std::unordered_map<int, image_t> _lights ;
	
public:
	lightmask(const std::filesystem::path &uodir = std::filesystem::path());
	auto load(const std::filesystem::path &uodir) ->bool ;
	
	auto size() const ->size_t ;
	auto lights() const ->const std::unordered_map<int, image_t>& ;
	auto lights()  -> std::unordered_map<int, image_t>& ;

};
#endif /* lightmask_hpp */
