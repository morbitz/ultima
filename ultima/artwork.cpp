//Copyright © 2022 Charles Kerr. All rights reserved.

#include "artwork.hpp"

#include <iostream>
#include <fstream>
using namespace std::string_literals;
//=========================================================
artwork::artwork(const std::filesystem::path &uodir) {
	if (!uodir.empty()){
		load(uodir) ;
	}
}
//=========================================================
auto artwork::load(const std::filesystem::path &uodir) ->bool {
	auto path = uodir / std::filesystem::path("artLegacyMul.uop") ;
	_art.clear() ;
	_terrain.clear();
	return loadUOP(path.string(), 0xa761+0x4000, "build/artlegacymul/%08u.tga");
}
//=========================================================
auto artwork::size(tiletype_t type) const ->size_t {
	if (type == tiletype_t::terrain){
		return _terrain.size();
	}
	return _art.size() ;
}

//=========================================================
auto artwork::image(tileid_t tileid, tiletype_t type) ->image_t& {
	if (type == tiletype_t::terrain){
		return _terrain.at(tileid);
	}
	return _art.at(tileid);

}
//=========================================================
auto artwork::image(tileid_t tileid, tiletype_t type) const -> const image_t& {
	if (type == tiletype_t::terrain){
		return _terrain.at(tileid);
	}
	return _art.at(tileid);

}

//=========================================================
auto artwork::images(tiletype_t type) const -> const std::map<tileid_t,image_t> & {
	if (type == tiletype_t::terrain){
		return _terrain;
	}
	return _art;
}

//=========================================================
auto artwork::processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool {
	if (!data.empty()){
		if (index < 0x4000) {
			_terrain.insert_or_assign(static_cast<tileid_t>(index), convertToTerrain(data));
		}
		else {
			_art.insert_or_assign(static_cast<tileid_t>(index-0x4000), convertToArt(data));
			
		}
	}
	return true ;
	
}
//=========================================================
auto artwork::convertToTerrain(std::vector<std::uint8_t> &data) ->image_t {
	auto image = image_t(44,44,0xFFFF) ;
	const std::uint16_t *ptrData = reinterpret_cast<const std::uint16_t*>(data.data());
	auto run = 2 ;
	int xloc = 21 ;
	auto data_index = 0 ;
	for (auto height = 0 ; height<22;height++){
		for (auto offset = 0  ; offset < run; offset++){
			image.at(xloc+offset,height) = *(ptrData + data_index) ; //pixel[data_index] ;
			data_index += 1 ;
		}
		xloc-- ;
		run +=2 ;
	}
	xloc = 0 ;
	run = 44 ;
	for (auto height = 22 ; height<44;height++){
		for (auto offset = 0  ; offset < run; offset++){
			image.at(xloc+offset,height) = *(ptrData + data_index);//pixel[data_index] ;
			data_index += 1 ;
		}
		xloc++ ;
		run -=2 ;
	}

	
	return image ;
}
//=========================================================
auto artwork::convertToArt(std::vector<std::uint8_t> &data) ->image_t {
	const std::uint16_t *ptrData = reinterpret_cast<const std::uint16_t*>(data.data());
	auto delta = 2 ;
	auto width = *(ptrData+delta);
	auto height = *(ptrData+(delta+1));
	auto image = image_t(0,0,0xFFFF);
	if (!((width >= 1024) || (width ==0)|| (height >=1024) || (height ==0))){
		image =  image_t(width,height,0xFFFF);
		std::vector<unsigned short> offsets(height,0);
		// Read in the row offsets for the data
		for (auto index = 0 ; index < height; index++){
			offsets[index] = *(ptrData+(2+index+delta));
		}
		auto x= 0 ;
		auto y = 0 ;
		// Set up an offset to just past the offset table
		auto dataindex = height+2+delta ;
		// Point to the first data for y=0
		auto curindex = dataindex + offsets[y] ;
		while (y < height){
			auto xoffset = *(ptrData+curindex++) ;
			auto run = *(ptrData+curindex++);
			if ((xoffset+run) >= 2048) {
				break ;
			}
			else if ((xoffset+run)!=0) {
				x += xoffset ;
				for (auto i = 0 ; i< run; i++){
					image.at(x++,y) = *(ptrData+curindex++);
				}
			}
			else {
				x = 0 ;
				y++ ;
				curindex = dataindex + offsets[y];
			}
		}

	}

	return image ;
}
