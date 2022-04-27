//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef gumpart_hpp
#define gumpart_hpp

#include <cstdint>
#include <string>
#include <filesystem>
#include <map>

#include "uopfile.hpp"
#include "image.hpp"
//=========================================================
class gumpart : public uopfile {
	std::map<int,image_t> _gumpart ;
	auto processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool final;

public:
	gumpart(const std::filesystem::path &uodir = std::filesystem::path());
	bool load(const std::filesystem::path &uodir) ;
	auto size() const ->size_t ;
	auto operator[](int id) const ->const image_t & ;
	auto operator[](int id)  -> image_t & ;
	auto gumps() const ->const std::map<int,image_t>&;

};
#endif /* gumpart_hpp */
