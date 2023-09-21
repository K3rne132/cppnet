#pragma once
#include <cinttypes>
#include "address.hpp"

namespace net {
	class address_v4 : public address {
	private:
		in_addr _Numeric = {};

	public:
		address_v4() = default;
		address_v4(const char* rawaddress) { make_address(rawaddress); }
		address_v4(uint8_t octets[4]) { make_address(octets); }

		// Returns address in bytes in network byte order
		in_addr bytes() const { return _Numeric; }

		// Makes address from raw string address represented as X.X.X.X
		// where X is a decimal value from 0 to 255
		virtual bool make_address(const char* rawaddress) override;

		// Makes address from four bytes in network byte order
		bool make_address(uint8_t bytes[4]);

		// Determines whether the address is IPV4
		virtual bool isIPv4() const override { return true; }

		// Determines whether the address is IPV4
		virtual bool isIPv6() const override { return false; }
	};
} // namespace net