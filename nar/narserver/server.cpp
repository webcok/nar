/*
Author: dogu
Task: Server side for listen()
Date: 06.11.16
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>

using namespace std;

void myread(int sock_num)
{
	int n;
	char buffer[10000];

	ofstream fout;
    
    fout.open("file", ios::binary | ios::out);

	bzero(buffer,10000);
    n = read(sock_num, buffer, 9999);
    std::cout << n  << std::endl;
    if(n < 0 ){
    	cout << "problem in reading: " << endl;
    }
    //print the message from client.
	cout << buffer << endl;
    fout << buffer;
    //fout.write(buffer, n);
    //optional
	
    fout.close();
}

void writeResp(int sock_num){
	int n;
	char buffer[10000];
	const char tmp[500] = "\"header\": {\"channel\": \"sp\",\"status-code\": \"2xx\" OR \"3xx\"\"reply-to\": \"file_push_request\" },\"payload\": {\"chunk-size\": chunk-size,\"peer-list\": [\"1\": {\"peer_id\": peer_id1, \"chunk_id\": chunk_id1}, \"2\": {\"peer_id\": peer_id2, \"chunk_id\": chunk_id2}],\"file-id\": file_id }";
	int i;
	for( i = 0; i < strlen(tmp); ++i){
	buffer[i] = tmp[i];
	}
	buffer[i] = '\0';

	std::cout << buffer << std::endl;

    n = write(sock_num, buffer , strlen(buffer));
    if(n < 0){
    	cout << "problem in writing: " << endl;
    }
}

int main(int argc, char *argv[])
{
	int socket_fd, newsocket_fd, port_no;//size of client address for accept() call
	socklen_t client_len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	//domain, type, protocol
	socket_fd = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP

	if(socket_fd < 0){
		cout << "ERROR on socket() " << endl;
		exit(1);
	}

	/*
	 * Socket address, internet style.
	 *
	struct sockaddr_in {
		short	sin_family; //IPv4 or IPv6
		u_short	sin_port;  //port no
		struct	in_addr sin_addr; //in_addr only contains s_addr
		char	sin_zero[8];
	};
	*/

	bzero( (char *) &server_addr, sizeof(server_addr)); //values in buffer are set to 0

	port_no = atoi(argv[1]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_no); //converts to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int on = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on));

    if(bind(socket_fd,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0 ){
    	cout << "ERROR on bind() " << endl;
    	exit(2);
    }

    listen(socket_fd,10);

    client_len = sizeof(client_addr);

    int pid;
    while (1) {
	     newsocket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_len);
	     if (newsocket_fd < 0) 
	         cout << ("ERROR on accept() ") << endl;
	     pid = fork();
	     if (pid < 0)
	         cout << ("ERROR on fork() ") << endl;
	     if (pid == 0) { //child
	         close(socket_fd);
	         myread(newsocket_fd);
			 //myread(newsocket_fd);
			 writeResp(newsocket_fd);
	         exit(0);
	     }
	     else 
	     	close(newsocket_fd);
	 }
	return 0;
}
