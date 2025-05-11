#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Global variable that will be updated by one thread and read by another
volatile int global_var = 0;

// Flag to control when the program should stop
volatile int running = 1;

// Mutex to prevent race conditions while accessing `global_var`
pthread_mutex_t lock;

// Thread function: Periodically updates `global_var`
void *
update_var(void *arg)
{
    while (running) {  // Continue running until stopped
        pthread_mutex_lock(&lock);  // Lock before modifying global_var
        global_var++;               // Increment global_var
        pthread_mutex_unlock(&lock); // Unlock after modification
        usleep(500000);             // Sleep for 0.5 seconds
    }
    return NULL;
}

// Thread function: Periodically prints the value of `global_var`
void *
report_var(void *arg)
{
    while (running) {  // Continue running until stopped
        pthread_mutex_lock(&lock);  // Lock before reading global_var
        printf("Current value: %d\n", global_var);  // Print current value
        pthread_mutex_unlock(&lock); // Unlock after reading
        usleep(1000000);             // Sleep for 1 second
    }
    return NULL;
}

// Main function: Creates and manages threads, waits for user input to stop execution
int
main()
{
    pthread_t updater, reporter;  // Thread identifiers

    pthread_mutex_init(&lock, NULL); // Initialize mutex

    printf("main: begin\n");

    // Create two threads: one for updating and one for reporting
    pthread_create(&updater, NULL, update_var, NULL);
    pthread_create(&reporter, NULL, report_var, NULL);

    // Wait for user input to stop execution
    printf("Press Enter to stop...\n");
    getchar();  // Waits for user to press Enter

    running = 0;  // Set flag to stop both threads

    // Wait for threads to finish before exiting
    pthread_join(updater, NULL);
    pthread_join(reporter, NULL);

    pthread_mutex_destroy(&lock); // Clean up mutex

    printf("main: done\n");

    return 0; 
}
