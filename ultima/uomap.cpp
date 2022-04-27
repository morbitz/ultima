//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uomap.hpp"

#include "tileinfo.hpp"
#include "image.hpp"
#include "radarcolor.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

//=========================================================
uomap::uomap(int mapnum,const tileinfo *tileInfo):_diffcount(0){
	_tileInfo = tileInfo ;
	_mapnum = mapnum ;
	auto size = _mapsizes.at(mapnum) ;
	_width = size.first ;
	_height = size.second ;
	_terrain.resize( (_width/8) * (_height/8) );
	_art.resize(_terrain.size()) ;
}
//=========================================================
auto uomap::width() const ->int {
	return _width ;
}
//=========================================================
auto uomap::height() const ->int {
	return _height ;
}
//=========================================================
auto uomap::load(const std::filesystem::path &uodir,bool includeDiff) ->bool {
	auto rvalue = loadTerrain(uodir);
	if (rvalue){
		rvalue = loadArt(uodir,includeDiff);
	}
	return rvalue ;
}
//=========================================================
auto  uomap::loadTerrain(const std::filesystem::path &uodir) ->bool{
	auto hash = this->format("build/map%ilegacymul/%s", _mapnum,"%.8u.dat");
	auto filename = this->format("map%iLegacyMUL.uop",_mapnum);
	auto path = uodir / std::filesystem::path(filename) ;
	return loadUOP(path.string(), 0x300, hash);
}
//=========================================================
auto uomap::loadArt(const std::filesystem::path &uodir, bool includeDiff) ->bool {
	_diffcount = 0 ;
	
	auto idxfile  = uodir / std::filesystem::path(format("staidx%i.mul",_mapnum));
	auto mulfile = uodir / std::filesystem::path(format("statics%i.mul",_mapnum));
	auto idx = std::ifstream(idxfile.string(),std::ios::binary);
	auto rvalue = idx.is_open();
	if (rvalue){
		auto mul = std::ifstream(mulfile.string(),std::ios::binary);
		rvalue = mul.is_open();
		if (rvalue) {
			auto offset = std::uint32_t(0) ;
			auto length = std::int32_t(0) ;
			auto extra = std::uint32_t(0) ;
			auto block = 0 ;
			while (idx.good()&& !idx.eof()){
				idx.read(reinterpret_cast<char*>(&offset),4);
				idx.read(reinterpret_cast<char*>(&length),4);
				idx.read(reinterpret_cast<char*>(&extra),4);
				if ((length >0) && (idx.gcount()==4)) {
					mul.seekg(offset,std::ios::beg);
					if (block < _art.size()){
						_art[block].load(length, mul, _tileInfo);
					}
				}
				++block ;
			}
			if (includeDiff){
				applyDiff(uodir);
			}
		}
	}
	return rvalue ;
}
//=========================================================
auto uomap::applyDiff(const std::filesystem::path &uodir) ->int {
	_diffcount = 0 ;
	auto diffblock = std::ifstream((uodir / std::filesystem::path(format("stadifl%i.mul",_mapnum))).string(),std::ios::binary) ;
	if (diffblock.is_open()){
		auto diffidx = std::ifstream((uodir / std::filesystem::path(format("stadifi%i.mul",_mapnum))).string(),std::ios::binary) ;
		if (diffidx.is_open()){
			auto diff = std::ifstream((uodir / std::filesystem::path(format("stadif%i.mul",_mapnum))).string(),std::ios::binary) ;
			if (diff.is_open()){
				auto blocknum = std::uint32_t(0) ;
				auto offset = std::uint32_t(0);
				auto length = std::int32_t(0) ;
				auto extra = std::uint32_t(0);
				while (diffblock.good() && !diffblock.eof()){
					diffblock.read(reinterpret_cast<char*>(&blocknum),4);
					diffidx.read(reinterpret_cast<char*>(&offset),4);
					diffidx.read(reinterpret_cast<char*>(&length),4);
					diffidx.read(reinterpret_cast<char*>(&extra),4);
					if ((diffblock.gcount()==4) && (diffidx.gcount()==4)){
						if (blocknum< _art.size()){
							if (length >0){
								diff.seekg(offset,std::ios::beg);
								_art[blocknum].load(length, diff, _tileInfo);
							}
							else {
								_art[blocknum].clear();
							}
						}
						++_diffcount;
					}
				}
			}
		}

	}
	return _diffcount ;
}

