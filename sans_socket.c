#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "include/sans.h"
#include "include/rudp.h"
#include "include/socket_map.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_ENTRIES 10
#define PACKET_SIZE 1400
#define MAX_ATTEMPTS 10

socket_entry_t socket_list[MAX_SOCKET_COUNT];

// Adding entry for socket
void add_socket_to_list(int sock_fd, struct sockaddr *addr_info, socklen_t addr_len) {
    int idx;
    // loop to check if space in socket array
    for (idx = 0; idx < MAX_ENTRIES; idx++) {
        if (socket_list[idx].socket_id == 0) {
            socket_list[idx].socket_id = sock_fd;
            socket_list[idx].addr_len = addr_len;
            socket_list[idx].address = malloc(addr_len);
            memcpy(socket_list[idx].address, addr_info, addr_len);
            socket_list[idx].seqnum = 2; // set initial seqnum
            break;
        }
    }
}

// make connection to host
int sans_connect(const char* hostname, int port_num, int protocol) {
    if (protocol != IPPROTO_TCP && protocol != IPPROTO_RUDP) {
        return -1; // return failure if protocol not TCP/RUDP
    }
    char port_str[10];
    snprintf(port_str, sizeof(port_str), "%d", port_num);
    struct addrinfo hints, *results, *rp;
    int sock_fd = -1;

    memset(&hints, 0, sizeof(hints)); // empty hints struct
    hints.ai_family = AF_UNSPEC; // family not matter
    hints.ai_socktype = (protocol == IPPROTO_RUDP) ? SOCK_DGRAM : SOCK_STREAM;

    int getaddr_status = getaddrinfo(hostname, port_str, &hints, &results);
    if (getaddr_status != 0) {
        fprintf(stderr, "getaddrinfo fail: %s\n", gai_strerror(getaddr_status));
        return -1;
    }

    for (rp = results; rp != NULL; rp = rp->ai_next) {
        sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock_fd == -1) continue;

        if (protocol == IPPROTO_TCP) {
            // attempt to connect for TCP
            if (connect(sock_fd, rp->ai_addr, rp->ai_addrlen) == -1) {
                close(sock_fd);
                sock_fd = -1;
                continue;
            }
            break;
        } else {
            // setup RUDP stuff
            struct timeval wait_time = { .tv_usec = 200000 };
            setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &wait_time, sizeof(wait_time));
            
            rudp_packet_t syn_packet = { .type = SYN, .seqnum = 0 };
            int try_count = 0;
            while (try_count < MAX_ATTEMPTS) {
                socklen_t len_addr = rp->ai_addrlen;
                int send_len = sendto(sock_fd, &syn_packet, sizeof(syn_packet), 0, rp->ai_addr, len_addr);
                if (send_len < 0) {
                    perror("SYN send error");
                    continue;
                }
                
                rudp_packet_t syn_ack_packet;
                int recv_len = recvfrom(sock_fd, &syn_ack_packet, sizeof(syn_ack_packet), 0, rp->ai_addr, &len_addr);
                if (recv_len > 0 && (syn_ack_packet.type & SYN) && (syn_ack_packet.type & ACK)) {
                    rudp_packet_t ack_packet = { .type = ACK, .seqnum = 1 };
                    sendto(sock_fd, &ack_packet, sizeof(ack_packet), 0, rp->ai_addr, len_addr);
                    add_socket_to_list(sock_fd, rp->ai_addr, len_addr);
                    break;
                }
                try_count++;
            }
            if (try_count == MAX_ATTEMPTS) {
                close(sock_fd);
                sock_fd = -1;
            }
        }
    }

    freeaddrinfo(results); // free addrinfo
    return sock_fd;
}

// accept incoming connections
int sans_accept(const char* if_name, int port_num, int protocol) {
    if (protocol != IPPROTO_TCP && protocol != IPPROTO_RUDP) {
        return -1; // bad protocol
    }
    struct addrinfo hints, *results, *rp;
    int sock_fd = -1;
    char port_str[10];
    snprintf(port_str, sizeof(port_str), "%d", port_num);

    memset(&hints, 0, sizeof hints); // zero out hints
    hints.ai_family = AF_UNSPEC;  
    hints.ai_socktype = (protocol == IPPROTO_RUDP) ? SOCK_DGRAM : SOCK_STREAM;

    int getaddr_status = getaddrinfo(if_name, port_str, &hints, &results);
    if (getaddr_status != 0) {
        fprintf(stderr, "getaddrinfo fail: %s\n", gai_strerror(getaddr_status));
        exit(EXIT_FAILURE);
    }

    for (rp = results; rp != NULL; rp = rp->ai_next) {
        sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock_fd == -1) continue;

        if (bind(sock_fd, rp->ai_addr, rp->ai_addrlen) == -1) {
            perror("Binding fail");
            close(sock_fd);
            sock_fd = -1;
            continue;
        }
        if (protocol == IPPROTO_TCP) {
            // listen for TCP connections
            if (listen(sock_fd, MAX_ENTRIES) == -1) {
                close(sock_fd);
                sock_fd = -1;
                continue;
            }
            add_socket_to_list(sock_fd, rp->ai_addr, rp->ai_addrlen);
            int new_sock = accept(sock_fd, NULL, NULL);
            close(sock_fd);
            return new_sock;
        } else {
            // handle RUDP connections
            socklen_t len_addr = rp->ai_addrlen;
            while (1) {
                rudp_packet_t request;
                int recv_len = recvfrom(sock_fd, &request, sizeof(request), 0, rp->ai_addr, &len_addr);
                if (recv_len > 0 && request.type & SYN) {
                    int retry_count = 0;
                    struct timeval wait_time = { .tv_usec = 200000 };
                    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &wait_time, sizeof(wait_time));

                    rudp_packet_t syn_ack_packet = { .type = SYN | ACK, .seqnum = request.seqnum };
                    while (retry_count < MAX_ATTEMPTS) {
                        int send_len = sendto(sock_fd, &syn_ack_packet, sizeof(rudp_packet_t), 0, rp->ai_addr, rp->ai_addrlen);
                        if (send_len < 0) {
                            perror("SYN-ACK send error");
                        }
                        rudp_packet_t ack_packet;
                        recv_len = recvfrom(sock_fd, &ack_packet, sizeof(ack_packet), 0, rp->ai_addr, &len_addr);
                        if (recv_len >= 0) {
                            add_socket_to_list(sock_fd, rp->ai_addr, rp->ai_addrlen);
                            return sock_fd;
                        } else {
                            perror("ACK recv error");
                            retry_count++;
                        }
                    }
                }
            }
        }
    }

    freeaddrinfo(results); // free memory
    return -1;
}

// disconnecting socket
int sans_disconnect(int sock_fd) {
    if (sock_fd != -1) {
        return close(sock_fd);
    }
    return -1;
}
