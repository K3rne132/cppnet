#pragma once
#include "../test.hpp"
#include "address_v6.hpp"

namespace test {
	class address_v6_tests {
	private:
		static void makeAddressFromString();
		static void makeAddressFromBytes();

	public:
		static void run() {
			makeAddressFromString();
			makeAddressFromBytes();
		}
	};
}