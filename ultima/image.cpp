//Copyright © 2022 Charles Kerr. All rights reserved.

#include "image.hpp"

#include <iostream>
//===========================================================
// image_t
//=========================================================

//=========================================================
auto image_t::isGray(colorid_t color) const ->bool {
	auto red =(color>>10)&0x1f;
	return (red == ((color>>5) & 0x1f)) && (red == (color&0x1f)) ;
}
//=========================================================
auto image_t::hueIndex(colorid_t color) const ->int {
	return ((color>>10)&0x1f);
}

//=========================================================
image_t::image_t(int width, int height, colorid_t fill):_fill(fill){
	setSize(width, height,fill);
}
//=========================================================
auto image_t::fill(colorid_t value) ->void {
	_fill = value;
	for (auto &entry : data){
		entry = value ;
	}
}
//=========================================================
auto image_t::setSize(int width, int height, colorid_t fill)->void {
	_fill = fill ;
	this->width = width ;
	this->height = height ;
	data = std::vector<std::uint16_t>(width*height,fill);
}
//=========================================================
auto image_t::colorAt(int x, int y) ->colorid_t {
	return data[(y*width)+x];
}
//=========================================================
auto image_t::setColorAt(int x, int y, colorid_t value) ->void {
	data[(y*width)+x] = value ;
}
//=========================================================
auto image_t::saveBMP(std::ostream &output,bool invert) const ->void {
	auto temp32 = std::int32_t(0) ;
	auto temp16 = std::int16_t(0) ;
	auto padvalue = std::int16_t(0) ;
	auto shouldPad = (width%2==0?false:true) ;
	output <<'B' <<'M' ; // Write the signature
	// File size
	temp32 = ((width * height ) *2) + 54 ;
	if (shouldPad){
		temp32 += (width*2) ;
	}
	output.write(reinterpret_cast<char*>(&temp32),4);
	// unused
	temp32 = 0 ;
	output.write(reinterpret_cast<char*>(&temp32),4);
	// Write the offset from the beginning of the file to start of pixel data
	temp32 = 54  ; // infoheader(40) + signature(2)+ + file size(4) + offset(4)
	output.write(reinterpret_cast<char*>(&temp32),4);
	//****** write the info header **********
	temp32 = 40 ; // Size of the info header, 40 bytes
	output.write(reinterpret_cast<char*>(&temp32),4);
	// Write the width
	temp32 = static_cast<std::int32_t>(width)  ;
	output.write(reinterpret_cast<char*>(&temp32),4);
	// Write the height
	temp32 =static_cast<std::int32_t>(height) ;
	output.write(reinterpret_cast<char*>(&temp32),4);
	temp16 = 1 ; // Number of planes
	output.write(reinterpret_cast<char*>(&temp16),2);
	temp16 = 16 ; // Bits per pixel
	output.write(reinterpret_cast<char*>(&temp16),2);
	temp32 = 0 ; // compression, none
	output.write(reinterpret_cast<char*>(&temp32),4);
	// Image size compressed, 0 since no compresison
	output.write(reinterpret_cast<char*>(&temp32),4);
	// XpixelsPerM  Horizon resolution
	output.write(reinterpret_cast<char*>(&temp32),4);
	// YpixelsPerM  Vertical resolution
	output.write(reinterpret_cast<char*>(&temp32),4);
	// Colors used
	output.write(reinterpret_cast<char*>(&temp32),4);
	// Important colors
	output.write(reinterpret_cast<char*>(&temp32),4);

	// Write the data
	if (!invert) {
		for (auto i = 0;i<height;++i){
			output.write(reinterpret_cast<const char*>((data.data()+(i*width))),width*2);
			if(shouldPad){
				output.write(reinterpret_cast<const char*>(&padvalue),2);
			}
		}
	}
	else {
		 
		for (auto i = height;i>0;--i){
			output.write(reinterpret_cast<const char*>((data.data()+((i-1)*width))),width*2);
			if(shouldPad){
				output.write(reinterpret_cast<const char*>(&padvalue),2);
			}
		}
	}
	
}
//=========================================================
auto image_t::at(int col, int row) const ->const colorid_t & {
	return data[row*width + col] ;
}
//=========================================================
auto image_t::at(int col, int row) ->colorid_t & {
	return data[row*width + col] ;
}

//=========================================================
auto image_t::hue(const std::vector<colorid_t> &huevalues,bool partial) ->void {
	for (auto y = 0 ; y < height;++y){
		for (auto x=0 ; x<width;++x) {
			auto hue = false ;
			if (this->at(x, y) != _fill) {
				hue = true ;
				if (partial) {
					hue = isGray(this->at(x,y));
				}
			}
			if (hue) {
				this->at(x,y) = huevalues[hueIndex(this->at(x,y))];
			}
		}
	}
}
//===========================================================
auto image_t::empty() const ->bool {
	return (width==0) || (height==0);
}
//===========================================================
// animframe_t
//===========================================================
animframe_t::animframe_t(int width , int height,colorid_t fill):image_t(width,height,fill),image_center_x(0),image_center_y(0) {
}