//=========================================================
auto uomap::calcBlock(int x,int y) const ->int {
	// How many blocks is x across?
	return  ((x/8)*(_height/8)) + (y/8) ;
}
//=========================================================
auto uomap::calcXYOffset(int block) const ->std::pair<int,int> {
	auto x = (block / (_height/8)) * 8 ;
	auto y = (block % (_height/8)) * 8;
	return std::make_pair(x, y);
}
//=========================================================
auto uomap::blockAndIndexFor(int x, int y) const ->std::tuple<int, int, int>{
	auto block = calcBlock(x, y);
	auto offset = calcXYOffset(block);
	return std::make_tuple(block,x-offset.first,y-offset.second);
}
//=========================================================
auto uomap::processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool {
	auto count = data.size()/196 ;
	auto block = (static_cast<int>(index) * 0xC4000)/196 ;
	for (auto i=0 ;i<count;++i){
		auto ptr = data.data()+(i*196);
		if (block < _terrain.size()){
			loadTerrainBlock(block, ptr);
		}
		++block ;
	}
	return true ;
}
//=========================================================
auto uomap::loadTerrainBlock(int blocknum,std::uint8_t *data)->void {
	_terrain[blocknum].load(data, _tileInfo);
}

//=========================================================
auto uomap::terrain(int x, int y) const ->const tile_t& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _terrain[blocknum].tile(xoffset, yoffset);
}
//=========================================================
auto uomap::terrain(int x, int y)  ->tile_t& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _terrain[blocknum].tile(xoffset, yoffset);

}
//=========================================================
auto uomap::art(int x, int y) const ->const std::vector<tile_t>& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _art[blocknum].tile(xoffset, yoffset);

}
//=========================================================
auto uomap::art(int x, int y)  ->std::vector<tile_t>& {
	auto [blocknum,xoffset,yoffset] = blockAndIndexFor(x, y);
	return _art[blocknum].tile(xoffset, yoffset);
}
//=========================================================
auto uomap::radar(const radarcolor &radarColor,int x,int y, int width,int height ) const -> image_t {
	
	if (width == 0){
		width = _width ;
	}
	if (height == 0){
		height = _height;
	}
	if (((x+width) > _width) || ((y+height) > _height)) {
		throw std::out_of_range(format("%i,%i with width %i and height %i exceeds map size of %i,%i",x,y,width,height,_width,_height));
	}
	auto image = image_t(width,height);
	for (auto xloc=0;xloc<width;++xloc){
		for (auto yloc=0;yloc<height;++yloc){
			image.at(xloc,yloc) = radarColor.color(this->terrain(xloc+x, yloc+y).tileid, tiletype_t::terrain);
			
			if (!this->art(xloc+x, yloc+y).empty()) {
				auto pair = std::minmax_element(this->art(xloc+x, yloc+y).begin(), this->art(xloc+x, yloc+y).end(), [](const tile_t &lhs,const tile_t &rhs){
					return (lhs.altitude + lhs.artInfo->height) < (rhs.altitude + rhs.artInfo->height) ;
				});
				if ((pair.second->altitude + pair.second->artInfo->height) > this->terrain(xloc+x, yloc+y).altitude) {
					image.at(xloc,yloc) = radarColor.color(pair.second->tileid, tiletype_t::art);
				}
				
			}
			
		}
	}
	return image ;

}
