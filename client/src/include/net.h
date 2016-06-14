#ifndef NET_H
#define NET_H
#define PORT 22123
int init_net(const char *hostname, unsigned short port);
int update_net();
int free_net();

#endif //NET_H
