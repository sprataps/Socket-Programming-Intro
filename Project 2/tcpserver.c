
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

#define SERV_TCP_PORT 65115

int main(void) {

   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr_TCP;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_TCP_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   //Initializing server variables
   struct Info
  {
      unsigned short clientCode;
      unsigned short serverCode;
      unsigned short size;
      char name[80];
    };

    struct Info s_info;//structure to send serverInfo
    struct Info c_info;//structure to receive clientInfo
    s_info.clientCode=htons(0);
    s_info.serverCode=htons(111);
    s_info.size=htons(0);
    strcpy(s_info.name,"");

   /* open a socket */
   	if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
     	perror("Server: can't open stream socket");
      	exit(1);
   	}

   /* initialize server address information */
   	memset(&server_addr_TCP, 0, sizeof(server_addr_TCP));
   	server_addr_TCP.sin_family = AF_INET;
   	server_addr_TCP.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   	server_port = SERV_TCP_PORT; /* Server will listen on this port */
   	server_addr_TCP.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   	if (bind(sock_server, (struct sockaddr *) &server_addr_TCP,
                                    sizeof (server_addr_TCP)) < 0) {
     	perror("Server: can't bind to local address");
      	close(sock_server);
      	exit(1);
   	}

   /* listen for incoming requests from clients */
   for(;;)
   {
   	if (listen(sock_server, 50) < 0)
		{    /* 50 is the max number of pending */
      			perror("Server: error on listen"); /* requests that will be queued */
      			close(sock_server);
      			exit(1);
   		}
   	printf("\nI am here to listen ... on TCP on port %hu\n\n", server_port);

   	client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */

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
      	int bytes_recd = recv(sock_connection, &c_info, 7, 0);
      	if (bytes_recd!=-1)
		      {
			         printf("\nReceived info: ");
               printf("\nClient Code: %hu",ntohs(c_info.clientCode));
	    	  }
	      else
	    	  {
	    		     perror("\nReceiving Failure");
	    		     exit(1);
	    	  }

        //echo the character back to the client
        s_info.clientCode=ntohs(c_info.clientCode); //echo the same clientCode back
      	int c_send=send(sock_connection, &s_info, 7,0);
      	if (c_send==-1)
		      {
			         printf("Failure sending message");
			         close(sock_connection);
			         break;
		      }


		    printf("\n\t*---Request served---*");

	//wait for another call from client
		    //printf("\nWaiting for another request or another client\n\n");
        if(close(sock_connection)==-1)
        {
            perror("Error in closing the connection");
        }

      }

}
