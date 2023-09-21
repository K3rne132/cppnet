#include "test/address_v4_tests.hpp"
#include "test/address_v6_tests.hpp"

using namespace test;

int main() {
	address_v4_tests::run();
	address_v6_tests::run();
}