#include <list>
#include "../include/address_v4.hpp"

bool net::address_v4::make_address(const char* rawaddress) {
	int32_t result = inet_pton(AF_INET, rawaddress, &_Numeric);
	if (result != 1)
		return false;
	Address.reserve(16);
	if (inet_ntop(AF_INET, &_Numeric, Address.data(), 16))
		return true;
	return false;
}

bool net::address_v4::make_address(uint8_t bytes[4]) {
	char buffer[16];
	if (!inet_ntop(AF_INET, bytes, buffer, 16))
		return false;
	return make_address(buffer);
}