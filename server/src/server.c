#include "server_args.h"
#include "server_signal.h"
#include "message.h"
#include "output.h"
#include "server_threads.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include "common_time.h"

#define MAX_THREADS 1000000

server_args_t args;

int init(int argc, char* argv[]){
    timeup_server = false;
    if(server_args_ctor(&args, argc, argv, 1000000)) return EXIT_FAILURE;
    if(server_install_handlers()) return EXIT_FAILURE;
    if(server_threads_init()) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

void cleanup(void){
    if(server_args_dtor(&args)){
        const char *buf = "Could not destruct args\n";
        write(STDERR_FILENO, buf, strlen(buf));
    }
    if(server_threads_clean()){
        const char *buf = "Could not clean server threads\n";
        write(STDERR_FILENO, buf, strlen(buf));
    }
}

int main(int argc, char *argv[]){
    int ret = EXIT_SUCCESS;

    if(atexit(cleanup)) return EXIT_FAILURE;
    if(init(argc, argv)) return EXIT_FAILURE;

    if(alarm(args.nsecs)) return EXIT_FAILURE;
    
    mkfifo(args.fifoname, 0660);
    
    
    message_t m;
    while(!timeup_server){
        // Open public fifo
        int fifo_des = open(args.fifoname, O_RDONLY);
        if(fifo_des == -1){
            if(errno == EINTR) break;
            else               return EXIT_FAILURE;
        }
        // Read messages
        int r;
        while((r = read(fifo_des, &m, sizeof(message_t))) == sizeof(message_t)){
            if(output(&m, op_RECVD)){ ret = EXIT_FAILURE; break; }
            if(server_create_thread(&m)){ ret = EXIT_FAILURE; break; }
        }
        // Close public fifo
        close(fifo_des);
    }
    
    if (server_close_service(args.fifoname)) ret = EXIT_FAILURE;

    return ret;
}