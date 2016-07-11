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
#include <set>

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
    can_go = 0;
    best_opponent = -1;
}


client_t::client_t(int n) {
    ships = new ship[amount_of_ships];
    mode = 0;
    alive = 1;
    num = n;
    memset(name, 0, 128);
    can_go = 0;
    best_opponent = -1;
}

client_t::client_t(int n, int battle) {
    ships = new ship[amount_of_ships];
    mode = 0;
    alive = 1;
    num = n;
    battle_idx = battle;
    memset(name, 0, 128);
    can_go = 0;
    best_opponent = -1;
}

void client_t::fill_in(char *src) {
    //char * prev_src = src;
    src = F.write_field(src);
    /* debug output
    for (char * c = prev_src; c != src; c++) {
        cout << (int)*c << ' ';
    }
    cout << endl;
    */
    for (int i = 0; i < amount_of_ships; i++) {
        //prev_src = src;
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
client_t clients[128];
set<int> unused_numbers;
bool is_unused_number[128];

int init_net() {
    local_udp_socket = make_local_udp_socket(PORT);
    if (local_udp_socket == -1) {
        printf("Error while initializing network: cannot make local tcp socket");
        return 1;
    }
    for (int i = 0; i < 128; i++) {
        unused_numbers.insert(i);
        is_unused_number[i] = true;
    }
    return 0;
}

int check_query() {
    if (message[1] < 0 or is_unused_number[message[1]] or clients[message[1]].mode != SHIP_MODE) {
        return -1;
    }
    clients[message[1]].fill_in(message + 2);

    message[2] = check(clients[message[1]].F, clients[message[1]].ships);
    for (int i = 0; i < message[2]; i++) {
        message[i + 3] = clients[message[1]].F.bombs[i];
    }
    message[3 + message[2]] = 0;
    clients[message[1]].can_go = (message[2] == 0);
    // output of all message only for debug
    /*
    cout << ' ' << message<< endl;
    for (int i = 0; i < msg_len; i++) {
        cout << (int)message[i] << ' ';
    }
    cout << endl;
    */
    return message[2] + 4;

}
int update_query() {
    message[0] = OK;
    char* ptr = message + 3;
    if (clients[message[1]].mode == INIT_MODE) {
        message[2] = client_count - 1;
        
        for (int i = 0; i < 128; i++) {
            if (i == message[1] or is_unused_number[i]) {
                continue;
            }
            *ptr = clients[i].name_len;
            memcpy(ptr + 1, clients[i].name, *ptr);
            ptr += (*ptr) + 1;
            *ptr = 0;
            bool cond1 = clients[i].best_opponent == message[1], cond2 = clients[message[1]].best_opponent == i;
            if (cond1 and cond2) {
                *ptr = 3;
            } else if (cond1) {
                *ptr = 1;
            } else if (cond2) {
                *ptr = 2;
            }
            ptr++;
        }
    }
    *ptr = clients[message[1]].can_go;
    ptr++;
    clients[message[1]].can_go = false;
    return ptr - message;
}

int BOC_query() {
    message[0] = OK;
    int i, cnt = 0;
    for (i = 0; i < 128 and cnt <= message[2]; i++) {
        if (is_unused_number[i]) {
            continue;
        }
        if (i != message[1]) {
            cnt++;
        }
    }
    i--;
    if (is_unused_number[i]) {
        message[0] = 0;
    } else {
        clients[message[1]].best_opponent = i;

    }

    cout << "player number " << (int)message[1] << " say bo is " << (int)message[2] << "; for us is :" << i << endl;
    return 2;
}

int update_net() {
    sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);
    memset(&src_addr, 0, sizeof(src_addr));
    int msg_len = 0;
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
            int curr_client_num = *unused_numbers.begin();
            clients[curr_client_num] = client_t(curr_client_num);
            clients[curr_client_num].name_len = message[1];
            memcpy(clients[curr_client_num].name, message + 2, message[1]);
            message[1] = curr_client_num;
            clients[curr_client_num].last_update = curr_time;
            cout << "client named " << clients[curr_client_num].name << " connected, users: " << client_count + 1 << endl;
            client_count++;
            is_unused_number[curr_client_num] = false;
            unused_numbers.erase(unused_numbers.begin());
            sendto(local_udp_socket, message, 2, 0, (sockaddr *)&src_addr, addrlen);

        } else if (message[0] == MSG_CHECK) {
            message[0] = OK;
            msg_len = check_query();
            sendto(local_udp_socket, message, msg_len, 0, (sockaddr *)&src_addr, addrlen);

        } else if (message[0] == MSG_GO) {
            message[0] = OK;
            message[2] = clients[message[1]].can_go;
            if (clients[message[1]].can_go) {
                next_mode(clients[message[1]].mode);
                cout << "client №" << (int)message[1] << " changed mode to " << clients[message[1]].mode << '\n';
            }
            sendto(local_udp_socket, message, 3, 0, (sockaddr *)&src_addr, addrlen);

        } else if (message[0] == MSG_SHOT) {
            if (clients[message[1]].mode != BATTLE_MODE) {
                message[0] = 0;
            } else {
                message[0] = OK;
            }

        } else if (message[0] == MSG_UPDATE) {
            clients[message[1]].last_update = curr_time;
            msg_len = update_query();
            sendto(local_udp_socket, message, msg_len, 0, (sockaddr *)&src_addr, addrlen);

        } else if (message[0] == MSG_BOC) {
            msg_len = BOC_query();
            sendto(local_udp_socket, message, msg_len, 0, (sockaddr *)&src_addr, addrlen);

        }
    }
    for (int i = 0; i < 128; i++) {
        if (!is_unused_number[i] and curr_time - clients[i].last_update > MAX_NOT_UPDATE) {
            printf("client number %d disconnected\n", clients[i].num);
            unused_numbers.insert(clients[i].num);
            is_unused_number[i] = true;
            client_count--;
        }
    }
    return 0;
    
}
