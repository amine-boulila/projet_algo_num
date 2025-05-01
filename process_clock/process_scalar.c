#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define NUM_PROCESSES 4
#define PORT_BASE 5000
#define MAX_MSG_SIZE 128

typedef struct {
    int id;
    int scalar_clock;
} Process;

void print_scalar_clock(Process *p) {
    printf("[P%d] Clock: %d\n", p->id, p->scalar_clock);
}

void update_local(Process *p) {
    p->scalar_clock++;
    printf("[P%d] Local event -> ", p->id);
    print_scalar_clock(p);
}

int setup_server(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 5);
    return sock;
}

int connect_to_process(int target_port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(target_port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    return sock;
}

void send_message(Process *p, int target_id) {
    p->scalar_clock++;
    int sock = connect_to_process(PORT_BASE + target_id);
    if (sock < 0) return;

    write(sock, &p->scalar_clock, sizeof(int));
    printf("[P%d] Sent to P%d -> ", p->id, target_id);
    print_scalar_clock(p);
    close(sock);
}

void receive_message(Process *p, int server_sock) {
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int conn = accept(server_sock, (struct sockaddr*)&cli_addr, &cli_len);
    if (conn < 0) return;

    int incoming;
    read(conn, &incoming, sizeof(int));
    close(conn);

    if (p->scalar_clock < incoming)
        p->scalar_clock = incoming;
    p->scalar_clock++;
    printf("[P%d] Received -> ", p->id);
    print_scalar_clock(p);
}

void *receive_thread(void *arg) {
    Process *p = (Process *)arg;
    int server_sock = setup_server(PORT_BASE + p->id);
    while (1) {
        receive_message(p, server_sock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./process_scalar <id> <clock_type>\n");
        return 1;
    }

    Process p;
    p.id = atoi(argv[1]);
    p.scalar_clock = 0;

    printf("[P%d] Starting Scalar Clock\n", p.id);

    pthread_t rcv_thread;
    pthread_create(&rcv_thread, NULL, receive_thread, &p);

    sleep(2); // Allow server setup

    for (int i = 0; i < 5; i++) {
        update_local(&p);
        sleep(1);
    }

    for (int i = 1; i <= 4; i++) {
        int target = (p.id + i) % NUM_PROCESSES;
        send_message(&p, target);
        sleep(2);
    }

    pthread_join(rcv_thread, NULL);
    return 0;
}