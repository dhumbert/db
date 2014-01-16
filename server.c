#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "db.h"

void error(char *msg)
{
  perror(msg);
  exit(1);
}

void run_server(int portno)
{
    /*
     * code taken from http://www.linuxhowtos.org/C_C++/socket.htm
     */
    int sockfd, newsockfd, clilen, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
    if (newsockfd < 0) error("ERROR on accept");

    bzero(buffer,256);
    
    n = read(newsockfd,buffer,255);
    
    if (n < 0) error("ERROR reading from socket");
    
    printf("Here is the message: %s\n",buffer);
    
    n = write(newsockfd,"I got your message",18);
    
    if (n < 0) error("ERROR writing to socket");
    
    close(newsockfd);
    close(sockfd);
}

int main(int argc, char *argv[])
{
    int port = 4486;
    run_server(port);
    Db *db = db_init();
    db_set(db, "key1", "value1");
    const char *value = db_get(db, "key1");
    printf("%s\n", value);
}

/*
p_Dict d = dict_init();
    dict_add(&d, "key1", "value1");
    dict_add(&d, "key2", "value2");

    p_DictEntry cursor = dict_iter(&d);

    while (cursor != NULL) {
        printf("%s\n", cursor->key);
        cursor = dict_iter(&d);
    }

    dict_reset(&d);

    const char *value = dict_get(d, "key1");
    printf("Found value: %s\n", value);
*/