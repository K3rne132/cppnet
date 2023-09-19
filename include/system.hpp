#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define getLastError() WSAGetLastError()
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH

#else
#define UNIX
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#define SOCKET uint64_t
#define INVALID_SOCKET UINT64_MAX
#define SOCKET_ERROR SO_ERROR
#define closesocket(fd) ::close(fd)
#define getLastError() errno

#endif