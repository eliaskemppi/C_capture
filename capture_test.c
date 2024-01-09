#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include "my_capture.h"
#include <time.h>

pthread_mutex_t mutex;//initializing mutex

int increment = 0;//initializing incrementer

//function for threads
void* get_rand_int() {
    pthread_mutex_lock(&mutex); //mutex locking
    int r = rand()%10; //rng 
    int* number = malloc(sizeof(int)); //initialzing return value
    *number = increment + r; //return value
    usleep(1000);//sleep to test mutexes
    increment += 10;//incerement by 10
    //printf("%d ",*number);
    pthread_mutex_unlock(&mutex);//mutex unlocking
    return (void*) number; //returning return value
}

int main(int argc, char* argv[]) {
    //testing parameters are valid
    if(argc!=3){
        printf("usage: ./capture_test [amount of numbers] [log file]\n");
        exit(1);
    }
    int n = atoi(argv[1]);//changing number parameter from string to int
    //handing errors in number parameter (must be between 0 and 10 and a number)
    if(n<=0 || n>10) {
        printf("please choose number between 1 and 10\n");
        exit(1);
    }
    //capture(argv[2]);
    srand(time(NULL)); //random seed
    //creating fifo for inter-process communication
    if(mkfifo("myfifo",0777)==-1) {
        if(errno != EEXIST) {
            exit(1);
        }
    }
    int pid = fork();
    if(pid==0){
    //in child process
        int fifo_fd = open("myfifo",O_WRONLY); //opening fifo for writing 

        pthread_t p[n]; //initializing n threads
        //creating threads
        for(int i=0;i<n;i++) { 
            pthread_create(&p[i],NULL,&get_rand_int,NULL);
        }
        pthread_mutex_init(&mutex, NULL);//initializing mutex
        //joining threads and using writing return value to fifo
        for(int i = 0; i<n;i++) {
            int* res; //initializing result
            pthread_join(p[i], (void**) &res);//joining thread and getting return value
            write(fifo_fd, res,sizeof(int));//writing to fifo
            free(res); //freeing result from memory
        }
    }else{
        printf("capturing...\n");
        capture(argv[2]); //my own capture function, captures to file given as argument
        int buffer[1]; //initializing buffer for reading
        int fifo_fd_read = open("myfifo", O_RDONLY);//opening file for reading
        //reading each number from fifo and printing them (printout gets captured)
        for(int j=0;j<n;j++){
            read(fifo_fd_read, &buffer[0], sizeof(int));
            printf("%d ",buffer[0]);
        }
            printf("\n");
        wait(NULL);
    }
    return 0;
}