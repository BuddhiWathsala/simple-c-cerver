#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#ifndef GET_RESPONSE
#define GET_RESPONSE

#define MAX_READ_FILE_SIZE 204800000
extern void get_response(int* fd_client, char* buf);
extern char* get_response_https(char* buf);
extern void send_img(int* fd_client, char* buf);
extern char* send_img_https(char* buf);
static char ERROR_FILE_NOT_FOUND[] = "404.html";
static char ERROR_FILE_NOT_FOUND_HEADER[] = "HTTP/1.1 404 Not Found \r\n";
static char DIRECTORY[] = "../resources/";
static char HTTP_HEADER[] = "HTTP/1.1 200 OK \r\n";
static char HTTP_CONTENT_HEADER[] = "Content-Type:text/html; charset=UTF-8\r\n\r\n";
static char HTTP_CONTENT_JPG_HEADER[] = "HTTP/1.1 200 OK \r\nContent-Type: image/jpeg\r\n\r\n";
static char FORMAT_HTML[] = "html";
static char FORMAT_JPG[] = "jpg";
#endif
