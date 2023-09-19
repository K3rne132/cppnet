#pragma once
#include <string>

namespace net {
	class address {
	protected:
		std::string Address;

	public:
		virtual const char* c_str() const { return Address.c_str(); }
		virtual bool make_address(const char* rawaddress) = 0;
		virtual bool isIPV4() const = 0;
		virtual bool isIPV6() const = 0;

		bool operator==(const address& rhs) const {
			return Address == rhs.Address &&
				isIPV4() == rhs.isIPV4() &&
				isIPV6() == rhs.isIPV6();
		}

		operator const char*() const {
			return Address.c_str();
		}
	};
} // namespace net