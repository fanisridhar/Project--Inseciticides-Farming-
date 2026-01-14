#include <netinet/in.h>
#include <stddef.h>

#define MAX_SOCKET_COUNT 10  // we have maximum of 10 sockets

// This be for each socket's info we track
typedef struct {
    int sock_id;               // socket identifier
    struct sockaddr *sock_addr; // where we send or get from (socket address)
    socklen_t sock_addr_len;    // length of address, maybe needed when sending/receiving
    int packet_num;            // keep track of sequence number for packets
} socket_info_t;

// This map keep info for many sockets
extern socket_info_t sock_map[MAX_SOCKET_COUNT];  // array to hold max 10 sockets
