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
    mode = 0;
    alive = 1;
    memset(name, 0, 128);
}


client_t::client_t(int n) {
    ships = new ship[amount_of_ships];
    mode = 0;
    alive = 1;
    num = n;
    memset(name, 0, 128);
}

client_t::client_t(int n, int battle) {
    ships = new ship[amount_of_ships];
    mode = 0;
    alive = 1;
    num = n;
    battle_idx = battle;
    memset(name, 0, 128);
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

int check_query() {
    if (clients.size() <= message[1] or message[1] < 0) {
        return 1;
    }
    clients[message[1]].fill_in(message + 2);

    message[2] = check(clients[message[1]].F, clients[message[1]].ships);
    for (int i = 0; i < message[2]; i++) {
        message[i + 3] = clients[message[1]].F.bombs[i];
    }
    message[3 + message[2]] = 0;
    // output of all message only for debug
    /*
    cout << ' ' << message<< endl;
    for (int i = 0; i < msg_len; i++) {
        cout << (int)message[i] << ' ';
    }
    cout << endl;
    */
    return 0;

}
int update_query() {
    message[0] = OK;
    message[2] = client_count - 1;
    
    char* ptr = message + 3;
    for (int i = 0; i < client_count; i++) {
        if (i == message[1]) {
            continue;
        }
        *ptr = clients[i].name_len;
        memcpy(ptr + 1, clients[i].name, *ptr);
        ptr += (*ptr) + 1;
    }
    return ptr - message;
}

int update_net() {
    sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);
    memset(&src_addr, 0, sizeof(src_addr));
    int msg_len = 0, client_sock;
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
            clients.push_back(client_t(client_count));
            clients.back().name_len = message[1];
            memcpy(clients.back().name, message + 2, message[1]);
            message[1] = client_count;
            cout << "client named " << clients.back().name << " connected, users: " << client_count + 1 << endl;
            client_count++;
            
            sendto(local_udp_socket, message, 2, 0, (sockaddr *)&src_addr, addrlen);
        } else if (message[0] == MSG_CHECK) {
            message[0] = OK;
            check_query();
            sendto(local_udp_socket, message, message[2] + 4, 0, (sockaddr *)&src_addr, addrlen);
        } else if (message[0] == MSG_GO) {
            message[0] = OK;
            check_query();
            if (message[2] == 0) {
                next_mode(clients[message[1]].mode);
                cout << "client №" << (int)message[1] << " changed mode to " << clients[message[1]].mode << '\n';
            }
            sendto(local_udp_socket, message, message[2] + 4, 0, (sockaddr *)&src_addr, addrlen);
        } else if (message[0] == MSG_SHOT) {
            if (clients[message[1]].mode != BATTLE_MODE) {
                message[0] = 0;
            } else {
                message[0] = OK;
            }
        } else if (message[0] == MSG_UPDATE) {
            int msg_size = update_query();
            sendto(local_udp_socket, message, msg_size + 1, 0, (sockaddr *)&src_addr, addrlen);
        }
    }
    return 0;
    
}
