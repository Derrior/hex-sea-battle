#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
int make_local_tcp_socket(unsigned short port);
int make_local_udp_socket(unsigned short port);
#ifdef __cplusplus
}
#endif

#endif // NET_SOCKET_H
