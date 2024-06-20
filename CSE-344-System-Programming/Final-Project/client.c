#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>

#define MAX_CLIENTS 9999

typedef struct {
    int id;
    float x;
    float y;
} Client;

typedef struct {
    int sock;
    struct sockaddr_in server_addr;
} Connection;

typedef struct {
    int client_id;
    char message[256];
} Response;

typedef struct {
    int id;
    int x;
    int y;
} Request;

pthread_mutex_t order_mutex = PTHREAD_MUTEX_INITIALIZER;

int num_clients;
int is_finished = 0;
int is_connected = 0;
Client clients[MAX_CLIENTS];
Connection connection;
volatile sig_atomic_t keep_running = 1;

void cancel_order(int client_id);
void generate_clients(int num_clients, int p, int q);
void connect_to_server(const char *ip, int port);
void send_order(int client_id, int p, int q);
void cancel_order(int client_id);
void run_clients(int p, int q);
void setup_signal_handlers();
void handle_sigint(int sig);
void* handle_connection(void* arg);

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <server_ip> <port> <num_clients> <town_size_p> <town_size_q>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char server_ip[100];
    strcpy(server_ip, argv[1]);

    int port = atoi(argv[2]);
    num_clients = atoi(argv[3]);
    int p = atoi(argv[4]);
    int q = atoi(argv[5]);

    if (num_clients > MAX_CLIENTS) {
        fprintf(stderr, "Number of clients exceeds maximum allowed (%d)\n", MAX_CLIENTS);
        exit(EXIT_FAILURE);
    }
    if (p <= 0 || q <= 0) {
        fprintf(stderr, "Town size must be positive\n");
        exit(EXIT_FAILURE);
    }
    if(p > 9999 || q > 9999){
        fprintf(stderr, "Town size must be less than 10000\n");
        exit(EXIT_FAILURE);
    }

    setup_signal_handlers();

    generate_clients(num_clients, p, q);
    connect_to_server(server_ip, port);
    run_clients(p, q);

    close(connection.sock);
    printf("Client shut down gracefully\n");
    return 0;
}

void generate_clients(int num_clients, int p, int q) {
    srand(time(NULL));
    for (int i = 0; i < num_clients; i++) {
        clients[i].id = i;
        clients[i].x = (float)rand() / RAND_MAX * p;
        clients[i].y = (float)rand() / RAND_MAX * q;
    }
}

void connect_to_server(const char *ip, int port) {
    connection.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connection.sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    connection.server_addr.sin_family = AF_INET;
    connection.server_addr.sin_port = htons(port);
    //connection.server_addr.sin_addr.s_addr = INADDR_ANY;
    
    if (inet_pton(AF_INET, ip, &connection.server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(connection.sock);
        exit(EXIT_FAILURE);
    }
    if (connect(connection.sock, (struct sockaddr *)&connection.server_addr, sizeof(connection.server_addr)) < 0) {
        perror("Connection to server failed");
        close(connection.sock);
        exit(EXIT_FAILURE);
    }
}

void send_order(int client_id, int p, int q) {

    char buffer[42];
    snprintf(buffer, sizeof(buffer), "ORDER %03d %07.3f %07.3f %04d %04d %04d\n", client_id, clients[client_id].x, clients[client_id].y, p, q, num_clients);

    if (send(connection.sock, buffer, strlen(buffer), 0) < 0) {
        perror("Send failed");
    }
    is_connected = 1;
    usleep(25555);
}

void cancel_order(int client_id) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "CANCEL %d %.2f %.2f %d %d\n", clients[client_id].id, -1.0, -1.0, -1, -1);
    if (send(connection.sock, buffer, strlen(buffer), 0) < 0) {
        perror("Cancel send failed");
    }
}

void run_clients(int p, int q) {
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_connection, NULL) != 0) {
        perror("Thread creation failed");
    }

    for (int i = 0; i < num_clients; i++) {
        if(is_finished == 1) break;
        send_order(i, p, q);
    }
    while(is_finished == 0) {};
}

void* handle_connection(void* arg) {
    char temp[256] = {0};
    while (keep_running) {
        char buffer[21];
        int bytes_received = recv(connection.sock, buffer, sizeof(buffer) - 1, 0);
        if (strcmp(temp, buffer) == 0 || strlen(buffer) == 0) {
            continue;
        }
        if (bytes_received < 0) {
            perror("Receive failed");
            break;
        } else if (bytes_received == 0) {
            printf("Server closed connection\n");
            break;
        }
        buffer[bytes_received+1] = '\0';
        int client_id;
        char message[256];

        sscanf(buffer, "%s %d", message, &client_id);

        printf("Client %d received: %s\n", client_id, message);
        is_connected = 1;
        if (strcmp(message, "EXIT") == 0) break;

        strcpy(temp, buffer);
    }
    is_finished = 1;
    close(connection.sock); // Ensure the socket is closed when done
    return NULL;
}

void setup_signal_handlers() {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    //close(server_sock);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Error setting up SIGTERM handler");
        exit(EXIT_FAILURE);
    }
}

void handle_sigint(int sig) {
    printf("Caught signal %d, shutting down...\n", sig);
    if(is_connected == 1) send_order(-2, 0, 0);
    else {
        close(connection.sock);
        exit(EXIT_SUCCESS);
    }
}

