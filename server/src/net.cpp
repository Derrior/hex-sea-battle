#ifdef _WIN32
	#pragma comment ( lib, "ws2_32.lib" )
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif



#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <time.h>

    #define SYSTEM_PAUSE system("pause")

	typedef unsigned int in_addr_t;
	typedef int socklen_t;
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


#include <net/socket.h>
#include <net/net.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <vector>


#define MAX_CLIENTS 256
#define PORT 22123
#define GET_TCP(TYPE, VAL) do { \
    if (recv(local_tcp_socket, (void *)&VAL, sizeof(TYPE), MSG_WAITALL) <= 0) { \
        printf("Error while reveiving resources: cannot read from socket (%s)", strerror(errno)); \
        return 1; \
    } \
} while (0)


using namespace std;


client_t::client_t() {
    ships = new ship[amount_of_ships];
    alive = 1;
}


client_t::client_t(int n) {
    ships = new ship[amount_of_ships];
    alive = 1;
    num = n;
}

void client_t::fill_in(char *src) {
    char * prev_src = src;
    src = F.write_field(src);
    /* debug output
    for (char * c = prev_src; c != src; c++) {
        cout << (int)*c << ' ';
    }
    cout << endl;
    */
    for (int i = 0; i < amount_of_ships; i++) {
        prev_src = src;
        src = ships[i].write_ship(src);    
        /* debug output
        for (char * c = prev_src; c != src; c++) {
            cout << (int)*c << ' ';
        }
        cout << endl;
        */
    }
}


char message[BUFF_LEN];

SOCKET local_udp_socket;
int client_count;
vector<client_t> clients;

int init_net() {
    local_udp_socket = make_local_udp_socket(PORT);
    if (local_udp_socket == -1) {
        printf("Error while initializing network: cannot make local tcp socket");
        return 1;
    }
    return 0;
}

int update_net() {
    sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);
    memset(&src_addr, 0, sizeof(src_addr));
    int msg_len = 0, client_sock;
    int cnt = 1;
    /*
    SOCKET client_socket = accept(local_tcp_socket, (sockaddr*)&src_addr, &addrlen);
    if (client_socket > 0) {
        clients.push_back(client_t());
        clients.back().tcp = client_socket;
        clients.back().alive = 1;
        cout <<"somebody tries to connect!" << cnt << endl;

    }
    or (int i = 0; i < (int)clients.size(); i++) {
        if ((msg_len = recvfrom(clients[i].tcp, message, BUFF_LEN, MSG_DONTWAIT, (sockaddr *) &src_addr, &addrlen)) > 0) {
            cout << ' ' << message<< endl;
        }
    }
    */
    while ((msg_len = recvfrom(local_udp_socket, message, BUFF_LEN, MSG_DONTWAIT, (sockaddr *) &src_addr, &addrlen)) > 0) {
        if (message[0] == MSG_HELLO) {
            message[0] = OK;
            message[1] = client_count;
            clients.push_back(client_t(client_count));
            client_count++;
            sendto(local_udp_socket, message, 2, 0, (sockaddr *)&src_addr, addrlen);
        } else if (message[0] == MSG_CHECK) {
            if (clients.size() <= message[1] or message[1] < 0) {
                continue;
            }
            clients[message[1]].fill_in(message + 2);

            message[2] = check(clients[message[1]].F, clients[message[1]].ships);
            for (int i = 0; i < message[2]; i++) {
                message[i + 3] = clients[message[1]].F.bombs[i];
            }
            message[3 + message[2]] = 0;
            sendto(local_udp_socket, message, message[2] + 4, 0, (sockaddr *)&src_addr, addrlen);
            // output of all message only for debug
            /*
            cout << ' ' << message<< endl;
            for (int i = 0; i < msg_len; i++) {
                cout << (int)message[i] << ' ';
            }
            cout << endl;
            */
        }
    }
    return 0;
    
}
