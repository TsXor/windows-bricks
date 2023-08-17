/*
  This file is originally written for test,
  but it suddenly hit me that some simple Python script was enough,
  so I threw it here.
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdint.h>

#define IP_V4(i1, i2, i3, i4) ((uint32_t)((i1 << 24) | (i2 << 16) | (i3 << 8) | i4))

namespace WSA {

typedef in6_addr ip_v6_t;
typedef uint32_t ip_v4_t;
typedef uint16_t port_t;
#define sockaddrv4_padding {0, 0, 0, 0, 0, 0, 0, 0}

union sockaddr_any {
    struct sockaddr base;
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
    struct sockaddr_in6_old in6_old;
};

class Error {
private:
    int _retcode;
    char* _msg;
    size_t _msglen;
    char* _what_content;
public:
    Error(const char* msg, size_t msglen) {
        this->_retcode = WSAGetLastError();
        this->_msglen = msglen;
        this->_msg = (char* )malloc((msglen + 1) * sizeof(char));
        memcpy(this->_msg, msg, msglen * sizeof(char));
        this->_msg[msglen] = '\0';
        this->_what_content = NULL;
    }
    Error(const char* msg):
        Error(msg, strlen(msg)) {}
    ~Error() {
        free(this->_msg);
        if (this->_what_content) free(this->_what_content);
    }
    int code(void) {
        return this->_retcode;
    }
    char* what(void) {
        if (!this->_what_content) {
            this->_what_content = (char* )malloc((this->_msglen + 1 + 32) * sizeof(char));
            sprintf(this->_what_content, "[WSAError %ld] %s", this->_retcode, this->_msg);
        }
        return this->_what_content;
    }
};

class Environment {
private:
    WSADATA _data;
public:
    Environment() {
        int retcode = WSAStartup(MAKEWORD(2, 2), &(this->_data));
        if (retcode != 0) throw Error("WSAStartup failed");
    }
    ~Environment() {
        WSACleanup();
    }
    WSADATA data(void) {
        return this->_data;
    }
};

class Socket {
private:
    SOCKET _socket;
    int _family;
    Socket(SOCKET id) {
        this->_socket = id;
    }
public:
    Socket(int protocol, int family = AF_INET, int type = SOCK_STREAM) {
        this->_socket = INVALID_SOCKET;
        this->_family = family;
        this->_socket = socket(family, type, protocol);
        if (this->_socket == INVALID_SOCKET) throw Error("failed to create socket");
    }
    ~Socket() {
        if (this->_socket != INVALID_SOCKET) closesocket(this->_socket);
    }
    void connect_(const sockaddr* addr) {
        int retcode = connect(this->_socket, addr, sizeof(sockaddr));
        if (retcode == SOCKET_ERROR) throw Error("failed to connect");
    }
    void connect_(ip_v4_t ip, port_t port) {
        struct in_addr inv4; inv4.S_un.S_addr = htonl(ip);
        sockaddr_in addr = {this->_family, htons(port), inv4, sockaddrv4_padding};
        this->connect_((sockaddr* )(&addr));
    }
    void bind_(const sockaddr* addr) {
        int retcode = bind(this->_socket, addr, sizeof(sockaddr));
        if (retcode == SOCKET_ERROR) throw Error("failed to bind");
    }
    void bind_(ip_v4_t ip, port_t port) {
        struct in_addr inv4; inv4.S_un.S_addr = htonl(ip);
        sockaddr_in addr = {this->_family, htons(port), inv4, sockaddrv4_padding};
        this->bind_((sockaddr* )(&addr));
    }
    void listen_(void) {
        int retcode = listen(this->_socket, SOMAXCONN);
        if (retcode == SOCKET_ERROR) throw Error("failed to listen");
    }
    Socket accept_(void) {
        SOCKET client_socket_id = INVALID_SOCKET;
        client_socket_id = accept(this->_socket, NULL, NULL);
        if (client_socket_id == INVALID_SOCKET) throw Error("failed to accpet");
        Socket client_socket = {client_socket_id};
        return client_socket;
    }
    template <typename T>
    int send_(const T* buf, int len) {
        int retcode = send(this->_socket, (const char* )buf, len, 0);
        if (retcode < 0) throw Error("failed to send");
        return retcode;
    }
    template <typename T>
    int recv_(T* buf, int len) {
        int retcode = recv(this->_socket, (char* )buf, len, 0);
        if (retcode < 0) throw Error("failed to recv");
        return retcode;
    }
    void shutdown_(int how) {
        int retcode = shutdown(this->_socket, how);
        if (retcode == SOCKET_ERROR) throw Error("failed to shutdown");
    }
};

} // namespace WSA