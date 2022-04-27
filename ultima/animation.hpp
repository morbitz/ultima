//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef animation_hpp
#define animation_hpp

#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <istream>

#include "ultima_types.hpp"
#include "image.hpp"


//=========================================================
class animation {
	std::unordered_map<int, std::vector<animframe_t> > _animations ;
	int _anim_number ;
	auto processFrame(std::istream &input, const std::vector<colorid_t> &pallette, int width,int height, int image_x,int image_y) const ->animframe_t ;
public:
	static constexpr auto number_animation=5 ;
	animation(int anim_number=1, const std::filesystem::path &uodir = std::filesystem::path());
	auto load(const std::filesystem::path &uodir) ->bool ;
	
	auto size() const ->size_t ;
	auto animations() const ->const std::unordered_map<int, std::vector<animframe_t> >& ;
	auto animations()  -> std::unordered_map<int, std::vector<animframe_t> >& ;
	auto operator[](int index) const ->const std::vector<animframe_t> & ;
	auto operator[](int index)  -> std::vector<animframe_t> & ;

};
#endif /* animation_hpp */
