//Copyright © 2022 Charles Kerr. All rights reserved.

#include "lightmask.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

//=========================================================
auto lightmask::size() const ->size_t {
	return _lights.size();
}
//=========================================================
auto lightmask::lights() const ->const std::unordered_map<int, image_t>& {
	return _lights;
}
//=========================================================
auto lightmask::lights()  -> std::unordered_map<int, image_t>& {
	return _lights;
}
//=========================================================
lightmask::lightmask(const std::filesystem::path &uodir) {
	if (!uodir.empty()){
		load(uodir);
	}
}
//=========================================================
auto lightmask::load(const std::filesystem::path &uodir) ->bool {
	auto rvalue = false ;

	auto idx = std::ifstream((uodir / std::filesystem::path("lightidx.mul")).string(),std::ios::binary);
	if (idx.is_open()){
		auto mul = std::ifstream((uodir / std::filesystem::path("light.mul")).string(),std::ios::binary);
		if (mul.is_open()){
			rvalue = true ;
			auto offset = std::uint32_t(0) ;
			auto length = std::uint32_t(0) ;
			auto width = std::uint16_t(0) ;
			auto height = std::uint16_t(0);
			auto value = std::int8_t(0) ;
			auto entry = 0 ;
			while (idx.good() && !idx.eof()){
				idx.read(reinterpret_cast<char*>(&offset),4);
				idx.read(reinterpret_cast<char*>(&length),4);
				idx.read(reinterpret_cast<char*>(&height),2);
				idx.read(reinterpret_cast<char*>(&width),2);
				if ((idx.gcount()==2) && (offset <0xFFFFFFFE) && (length >0) && (length<0xFFFFFFFE)){
					mul.seekg(offset,std::ios::beg);
					//auto image = image_t(width,height,16912);
					auto image = image_t(width,height,0);
					for (auto y = 0 ; y<height ;++y){
						for (auto x=0 ; x<width ; ++x){
							mul.read(reinterpret_cast<char*>(&value),1);
							//image.hue(value, x, y);
							auto index = std::abs(value);
							image.at(x,y) = (index<<10) | (index<<5) | (index) ;
						}
					}
					_lights.insert_or_assign(entry, std::move(image));
				}
				++entry ;
			}
		}

	}
	return rvalue ;
}
