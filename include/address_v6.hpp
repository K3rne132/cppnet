#pragma once
#include <cinttypes>
#include "address.hpp"

namespace net {
	class address_v6 : public address {
	private:
		in6_addr _Numeric = {};

	public:
		address_v6() = default;
		address_v6(const char* rawaddress) { make_address(rawaddress); }
		address_v6(uint16_t words[8]) { make_address(words); }

		// Returns address in bytes in network byte order
		in6_addr bytes() const { return _Numeric; }

		// Makes address from raw string address represented as X:X:X:X:X:X:X:X
		// where X is a hexadecimal value from 0000h to FFFFh, address may also
		// include one :: special character to skip blocks of 0s
		virtual bool make_address(const char* rawaddress) override;

		// Makes address from eight words in network byte order
		bool make_address(uint16_t words[8]);

		// Determines whether the address is IPV4
		virtual bool isIPv4() const override { return false; }

		// Determines whether the address is IPV4
		virtual bool isIPv6() const override { return true; }
	};
} // namespace net