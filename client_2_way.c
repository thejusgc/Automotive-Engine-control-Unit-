#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket,server_socket,flag=0;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    char updateBuffer[MAX_BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);//binary byte order formet
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Receive and print the three text messages
    for (int i = 0; i < 3; ++i) {
    flag=0;
    	while(flag==0)
    	{
        memset(buffer, 0, sizeof(buffer));
        if(recv(client_socket, buffer, sizeof(buffer), 0)<0)
        {
        	perror("Cannot receive from server\n");
        }
        printf("\nData Received from server: %s\n", buffer);
        int intbuf=atoi(buffer);
        //printf("reveived int %d\n",intbuf);
        bzero(buffer,strlen(buffer));
	if (intbuf>=1000 && intbuf<=2000)
	{
		strcpy(buffer,"OK: Quantity is within range");
		if(send(client_socket, buffer, strlen(buffer), 0)<0)
		{
			perror("Cannot send to server\n");
		}
		flag=1;
	}
	else if(intbuf<1000)
	{
		strcpy(buffer,"FAULT: Quantity is less then minimum range");
		if(send(client_socket,buffer,strlen(buffer),0)<0)
		{
			perror("Cannot send to server\n");
		}
		
	}
	else
	{
		strcpy(buffer,"FAULT: Quantity is more then minimum range");
		if(send(client_socket, buffer, strlen(buffer), 0)<0)
		{
			perror("Cannot send to server\n");
		}
		
	}
	}
    }
    // Close the socket
    close(client_socket);

    return 0;
}

