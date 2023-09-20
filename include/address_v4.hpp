#pragma once
#include <cinttypes>
#include "address.hpp"
#include "network_bytes.hpp"

namespace net {
	class address_v4 : public address {
	private:
		IPv4Numeric _Numeric;

	public:
		address_v4() = default;
		address_v4(const char* rawaddress) { make_address(rawaddress); }
		address_v4(uint8_t octets[4]) { make_address(octets); }
		address_v4(IPv4Numeric numeric) { make_address(numeric); }

		// Returns address in bytes in network byte order
		IPv4Numeric bytes() const { return _Numeric; }

		// Makes address from raw string address represented as X.X.X.X
		// where X is a decimal value from 0 to 255
		virtual bool make_address(const char* rawaddress) override;

		// Makes address from four octets in network byte order
		void make_address(uint8_t octets[4]);
		void make_address(IPv4Numeric numeric);

		// Determines whether the address is IPV4
		virtual bool isIPV4() const override { return true; }

		// Determines whether the address is IPV4
		virtual bool isIPV6() const override { return false; }
	};
} // namespace net