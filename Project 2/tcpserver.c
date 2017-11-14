
#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024

/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 48111

int main(void) {

   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   //Initializing server variables
   char sentence[STRING_SIZE];
   char receivedInfo[STRING_SIZE];
   unsigned short server_code=111;
   char server_code_string[10];
   char server_name[100]="siddharth-singh";
   char *size="15";
   sprintf(server_code_string,"%d",server_code);
   strcat(sentence,server_code_string);
   strcat(sentence,"0");
   strcat(sentence,size);
   strcat(sentence,"Name");
   strcat(sentence,server_name);
   printf("%s",sentence);

   /* open a socket */
   	if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
     	perror("Server: can't open stream socket");
      	exit(1);
   	}

   /* initialize server address information */
   	memset(&server_addr, 0, sizeof(server_addr));
   	server_addr.sin_family = AF_INET;
   	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   	server_port = SERV_TCP_PORT; /* Server will listen on this port */
   	server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   	if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
     	perror("Server: can't bind to local address");
      	close(sock_server);
      	exit(1);
   	}

   /* listen for incoming requests from clients */

   	if (listen(sock_server, 50) < 0)
		{    /* 50 is the max number of pending */
      			perror("Server: error on listen"); /* requests that will be queued */
      			close(sock_server);
      			exit(1);
   		}
   	printf("I am here to listen ... on port %hu\n\n", server_port);

   	client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */
   for(;;)
    {
      	sock_connection = accept(sock_server, (struct sockaddr *) &client_addr,
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
      	if (sock_connection < 0)
		      {
         		   perror("Server: accept() error\n");
         		   close(sock_server);
         		   exit(1);
      		}
          printf("Server is here");
	       //receive the clientInformation
      	int bytes_recd = recv(sock_connection,&receivedInfo , STRING_SIZE, 0);
      	if (bytes_recd!=-1)
		      {
			         printf("\nReceived info: %s",receivedInfo);
	    	  }
	      else
	    	  {
	    		     perror("\nReceiving Failure");
	    		     exit(1);
	    	  }

        //echo the character back to the client
      	int c_send=send(sock_connection, &sentence, STRING_SIZE,0);
      	if (c_send==-1)
		      {
			         printf("Failure sending message");
			         close(sock_connection);
			         break;
		      }


		    printf("\n\t*---Request served---*");

	//wait for another call from client
		    printf("\nWaiting for another request or another client\n\n");
        if(close(sock_connection)==-1) {perror("Error in closing the connection");}
      }

}
