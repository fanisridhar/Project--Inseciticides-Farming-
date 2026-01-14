#include <netinet/in.h>
#include "include/sans.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int BUFFER_MAX = 1024;

int find_header_length(char *response) {
    char *header_end = "\r\n\r\n";
    size_t header_end_size = strlen(header_end);
    int response_length = strlen(response);
    int idx;

    for (idx = 0; idx <= response_length - header_end_size; idx++) {
        if (strncmp(&response[idx], header_end, header_end_size) == 0) {
            return idx + header_end_size;
        }
    }
    return -1;
}

int get_content_length(char *response) {
    char *content_length = "Content-Length: ";
    int content_len = 0;
    char *ptr = response;
    size_t header_size = strlen(content_length);

    while (*ptr) {
        if (strncmp(content_length, ptr, header_size) == 0) {
            ptr += header_size;
            content_len = strtol(ptr, NULL, 10);
            break;
        }
        ptr++;
    }
    return content_len;
}

int http_client(const char *server_address, int port_number) {
    char http_method[8];
    char resource_path[256];
    char adjusted_path[256];
    char http_request[1024];
    int socket_descriptor = 0;
    int received_bytes = 0;
    
    scanf("%7s", http_method);
    scanf("%255s", resource_path);
    
    int idx = 0, path_idx = 0;
    while (resource_path[path_idx] == '/') {
        path_idx++;
    }
    while (path_idx < strlen(resource_path)) {
        adjusted_path[idx] = resource_path[path_idx];
        idx++;
        path_idx++;
    }
    adjusted_path[idx] = '\0';
    
    socket_descriptor = sans_connect(server_address, port_number, IPPROTO_TCP);
    
    snprintf(http_request, sizeof(http_request),
             "%s /%s HTTP/1.1\r\n"
             "Host: %s:%d\r\n"
             "User-Agent: sans/1.0\r\n"
             "Cache-Control: no-cache\r\n"
             "Connection: close\r\n"
             "Accept: */*\r\n\r\n",
             http_method, adjusted_path, server_address, port_number);
    
    sans_send_pkt(socket_descriptor, http_request, strlen(http_request));
    
    char http_response[1024];
    received_bytes = sans_recv_pkt(socket_descriptor, http_response, BUFFER_MAX - 1);
    
    int content_length = get_content_length(http_response);
    int header_length = find_header_length(http_response);
    idx = received_bytes - header_length;
    http_response[received_bytes] = '\0';
    
    printf("%s", http_response);
    
    while (idx < content_length) {
        received_bytes = sans_recv_pkt(socket_descriptor, http_response, BUFFER_MAX - 1);
        http_response[received_bytes] = '\0';
        printf("%s", http_response);
        idx = idx + received_bytes;
    }
    
    sans_disconnect(socket_descriptor);
    return 0;
}
