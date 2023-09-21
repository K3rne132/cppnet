#include "address_v6_tests.hpp"

void test::address_v6_tests::makeAddressFromString() {
	net::address_v6 address;
	const char* correct = "0FE0::0001";
	const char* zeros = "::";
	const char* full = "ffff:fFFf:ffFF:FFff:fFfF:FFfF:FFFf:FFFF";
	const char* incorrect = "F::1::";

	ASSERT_TRUE(address.make_address(correct));
	ASSERT_TRUE(std::strcmp(address, "fe0::1") == 0);
	ASSERT_TRUE(address.make_address(zeros));
	ASSERT_TRUE(std::strcmp(address, zeros) == 0);
	ASSERT_TRUE(address.make_address(full));
	ASSERT_TRUE(std::strcmp(address,
		"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff") == 0);
	ASSERT_FALSE(address.make_address(incorrect));
}

void test::address_v6_tests::makeAddressFromBytes() {
	net::address_v6 address;
	unsigned short correct[8] = { 0xE00F, 0, 0, 0, 0, 0, 0, 0x100 };
	unsigned short zeros[8] = { 0 };
	unsigned short full[8];
	std::memset(full, 0xFF, 16);

	ASSERT_TRUE(address.make_address(correct));
	ASSERT_TRUE(std::strcmp(address, "fe0::1") == 0);
	ASSERT_TRUE(address.make_address(zeros));
	ASSERT_TRUE(std::strcmp(address, "::") == 0);
	ASSERT_TRUE(address.make_address(full));
	ASSERT_TRUE(std::strcmp(address,
		"ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff") == 0);
}