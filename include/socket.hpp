#pragma once
#include "system.hpp"
#include <cinttypes>
#include "address_v4.hpp"
#include "address_v6.hpp"

#define SUCCESS 0
#define NONE 0

namespace net {
	template <int32_t _SockType, int32_t _Protocol>
	class socket {
	private:
		SOCKET _Server; // Used only by server to listen
		SOCKET _Client; // Used by client and server to exchange data
		bool _Connected; // Checks whether client is connected or server accepted connection

		int32_t connect(const char* address, const char* port, int32_t ip) {
			addrinfo hints = {};
			hints.ai_family = ip;
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
			return SUCCESS;
		}

		int32_t listen(const char* port, int32_t ipversion) {
			addrinfo hints = {};
			hints.ai_family = ipversion;
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
			error = ::listen(_Server, SOMAXCONN);
			if (error == SOCKET_ERROR) {
				freeaddrinfo(result);
				close();
				return getLastError();
			}
			return SUCCESS;
		}

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

		// Listens on port IPv4
		int32_t listen(const char* port) {
			return listen(port, AF_INET);
		}

		// Listens on port IPv6
		int32_t listen_v6(const char* port) {
			return listen(port, AF_INET6);
		}

		// Accepts connection by listening server
		int32_t accept() {
			_Client = ::accept(_Server, NONE, NONE);
			if (_Client == INVALID_SOCKET)
				return getLastError();
			_Connected = true;
			return SUCCESS;
		}

		// Connects client to IPv4 address on specified port
		int32_t connect(address_v4 address, const char* port) {
			int32_t result = connect(address.c_str(), port, AF_INET);
			if (result == SUCCESS)
				_Connected = true;
			return result;
		}

		// Connects client to IPv6 address on specified port
		int32_t connect(address_v6 address, const char* port) {
			int32_t result = connect(address.c_str(), port, AF_INET6);
			if (result == SUCCESS)
				_Connected = true;
			return result;
		}

		// Sends data from buffer with specified size
		void send(const char* buffer, int32_t size) {
			::send(_Client, buffer, size, NONE);
		}

		// Receives data from accepted socket, need to pass receiving = true
		// to derermine whether accepted/connected socket is sending data
		void recv(char* buffer, int32_t size, bool& receiving) {
			int32_t upcoming = ::recv(_Client, buffer, size - 1, MSG_PEEK);
			int32_t length = ::recv(_Client, buffer, size - 2, NONE);
			if (upcoming == length) {
				receiving = false;
				buffer[length] = 0;
			}
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

	typedef socket<SOCK_STREAM, IPPROTO_TCP> socket_tcp;
	typedef socket<SOCK_DGRAM, IPPROTO_UDP> socket_udp;
} // namespace net