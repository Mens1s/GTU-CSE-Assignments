#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <complex.h>

#define MAX_PENDING_CONNECTIONS 10
#define ORDER_SIZE 10000
#define ROWS 30
#define COLS 40

typedef struct {
    int id;
    float x;
    float y;
    int status; 
    time_t order_time;
    time_t cook_time;
    time_t delivery_time;
} Order;

typedef struct {
    Order order;
    int client_sock;
} OrderTask;

typedef struct {
    int id;
    int client_sock;
    struct sockaddr_in client_addr;
} Client;

typedef struct {
    int id;
    int package_count;
    int status; 
    OrderTask orders[3];
    int total_package_count;
    int total_distance;
    int total_time;
    int current_location;
} DeliveryPerson;

typedef struct {
    int id;
    int cooked_count;
    int has_cooking_order;
    int order_in_oven[6];
    int order_in_oven_count;
    time_t start_cooking_time[6];
    time_t end_cooking_time[6];
    OrderTask orders[6];
} Cook;

/* GLOBAL VARIABLES */
int server_sock;
int cook_pool_size;
int delivery_pool_size;
int order_queue_size;
int order_queue_capacity;
int ready_order_queue_size;
int ready_order_queue_capacity;
int isFinished;
int p;
int q;
int fd;
int client_socket;
int total_package_count;
int total_delivered_package_count;
int control_c;
double inverse_calculation_time;
float delivery_speed;
time_t last_updated;

pthread_t *cook_threads;
pthread_t *delivery_threads;
pthread_t manager_thread;
pthread_t thread;
pthread_t control_thread;

pthread_mutex_t order_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t delivery_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oven_open_door_mutex = PTHREAD_MUTEX_INITIALIZER;   
pthread_mutex_t delivery_open_door_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t message_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t oven_size;
sem_t kurekler;
pthread_cond_t order_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t ready_order_cond = PTHREAD_COND_INITIALIZER;
OrderTask *order_queue;
OrderTask *ready_order_queue;
DeliveryPerson *delivery_people;
Cook *cooks;

void initialize_server(const char *ip, int port, int cook_pool_size, int delivery_pool_size);
void setup_signal_handlers();
void *cook_thread(void *arg);
void cook_oven_control(int id, int wait);
void *delivery_thread(void *arg);
void handle_new_order(int client_sock, Order order);
void *client_handler(void *arg);
void send_response(int client_sock, const char *message);
Order receive_order(int client_sock);
void log_activity(int order_id, const char *message);
void cleanup();
void handle_sigint(int sig);
void initialize_order_queue(int capacity);
void initialize_delivery_people(int num_people);
void add_order_to_queue(OrderTask task);
void initialize_ready_order_queue(int capacity);
void add_ready_order_to_queue(OrderTask task);
void handle_order_ready(int client_sock, int order_id);
void *delivery_manager(void *arg);
void *control_server(void *arg);
void start_server(const char *ip, int port);
void initialize_cooks(int cook_pool_size);
void give_promotion();
void initialize_threads();

