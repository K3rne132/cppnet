#include <list>
#include <sstream>
#include "../include/address_v6.hpp"

bool net::address_v6::make_address(const char* rawaddress) {
	std::string helper(rawaddress);
	std::list<std::string> values;
	uint8_t words = 0;
	size_t oldoffset = 0;
	size_t offset = 0;
	uint8_t specialchar = UINT8_MAX;

	while (words <= 8) { // prevents from unnecessary iterating
		offset = helper.find(':', offset);
		if (offset == std::string::npos) {
			std::string tmp = helper.substr(oldoffset, helper.length() - oldoffset);
			if (tmp.empty()) {
				break;
			}
			words++;
			values.push_back(helper.substr(oldoffset, helper.length() - oldoffset));
			break;
		}
		std::string tmp = helper.substr(oldoffset, offset - oldoffset);
		if (tmp.empty()) {
			specialchar = words;
			offset++;
			oldoffset = offset;
			continue;
		}
		words++;
		values.push_back(tmp);
		offset++;
		oldoffset = offset;
	}
	if (specialchar < 8) {
		auto it = values.begin();
		for (uint8_t i = 0; i < specialchar; i++)
			it++;
		uint8_t end = 8 - words;
		for (uint8_t i = 0; i < end; i++) {
			words++;
			values.insert(it, "0");
		}
	}
	if (words != 8)
		return false;
	std::list<uint16_t> numeric;
	for (auto& value : values) {
		int32_t check = strtol(value.c_str(), nullptr, 16);
		if (check == 0 && value != "0" && value != "0000")
			return false;
		numeric.push_back(check);
	}
	for (uint8_t i = 0; i < 8; i++) {
		_Numeric[i] = numeric.front();
		numeric.pop_front();
	}
	Address.clear();
	std::stringstream stream;
	for (uint8_t i = 0; i < 8; i++) {
		stream << std::hex << _Numeric[i];
		if (i != 7)
			stream << ":";
	}
	Address = stream.str();
	return true;
}

void net::address_v6::make_address(uint16_t words[8]) {
	make_address(IPv6Numeric(words));
}

void net::address_v6::make_address(IPv6Numeric numeric) {
	_Numeric = numeric;
	Address.clear();
	std::stringstream stream;
	for (uint8_t i = 0; i < 8; i++) {
		stream << std::hex << numeric[i];
		if (i != 7)
			stream << ':';
	}
	Address = stream.str();
}