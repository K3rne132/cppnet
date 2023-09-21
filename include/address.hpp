#pragma once
#include "system.hpp"
#include <string>

namespace net {
	class address {
	protected:
		std::string Address;

	public:
		virtual const char* c_str() const { return Address.c_str(); }
		virtual bool make_address(const char* rawaddress) = 0;
		virtual bool isIPv4() const = 0;
		virtual bool isIPv6() const = 0;

		bool operator==(const address& rhs) const {
			return Address == rhs.Address &&
				isIPv4() == rhs.isIPv4() &&
				isIPv6() == rhs.isIPv6();
		}

		operator const char*() const {
			return Address.c_str();
		}
	};
} // namespace net