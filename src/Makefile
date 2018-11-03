all:
	gcc -o server-http server-http.c get_response.c -lssl -lcrypto
	gcc -o server-https server-https.c get_response.c -lssl -lcrypto

run:
	./server-http &
	./server-https &
