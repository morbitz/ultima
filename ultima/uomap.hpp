//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef uomap_hpp
#define uomap_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <utility>
#include <filesystem>
#include <tuple>

#include "ultima_types.hpp"
#include "uopfile.hpp"
#include "terrainblock.hpp"
#include "artblock.hpp"

class tileinfo ; // Forward declaratin
class image_t ;
class radarcolor;
//=========================================================
class uomap : public uopfile {
	std::vector<terrainblock> _terrain ;
	std::vector<artblock> _art;
	
	static constexpr int _totalmaps = 6 ;
	static constexpr std::array<std::pair<int,int>,_totalmaps> _mapsizes{{
		{7168,4096},{7168,4096},{2304,1600},
		{2560,2048},{1448,1448},{1280,4096}
	}};
	int _mapnum ;
	int _width ;
	int _height ;
	const tileinfo *_tileInfo ;
	int _diffcount ;
	auto processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool final ;
	auto calcBlock(int x,int y) const ->int ;
	auto calcXYOffset(int block) const ->std::pair<int,int> ;
	auto loadTerrainBlock(int blocknum,std::uint8_t *data)->void ;
public:
	uomap(int mapnum,const tileinfo *tileInfo = nullptr);
	auto width() const ->int ;
	auto height() const ->int ;
	auto diffCount() const ->int ;
	
	auto load(const std::filesystem::path &uodir,bool includeDiff=true) ->bool;
	auto loadTerrain(const std::filesystem::path &uodir) ->bool ;
	auto loadArt(const std::filesystem::path &uodir, bool includeDiff = true) ->bool ;
	auto applyDiff(const std::filesystem::path &uodir) ->int ;
	
	auto blockAndIndexFor(int x, int y) const ->std::tuple<int, int, int>;

	
	auto terrain(int x, int y) const ->const tile_t& ;
	auto terrain(int x, int y)  ->tile_t& ;
	auto art(int x, int y) const ->const std::vector<tile_t>& ;
	auto art(int x, int y)  ->std::vector<tile_t>& ;

	auto radar(const radarcolor &radarColor,int x=0,int y=0, int width=0,int height=0 ) const -> image_t ;
};
#endif /* uomap_hpp */
