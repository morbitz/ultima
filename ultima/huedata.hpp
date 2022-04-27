//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef huedata_hpp
#define huedata_hpp

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "ultima_types.hpp"
//=========================================================
class huedata {
	std::vector<std::vector<colorid_t>> _hues ;
	
public:
	huedata(const std::filesystem::path &uodir = std::filesystem::path());
	auto load(const std::filesystem::path &uodir) ->bool ;
	
	auto size() const ->size_t ;
	
	auto hues() const ->const std::vector<std::vector<colorid_t>>& ;
	auto hues()  -> std::vector<std::vector<colorid_t>>& ;

	auto operator[](int hueid) const ->const std::vector<colorid_t>& ;
	auto operator[](int hueid)  -> std::vector<colorid_t>& ;
	
};
#endif /* huedata_hpp */
