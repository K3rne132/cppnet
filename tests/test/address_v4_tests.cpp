#include "address_v4_tests.hpp"

void test::address_v4_tests::makeAddressFromString() {
	net::address_v4 address;
	const char* correct = "127.0.0.1";
	const char* zeros = "0.0.0.0";
	const char* full = "255.255.255.255";
	const char* incorrect = "255.255.255.256";
	const char* dots = "...";
	const char* triplezeros = "000.000.000.000";

	ASSERT_TRUE(address.make_address(correct));
	ASSERT_TRUE(std::strcmp(address, correct) == 0);
	ASSERT_TRUE(address.make_address(zeros));
	ASSERT_TRUE(std::strcmp(address, zeros) == 0);
	ASSERT_TRUE(address.make_address(full));
	ASSERT_TRUE(std::strcmp(address, full) == 0);
	ASSERT_FALSE(address.make_address(incorrect));
	ASSERT_FALSE(address.make_address(dots));
	ASSERT_FALSE(address.make_address(triplezeros));
}

void test::address_v4_tests::makeAddressFromBytes() {
	net::address_v4 address;
	unsigned char correct[4] = { 127, 0, 0, 1 };
	unsigned char zeros[4] = { 0, 0, 0, 0 };
	unsigned char full[4] = { 255, 255, 255, 255 };

	ASSERT_TRUE(address.make_address(correct));
	ASSERT_TRUE(std::strcmp(address, "127.0.0.1") == 0);
	ASSERT_TRUE(address.make_address(zeros));
	ASSERT_TRUE(std::strcmp(address, "0.0.0.0") == 0);
	ASSERT_TRUE(address.make_address(full));
	ASSERT_TRUE(std::strcmp(address, "255.255.255.255") == 0);
}