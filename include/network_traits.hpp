#pragma once
#include "system.hpp"
#include <cinttypes>
#include "address_v4.hpp"
#include "address_v6.hpp"

namespace net {
	enum class IPVersion {
		UNUSED,
		IPv4,
		IPv6
	};
	
	template <IPVersion>
	struct address_info_;
	
	template<>
	struct address_info_<IPVersion::UNUSED> {
		using _Type = void;
	};

	template<>
	struct address_info_<IPVersion::IPv4> {
		using _Type = address_v4;
	};

	template<>
	struct address_info_<IPVersion::IPv6> {
		using _Type = address_v6;
	};

	template <IPVersion _Version>
	class network_traits {
	public:
		static const int32_t Family;
		using Address = typename address_info_<_Version>::_Type;
	};

	const int32_t network_traits<IPVersion::UNUSED>::Family = 0;
	const int32_t network_traits<IPVersion::IPv4>::Family = AF_INET;
	const int32_t network_traits<IPVersion::IPv6>::Family = AF_INET6;
} // namespace net