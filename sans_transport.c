#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "include/sans.h"
#include "include/rudp.h"
#include "include/socket_map.h"

int rudp_send_data(int sock_fd, const char* msg_data, int msg_len) {
    // Find socket info in map, need this to send
    struct sockaddr *sock_dest;
    socklen_t sock_dest_len;
    int pkt_seq_num;
    
    for (size_t idx = 0; idx < MAX_SOCKET_COUNT; idx++) {
        if (socket_map[idx].socket_id == sock_fd) { 
            sock_dest = socket_map[idx].address; // destination address to send to
            sock_dest_len = socket_map[idx].addr_len; // address length
            pkt_seq_num = socket_map[idx].seqnum; // get current sequence number
            socket_map[idx].seqnum++; // increment sequence number for next packet
            break; // socket found, stop loop
        }
    }

    // Create a packet and fill it with data
    rudp_packet_t *data_packet = malloc(sizeof(rudp_packet_t) + msg_len);
    data_packet->type = DAT; // Set packet type to data
    data_packet->seqnum = pkt_seq_num; // Sequence number of packet
    memcpy(data_packet->payload, msg_data, msg_len); // Copy message data into packet's payload

    // Send packet over socket
    int sent_bytes = sendto(sock_fd, data_packet, sizeof(rudp_packet_t) + msg_len, 0, sock_dest, sock_dest_len);
    free(data_packet); // free memory after sending
    return sent_bytes; // Return number of bytes sent
}

int rudp_recv_data(int sock_fd, char* recv_buf, int buf_size) {
    // Get socket information to receive data
    struct sockaddr *recv_src;
    socklen_t recv_src_len;
    
    for (size_t idx = 0; idx < MAX_SOCKET_COUNT; idx++) {
        if (socket_map[idx].socket_id == sock_fd) {
            recv_src = socket_map[idx].address; // source address from which data is received
            recv_src_len = socket_map[idx].addr_len; // address length of source
            break; // socket found, stop loop
        }
    }

    // Receive the packet
    rudp_packet_t received_packet; // Create a packet structure to receive data
    int recvd_bytes = recvfrom(sock_fd, &received_packet, sizeof(received_packet), 0, recv_src, &recv_src_len);
    int data_size = recvd_bytes - sizeof(rudp_packet_t); // Calculate actual data size

    // Copy received data into buffer
    memcpy(recv_buf, received_packet.payload, data_size);
    recv_buf[data_size] = '\0'; // Null-terminate the buffer to make it a valid string

    return data_size; // Return size of received data
}
