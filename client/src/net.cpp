

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


char message[BUFF_LEN];
SOCKET local_udp_socket;
int my_number;
long double last_upd_time;
sockaddr_in server;
unsigned int server_addrlen;

int recvfrom_timeout() {
    fd_set readfds;
//    readfds.fd_array = (SOCKET *)malloc(readfds.fd_count * sizeof(SOCKET));
    FD_ZERO(&readfds);
    FD_SET(local_udp_socket, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 500;

    if (select(local_udp_socket + 1, &readfds, NULL, NULL, &timeout) > 0) {
        return recvfrom(local_udp_socket, message, BUFF_LEN, MSG_WAITALL, (sockaddr *) &server, &server_addrlen);
    } else {
        return -1;
    }

}

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
    int name_len = strlen(name);
    message[0] = MSG_HELLO;
    message[1] = name_len;
    memcpy(message + 2, name, name_len);
    sendto(local_udp_socket, message, 2 + name_len, 0, (struct sockaddr *)&server, server_addrlen); 
    int msg_len = 0;
    if ((msg_len = recvfrom_timeout()) > 0) {
        if (message[0] == OK and msg_len == 2) {
            
            my_number = message[1];
        } else {
            printf("Failed to connect: bad answer: %d %d \n", (int)message[0], (int)message[1]);
            return 1;
        }
    } else {
        printf("Failed to connect: no answer\n");
        return 1;
    }
    return 0;
}

int check_query(char type) {
    char* ptr = message + 2;
    message[0] = type;
    message[1] = my_number;
    ptr = field1.print_field(ptr);        
    for (char* c = message + 2; c != ptr; c++) {
        printf("%d ", (int)*c);
    }
    printf("\n");
    for (int i = 0; i < amount_of_ships; i++) {
        char* prev_ptr = ptr;
        ptr = ships[i].print_ship(ptr);
        for (char* c = prev_ptr; c != ptr; c++) {
            printf("%d ", (int)*c);
        }
        printf("\n");
    }
    sendto(local_udp_socket, message, ptr - message, 0, (sockaddr *)&server, server_addrlen); 
    int msg_len = recvfrom_timeout();
    if (msg_len == -1) {
        return 1;
    }
    cout << (int)message[0] << ' ' << (int)message[1] << ' ' << (int)my_number << endl;
    if (message[0] != OK or message[1] != my_number) {
        return 1;
    }
    field1.bombs.resize(message[2]);
    for (int i = 0; i < message[2]; i++) {
        field1.bombs[i] = message[3 + i];
    }
    check_result = (message[2] == 0);
    return 0;
}

int shoot_query() {
    message[0] = MSG_SHOT;
    message[1] = my_number;
    message[2] = shoot_cell;
    sendto(local_udp_socket, message, 3, 0, (sockaddr *)&server, server_addrlen); 
    int msg_len = recvfrom_timeout();
    if (msg_len == -1) {
        return 1;
    }
    cout << (int)message[0] << ' ' << (int)message[1] << ' ' << (int)my_number << endl;
    if (message[0] != OK or message[1] != my_number) {
        return 1;
    }
    if (message[2]) {
        char* ptr = message + 3;
        field2.bombs.resize(*ptr);
        ptr++;
        for (int i = 0; i < message[3]; i++, ptr++) {
            field2.bombs[i] = *ptr;
        }
        field2.aqua.resize(*ptr);
        ptr++;
        for (int i = 0; i < (int)field2.aqua.size(); i++, ptr++) {
            field2.aqua[i] = *ptr;
        }
    }
    return 0;
}

int update_query() {
    message[0] = MSG_UPDATE;
    message[1] = my_number;
    sendto(local_udp_socket, message, 2, 0, (sockaddr *)&server, server_addrlen); 
    int msg_len = recvfrom_timeout();
    if (msg_len <= 0 or message[0] != OK) {
        return 1;
    }

    if (mode == INIT_MODE) {
        candidates.resize(message[2]);
        char* ptr = message + 3;
        for (int i = 0; i < (int)candidates.size(); i++) {
            memcpy(candidates[i].name, ptr + 1, *ptr);
            candidates[i].name_len = *ptr;
            ptr += (*ptr);
            ptr++;
        }
    } else if (mode == SHIP_MODE) {
        opponent.is_ready = message[2];
    }
}

int update_net() {
    if (curr_time - last_upd_time > BETWEEN_UPDATES) {
        if (update_query()) {
            cout << "network error - update" << endl;
            return 1;
        }
        last_upd_time = curr_time;

    }
    if (check_pressed) {
        printf("here\n");
        if (check_query(MSG_CHECK)) {
            cout << "network error - check" << endl;
            return 1;
        }
    }
    if (go_pressed) {
        if (mode == INIT_MODE) {
            go_allowed = true;
        } else if (mode == SHIP_MODE) {
            if (check_query(MSG_GO)) {
                cout << "network error - go" << endl;
                return 1;
            }
            go_allowed = check_result;        
        } else {
            go_allowed = false;
        }
    }
    if (shoot_pressed) {
        if (shoot_query()) {
            cout << "network error - shot" << endl;
            return 1;
        }
    }
    check_pressed = go_pressed = shoot_pressed = 0;
}
int free_net() {
//    closesocket(local_tcp_socket);
}
