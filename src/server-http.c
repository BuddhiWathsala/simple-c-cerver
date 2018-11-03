#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <regex.h>

#include "get_response.h"

#define BUFFER_SIZE 20480
#define PORT 8080
#define MAX_FILE_SIZE 40000
#define MAX_CHAR_MATCH 16

int main(int argc, char* argv[]){
	
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client, fd_server_https, fd_client_https, fdimg, on=1;    
    int ret_jpg, ssl_err;
	char buf[BUFFER_SIZE];
    regex_t regex_jpg;
    
	
    fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0){
		perror("Socket");
		exit(1);
	}
	
	//HTTP socket
	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
		
		perror("bind http");
		close(fd_server);
		exit(1);
	}


	if(listen(fd_server, 10) == -1){	
		
		perror("bind");
		close(fd_server);
		exit(1);
	}

	
	while(1){
	
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);
		
        if(fd_client == -1){	
			perror("Connection failed.\n");
			continue;
		}
		

		printf("Got HTTP Connection.\n");
		
		if (!fork()) {
			
			//Child process
			close(fd_server);
			memset(buf, 0, BUFFER_SIZE);
			read(fd_client, buf, BUFFER_SIZE-1);
            ret_jpg = regcomp(&regex_jpg, "/*.jpg", 0);
            if (ret_jpg){
                fprintf(stderr, "Could not compile regex\n");
                exit(1);
            }
            ret_jpg = regexec(&regex_jpg, buf, 0, NULL, 0);
            if (!ret_jpg) {
                send_img((int*)&fd_client, buf);
            } else {
			    get_response(&fd_client, buf);
		    }

            
			close(fd_client);
			printf("Closed HTTP.\n");
			exit(0);
		}
		//Parent process
		close(fd_client);	

	}

	return 0;
 
}



