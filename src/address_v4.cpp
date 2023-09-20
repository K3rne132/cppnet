#include <list>
#include "../include/address_v4.hpp"

bool net::address_v4::make_address(const char* rawaddress) {
	std::string helper(rawaddress);
	std::list<std::string> values;
	uint8_t octets = 0;
	size_t oldoffset = 0;
	size_t offset = 0;

	while (octets <= 4) { // prevents from unnecessary iterating
		offset = helper.find('.', offset);
		if (offset == std::string::npos) {
			octets++;
			values.push_back(helper.substr(oldoffset, helper.length() - oldoffset));
			break;
		}
		octets++;
		values.push_back(helper.substr(oldoffset, offset - oldoffset));
		offset++;
		oldoffset = offset;
	}

	if (octets != 4)
		return false;
	std::list<uint8_t> numeric;
	for (auto& value : values) {
		int32_t check = strtol(value.c_str(), nullptr, 10);
		if (check == 0 && value != "0")
			return false;
		if (check < 0 || check > 255)
			return false;
		numeric.push_back(check);
	}
	for (uint8_t i = 0; i < 4; i++) {
		_Numeric[i] = numeric.front();
		numeric.pop_front();
	}
	Address = rawaddress;
	return true;
}

void net::address_v4::make_address(uint8_t octets[4]) {
	make_address(IPv4Numeric(octets));
}

void net::address_v4::make_address(IPv4Numeric numeric) {
	_Numeric = numeric;
	Address.clear();
	for (uint8_t i = 0; i < 4; i++) {
		Address += std::to_string(numeric[i]);
		if (i != 3)
			Address += '.';
	}
}