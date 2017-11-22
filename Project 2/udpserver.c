/* udp_server.c */
/* Programmed by Adarsh Sethi */
/* September 13, 2017 */

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, sendto, and recvfrom */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024

/* SERV_UDP_PORT is the port number on which the server listens for
   incoming messages from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_UDP_PORT 65115

int main(void) {

   int sock_server;  /* Socket on which server listens to clients */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

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

    FILE *server_info;
    server_info=fopen("serverInfo.txt","w");
   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Server: can't open datagram socket\n");
      exit(1);
   }

   /* initialize server address information */

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   server_port = SERV_UDP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address\n");
      close(sock_server);
      exit(1);
   }

   /* wait for incoming messages in an indefinite loop */

   printf("Waiting for incoming messages on port %hu\n\n",
                           server_port);

   client_addr_len = sizeof (client_addr);

   for (;;)
   {

      int bytes_recd_UDP = recvfrom(sock_server, &c_info, 7, 0,(struct sockaddr *) &client_addr, &client_addr_len);
      if (bytes_recd>-1)
      {
           printf("\nReceived info: ");
           printf("\nClient Code: %hu",ntohs(c_info.clientCode));
           if (ntohs(c_info.serverCode)==111)
           {
             printf("\nCorrect Server Code");
             strcpy(s_info.name,"siddharth-singh");
             s_info.size=htons(15);
             fprintf(server_info,"%hu,%s\n",ntohs(c_info.clientCode),c_info.name);
           }
      }
      else
      {
           perror("\nReceiving Failure");
           exit(1);
      }
      s_info.clientCode=ntohs(c_info.clientCode); //echo the same clientCode back
      int bytes_sent_UDP = sendto(sock_server, &s_info, ntohs(s_info.size)+7, 0,(struct sockaddr*) &client_addr, client_addr_len);
      if (bytes_sent==-1)
      {
           printf("Failure sending message");
           close(sock_server);
           break;
      }

   }
   fclose(server_info);
}
