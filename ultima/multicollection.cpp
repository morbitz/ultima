//Copyright © 2022 Charles Kerr. All rights reserved.

#include "multicollection.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>


#include "tileinfo.hpp"
using namespace std::string_literals;

//=========================================================
multicollection::multicollection(const std::filesystem::path &uodir ,const std::string &housingbin,const tileinfo *tileInfo ):_tileInfo(nullptr){
	if (!uodir.empty()){
		load(uodir,housingbin,tileInfo);
	}
}
//=========================================================
auto multicollection::load(const std::filesystem::path &uodir,const std::string &housingbin,const tileinfo *tileInfo) ->bool {
	_multis.clear();
	auto multifile = uodir / std::filesystem::path("MultiCollection.uop");
	auto hash = "build/multicollection/%.6u.bin"s;
	_housingbin = housingbin ;
	_tileInfo = tileInfo ;
	return loadUOP(multifile.string(), 0x10000, hash);
}

//=========================================================
auto multicollection::processHash(std::uint64_t hash,std::size_t entry , std::vector<std::uint8_t> &data) ->bool {
	if (hash == 0x126D1E99DDEDEE0ALL){
		if (!_housingbin.empty()){
			auto output = std::ofstream(_housingbin,std::ios::binary);
			if (output.is_open()){
				output.write(reinterpret_cast<char*>(data.data()),data.size());
			}
		}
		return false ;  // This is the hash for the housing.bin file
	}
	return true ;

}
//=========================================================
auto multicollection::processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool {
	if (data.size()>8) {
		auto offset = std::uint32_t(4) ;
		// We skip the first 32 bit word (why offset is 4).
		// The next 32 bit word is the number of entries in this data ;
		
		auto count = std::int32_t(0) ;
		std::copy(data.data()+offset,data.data()+offset+4,reinterpret_cast<std::uint8_t*>(&count));
		offset += 4; ;
		// we increment offfset now that we have read the count
		// at this point,we are now looking at the data for each entry ;
		// before we start looping, make some dummy read space we can get data
		// out of the vector
		auto flag = std::int16_t(0) ;
		auto clilocs = std::uint32_t(0) ;
		auto multi = collection_item() ;
		multi.items.resize(count);
		auto first = true ;
		for (auto entry=0 ; entry < count ; ++entry) {
			auto item = multi_item() ;
			// Get the tileid
			std::copy(data.begin()+offset, data.begin()+offset+2,reinterpret_cast<unsigned char*>(&item.tileid));
			if (_tileInfo != nullptr){
				item.info = &(_tileInfo->art(item.tileid));
			}
			offset += 2 ;
			// Now get the xoffset  --
			std::copy(data.begin()+offset, data.begin()+offset+2,reinterpret_cast<unsigned char*>(&flag));
			item.offsetx = static_cast<int>(flag) ;
			offset +=2 ;
			// Now get the yoffset
			std::copy(data.begin()+offset, data.begin()+offset+2,reinterpret_cast<unsigned char*>(&flag));
			item.offsety = static_cast<int>(flag) ;
			offset +=2 ;
			// Now get the z
			std::copy(data.begin()+offset, data.begin()+offset+2,reinterpret_cast<unsigned char*>(&flag));
			item.altitude = static_cast<int>(flag) ;
			offset +=2 ;
			// now we need to get the flag
			std::copy(data.begin()+offset, data.begin()+offset+2,reinterpret_cast<unsigned char*>(&flag));
			offset +=2 ;
			switch (flag){
				default:
				case 0:
					item.flag=1 ;
					break;
				case 1:
					break;
				case 257:
					item.flag = 0x800 ;
					break;
			}
			if (flag ==0){
				if (first){
					multi.max_y = item.offsety ;
					multi.min_y = item.offsety ;
					multi.max_x = item.offsetx ;
					multi.min_x = item.offsetx ;
					multi.max_alt = item.altitude ;
					multi.min_alt = item.altitude ;
					first = false ;
				}
				else {
					multi.max_y = std::max(item.offsety,multi.max_y);
					multi.min_y = std::max(item.offsety,multi.min_y);
					multi.max_x = std::max(item.offsetx,multi.max_x);
					multi.min_x = std::max(item.offsetx,multi.min_x);
					multi.max_alt = std::max(item.altitude,multi.max_alt);
					multi.min_alt = std::max(item.altitude,multi.min_alt);
				}
			}
			// we now have some clilocs to read past, have to find out how many
			std::copy(data.begin()+offset, data.begin()+offset+4,reinterpret_cast<unsigned char*>(&clilocs));
			offset += 4 ;
			// Now skip past them;
			offset += (clilocs*4) ;
			multi.items[entry] = std::move(item) ;
		}
		_multis.insert_or_assign(static_cast<int>(index), std::move(multi));
	}
	
	return true ;
}
//=========================================================
auto multicollection::size() const ->size_t {
	return _multis.size();
}
//=========================================================
auto multicollection::multis() const -> const std::unordered_map<int, collection_item>& {
	return _multis;
}
//=========================================================
auto multicollection::multis()  ->  std::unordered_map<int, collection_item>& {
	return _multis;
}
