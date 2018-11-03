#include "get_response.h"

void get_response(int* fd_client, char* buf){
    
    char filepath[100];
    char* indexfile = "index.html";
    char *request, *filename;
    const char delimiter[2] = " ";
    const char delimiter1[2] = "/";
    int file_exists = 1;
    int fd;
    struct stat stbuf;
   
      
    strcpy(filepath, DIRECTORY);
    request = strtok(buf, delimiter);
    request = strtok(NULL, delimiter);
    filename = strtok(request, delimiter1);
    if(!filename){
        strcat(filepath, indexfile);
    }else{
        strcat(filepath, filename);
        if(access(filepath, F_OK) == -1){
            printf("File does't exists..: %s\n",filepath);
            strcpy(filepath, DIRECTORY);
            strcat(filepath, ERROR_FILE_NOT_FOUND);
            file_exists = 0;
        }

    }

    if((fd = open(filepath, O_RDONLY)) < 0){
        perror("File open error\n");
        exit(1);
    }

    
    if(file_exists){
        write(*fd_client, HTTP_HEADER, sizeof(HTTP_HEADER) - 1);
    }else{
        write(*fd_client, ERROR_FILE_NOT_FOUND_HEADER, sizeof(ERROR_FILE_NOT_FOUND_HEADER) - 1);
    }
    fstat(fd, &stbuf);
    write(*fd_client,HTTP_CONTENT_HEADER , sizeof(HTTP_CONTENT_HEADER) - 1);
    sendfile(*fd_client, fd, 0, stbuf.st_size);
}

void send_img(int* fd_client, char* buf){
    int fdimg;
    char filepath[100];
    char *filename, *request, *buffer, *read_string;    
    const char delimiter[2] = " ";
    const char delimiter1[2] = "/";
    FILE* fp;
    long length;
    
    strcpy(filepath, DIRECTORY);
    request = strtok(buf, delimiter);
    request = strtok(NULL, delimiter);
    filename = strtok(request, delimiter1);
    
    strcat(filepath, filename);
    if(access(filepath, F_OK) == -1){
        
        strcpy(filepath, DIRECTORY);
        strcat(filepath, ERROR_FILE_NOT_FOUND);
        fp = fopen(filepath, "r");
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = malloc(sizeof(char)*(length+strlen(HTTP_HEADER)+2*strlen(HTTP_CONTENT_HEADER)));
        read_string = malloc(sizeof(char)*length);
        fread(read_string, 1, length, (FILE*)fp);
        strcat(buffer, ERROR_FILE_NOT_FOUND_HEADER);
        strcat(buffer, HTTP_CONTENT_HEADER);
        strcat(buffer, read_string);
        write(*fd_client, buffer, strlen(buffer));
    }else{
        write(*fd_client, HTTP_CONTENT_JPG_HEADER, sizeof(HTTP_CONTENT_JPG_HEADER) - 1);
        fdimg = open(filepath, O_RDONLY);
        int sent = sendfile(*fd_client, fdimg, NULL, 100000);
        close(fdimg);
    }
}

char buffer[MAX_READ_FILE_SIZE];
char* get_response_https(char* buf){
    
    char filepath[100];
    char* indexfile = "index.html";
    char *request, *filename;
    const char delimiter[2] = " ";
    const char delimiter1[2] = "/";
    int file_exists = 1;
    FILE *fs;
    int fd;
    struct stat stbuf;
    char *temp_read;
   
      
    strcpy(filepath, DIRECTORY);
    request = strtok(buf, delimiter);
    request = strtok(NULL, delimiter);
    filename = strtok(request, delimiter1);
    if(!filename){
        strcat(filepath, indexfile);
    }else{
        strcat(filepath, filename);
        if(access(filepath, F_OK) == -1){
            printf("File does't exists..: %s\n",filepath);
            strcpy(filepath, DIRECTORY);
            strcat(filepath, ERROR_FILE_NOT_FOUND);
            file_exists = 0;
        }

    }


    if((fd = open(filepath, O_RDONLY)) < 0){
        perror("File open error\n");
        exit(1);
    }
    
    if(file_exists){
        strcpy(buffer, HTTP_HEADER);
    }else{
        strcpy(buffer, ERROR_FILE_NOT_FOUND_HEADER);
    }
    fstat(fd, &stbuf);
    close(fd);

    if((fs = fopen(filepath, "r")) < 0){
        perror("File open error\n");
        exit(1);
    }
    strcat(buffer, HTTP_CONTENT_HEADER);
    temp_read = malloc(sizeof(char)*(stbuf.st_size));
    fread(temp_read, stbuf.st_size, 1, fs);
    strcat(buffer, temp_read);
    return buffer;

}

char* send_img_https(char* buf){
    int fdimg;
    char filepath[100];
    char *filename, *request, *buffer, *read_string;    
    const char delimiter[2] = " ";
    const char delimiter1[2] = "/";
    FILE* fp;
    long length;
    
    strcpy(filepath, DIRECTORY);
    request = strtok(buf, delimiter);
    request = strtok(NULL, delimiter);
    filename = strtok(request, delimiter1);
    
    strcat(filepath, filename);
    if(access(filepath, F_OK) == -1){
        
        strcpy(filepath, DIRECTORY);
        strcat(filepath, ERROR_FILE_NOT_FOUND);
        fp = fopen(filepath, "r");
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = malloc(sizeof(char)*(length+strlen(HTTP_HEADER)+2*strlen(HTTP_CONTENT_HEADER)));
        read_string = malloc(sizeof(char)*length);
        fread(read_string, 1, length, (FILE*)fp);
        strcpy(buffer, ERROR_FILE_NOT_FOUND_HEADER);
        strcat(buffer, HTTP_CONTENT_HEADER);
        strcat(buffer, read_string);
    }else{
        strcpy(filepath, DIRECTORY);
        strcat(filepath, filename);
        fp = fopen(filepath, "r");
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = malloc(sizeof(char)*(length+strlen(HTTP_HEADER)+2*strlen(HTTP_CONTENT_HEADER)));
        read_string = malloc(sizeof(char)*length);
        fread(read_string, 1, 3000000000, (FILE*)fp);
        strcpy(buffer, HTTP_CONTENT_JPG_HEADER);
        strcat(buffer, read_string);

    }
    return buffer;
}
