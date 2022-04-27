//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef multicollection_hpp
#define multicollection_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "ultima_types.hpp"
#include "uopfile.hpp"

class tileinfo ;
//=========================================================
class multicollection : public uopfile {
	const tileinfo *_tileInfo ;
	std::unordered_map<int, collection_item> _multis ;
	
	std::string _housingbin ;
	auto processEntry(std::size_t entry, std::size_t index, std::vector<std::uint8_t> &data) ->bool final ;
	auto processHash(std::uint64_t hash,std::size_t entry , std::vector<std::uint8_t> &data) ->bool final ;

public:
	multicollection(const std::filesystem::path &uodir = std::filesystem::path(),const std::string &housingbin="",const tileinfo *tileInfo = nullptr);
	auto load(const std::filesystem::path &uodir,const std::string &housingbin,const tileinfo *tileInfo) ->bool ;
	auto size() const ->size_t ;
	auto multis() const -> const std::unordered_map<int, collection_item>& ;
	auto multis()  ->  std::unordered_map<int, collection_item>& ;

};
#endif /* multicollection_hpp */
