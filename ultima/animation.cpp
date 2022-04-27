//Copyright © 2022 Charles Kerr. All rights reserved.

#include "animation.hpp"

#include <iostream>
#include <fstream>
using namespace std::string_literals;
//=========================================================
auto animation::size() const ->size_t {
	return _animations.size();
}
//=========================================================
auto animation::animations() const ->const std::unordered_map<int, std::vector<animframe_t> >& {
	return _animations;
}
//=========================================================
auto animation::animations()  -> std::unordered_map<int, std::vector<animframe_t> >& {
	return _animations;
}

//=========================================================
animation::animation(int anim_number, const std::filesystem::path &uodir ):_anim_number(anim_number){
	if (!uodir.empty()){
		load(uodir);
	}
}
//=========================================================
auto animation::load(const std::filesystem::path &uodir) ->bool {
	auto rvalue = false ;
	auto idxfile = uodir / std::filesystem::path("anim"s + (_anim_number >1?std::to_string(_anim_number):""s)+".idx"s) ;
	auto mulfile = uodir / std::filesystem::path("anim"s + (_anim_number >1?std::to_string(_anim_number):""s)+".mul"s) ;
	auto idx = std::ifstream(idxfile.string() , std::ios::binary);
	if (idx.is_open()){
		auto mul = std::ifstream(mulfile.string(),std::ios::binary);
		if (mul.is_open()){
			rvalue = true ;
			auto entry = 0 ;
			auto offset = std::uint32_t(0) ;
			auto length = std::uint32_t(0) ;
			auto extra = std::uint32_t(0) ;
			while (idx.good() && !idx.eof()){
				idx.read(reinterpret_cast<char*>(&offset),4);
				idx.read(reinterpret_cast<char*>(&length),4);
				idx.read(reinterpret_cast<char*>(&extra),4);
				if ((idx.gcount()==4)&& (length >0) && (offset < 0xFFFFFFFE)){
					mul.seekg(offset,std::ios::beg) ;
					std::vector<colorid_t> pallette(256,0) ;
					mul.read(reinterpret_cast<char*>(pallette.data()),512);
					auto anchor = static_cast<std::uint32_t>(mul.tellg()) ;
					auto count = std::uint32_t(0) ;
					mul.read(reinterpret_cast<char*>(&count),4);
					auto offsets = std::vector<std::uint32_t>(count,0);
					mul.read(reinterpret_cast<char*>(offsets.data()),offsets.size()*4);
					auto width = std::int16_t(0) ;
					auto height = std::int16_t(0) ;
					auto image_centerx = std::int16_t(0) ;
					auto image_centery = std::int16_t(0);
				
					for (auto frame = 0 ; frame < count ; ++frame) {
						mul.seekg(anchor+offsets[frame],std::ios::beg) ;
						mul.read(reinterpret_cast<char*>(&image_centerx),2);
						mul.read(reinterpret_cast<char*>(&image_centery),2);
						mul.read(reinterpret_cast<char*>(&width),2);
						mul.read(reinterpret_cast<char*>(&height),2);
						// Now, process the frame !
						if ((width !=0) && (height!=0)) {
							auto frameimage = processFrame(mul, pallette, width, height, image_centerx, image_centery);
							_animations[entry].push_back(std::move(frameimage));
						}
					}
				}
				++entry ;
			}
		}
	}
	return rvalue ;
}

//=================================================================
auto animation::processFrame(std::istream &input, const std::vector<colorid_t> &pallette,int width,int height, int image_x,int image_y) const ->animframe_t {
	constexpr std::int32_t mask = (0x200 << 22) | (0x200 <<12) ;

	auto anim = animframe_t(width,height,0x7fff) ;
	anim.image_center_x = image_x ;
	anim.image_center_y = image_y ;
	
	auto xBase = image_x - 0x200;
	auto yBase = (image_y+height) - 0x200;
	auto offset = xBase  + yBase*width ;
	auto header = std::int32_t(0) ;
	input.read(reinterpret_cast<char*>(&header),4);
	while (header != 0x7fff7fff){
		header ^=mask ;
		auto current = offset + ((((header >> 12) & 0x3FF) * width)  + ((header >> 22) & 0x3FF));
		auto end = current + (header & 0xFFF);
		std::uint8_t color ;
		while (current < end){
			input.read(reinterpret_cast<char*>(&color),1);
			anim.at(current%width,current/width)=pallette[color] ;
			current++;
		}
		input.read(reinterpret_cast<char*>(&header),4);
	}
	return anim ;
	
}
