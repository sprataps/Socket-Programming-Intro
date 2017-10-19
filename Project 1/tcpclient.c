/* tcp_ client.c */ 
/* Programmed by Adarsh Sethi */
/* September 13, 2017 */     

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

   printf("\nEnter hostname of server: ");
   scanf("%s",server_hostname);
   printf("Enter port number of server: ");
   scanf("%hu",&server_port);

   /* user interface */
   int ch=1;
   int ch1;
   int integer;
   while(1)
   {
	if ((sock_client=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
	{
		perror("\n Client: Cannot open socket");
		exit(1);
	}
	if ((server_hp=gethostbyname(server_hostname))==NULL)
	{
		perror("\n Invalid hostname");
		if (close(sock_client)<0) 
			{ perror("\n Error closing socket");
			exit(1);
			}
	}

	memset(&server_addr, 0, sizeof(server_addr));
   	server_addr.sin_family = AF_INET;
   	memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   	server_addr.sin_port = htons(server_port);

	if (connect(sock_client,(struct sockaddr *) &server_addr,
					sizeof(server_addr)) <0)
	{
		perror("\nClient cannot connect to server");
		close(sock_client);
		exit(1);
	} 
	char character;
	char c;
   	printf("\nPlease input a character (N or C): ");
	scanf(" %c",&c);
	if(/* strlen(character_c)==1 &&*/ c=='N' || c=='C') 
		{
		character=c;	
		}
	else
	     {
		while(1)
	     	{ 	
	      		printf("\nYou have entered a wrong input, you need to enter either N or C\n");
   	      		printf("\nEnter the character again: ");
       	      		scanf(" %c",&c);
		//	int character_c=*c;
			if (/*strlen(character_c)==1 &&*/c=='N' ||c=='C')
				{
				character=c;
				break;
				}
	     	}
	     }
	printf("Enter a number between 1-65535: ");
	scanf("%i",&integer);
        while (integer>65535 || integer <1)
	    {
	     	printf("\nYou have entered a wrong integer, please enter in the range 1-65535, please enter again\n");
             	printf("Enter the number in range 1-65535: ");
             	scanf("%i",&integer);
	    }

	//Sending Character and integer
	int c_send; int i_send;  int c_recv; int i_recv;
	if (c_send=send(sock_client,&character,1,0)==-1)
		{
			printf("Sending Failure");
			break;
		}
	char character_r; 
	if (c_recv=recv(sock_client, &character_r, 1,0) ==-1)
		{
			perror("\nError receiving");
			exit(1);
		}
	printf("Character Received from server: %c",character_r);
	int integer_s=htons(integer);
	if (i_send=send(sock_client,&integer_s,2,0)==-1)
		{
			printf("\nSending Failure");
			break;
		}
	int integer_r;
	if (i_recv=recv(sock_client,&integer_r,2,0)==-1)
		{
			perror("\nError Receiving");
			exit(1);
		}
	printf("\nInteger received from the server: %i",htons(integer_r));
	integer=htons(integer_r);
	//Start receiving stream of numbers/letters
	int n_transmission=0;
	int n_items=0;
	int total_bytes=0;
	int limit;  
	unsigned short int_checksum=0;
	int char_checksum;
	while(integer>0)
	{	printf("\n Receiving %i packet",n_transmission+1);
		int bytes_recv;
		unsigned short int_buf[40];
		char char_buf[80];
		int buffer[80];
		if (character_r=='N')
		{ 
			if (integer<40)
				{limit=integer;}
		 	else {limit=40;}
		}

		if (character_r=='C')
		{
			if (integer<80)
				{
				limit=integer;
				}
			else {limit=80;}
		}

		if (character_r=='N')
		{
			bytes_recv=recv(sock_client,int_buf,limit*2,0);
		}
		else if (character_r=='C')
		{
			bytes_recv=recv(sock_client,char_buf,limit,0);
		}
		if (bytes_recv<0) 
		{
			perror("\n receiving error"); 
			exit(1);
		}
		int i=0;

		if (character_r=='N')
		{	while(i<limit)
			{
				unsigned short n=htons(int_buf[i]);
				i+=1;
				int_checksum+=n;
				n_items+=1;
			}
		}
		else if (character_r=='C')
		{	while(i<limit)
			{
				char n=char_buf[i];
				i+=1;
				//printf("\n %c",n);
				char_checksum+=n;
				n_items+=1;
			}
		}
		n_transmission+=1;
		total_bytes+=bytes_recv;
		integer-=limit;
		//wait for some time
		int wait_time=rand()%10;
		//printf("\n Integer %i",integer);
		if (integer==0) 
			{break;}

		printf("\n\tWaiting for %i secs",wait_time);
		sleep(wait_time);
		printf("\n\tResuming");
	}
	printf("\nTotal items received: %i",n_items);
	printf("\nTotal number of bytes received: %i",total_bytes);
	printf("\nTotal number of transmission: %i",n_transmission);
	if (character_r=='N')
	{
		printf("\nChecksum: %hu",int_checksum);
	}
	else
	{
		printf("\nChecksum: %i",char_checksum);
	}
	printf("\n*---Request finished---*");
	int cl=close(sock_client);
	if (cl<0) 
	{
		perror("\nError closing socket");
		exit(1);
	}
	//i_send=send(sock_client,&integer,1,0);
	printf("\n\nDo you wish to continue to send another request? Enter 1 for yes else enter anything else\n");
	scanf("%i",&ch1);
	if (ch1!=1)
	    {
		printf("\n Exiting\n");
	 	break;
	    }
   }
}
