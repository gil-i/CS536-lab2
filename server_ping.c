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
int main (){
	char *address = "128.10.112.133";
	int socketfd;
	struct sockaddr_in saddress;
	if((socketfd = socket(AF_INET, SOCK_DGRAM, 17))<0){
		perror("Error:");
	}
	int def_port = PORT;
	int tries =0;
	saddress.sin_family = AF_INET;
	saddress.sin_addr.s_addr =inet_addr(address);
	saddress.sin_port = htons(def_port);
	while(bind(socketfd, (struct sockaddr*)&saddress,sizeof(saddress))<0){
		tries++;
		def_port++;
		saddress.sin_port = htons(def_port);
		if (tries>9){
			perror("Error with binding. Reached maximum number or tries.\n Error ");
			exit(BIND_ERROR);
		}
	}

	char  recvline[MAX_MSG];

	printf("Waiting for client request...\n");
	recvfrom(socketfd,recvline,10000,0,NULL,NULL);
	printf("Received: %s\n",recvline);

}