// matrix operation ... 
void fill_matrix(double complex matrix[ROWS][COLS]);
void print_matrix(double complex matrix[ROWS][COLS]);
void transpose_matrix(double complex matrix[ROWS][COLS], double complex transpose[COLS][ROWS]);
void multiply_matrices(int rows_A, int cols_A, int cols_B, double complex A[rows_A][cols_A], double complex B[cols_A][cols_B], double complex result[rows_A][cols_B]);
void invert_matrix(int n, double complex A[n][n], double complex A_inv[n][n]);
void calculate_pseudo_inverse(double complex A[ROWS][COLS], double complex A_pinv[COLS][ROWS]);
void simulate_order_preparation();

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <IP> <Port> <Cook Pool Size> <Delivery Pool Size> <delivery_speed>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /* Getting order preparation time by calculating matrix inverse */
    simulate_order_preparation();
    /* Initilization of variables */
    fd = open("./logServer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    close(fd);
    
    /* Getting Arguments and Parse Them */
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    cook_pool_size = atoi(argv[3]);
    delivery_pool_size = atoi(argv[4]);
    delivery_speed = atoi(argv[5])/1000/60; // metre/minute to km/second
    setup_signal_handlers();

    fd = open("./logServer.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    
    initialize_server(ip, port, cook_pool_size, delivery_pool_size);
    //close(fd);
    initialize_threads();
    while(1){
        client_socket = -1;
        inverse_calculation_time = 1.2;
        total_package_count = 0;
        total_delivered_package_count = 0;
        last_updated = time(NULL);
        isFinished = 0;
        order_queue_size = 0;
        ready_order_queue_size = 0;
        control_c = 0;
        sem_init(&oven_size, 0, 6);
        sem_init(&kurekler, 0, 3);


        last_updated = time(NULL);
        
        start_server(ip, port);
    }
    cleanup();

    return 0;
}

void start_server(const char *ip, int port) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    //while(isFinished == 0){
    // Getting only one client..
    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    client_socket = client_sock;
    if (client_sock < 0) perror("Accept failed");

    int *client_sock_ptr = malloc(sizeof(int));
    if (!client_sock_ptr) {
        perror("Failed to allocate memory");
        close(client_sock);
    }
    *client_sock_ptr = client_sock;

    // Create a new thread to handle the client
    if (pthread_create(&thread, NULL, client_handler, client_sock_ptr) != 0) {
        perror("Failed to create client handler thread");
        free(client_sock_ptr);
        close(client_sock);
    } else pthread_detach(thread);
    //}
    while(isFinished == 0){
    }
}

void initialize_server(const char *ip, int port, int cook_pool_size, int delivery_pool_size) {
    struct sockaddr_in server_addr;

    /* Create server socket */
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, MAX_PENDING_CONNECTIONS) < 0) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    char logMessage[512];
    sprintf(logMessage, "Server started on %s:%d", ip, port);
    log_activity(-1, logMessage);
    printf("%s\n", logMessage);

}   

