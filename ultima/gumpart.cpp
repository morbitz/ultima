//Copyright © 2022 Charles Kerr. All rights reserved.

#include "gumpart.hpp"

#include <iostream>
#include <algorithm>

using namespace std::string_literals ;
//=========================================================
gumpart::gumpart(const std::filesystem::path &uodir ){
	if (!uodir.empty()){
		load(uodir);
	}
}
//=========================================================
bool gumpart::load(const std::filesystem::path &uodir) {
	auto path = uodir / std::filesystem::path("gumpartLegacyMUL.uop");
	return loadUOP(path.string(), 0x7FFFF, "build/gumpartlegacymul/%08u.tga","build/gumpartlegacymul/%07u.tga");
}
//=========================================================
auto gumpart::size() const ->size_t {
	return _gumpart.size();
}
//=========================================================
auto gumpart::operator[](int id) const ->const image_t & {
	return _gumpart.at(id) ;
}
//=========================================================
auto gumpart::operator[](int id)  -> image_t & {
	return _gumpart.at(id) ;
}
//=========================================================
auto gumpart::gumps() const ->const std::map<int,image_t>&{
	return _gumpart ;
}
//=========================================================
auto  gumpart::processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool {
	if (data.size() >= 8) {
		auto ptr = reinterpret_cast<const std::uint32_t*>(data.data());
		auto width = *ptr ;
		auto height = *(ptr+1) ;
		if ((width > 0 ) && (height>0)) {
			auto image = image_t(width,height,0xFFFF);
			std::vector<unsigned int> lookup(height,0) ;
			std::copy(data.data()+8,data.data()+8+(height*4),reinterpret_cast<unsigned char*>(lookup.data()));

			auto iter = lookup.begin() ;
			auto y = 0 ;
			while (iter != lookup.end()){
				auto amount = (data.size()-8) - ((*iter)*4) ;
				if ((iter+1) < lookup.end()){
					amount = ((*(iter+1)) - (*iter)) *4 ;
				}
				std::vector<unsigned short> rundata(amount/2,0);
				std::copy(data.data()+8+((*iter)*4),data.data()+8+((*iter)*4)+amount,reinterpret_cast<unsigned char*>(rundata.data()));
				auto piter = rundata.begin() ;
				auto x = 0 ;
				while (piter != rundata.end()){
					auto value = *(piter++);
					auto run = *(piter++) ;
					for (auto i=0 ; i< run ; i++){
						image.at(x++,y) = value ;
					}
				}
				y++ ;
				iter++;
			}
			_gumpart.insert_or_assign(static_cast<int>(index), image);
		}

	}
	return true ;
}

