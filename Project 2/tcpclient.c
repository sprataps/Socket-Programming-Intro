
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

    struct sockaddr_in server_addr_TCP;  /* Internet address structure that
                                        stores TCP server address */
    struct sockaddr_in server_addr_UDP;/* Internet address structure that
                                        stores UDP servers address*/
    struct sockaddr_in client_addr; /* Internet address structure that
                                        stores client address */
    unsigned short client_port;  /* Port number used by client (local port) */
    struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
    char server_hostname[STRING_SIZE]; /* Server's hostname */
    unsigned short server_port;  /* Port number used by server (remote port) */

    char sentence[STRING_SIZE];  /* send message */
    char modifiedSentence[STRING_SIZE]; /* receive message */
    unsigned int msg_len;  /* length of message */
    int bytes_sent, bytes_recd; /* number of bytes sent or received */

    struct Info
    {
      unsigned short clientCode;
      unsigned short serverCode;
      unsigned short size;
      char name[80];
    };


    struct Info sTCP_info;//structure to hold TCP server information
    struct Info sUDP_info;//structure to hold UDP servers Information
    FILE *client_file;
    client_file=fopen("clientFile.txt","w");

    //connect the socket
    printf("\nEnter hostname of server: ");
    scanf("%s",server_hostname);
    unsigned short port=65115;
    while(port<65116)
    {

   	   //printf("\nEnter port number of server: ");

   	   server_port=port;
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

	    memset(&server_addr_TCP, 0, sizeof(server_addr_TCP));
   	  server_addr_TCP.sin_family = AF_INET;
   	  memcpy((char *)&server_addr_TCP.sin_addr, server_hp->h_addr,server_hp->h_length);
   	  server_addr_TCP.sin_port = htons(server_port);

	    if (connect(sock_client,(struct sockaddr *) &server_addr_TCP,sizeof(server_addr_TCP)) <0)
	     {
		       perror("\nClient cannot connect to server");
		       close(sock_client);
		       //exit(0);
	     }
      else
      {

        int send_bytes=0;
        int recv_bytes=0;
        //Initialise packet to send to TCP server
        struct Info c_info;
        c_info.clientCode=htons(11);
        c_info.serverCode=htons(0);
        c_info.size=htons(0);
        strcpy(c_info.name,"");
        if(send_bytes=send(sock_client,&c_info,6,0)<0)
        {
            perror("\nClient Sending Error");
            close(sock_client);
            //break;
        }
        printf("\nWaiting for response from TCP Server....");
        if(recv_bytes=recv(sock_client,&sTCP_info,6,0)<0)
        {
            perror("\nClient Receiving Error");
            close(sock_client);
            //break;
        }

	      printf("\nReceived Information from TCP server: ");
        printf("\nReceived Client Code: %hu",ntohs(sTCP_info.clientCode));
        printf("\nReceived Server Code: %hu",ntohs(sTCP_info.serverCode));
        c_info.serverCode=(sTCP_info.serverCode); //set Servercode received from TCPserver
        //printf("\nReceived Size: %d",sTCP_info.size);
        //printf("\nReceived Server Name: %s",sTCP_info.name);

	      //print all the information calculated

	      printf("\n\t*---Request finished for TCP Server---*\n\n");
	      int cl=close(sock_client);
	      if (cl<0)
	       {
		         perror("\nError closing socket");
		         exit(1);
	       }

         //ask user for more requests
 	       //printf("\n\nDo you wish to continue to send another request? Enter 1 for yes else enter anything else\n");
 	       //scanf("%i",&ch);

         if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
         {
            perror("Client: can't open datagram socket\n");
            exit(1);
         }

         memset(&client_addr, 0, sizeof(client_addr));
         client_addr.sin_family = AF_INET;
         client_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                        any host interface, if more than one
                                        are present */
         client_addr.sin_port = htons(client_port);

         /* bind the socket to the local client port */

         if (bind(sock_client, (struct sockaddr *) &client_addr,sizeof (client_addr)) < 0)
         {
              perror("Client: can't bind to local address\n");
              close(sock_client);
              exit(1);
         }

         /* Clear server address structure and initialize with server address */
         memset(&server_addr_UDP, 0, sizeof(server_addr_UDP));
         server_addr_UDP.sin_family = AF_INET;
         memcpy((char *)&server_addr_UDP.sin_addr, server_hp->h_addr,server_hp->h_length);
         server_addr_UDP.sin_port = htons(server_port);

         //strcpy(c_info.name,"");
         int bytes_sent_UDP=sendto(sock_client,&c_info,7,0,(struct sockaddr *)&server_addr_UDP,sizeof(server_addr_UDP));

         //receive bytes from UDP server
         printf("\nWaiting for response from UDP Server....");
         int bytes_recd_UDP=recvfrom(sock_client,&sUDP_info,STRING_SIZE,0,(struct sockaddr *)  0, (int *) 0);
         printf("\nThe response from the UDP server is: ");
         printf("\nReceived Client Code: %hu",ntohs(sUDP_info.clientCode));
         printf("\nReceived Server Code: %hu",ntohs(sUDP_info.serverCode));
         printf("\nReceived Size: %hu",ntohs(sUDP_info.size));
         printf("\nReceived Server Name: %s",sUDP_info.name);
         fprintf(client_file,"%hu,%hu,%s\n",port,ntohs(sUDP_info.serverCode),sUDP_info.name);
         close(sock_client);

         printf("\n\t*---Request finished for UDP Server---*\n\n");

	       //ask user for more requests
	       //printf("\n\nDo you wish to continue to send another request? Enter 1 for yes else enter anything else\n");
	       //scanf("%i",&ch);
       }
     port=port+1;
     if (port==65116)
	   {
		      printf("/nExiting/n");
	 	      break;
	   }
	}
  fclose(client_file);
}
