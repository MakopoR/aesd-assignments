#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "sys/queue.h" // queue taken from FreeBSD 10
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <syslog.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>

struct thread_data{
    /*
     * TODO: add other values your thread will need to manage
     * into this structure, use this structure to communicate
     * between the start_thread_obtaining_mutex function and
     * your thread implementation.
     */

    /**
     * Set to true if the thread completed with success, false
     * if an error occurred.
     */
    int *connfd;
    int *packet_memory_fd;
    struct sockaddr_in clnt_addr;
    bool complete;
    pthread_mutex_t *mutex;

};