void initialize_threads(){
    cook_threads = (pthread_t *)malloc(cook_pool_size * sizeof(pthread_t));
    delivery_threads = (pthread_t *)malloc(delivery_pool_size * sizeof(pthread_t));

    if (!cook_threads || !delivery_threads) {
        perror("Failed to allocate memory for thread pools");
        cleanup();
        exit(EXIT_FAILURE);
    }

    initialize_cooks(cook_pool_size);
    // Initialize cook threads
    for (int i = 0; i < cook_pool_size; i++) {
        int *id = malloc(sizeof(int));
        if (id == NULL) {
            perror("Failed to allocate memory");
            cleanup();
            exit(EXIT_FAILURE);
        }
        *id = i;
        if (pthread_create(&cook_threads[i], NULL, cook_thread, id) != 0) {
            perror("Failed to create cook thread");
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    initialize_delivery_people(delivery_pool_size);
    // Initialize delivery threads
    for (int i = 0; i < delivery_pool_size; i++) {
        int *id = malloc(sizeof(int));
        if (id == NULL) {
            perror("Failed to allocate memory");
            cleanup();
            exit(EXIT_FAILURE);
        }
        *id = i;
        if (pthread_create(&delivery_threads[i], NULL, delivery_thread, id) != 0) {
            perror("Failed to create delivery thread");
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    // Initialize the order queue
    initialize_order_queue(ORDER_SIZE);  
    initialize_ready_order_queue(ORDER_SIZE);
    last_updated = time(NULL);

    
    if (pthread_create(&manager_thread, NULL, delivery_manager, NULL) != 0) {
        perror("Failed to create delivery manager thread");
        close(server_sock);
        exit(1);
    }

    if (pthread_create(&control_thread, NULL, control_server, NULL) != 0) {
        perror("Failed to create control thread");
        close(server_sock);
        exit(1);
    }
}

void *delivery_manager(void *arg) {
    while(1){
        
        pthread_mutex_lock(&delivery_mutex);

        while (ready_order_queue_size == 0) pthread_cond_wait(&ready_order_cond, &delivery_mutex);
        
        /* find the best delivery person who nearest to 3 and not delivering */
        int best_delivery_person = -1;
        float best_package_count = 4;
        float score = 0;
        for (int i = 0; i < delivery_pool_size; i++) {
            if (delivery_people[i].status == 0) {
                if (best_package_count == 4 || 3 - delivery_people[i].package_count < best_package_count) {
                    best_package_count = 3 - delivery_people[i].package_count ;
                    best_delivery_person = i;
                }
            }
        }
        OrderTask order = ready_order_queue[0];
        
        if (best_delivery_person != -1) {
            for (int i = 0; i < ready_order_queue_size - 1; i++) {
                ready_order_queue[i] = ready_order_queue[i + 1];
            }
            order.order.status = 1;
            ready_order_queue_size--;
            
            delivery_people[best_delivery_person].orders[delivery_people[best_delivery_person].package_count] = order;

            delivery_people[best_delivery_person].package_count++;
            char logMessage[512];
            sprintf(logMessage, "Manager assigned order: %d to %d delivery person", order.order.id, best_delivery_person);
            log_activity(-1, logMessage);

            if(delivery_people[best_delivery_person].package_count == 3)
                delivery_people[best_delivery_person].status = 1;
            
        } 
        
        pthread_mutex_unlock(&delivery_mutex);
    }
    return NULL;
}

void *control_server(void *arg){
    while(1){
        if(total_package_count != 0 && total_delivered_package_count == total_package_count && total_package_count != 0 && time(NULL) - last_updated > 5 && isFinished == 0){
            char logMessage[512];
            log_activity(-1, "All orders are delivered. ");
            printf("All orders are delivered. \n");
            control_c = 0;
            send_response(client_socket, "EXIT 10");
            isFinished = 1;
        }
    }

    return NULL;
}

void *cook_thread(void *arg) {
    int id = *(int *)arg;
    char message[256];
    cooks[id].cooked_count = 0;
    while (1) {
        char logMessage[512];

        pthread_mutex_lock(&order_mutex);
        cook_oven_control(id, 0);

        // Wait for an order to be placed in the queue
        while (order_queue_size == 0) {
            
            pthread_cond_wait(&order_cond, &order_mutex);
        }

        OrderTask task = order_queue[0];
        for (int i = 0; i < order_queue_size - 1; i++) {
            order_queue[i] = order_queue[i + 1];
        }
        task.order.status = 1;
        order_queue_size--;
        pthread_mutex_unlock(&order_mutex);
        // Prepare the order
        snprintf(message, sizeof(message), "ORDER_PREPARING %d", task.order.id);
        send_response(task.client_sock, message); 
        sprintf(logMessage, "Cook %d is preparing order %d", id, task.order.id);
        log_activity(task.order.id, logMessage);

        sleep(inverse_calculation_time); // do not need to calculate it again as said in the document i calculated once and used them all

        /* Oven is full and if cook has a pide in the oven first take it after place it */
        if(sem_trywait(&oven_size) == -1) {
            cook_oven_control(id, 1);
            sem_wait(&oven_size);
        }
        else {
            cook_oven_control(id, 0);
        }


        sem_wait(&kurekler);
        pthread_mutex_lock(&oven_open_door_mutex);
        
        // Cook the order
        sprintf(logMessage, "ORDER_COOKING %d", task.order.id);
        log_activity(task.order.id, logMessage);
        cooks[id].has_cooking_order = 1;
        cooks[id].start_cooking_time[cooks[id].order_in_oven_count] = time(NULL);
        cooks[id].order_in_oven[cooks[id].order_in_oven_count] = task.order.id;
        cooks[id].orders[cooks[id].order_in_oven_count] = task;
        cooks[id].order_in_oven_count++;
        pthread_mutex_unlock(&oven_open_door_mutex);
        sem_post(&kurekler);       

    }
    return NULL;
}

void cook_oven_control(int id, int wait){
    char logMessage[512];
    char message[256];
    if(cooks[id].has_cooking_order == 1){
        int order_id = cooks[id].order_in_oven[0];
        OrderTask task = cooks[id].orders[0];

        if(wait == 1){
            while(time(NULL) - cooks[id].start_cooking_time[order_id] < inverse_calculation_time/2);
        }
        sem_wait(&kurekler);
        pthread_mutex_lock(&delivery_open_door_mutex);
        sprintf(logMessage, "Cook %d is getting %d. the order from oven", id, order_id);
        log_activity(order_id, logMessage);
        pthread_mutex_unlock(&delivery_open_door_mutex);
        task.order.cook_time = time(NULL);
        sem_post(&kurekler);

        // Notify that the order is ready for delivery
        cooks[id].cooked_count++;
        sprintf(logMessage, "ORDER_READY %d", order_id);
        log_activity(order_id, logMessage);
        printf("Cook %d is ready for delivery order %d\n", id, order_id);
        snprintf(message, sizeof(message), "ORDER_READY %d", order_id);
        send_response(client_socket, message);
        // update cook
        for(int i = 0; i < cooks[id].order_in_oven_count - 1; i++){
            cooks[id].order_in_oven[i] = cooks[id].order_in_oven[i+1];
            cooks[id].start_cooking_time[i] = cooks[id].start_cooking_time[i+1];
            cooks[id].orders[i] = cooks[id].orders[i+1];
        }
        cooks[id].order_in_oven_count--;
        if(cooks[id].order_in_oven_count == 0)
            cooks[id].has_cooking_order = 0;
        
        sem_post(&oven_size);
        // Update the order status to cookeddelivery_mutex
        add_ready_order_to_queue(task);
        last_updated = time(NULL);
    }
}

void *delivery_thread(void *arg) {
    int id = *(int *)arg;
    while (1) {

        // if status is 1, then the delivery person is delivering
        if(delivery_people[id].status == 1){

            int package_count = delivery_people[id].package_count;
            
            float distance = 0;
            if(delivery_people[id].current_location == 0){
                distance += sqrt(pow(p/2 - delivery_people[id].orders[0].order.x, 2) 
                                + pow(q/2 - delivery_people[id].orders[0].order.y, 2));
            }
            else if(delivery_people[id].current_location == 1){
                distance += sqrt(pow(delivery_people[id].orders[1].order.x - delivery_people[id].orders[0].order.x, 2) 
                                + pow(delivery_people[id].orders[1].order.y - delivery_people[id].orders[0].order.y, 2));
            }

            else if(delivery_people[id].current_location == 2){
                distance += sqrt(pow(delivery_people[id].orders[1].order.x - delivery_people[id].orders[2].order.x, 2) 
                                + pow(delivery_people[id].orders[1].order.y - delivery_people[id].orders[2].order.y, 2));
            }

            else if(delivery_people[id].current_location == 2){ // last customer and go to the restaurant
                distance += sqrt(pow(p/2 - delivery_people[id].orders[2].order.x, 2) 
                                + pow(q/2 - delivery_people[id].orders[2].order.y, 2));
            }    
            usleep((distance/delivery_speed)*1000000); // Simulate delivery time

            if( control_c == 0)  {
                delivery_people[id].total_distance += distance;
                delivery_people[id].total_time += (distance/delivery_speed);       
                delivery_people[id].current_location += 1;
            }
            
            // order info
            time_t current_time = time(NULL);
            struct tm *local = localtime(&current_time);
            
            char logMessage[512];
            sprintf(logMessage, "Delivery person %d is delivering %d order. Distance : %f, Time : %f", id, delivery_people[id].orders[package_count-1].order.id, distance, (distance/delivery_speed));
            log_activity(delivery_people[id].orders[package_count-1].order.id, logMessage);
            
            delivery_people[id].orders[package_count-1].order.status = 0;
            delivery_people[id].orders[package_count-1].order.delivery_time = time(NULL);

            if( control_c == 1)
                printf("CANCELLED: ");
            // print the order info
            local = localtime(&delivery_people[id].orders[package_count-1].order.order_time);
            printf("Order id: %d ", delivery_people[id].orders[package_count-1].order.id);
            printf("Order Time: %d:%d:%d ", local->tm_hour, local->tm_min, local->tm_sec);
            local = localtime(&delivery_people[id].orders[package_count-1].order.cook_time);
            printf("Cook Time: %d:%d:%d ", local->tm_hour, local->tm_min, local->tm_sec);
            local = localtime(&delivery_people[id].orders[package_count-1].order.delivery_time);
            printf("Delivery Time: %d:%d:%d\n", local->tm_hour, local->tm_min, local->tm_sec);

            char message[256];
            snprintf(message, sizeof(message), "ORDER_DELIVERED %d", delivery_people[id].orders[package_count-1].order.id);
            sprintf(logMessage, "Delivery person %d delivered order %d", id, delivery_people[id].orders[package_count-1].order.id);
            log_activity(delivery_people[id].orders[package_count-1].order.id, logMessage);
            send_response(delivery_people[id].orders[package_count-1].client_sock, message);
            if( control_c == 0)  {
                delivery_people[id].total_package_count += 1;
            }
            pthread_mutex_lock(&delivery_mutex);
            total_delivered_package_count++;
            pthread_mutex_unlock(&delivery_mutex);
            last_updated = time(NULL);
            delivery_people[id].package_count -= 1;
            if(delivery_people[id].package_count == 0){ 
                delivery_people[id].status = 0;
                delivery_people[id].current_location = 0;
            }
        }else{
            pthread_mutex_lock(&delivery_mutex);
            if( (time(NULL) - last_updated > 20 || (total_package_count - total_delivered_package_count) <= 2  ) && delivery_people[id].package_count > 0 && isFinished == 0 && total_package_count != 0){
                if(control_c == 1)
                    printf("CANCELLED: ");
                printf("There is no new order, so %d delivery person goes to deliver packages.\n", id);
                char logMessage[512];
                sprintf(logMessage, "There is no new order, so %d delivery person goes to deliver packages.", id);
                log_activity(-1, logMessage);

                delivery_people[id].status = 1;
            }
            pthread_mutex_unlock(&delivery_mutex);
        }
    }
}

void initialize_order_queue(int capacity) {
    order_queue = (OrderTask *)malloc(capacity * sizeof(OrderTask));
    order_queue_capacity = capacity;
    order_queue_size = 0;

    if (!order_queue) {
        perror("Failed to allocate memory for order queue");
        exit(EXIT_FAILURE);
    }
}

void initialize_ready_order_queue(int capacity) {
    ready_order_queue = (OrderTask *)malloc(capacity * sizeof(OrderTask));
    ready_order_queue_capacity = capacity;
    ready_order_queue_size = 0;

    if (!ready_order_queue) {
        perror("Failed to allocate memory for ready order queue");
        exit(EXIT_FAILURE);
    }
}

void initialize_cooks(int cook_pool_size){
    cooks = (Cook *)malloc(cook_pool_size * sizeof(Cook));
    if (!cooks) {
        perror("Failed to allocate memory for cooks");
        cleanup();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < cook_pool_size; i++) {
        cooks[i].id = i;
        cooks[i].cooked_count = 0;
        cooks[i].has_cooking_order = 0;
        for(int j = 0; j < 6; j++){
            cooks[i].start_cooking_time[j] = -1;
        }
        for(int j = 0; j < 6; j++){
            cooks[i].end_cooking_time[j] = -1;
        }
        cooks[i].order_in_oven_count = 0;
        for(int j = 0; j < 6; j++){
            cooks[i].order_in_oven[j] = -1;
        }
    }
}

void initialize_delivery_people(int num_people) {
    delivery_people = (DeliveryPerson *)malloc(num_people * sizeof(DeliveryPerson));
    if (!delivery_people) {
        perror("Failed to allocate memory for delivery people");
        exit(EXIT_FAILURE);
    }
    // assing the defalt values to the delivery workers
    for (int i = 0; i < num_people; i++) {
        delivery_people[i].id = i;
        delivery_people[i].package_count = 0;
        delivery_people[i].status = 0;
        delivery_people[i].total_package_count = 0;
        delivery_people[i].total_distance = 0;
        delivery_people[i].total_time = 0;
        delivery_people[i].current_location = 0;
    }
}

void add_order_to_queue(OrderTask task) {
    pthread_mutex_lock(&order_mutex);

    if (order_queue_size < order_queue_capacity) {
        order_queue[order_queue_size++] = task;
        task.order.order_time = time(NULL);
        pthread_cond_signal(&order_cond);
    } else {
        log_activity(-1, "Order queue is full. Cannot add new order.");
        printf("Order queue is full. Cannot add new order.\n");
    }

    pthread_mutex_unlock(&order_mutex);

}

void add_ready_order_to_queue(OrderTask task) {
    pthread_mutex_lock(&delivery_mutex);

    if (ready_order_queue_size < ready_order_queue_capacity) {
        ready_order_queue[ready_order_queue_size++] = task;
        pthread_cond_signal(&ready_order_cond);
    } else {
        log_activity(-1, "Ready order queue is full. Cannot add new order.");
        printf("Ready order queue is full. Cannot add new order.\n");
    }

    pthread_mutex_unlock(&delivery_mutex);
}

void handle_new_order(int client_sock, Order order) {
    OrderTask task;
    task.order = order;
    task.client_sock = client_sock;

    // Add the new order to the queue
    task.order.order_time = time(NULL);
    char message[256];
    snprintf(message, sizeof(message), "ORDER_RECEIVED %d", task.order.id);
    printf("Received new order: ID=%d\n", order.id);
    send_response(client_sock, message);

    add_order_to_queue(task);
   
    // Log the new order
    char log_message[256];
    snprintf(log_message, sizeof(log_message), "Received new order: ID=%d, Position=(%.2f, %.2f)", order.id, order.x, order.y);
    log_activity(-1, log_message);
}

void handle_order_ready(int client_sock, int order_id) {
    pthread_mutex_lock(&delivery_mutex);

    int found = 0;
    for (int i = 0; i < ready_order_queue_size; i++) {
        if (ready_order_queue[i].order.id == order_id) {
            found = 1;
            for (int j = i; j < ready_order_queue_size - 1; j++) {
                ready_order_queue[j] = ready_order_queue[j + 1];
            }
            ready_order_queue_size--;
            break;
        }
    }

    pthread_mutex_unlock(&delivery_mutex);

    if (found) {
        char log_message[256];
        snprintf(log_message, sizeof(log_message), "Order ready for delivery: ID=%d", order_id);
        log_activity(-1, log_message);
        
        char message[256];
        snprintf(message, sizeof(message), "ORDER_READY %d", order_id);
        send_response(client_sock, message);
    } else {
        char message[256];
        snprintf(message, sizeof(message), "ORDER_NOT_FOUND %d", order_id);
        send_response(client_sock, message);
    }
}

void *client_handler(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);

    while (1) {
        Order order = receive_order(client_sock);
    }

}

Order receive_order(int client_sock) {
    Order order;
    char buffer[42];
    char leftover[1024] = ""; // To store incomplete tokens from previous recv
    char *token;
    int bytes_received;
    int num_clients;
    int printOnlyOnce = 0;
    while ((bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received+1] = '\0';
        // Append the new data to any leftover from the previous recv call
        strcat(leftover, buffer);

        token = strtok(leftover, "\n");
        while (token != NULL) {
            if (sscanf(token, "ORDER %d %f %f %d %d %d", &order.id, &order.x, &order.y, &p, &q, &num_clients) == 6) {
                // Check special order IDs
                if (order.id == -2) {
                    log_activity(-1, "USER CANCEL ORDERS");
                    printf("USER CANCEL ORDERS... Cancelling orders..\n");
                    control_c = 1;
                }else{
                    if(printOnlyOnce == 0){
                        total_package_count = num_clients;
                        printf("%d new customers.. Serving...\n", num_clients);
                        printOnlyOnce = 1;
                    }
                    order.status = 0;  // Set initial status
                    handle_new_order(client_sock, order);
                }
                last_updated = time(NULL);
            } else {
                printf("Invalid order format: %s\n", token);
                break;
            }
            token = strtok(NULL, "\n"); // Get next token
        }

        if(isFinished == 1){
            break;
        }

        if (token != NULL) {
            strcpy(leftover, token); 
        } else {
            leftover[0] = '\0'; 
        }
        
    }
    return order;
}

void send_response(int client_sock, const char *message) {
    pthread_mutex_lock(&message_mutex);

    char buffer[21]; 
    sprintf(buffer, "%-*s", 20, message);
    if(client_sock == -1){
        printf("There is no client server shut-down.. Order Cancelled...\n");
    }
    else if(control_c == 0){
        if (send(client_sock, buffer, 20, 0) < 0) {
            perror("Send response failed");
        }
    }else if(control_c == 1){
        sprintf(buffer, "%-*s", 20, "CANCELLED");

        if (send(client_socket, buffer, 20, 0) < 0) {
            perror("Send response failed");
        }
    }
    pthread_mutex_unlock(&message_mutex);
}

void give_promotion(){

    log_activity(-1, "Server shut down gracefully\n");
    printf("Server shut down gracefully\n");
    int most_package_count = 0;
    int most_package_count_id = 0;
    
    for(int i = 0; i < delivery_pool_size; i++){
        if(delivery_people[i].total_package_count > most_package_count){
            most_package_count = delivery_people[i].total_package_count;
            most_package_count_id = i;
        }
    }

    int most_cooked_count = 0;
    int most_cooked_count_id = 0;
    for(int i = 0; i < cook_pool_size; i++){
        if(cooks[i].cooked_count > most_cooked_count){
            most_cooked_count = cooks[i].cooked_count;
            most_cooked_count_id = i;
        }
    }

    char logMessage[512];

    // cooks info
    log_activity(-1, "Cook infos");
    sprintf(logMessage, "****** Cook %d cooked the most orders: %d ******", most_cooked_count_id, most_cooked_count);
    log_activity(-1, logMessage);

    printf("%s\n",logMessage);

    for(int i = 0; i < cook_pool_size; i++){
        sprintf(logMessage, "Cook %d cooked %d orders", i, cooks[i].cooked_count);
        log_activity(-1, logMessage);
    }

    // delivery infos
    log_activity(-1, "Delivery person infos");
    sprintf(logMessage, "****** Delivery person %d delivered the most packages: %d ******", most_package_count_id, most_package_count);
    log_activity(-1, logMessage);

    printf("%s\n",logMessage);
    for(int i = 0; i < delivery_pool_size; i++){
        sprintf(logMessage, "Delivery person %d delivered %d packages, total distance: %d, total time: %d", i, delivery_people[i].total_package_count, delivery_people[i].total_distance, delivery_people[i].total_time);
        log_activity(-1, logMessage);
    }

}

void log_activity(int order_id, const char *message) {
    
    if (fd == -1) {
        return;
    }

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        close(fd);
        return;
    }
    char logMessage[512];

    if(order_id == -1)
        snprintf(logMessage, sizeof(logMessage), "Server-%d %s\n", order_id, message);

    else
        snprintf(logMessage, sizeof(logMessage), "OrderId-%d %s\n", order_id, message);

    
    if (write(fd, logMessage, strlen(logMessage)) == -1) {
        perror("write");
        close(fd);
        return;
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl");
        close(fd);
        return;
    }


}

void setup_signal_handlers() {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

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
    log_activity(-1, "Server shut down due to signal");
    printf("Caught signal %d, shutting down...\n", sig);
    control_c = 0;
    cleanup();
    exit(EXIT_SUCCESS);
}

void cleanup() {
    send_response(client_socket, "EXIT 10");

    give_promotion();
    if (server_sock != -1) {
        close(server_sock);
    }
    sem_destroy(&kurekler);
    sem_destroy(&oven_size);
    if (cook_threads) {
        for (int i = 0; i < cook_pool_size; i++) {
            pthread_cancel(cook_threads[i]);
        }
        free(cook_threads);
    }
    if (delivery_threads) {
        for (int i = 0; i < delivery_pool_size; i++) {
            pthread_cancel(delivery_threads[i]);
        }
        free(delivery_threads);
    }
    if(manager_thread){
        pthread_cancel(manager_thread);
    }
    if(thread){
        pthread_cancel(thread);
    }
    if(control_thread){
        pthread_cancel(control_thread);
    }
    if(delivery_people){
        free(delivery_people);
    }
    if (order_queue) {
        free(order_queue);
    }
    if (ready_order_queue) {
        free(ready_order_queue);
    }
    if (fd != -1) {
        close(fd);
    }
    if (cooks) {
        free(cooks);
    }
    if(client_socket != -1){
        close(client_socket);
    }

}

void simulate_order_preparation() {
    double complex matrix[ROWS][COLS];
    double complex pseudo_inverse[COLS][ROWS];

    fill_matrix(matrix);

    clock_t start_time = clock();

    calculate_pseudo_inverse(matrix, pseudo_inverse);

    clock_t end_time = clock();

    inverse_calculation_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Pseudo-inverse calculation time: %f seconds\n", inverse_calculation_time);
}

void fill_matrix(double complex matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            double real_part = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
            double imag_part = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
            matrix[i][j] = real_part + imag_part * I;
        }
    }
}

