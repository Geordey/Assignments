#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

FILE *file;
pthread_mutex_t mutex;

void* handle_client(void* i)
{
    char buffer[1024] = {0};
    int port = (int)i;
    pthread_t thread_id = pthread_self();
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR: Failed to open socket\n");
        return 0;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //or INADDR_ANY
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "ERROR: bind() failed\n");
        fprintf(stderr, "Error code: %s\n", strerror(errno));
        return 0;
    }

    if (listen(sockfd,5) != 0) {
        fprintf(stderr, "ERROR: listen() failed\n");
        return 0;
    }

    fprintf(stderr, "----------\nThread %lu using socket %x\n", (unsigned long)thread_id, sockfd);
    for(;;)
    {   

        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            fprintf(stderr, "ERROR: accept() failed\n");
            return 0;
        }
		

        fprintf(stderr, "connection detected on port %d\n",port);
		#ifdef FULL
        pthread_mutex_lock(&mutex);
        fprintf(file, "Hit on port %d\n",port);
        fflush(file); 
        pthread_mutex_unlock(&mutex);
		#endif
        close(newsockfd);
    }
    close(sockfd);
    fclose(file);

}

int main( int argc, char *argv[] ) 
{
    int min = atoi(argv[1]);
    int max = atoi(argv[2]);

    char *filename;

    if(argv[3] != NULL){
        filename = argv[3];
    }
    else
    {
        fprintf(stderr,"Warning: missing <file>, Saving hits in <hits.log>\n");
        filename = "hits.log";
    }

    file = fopen(filename, "a");  // append the new logs
    if(file == NULL)
    {
        fprintf(stderr, "Could not open %s for writing: %s", filename,
                strerror(errno));
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);

    pthread_t mythread;

    for(int i = min; i <= max; i++)
    {
        pthread_create(&mythread, NULL, handle_client, (int*) i);
        printf("%d\n",i);
    }

    gets();

    pthread_mutex_destroy(&mutex);
    return 0;
}