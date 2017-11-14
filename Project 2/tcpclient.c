
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, connect, send, and recv */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024

int main(void) {
    int sock_client;  /* Socket used by client */

    struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
    struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
    char server_hostname[STRING_SIZE]; /* Server's hostname */
    unsigned short server_port;  /* Port number used by server (remote port) */

    char sentence[STRING_SIZE];  /* send message */
    char modifiedSentence[STRING_SIZE]; /* receive message */
    unsigned int msg_len;  /* length of message */
    int bytes_sent, bytes_recd; /* number of bytes sent or received */

   /* enter hostname and port number */

    //Initialising the client variables
    unsigned short client_code=11;
    char client_code_string[10];
    char client_name[100]="siddharth-singh";
    char *size="15";
    sprintf(client_code_string,"%d",client_code);
    strcat(sentence,client_code_string);
    strcat(sentence,"0");
    strcat(sentence,size);
    strcat(sentence,"Name");
    strcat(sentence,client_name);
    printf("%s",sentence);
    int ch=1;
    while(ch)
    {
	     //connect the socket
	     printf("\nEnter hostname of server: ");
	     scanf("%s",server_hostname);
   	   printf("Enter port number of server: ");
   	   scanf("%hu",&server_port);
	     if ((sock_client=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
	      {
		        perror("\n Client: Cannot open socket");
		        exit(1);
	      }
	     if ((server_hp=gethostbyname(server_hostname))==NULL)
	      {
		        perror("\n Invalid hostname");
		        if (close(sock_client)<0)
			         {
                 perror("\n Error closing socket");
			           exit(1);
			         }
	     }

	    memset(&server_addr, 0, sizeof(server_addr));
   	  server_addr.sin_family = AF_INET;
   	  memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,server_hp->h_length);
   	  server_addr.sin_port = htons(server_port);

	    if (connect(sock_client,(struct sockaddr *) &server_addr,sizeof(server_addr)) <0)
	     {
		       perror("\nClient cannot connect to server");
		       close(sock_client);
		       exit(1);
	     }
      int send_bytes=0;
      int recv_bytes=0;
      if(send_bytes=send(sock_client,&sentence,100,0)<0)
      {
          perror("\nClient Sending Error");
          close(sock_client);
          break;
      }
      if(recv_bytes=recv(sock_client,&modifiedSentence,100,0)<0)
      {
          perror("\nClient Receiving Error");
          close(sock_client);
          break;
      }

	   printf("\nReceived Information from server: %s",modifiedSentence);

	//print all the information calculated

	   printf("\n\t*---Request finished---*");
	   int cl=close(sock_client);
	   if (cl<0)
	    {
		      perror("\nError closing socket");
		      exit(1);
	    }

	   //ask user for more requests
	   printf("\n\nDo you wish to continue to send another request? Enter 1 for yes else enter anything else\n");
	   scanf("%i",&ch);
	   if (ch!=1)
	   {
		      printf("\n Exiting\n");
	 	      break;
	   }
	}
}
