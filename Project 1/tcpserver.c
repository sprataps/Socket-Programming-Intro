/* tcpserver.c */
/* Programmed by Adarsh Sethi */
/* September 13, 2017 */    

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

#define SERV_TCP_PORT 7007

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

   char sentence[STRING_SIZE];  /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

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

   for (;;)
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

      /* receive the character and send it back*/
      	char character; int integer;
	int bytes_recd1; int bytes_recd2;

      	bytes_recd1 = recv(sock_connection,&character , 1, 0);
      	if (bytes_recd1!=-1)
		{
			printf("\nReceived character is: %c",character);
	    	}
	else
	    	{
	    		perror("\nReceiving Failure");
	    		exit(1);
	    	}
	//printf("\nSending %c back.",character);
      	int c_send=send(sock_connection, &character, 1,0);
      	if (c_send==-1)
		{
			printf("Failure sending message");
			close(sock_connection);
			break;
		}
	//receive the integer and echo it back
	if (bytes_recd2=recv(sock_connection,&integer,2,0)!=-1)
		{
		printf("\n Received integer is : %i",htons(integer));
		}
	else
		{
			perror("\n Receiving error");
			exit(1);
		}
	//printf("Sending %i back",integer);
	int i_send=send(sock_connection,&integer,2,0);
	if (i_send==-1)	
		{
			perror("\n Failure in sending message");
			exit(1);
		}
	integer=htons(integer);
	//sending stream of data based on input from client
	int bytes_sent=0;
	int total_bytes=0;
	int n_transmission=0;
	int n_items=0;
	int char_checksum=0;
	unsigned short int_checksum=0;
	if (character=='N')
	{
		printf("\nPreparing random %i numbers to send to client",integer);
		while(integer>0 )
		{	printf("\nSending %i packet",n_transmission+1);
			unsigned short int_buf[50];
			int i=0;
			int limit;
			if (integer<=50)
				{limit=integer;}
			else 
				{limit=50;}
			for( i=0;i<limit;i++)
			{
				unsigned short n=rand()%100+1;
				int_checksum+=(n);
				//printf("\n %hu",n);
				int_buf[i]=htons(n);
			}
			n_items+=limit;
			integer=integer-limit;
			bytes_sent=send(sock_connection,int_buf,limit*2,0);
			n_transmission+=1;
			total_bytes+=bytes_sent;
			//printf("\n Integer %i",integer);
			if (integer==0) 
				{break;}
			int wait_time=rand()%10;
			printf("\n\tWaiting for %i secs",wait_time);
			sleep(wait_time);
			printf("\n\tResuming");
			}
		printf("\nNumber of integers transmitted: %i",n_items);
		printf("\nNumber of bytes sent : %i",total_bytes);
		printf("\nNumber of transmissions: %i",n_transmission);
		printf("\nChecksum: %hu",int_checksum);
		printf("\n*---Request server---*");

		}
	else
	{
		printf("\nPreparing random %i characters to send to client",integer);
		while(integer>0)
		{	
			char char_buf[100];
			int i=0;
			int limit;
			if (integer<=100) 
				{limit=integer;} 
			else 
				{limit=100; }
			for (i=0;i<limit;i++)
			{
				char randomletter='A' +(random()%26);
				char_buf[i]=randomletter;
				char_checksum+=randomletter;
			//printf("\n %c",randomletter);
			}
			integer=integer-limit;
			bytes_sent=send(sock_connection,char_buf,limit,0);
			n_items+=limit;
			n_transmission+=1;
			total_bytes+=bytes_sent;
			if (integer<=0) 
				{break;}
			int wait_time=rand()%10;
			printf("\n\tWaiting for %i secs",wait_time);
			sleep(wait_time);
			printf("\n\tResuming");
		}
		printf("\nNumber of characters transmitted: %i",n_items);
		printf("\nNumber of transmissions: %i",n_transmission);
		printf("\nNumber of bytes sent: %i",total_bytes);
		printf("\nChecksum: %i",char_checksum);
		printf("\n*---Request served---*");
	}
	printf("\nWaiting for another request or another client\n\n");
	}
        if(close(sock_connection)==-1) {perror("Error in closing the connection");}
	else {printf("\n Connection closed");}

}

