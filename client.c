#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int dictum_connect(const char *hostname, int port)
{
    int sockfd;
    struct hostent *server;
    struct sockaddr_in serv_addr;

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR connecting");

    return sockfd;
}

void dictum_disconnect(int socket)
{
    close(socket);
}

int main(int argc, char *argv[])
{
    int sockfd, port, n;

    char buffer[256];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    const char *hostname = argv[1];
    port = atoi(argv[2]);
    
    while (1) {
        printf("%s:%d> ", argv[1], port);

        bzero(buffer,256);
        fgets(buffer,255,stdin);

        sockfd = dictum_connect(hostname, port);

        n = write(sockfd,buffer,strlen(buffer));

        if (n < 0) error("ERROR writing to socket");

        bzero(buffer,256);
        
        n = read(sockfd,buffer,255);
        
        if (n < 0) error("ERROR reading from socket");

        printf("%s\n",buffer);
        
        dictum_disconnect(sockfd);
    }
    return 0;
}