void transpose_matrix(double complex matrix[ROWS][COLS], double complex transpose[COLS][ROWS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

void multiply_matrices(int rows_A, int cols_A, int cols_B, double complex A[rows_A][cols_A], double complex B[cols_A][cols_B], double complex result[rows_A][cols_B]) {
    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_B; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_A; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void invert_matrix(int n, double complex matrix[COLS][COLS], double complex inverse[COLS][COLS]) {

    double complex identity[COLS][COLS];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                identity[i][j] = 1.0 + 0.0 * I;
            } else {
                identity[i][j] = 0.0 + 0.0 * I;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inverse[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        double complex pivot = inverse[i][i];
        for (int j = 0; j < n; j++) {
            inverse[i][j] /= pivot;
            identity[i][j] /= pivot;
        }
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double complex factor = inverse[k][i];
                for (int j = 0; j < n; j++) {
                    inverse[k][j] -= factor * inverse[i][j];
                    identity[k][j] -= factor * identity[i][j];
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inverse[i][j] = identity[i][j];
        }
    }
}

void calculate_pseudo_inverse(double complex A[ROWS][COLS], double complex A_pinv[COLS][ROWS]) {
    double complex A_transpose[COLS][ROWS];
    double complex A_transpose_A[COLS][COLS];
    double complex A_transpose_A_inv[COLS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            A_transpose[j][i] = A[i][j];
        }
    }

    multiply_matrices(COLS, ROWS, COLS, A_transpose, A, A_transpose_A);

    invert_matrix(COLS, A_transpose_A, A_transpose_A_inv);

    multiply_matrices(COLS, COLS, ROWS, A_transpose_A_inv, A_transpose, A_pinv);
}
