//
//  main.cpp
//  ultima
//
//  Created by Charles Kerr on 4/24/22.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <vector>

#include "texmap.hpp"
#include "artwork.hpp"
#include "gumpart.hpp"
#include "image.hpp"
#include "uomap.hpp"
#include "tileinfo.hpp"
#include "radarcolor.hpp"
#include "multicollection.hpp"
#include "lightmask.hpp"
#include "huedata.hpp"
#include "animation.hpp"
using namespace std::string_literals;

int main(int argc, const char * argv[]) {
	
	auto rvalue = EXIT_FAILURE ;
	if (argc >=3) {
		rvalue = EXIT_SUCCESS;
		
		auto uodir = std::filesystem::path(std::string(argv[1]));
		auto datadir = std::filesystem::path(std::string(argv[2]));
		auto lightdir = datadir / std::filesystem::path("lights");
		auto animdir = datadir / std::filesystem::path("animation");
		auto lightMask = lightmask(uodir) ;
		auto hues = huedata(uodir) ;
		auto anim = animation(0,uodir) ;
		/*
		for (auto &entry : lightMask.lights()){
			auto output = std::ofstream((lightdir / std::filesystem::path(std::to_string(entry.first)+".bmp"s)));
			entry.second.saveBMP(output);
		}
		auto radarColor = radarcolor(uodir) ;
		auto tileInfo = tileinfo(uodir) ;
		
		auto multis = multicollection(uodir,"",&tileInfo);
		auto artWork = artwork(uodir) ;
		auto gumpArt = gumpart(uodir) ;
		auto texMap = texmap(uodir) ;
		auto map0 = uomap(0,&tileInfo);
		map0.load(uodir);
		auto map1 = uomap(1,&tileInfo);
		map1.load(uodir);
		auto map2 = uomap(2,&tileInfo);
		map2.load(uodir);
		auto map3 = uomap(3,&tileInfo);
		map3.load(uodir);
		auto map4 = uomap(4,&tileInfo);
		map4.load(uodir);
		auto map5 = uomap(5,&tileInfo);
		map5.load(uodir);
		 */
		for (const auto &graphic : anim.animations()){
			auto path = animdir / std::filesystem::path("anim-"s+std::to_string(graphic.first));
			std::filesystem::create_directory(path);
			auto frame = 0 ;
			for (const auto &entry : graphic.second){
				auto output = std::ofstream((path / std::filesystem::path("frame"s + std::to_string(frame)+".bmp")).string(), std::ios::binary);
				entry.saveBMP(output) ;
				frame++;
			}

		}
		return 0 ;
		/*
		auto artWork = artwork(uodir) ;
		auto path = datadir / std::filesystem::path("terrain") ;
		for (auto &entry : artWork.images(tiletype_t::terrain)){
			auto output = std::ofstream((path / std::filesystem::path(std::to_string(entry.first))).string()+".bmp"s,std::ios::binary);
			entry.second.saveBMP(output) ;
		}
		path = datadir / std::filesystem::path("art") ;
		for (auto &entry : artWork.images(tiletype_t::art)){
			auto output = std::ofstream((path / std::filesystem::path(std::to_string(entry.first))).string()+".bmp"s,std::ios::binary);
			entry.second.saveBMP(output) ;
		}

		auto gumpArt = gumpart(uodir) ;
		path = datadir / std::filesystem::path("gumpart");
		for (auto &entry : gumpArt.gumps()){
			auto output = std::ofstream((path / std::filesystem::path(std::to_string(entry.first))).string()+".bmp"s,std::ios::binary);
			entry.second.saveBMP(output,true) ;
		}
		*/
	}
	return rvalue ;
}
