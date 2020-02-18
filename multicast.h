/*
 * multicast.h
 *
 *  Created on: 17-Feb-2020
 *      Author: span89
 */

#ifndef MULTICAST_H_
#define MULTICAST_H_
#include<stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

struct server_args{
	//int  sock;
	struct ip_mreq group;
	char message[50];
}SA;
void data_send(int);
void client_receive(int);

void data_send(int sock){
	struct sockaddr_in addr;
	unsigned int addrlen = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(4321);
	addr.sin_addr.s_addr = inet_addr("226.1.1.4");
	while (1) {
		time_t t = time(0);
		sprintf(SA.message, "time is %-24.24s", ctime(&t));
		printf("sending: %s\n", SA.message);
		int cnt = sendto(sock, SA.message, sizeof(SA.message), 0,
				(struct sockaddr *) &addr, addrlen);
		if (cnt < 0) {
			perror("sendto");
			exit(1);
		}

		sleep(5);
	}
}

void data_receive(int sock){
	struct sockaddr_in addr;
	unsigned int addrlen = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(4321);
	/* if (bind(SA.sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
	         perror("bind");
		 exit(1);
	      }*/
	SA.group.imr_multiaddr.s_addr = inet_addr("226.1.1.4");
	SA.group.imr_interface.s_addr = inet_addr("0.0.0.0");
	//group.imr_interface.s_addr = inet_addr("192.168.78.154");

	//group.imr_interface.s_addr = htonl(INADDR_ANY);
	if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&(SA.group), sizeof(SA.group)) < 0)
	{
		perror("Adding multicast group error");
		close(sock);
		exit(1);
	}

	else
		printf("Adding multicast group...OK.\n");
	while (1) {
		int cnt = recvfrom(sock, SA.message, sizeof(SA.message), 0,
				(struct sockaddr *) &addr, &addrlen);
		if (cnt < 0) {
			perror("recvfrom");
			exit(1);
		} else if (cnt == 0) {
			break;
		}
		printf("%s: message = \"%s\"\n", inet_ntoa(addr.sin_addr), SA.message);
	}
}

static void socket_set_reuseaddr(int sock) {
	int reuse = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
	{
		perror("Setting SO_REUSEADDR error");
		close(sock);
		exit(1);
	}

	else
		printf("Setting SO_REUSEADDR...OK.\n");

}


#endif /* MULTICAST_H_ */
