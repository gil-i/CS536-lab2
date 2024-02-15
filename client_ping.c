#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_MSG 512
#define CLIENT_DOM "/tmp/myclient"
#define PORT 4444
#define BIND_ERROR 1
int main (int argc, char * argv[]){
	char *address = argv[1];
	char *arg_int = argv[2];
	int   def_port= atoi(arg_int);
	char *ownaddress = argv[3];
	int socketfd;
	struct sockaddr_in saddress, caddress;
	if((socketfd = socket(AF_INET, SOCK_DGRAM, 17))<0){
		perror("Error:");
	}

	// server info
	saddress.sin_family = AF_INET;
	saddress.sin_addr.s_addr =inet_addr(address);
	saddress.sin_port = htons(def_port);

	//client info
        caddress.sin_family = AF_INET;
        caddress.sin_addr.s_addr =inet_addr(ownaddress);
        caddress.sin_port = 0; // ephemeral client's address

	// binding to port (client)
	int tries =0;
	while(bind(socketfd, (struct sockaddr*)&caddress,sizeof(caddress))<0){
		tries++;
		if (tries>9){
			perror("Error with binding. Reached maximum number or tries.\n Error ");
			exit(BIND_ERROR);
		}
	}

	char* message = "012";
	socklen_t len;
	len =  sizeof(saddress);

// sending
        struct timeval start;	/* starting time */
	struct timeval end;
	unsigned long e_usec;
        gettimeofday(&start,0);

	sendto(socketfd,(const char *)message,strlen(message), MSG_CONFIRM, (const struct sockaddr *) &saddress, len) ;

	char  recvline[MAX_MSG];

	printf("Waiting for server response...\n");
	recvfrom(socketfd, recvline,1000, MSG_WAITALL, (struct sockaddr *)&saddress, &len );
	gettimeofday(&end,0);
	e_usec = ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);

	printf("Received: %s\n",recvline);
	printf("Time: %ld microsecs\n", e_usec);
}
