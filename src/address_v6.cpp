#include <list>
#include "../include/address_v6.hpp"

bool net::address_v6::make_address(const char* rawaddress) {
	int32_t result = inet_pton(AF_INET6, rawaddress, &_Numeric);
	if (result != 1)
		return false;
	Address.reserve(46);
	if (inet_ntop(AF_INET6, &_Numeric, Address.data(), 46))
		return true;
	return false;
}

bool net::address_v6::make_address(uint16_t words[8]) {
	char buffer[46];
	if (!inet_ntop(AF_INET6, words, buffer, 46))
		return false;
	return make_address(buffer);
}