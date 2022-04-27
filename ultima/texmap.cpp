//Copyright © 2022 Charles Kerr. All rights reserved.

#include "texmap.hpp"

#include <iostream>
#include <fstream>

//=========================================================
texmap::texmap(const std::filesystem::path &uodir ){
	if (!uodir.empty()){
		load(uodir) ;
	}
}
//=========================================================
auto texmap::load(const std::filesystem::path &uodir) ->bool {
	_textures.clear();
	auto idx = std::ifstream((uodir / (std::filesystem::path("texidx.mul"))).string(),std::ios::binary) ;
	auto rvalue = false ;
	if (idx.is_open()){
		auto mul = std::ifstream((uodir / (std::filesystem::path("texmaps.mul"))).string(),std::ios::binary) ;
		if (mul.is_open()){
			rvalue = true ;
			auto entrynum = texid_t(0) ;
			auto offset = std::uint32_t(0) ;
			auto length = std::uint32_t(0) ;
			auto extra = std::uint32_t(0) ;
			while (idx.good() && !idx.eof()){
				idx.read(reinterpret_cast<char*>(&offset), 4);
				idx.read(reinterpret_cast<char*>(&length), 4);
				idx.read(reinterpret_cast<char*>(&extra), 4);
				if ((idx.gcount() == 4) && (offset < 0xFFFFFFFE) && (length >0)){
					mul.seekg(offset,std::ios::beg);
					auto width = (length==0x2000?64:128);
					auto image = image_t(width,width) ;
					mul.read(reinterpret_cast<char*>(image.data.data()),length) ;
					_textures.insert_or_assign(entrynum, std::move(image)) ;
				}
				++entrynum ;
			}
		}
	}
	return rvalue ;
}

//=========================================================
auto texmap::size() const ->size_t {
	return _textures.size();
}
//=========================================================
auto texmap::texture(texid_t texid) const ->const image_t& {
	return _textures.at(texid) ;
}
//=========================================================
auto texmap::texture(texid_t texid) ->image_t& {
	return _textures.at(texid) ;
}
//=========================================================
auto texmap::textures() const ->const std::map<texid_t,image_t> & {
	return _textures ;
}
