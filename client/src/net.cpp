

#ifdef _WIN32
	#pragma comment ( lib, "ws2_32.lib" )
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#include <stdlib.h>
#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <time.h>
	typedef unsigned int in_addr_t;
	typedef int socklen_t;
    #define SYSTEM_PAUSE system("pause")
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <errno.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <poll.h>

	#define WSAGetLastError() errno 
	#define closesocket(X) close(X)
	#define SOCKET_ERROR -1
	#define WSACleanup() ;
	#define HOSTENT hostent
	#define SYSTEM_PAUSE system("wait")
	typedef	int SOCKET;
#endif

#include <socket.h>
#include <engine.h>
#include <net.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <iostream>


#define MAX_CLIENTS 256
#define GET_TCP(TYPE, VAL) do { \
    if (recv(local_tcp_socket, (void *)&VAL, sizeof(TYPE), MSG_WAITALL) <= 0) { \
        printf("Error while reveiving resources: cannot read from socket (%s)", strerror(errno)); \
        return 1; \
    } \
} while (0)


using namespace std;

char message[1024 * 1024];
SOCKET local_udp_socket;
int client_num;
sockaddr_in server;
unsigned int server_addrlen;

int init_net(const char *hostname, unsigned short port) {
    local_udp_socket = make_local_udp_socket();
    if (local_udp_socket == -1) {
        printf("Error while initializing network: cannot make local tcp socket\n");
        return 1;
    }
    struct hostent *host = gethostbyname(hostname);
    if (!host) {
        printf("Error while initializing network: cannot get %s's ip address\n", hostname);
        return 1;
    }
    server_addrlen = sizeof(struct sockaddr_in);
    server.sin_family = AF_INET;
    server.sin_port = htons(port == 0 ? PORT : port);
    server.sin_addr.s_addr = *(in_addr_t *) host->h_addr_list[0];
    /*if (connect(local_tcp_socket, (sockaddr *)&server, server_addrlen) == -1) {
        printf("Error while initializing network: cannot connect to server %s\n", host->h_name);
        return 1;

    }*/
    message[0] = 'H';
    message[1] = 'e';
    message[2] = 'l';
    message[3] = 'l';
    sendto(local_udp_socket, message, 4, 0, (struct sockaddr *)&server, server_addrlen); 

    return 0;

}

int update_net() {
    if (check_pressed) {
        printf("here\n");
        char* ptr = message;
        ptr = field1.print_field(message);        
        for (int i = 0; i < amount_of_ships; i++) {
            char* prev_ptr = ptr;
            ptr = ships[i].print_ship(ptr);
            for (char* c = prev_ptr; c != ptr; c++) {
                printf("%d ", (int)*c);
            }
            printf("\n");
            check_pressed = false;
        }
        sendto(local_udp_socket, message, ptr - message, 0, (sockaddr *)&server, server_addrlen); 

    }
    

}
int free_net() {
//    closesocket(local_tcp_socket);
}
