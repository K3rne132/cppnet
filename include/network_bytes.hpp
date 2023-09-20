#pragma once
#include <cinttypes>

namespace net {
	template <class T, uint8_t _count>
	class network_bytes {
	private:
		T _Numeric[_count] = { 0 };

	public:
		network_bytes() = default;
		network_bytes(T units[_count]) {
			read_bytes(units);
		}

		void read_bytes(T units[_count]) {
			for (uint8_t i = 0; i < _count; i++) {
				_Numeric[i] = units[i];
			}
		}

		T& operator[](uint8_t pos) {
			return _Numeric[pos];
		}
	};

	typedef network_bytes<uint8_t, 4> IPv4Numeric;
	typedef network_bytes<uint16_t, 8> IPv6Numeric;
} // namespace net