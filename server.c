#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/mman.h>
#include "db.h"
#include "util.h"


#ifndef  MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

static Db *db;

const char * handle_command(char *cmd_string)
{
    const char *result_msg;
    char *token, *cmd = "", *key = "", *value = "";
    int i = 0;

    token = strtok(cmd_string, " ");

    while (token != NULL) {
        if (i == 0) {
            cmd = token;
        } else if (i == 1) {
            key = token;
        } else if (i == 2) {
            value = token;
        }

        token = strtok(NULL, " ");
        i++;
    }

    to_lower(cmd);
    if (strcmp(cmd, "set") == 0) {
        if (key == NULL || strcmp(key, "") == 0 || value == NULL || strcmp(value, "") == 0) {
            result_msg = "SYNTAX: SET <key> <value>";
        } else {
            int result = db_set(db, key, value);
            sprintf(result_msg, "%d", result);
        }
    } else if (strcmp(cmd, "get") == 0) {
        result_msg = db_get(db, key);
        if (result_msg == NULL) {
            result_msg = "NULL";
        }
    } else {
        result_msg = "INVALID COMMAND";
    }

    return result_msg;
}

void * handle_connection(void *newsock)
{
    int n, sock = (int) newsock;
    char buffer[256];
    bzero(buffer,256);
    n = read(sock, buffer, 255);

    if (n < 0) {
        perror("ERROR reading from socket");
        pthread_exit(0);
    }

    char *cmd = strip_spaces(buffer);

    const char *result_msg = handle_command(cmd);

    n = write(sock, result_msg, strlen(result_msg));
    if (n < 0) perror("ERROR writing to socket");
    
    close(newsock);
    pthread_exit(0);
}

int run_server(const char *port)
{
    int sock;
    pthread_t thread;
    struct addrinfo hints, *res;
    int reuseaddr = 1; /* True */

    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 0;
    }

    freeaddrinfo(res);

    /* Listen */
    if (listen(sock, 10) == -1) {
        perror("listen");
        return 0;
    }

    /* Main loop */
    while (1) {
        size_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
        if (newsock == -1) {
            perror("accept");
        }
        else {
            printf("Got a connection from %s on port %d\n", 
                    inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
            if (pthread_create(&thread, NULL, &handle_connection, (void *) newsock) != 0) {
                fprintf(stderr, "Failed to create thread\n");
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    const char *port = "4486";
    db = mmap(NULL, sizeof(Db), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    db_init(db);
    db_set(db, "lorem", "ipsum");
    run_server(port);
    // TODO join threads
    //db_set(db, "key1", "value1");
    //const char *value = db_get(db, "key1");
    //printf("%s\n", value);
    munmap(db, sizeof(Db));
}
