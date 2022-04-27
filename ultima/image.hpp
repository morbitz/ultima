//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef image_hpp
#define image_hpp

#include <cstdint>
#include <vector>
#include <ostream>
#include "ultima_types.hpp"
//=========================================================
struct image_t {
protected:
	colorid_t _fill ;
	auto isGray(colorid_t color) const ->bool ;
	auto hueIndex(colorid_t color) const ->int ;
public:
	int width ;
	int height ;
	std::vector<colorid_t> data ;
	image_t(int width=0, int height=0, colorid_t fill=0);
	auto fill(colorid_t value) ->void ;
	auto setSize(int width, int height, colorid_t fill=0)->void ;
	auto colorAt(int x, int y) ->colorid_t ;
	auto setColorAt(int x, int y, colorid_t value) ->void ;
	auto saveBMP(std::ostream &output,bool invert = true) const ->void ;
	auto at(int col, int row) const ->const colorid_t & ;
	auto at(int col, int row) ->colorid_t & ;
	auto hue(const std::vector<colorid_t> &huevalues,bool partial) ->void ;
	auto empty() const ->bool ;
};

struct animframe_t : public image_t {
	int image_center_x ;
	int image_center_y ;
	animframe_t(int width = 0, int height=0,colorid_t fill=0) ;
};
#endif /* image_hpp */
