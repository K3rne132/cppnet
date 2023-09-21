#pragma once
#include "../test.hpp"
#include "address_v4.hpp"

namespace test {
	class address_v4_tests {
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