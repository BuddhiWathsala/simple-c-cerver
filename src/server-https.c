#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <regex.h>
#include "get_response.h"

#define PORT 8081
#define BUFFER_SIZE 20480


SSL_CTX *sslctx;
SSL *cSSL;
void InitializeSSL() {
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

void DestroySSL() {
    ERR_free_strings();
    EVP_cleanup();
}

void ShutdownSSL() {
    SSL_shutdown(cSSL);
    SSL_free(cSSL);
}

int main(int argc, char* argv[]){
	struct sockaddr_in client_addr_https;
	struct sockaddr_in server_addr_https;
	socklen_t sin_len_https = sizeof(client_addr_https);
    int fd_server_https, newsockfd;
	int ssl_err, ret_jpg, on = 1;
	char buf[BUFFER_SIZE];
	char *buffer;
	regex_t regex_jpg;
	InitializeSSL();
	
	fd_server_https = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_server_https< 0) {
	    return 0;
	}
	bzero((char *) &server_addr_https, sizeof(server_addr_https));
	server_addr_https.sin_family = AF_INET;
	server_addr_https.sin_addr.s_addr = INADDR_ANY;
	server_addr_https.sin_port = htons(PORT);

	bind(fd_server_https, (struct sockaddr *) &server_addr_https, sizeof(server_addr_https));

	listen(fd_server_https, 10);
	while(1){
        newsockfd = accept(fd_server_https, (struct sockaddr *) &client_addr_https, &sin_len_https);
        if(newsockfd == -1)
            continue;
        char* message = "<html><head><title>https</title></head><body><h1>HTTPS</h1></body></html>";
        
        sslctx = SSL_CTX_new( SSLv23_server_method());
        SSL_CTX_set_options(sslctx, SSL_OP_SINGLE_DH_USE);
        int use_cert = SSL_CTX_use_certificate_file(sslctx, "./localhost.crt" , SSL_FILETYPE_PEM);
        
        printf("Got HTTPS Connection.\n");
        int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, "./localhost.key", SSL_FILETYPE_PEM);

        cSSL = SSL_new(sslctx);
        SSL_set_fd(cSSL, newsockfd );
        ssl_err = SSL_accept(cSSL);
        if(ssl_err <= 0) {
            ShutdownSSL();
        }
  
        SSL_read(cSSL, buf, BUFFER_SIZE-1);
	    ret_jpg = regcomp(&regex_jpg, "/*.jpg", 0);
        if (ret_jpg){
            fprintf(stderr, "Could not compile regex\n");
            exit(1);
        }
	    ret_jpg = regexec(&regex_jpg, buf, 0, NULL, 0);
	    if (!ret_jpg) {
            buffer = send_img_https(buf);
            SSL_write(cSSL, buffer, strlen(buffer));
        } else {
            buffer = get_response_https(buf);
            SSL_write(cSSL, buffer, strlen(buffer));
	    }
        SSL_free(cSSL);
        close(newsockfd);
        printf("Closed HTTPS.\n");
            
	}
    close(newsockfd);
    SSL_CTX_free(sslctx); 	
    
    EVP_cleanup();
    return 0;
}
