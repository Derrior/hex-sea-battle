#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

int make_local_tcp_socket();
int make_local_udp_socket();

#ifdef __cplusplus
}
#endif

#endif // NET_SOCKET_H
