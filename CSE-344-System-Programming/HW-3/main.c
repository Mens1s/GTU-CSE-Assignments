#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_REAL_AUTOMOBILES 8
#define MAX_REAL_PICKUPS 4
#define MAX_TEMP_AUTOMOBILES 8
#define MAX_TEMP_PICKUPS 4

// Global variables
int mFree_automobile = 0;
int mFree_pickup = 0;
int temp_automobiles = 0;
int temp_pickups = 0;

sem_t inChargeForAutomobile;
sem_t newAutomobile;
sem_t inChargeForPickup;
sem_t newPickup;
sem_t printSemaphore;


// Function prototypes
void *carOwner(void *arg);
void *carAttendantAutomobile(void *arg);
void *carAttendantPickup(void *arg);
void printDetail(int delay);

int main() {
    // init  semaphores 
    sem_init(&inChargeForAutomobile, 0, MAX_REAL_AUTOMOBILES);
    sem_init(&newAutomobile, 0, 1);
    sem_init(&inChargeForPickup, 0, MAX_REAL_PICKUPS);
    sem_init(&newPickup, 0, 1);
    sem_init(&printSemaphore, 0, 1);

    // creating threads 
    pthread_t owner_thread, attendant_thread_automobile, attendant_thread_pickup;
    pthread_create(&owner_thread, NULL, carOwner, NULL);
    pthread_create(&attendant_thread_automobile, NULL, carAttendantAutomobile, NULL);
    pthread_create(&attendant_thread_pickup, NULL, carAttendantPickup, NULL);

    // waiting for threads 
    pthread_join(attendant_thread_automobile, NULL);
    pthread_join(attendant_thread_pickup, NULL);
    pthread_join(owner_thread, NULL);
   
    //  destroy semaphores and mutexes
    sem_destroy(&inChargeForAutomobile);
    sem_destroy(&newAutomobile);
    sem_destroy(&inChargeForPickup);
    sem_destroy(&newPickup);
    sem_destroy(&printSemaphore);

    printf("NO PARKING SPACE AVAILABLE\n");
    printf("All threads are finished.\n");

    return 0;
}

void *carOwner(void *arg) {
    while (1) {
        sem_wait(&newPickup);
        sem_wait(&newAutomobile);
        if ( (temp_pickups < MAX_TEMP_PICKUPS) || (temp_automobiles < MAX_TEMP_AUTOMOBILES) || (mFree_automobile < MAX_REAL_AUTOMOBILES) || (mFree_pickup < MAX_REAL_PICKUPS) ){
            // Temporary parking space available, park the vehicle
            int vehicleType = rand() % 2; // 0 for pickup, 1 for automobile
            if (vehicleType == 0) {
                if(temp_pickups == MAX_TEMP_PICKUPS){
                    printf("A pickup comes to park but there is no parking area...\n");
                }else{
                    sem_wait(&printSemaphore);
                    temp_pickups++;
                    printf("A pickup is parked in the temporary parking area. Total pickups in temp: %d\n", temp_pickups);
                    printDetail(100000);
                }
            } else {
                if(temp_automobiles == MAX_TEMP_AUTOMOBILES)
                    printf("A automobile comes to park but there is no parking area...\n");
                else{
                    sem_wait(&printSemaphore);
                    temp_automobiles++;
                    printf("An automobile is parked in the temporary parking area. Total automobiles in temp: %d\n", temp_automobiles);
                    printDetail(100000);
                }
            }
            sem_post(&newPickup);
            sem_post(&newAutomobile);
        } else{
            //printf("DOLDUK ABİ\n");
            sem_post(&newPickup);
            sem_post(&newAutomobile);
            // No parking space available, exit the loop
            break;
        }
    }
    return NULL;
}

void *carAttendantAutomobile(void *arg) {
    while (1) {
        sem_wait(&newAutomobile);
 
        // Check if all parking spaces are full
        if ((mFree_automobile == MAX_REAL_AUTOMOBILES) && (temp_automobiles == MAX_TEMP_AUTOMOBILES)) {
            //  exit 
            sem_post(&newAutomobile);
            break;
        }
        // Check if there are vehicles in temporary parking area and empty spaces in real parking area
        if ((temp_automobiles > 0) && ((mFree_automobile < MAX_REAL_AUTOMOBILES))) {
            sem_wait(&printSemaphore);
            temp_automobiles--;
            mFree_automobile++;
            printf("An automobile is moved from temporary parking to real parking area. Total automobiles in real: %d\n", mFree_automobile);
            printDetail(1000000);
            sem_post(&newAutomobile);
        } else {
            sem_post(&newAutomobile);
            continue;
        }
    }
    return NULL;
}

void *carAttendantPickup(void *arg){

    while (1) {
        sem_wait(&newPickup);

        // Check if all parking spaces are full
        if ((mFree_pickup == MAX_REAL_PICKUPS) && (temp_pickups == MAX_TEMP_PICKUPS)) {
            // exit
            sem_post(&newPickup);
            break;
        }

        if ((temp_pickups > 0) && ((mFree_pickup < MAX_REAL_PICKUPS))) {
            sem_wait(&printSemaphore);
            temp_pickups--;
            mFree_pickup++;
            printf("A pickup is moved from temporary parking to real parking area. Total pickups in real: %d\n", mFree_pickup);
            printDetail(1000000);
            sem_post(&newPickup);
        } else {
            sem_post(&newPickup);
            continue;
        }
    }

    return NULL;
}

void printDetail(int delay){
    // print how many spaces left how many cars parked etc. like table*
    printf("______________________________________________________________________________________________________________________________\n");
    printf("|Automobiles in Real Parking  |    Pickups in Real Parking   |    Automobiles in Temp Parking    |    Pickups in Temp Parking|\n");
    printf("|             %d/%d             |              %d/%d             |              %d/%d                  |             %d/%d           |\n", 
    mFree_automobile, MAX_REAL_AUTOMOBILES, mFree_pickup, MAX_REAL_PICKUPS, temp_automobiles, MAX_TEMP_AUTOMOBILES, temp_pickups, MAX_TEMP_PICKUPS);
    printf("|_____________________________|______________________________|___________________________________|___________________________|\n");
    sem_post(&printSemaphore);
    // wait 1 second
    usleep(delay);
}
