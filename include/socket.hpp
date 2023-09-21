#pragma once
#include "system.hpp"
#include <cinttypes>
#include "address_v4.hpp"
#include "address_v6.hpp"
#include "network_traits.hpp"

#define SUCCESS 0
#define NONE 0

namespace net {
	template <int32_t _SockType, int32_t _Protocol, IPVersion _IPVersion,
		typename traits = network_traits<_IPVersion>,
		typename Address = typename network_traits<_IPVersion>::Address>
	class socket {
	private:
		SOCKET _Server; // Used only by server to listen
		SOCKET _Client; // Used by client and server to exchange data
		bool _Connected; // Checks whether client is connected or server accepted connection

	public:
		socket() : _Server(INVALID_SOCKET), _Client(INVALID_SOCKET),
			_Connected(false) {}

		virtual ~socket() {
			disconnect();
			close();
		}

		// Closes listening (server) socket
		void close() {
			if (_Server != INVALID_SOCKET) {
				shutdown(_Server, SHUT_RDWR);
				closesocket(_Server);
				_Server = INVALID_SOCKET;
			}
		}

		// Disconnects accepted/connected sockets
		void disconnect() {
			if (_Connected) {
				shutdown(_Client, SHUT_RDWR);
				closesocket(_Client);
				_Client = INVALID_SOCKET;
				_Connected = false;
			}
		}

		// Informs that client will not send data anymore
		void stopsend() {
			shutdown(_Client, SHUT_WR);
		}

		// Informs that client will not receive data anymore
		void stoprecv() {
			shutdown(_Client, SHUT_RD);
		}

		// Listens on port - limited connections by maxconn
		int32_t listen(const char* port, int32_t maxconn) {
			addrinfo hints = {};
			hints.ai_family = traits::Family;
			hints.ai_socktype = _SockType;
			hints.ai_protocol = _Protocol;
			hints.ai_flags = AI_PASSIVE;
			addrinfo* result = nullptr;

			int32_t error = getaddrinfo(NONE, port, &hints, &result);
			if (error)
				return error;

			_Server = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (_Server == INVALID_SOCKET) {
				freeaddrinfo(result);
				return getLastError();
			}

			error = ::bind(_Server, result->ai_addr, (int32_t)result->ai_addrlen);
			if (error == SOCKET_ERROR) {
				freeaddrinfo(result);
				close();
				return getLastError();
			}

			error = ::listen(_Server, maxconn);
			if (error == SOCKET_ERROR) {
				freeaddrinfo(result);
				close();
				return getLastError();
			}
			return SUCCESS;
		}

		// Accepts connection by listening server
		int32_t accept() {
			_Client = ::accept(_Server, NONE, NONE);
			if (_Client == INVALID_SOCKET)
				return getLastError();
			_Connected = true;
			return SUCCESS;
		}

		// Connects client to address on specified port
		int32_t connect(Address address, const char* port) {
			addrinfo hints = {};
			hints.ai_family = traits::Family;
			hints.ai_socktype = _SockType;
			hints.ai_protocol = _Protocol;
			addrinfo* result = nullptr;
			addrinfo* ptr = nullptr;
			int32_t error = getaddrinfo(address, port, &hints, &result);

			if (error)
				return error;
			for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
				_Client = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
				if (_Client == INVALID_SOCKET) {
					freeaddrinfo(result);
					return getLastError();
				}

				error = ::connect(_Client, result->ai_addr, (int32_t)result->ai_addrlen);
				if (error == SOCKET_ERROR) {
					closesocket(_Client);
					_Client = INVALID_SOCKET;
					continue;
				}
				break;
			}
			freeaddrinfo(result);
			_Connected = true;
			return SUCCESS;
		}

		// Sends data from buffer with specified size
		void send(const char* buffer, int32_t size) {
			::send(_Client, buffer, size, NONE);
		}

		// Receives data from accepted socket, return false if there
		// will be no more data to receive
		bool recv(char* buffer, int32_t size) {
			int32_t upcoming = ::recv(_Client, buffer, size - 1, MSG_PEEK);
			int32_t length = ::recv(_Client, buffer, size - 2, NONE);
			if (upcoming == length) {
				buffer[length] = 0;
				return false;
			}
			return true;
		}
	};

	static int32_t initialize() {
	#ifdef WINDOWS
		WSADATA wsaData;
		int32_t error = WSAStartup(MAKEWORD(2, 2), &wsaData);
		return error;
	#else
		return SUCCESS;
	#endif
	}

	using socket_tcp = socket<SOCK_STREAM, IPPROTO_TCP, IPVersion::IPv4>;
	using socket_tcp_v6 = socket<SOCK_STREAM, IPPROTO_TCP, IPVersion::IPv6>;
	using socket_udp = socket<SOCK_DGRAM, IPPROTO_UDP, IPVersion::IPv4>;
	using socket_udp_v6 = socket<SOCK_DGRAM, IPPROTO_UDP, IPVersion::IPv6>;
} // namespace net