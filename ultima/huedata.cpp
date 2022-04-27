//Copyright © 2022 Charles Kerr. All rights reserved.

#include "huedata.hpp"

#include <iostream>
#include <fstream>

//=========================================================
huedata::huedata(const std::filesystem::path &uodir ){
	if (!uodir.empty()){
		load(uodir) ;
	}
}
//=========================================================
auto huedata::load(const std::filesystem::path &uodir) ->bool {
	auto rvalue = false ;
	_hues.clear() ;
	auto huefile = uodir / std::filesystem::path("hues.mul");
	if (std::filesystem::exists(huefile)){
		auto count = (std::filesystem::file_size(huefile)/2)/32 ;
		auto input = std::ifstream(huefile.string(),std::ios::binary);
		if (input.is_open()){
			rvalue = true ;
			_hues.resize(count);
			for (auto i=0;i<count ; ++i){
				_hues[i].resize(32,0);
				input.read(reinterpret_cast<char*>(_hues[i].data()),64) ;
			}
		}
	}
	return rvalue ;
}

//=========================================================
auto huedata::size() const ->size_t {
	return _hues.size() ;
}

//=========================================================
auto huedata::hues() const ->const std::vector<std::vector<colorid_t>>& {
	return _hues ;
}
//=========================================================
auto huedata::hues()  -> std::vector<std::vector<colorid_t>>& {
	return _hues;
}

//=========================================================
auto huedata::operator[](int hueid) const ->const std::vector<colorid_t>& {
	return _hues[hueid];
}
//=========================================================
auto huedata::operator[](int hueid)  -> std::vector<colorid_t>& {
	return _hues[hueid] ;
}

