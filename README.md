# simple-c-cerver
Simple HTTP and HTTPS server using C language

# Description
This is a simple HTTP server which helps you understand the basic implemetations of a server. It is really easy to configure and install this server. Just take less than a minute :). 

# Configure
1. Create certificate for HTTPS server. Use following command. Reference: (https://stackoverflow.com/questions/25946170/how-can-i-install-ssl-on-localhost-in-ubuntu)
    -  Go to the `src` directory.
    - `sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout localhost.key -out localhost.crt`
2. Go to the directory `src` and it contains the `Makefile`. Then runs the followings commands.
   - `$ make all`  (Compile the code and create executable code)
   - `$ make run`  (Run the executable code)
3. Then the HTTP server runs on 8080 default port and the HTTPS server runs on 8081.
4. Step 1 created certificate file and keyfile which are `localhost.crt` and `localhost.key` respectively. Add certificate to the browser that you currently using and jut connect to the server using a requests like below.
    - http://localhost:8080/
    - https://localhost
