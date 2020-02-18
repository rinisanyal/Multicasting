/*
 * multicast.c
 *
 *  Created on: 14-Feb-2020
 *      Author: span89
 */


/*

multicast.c

*/
#include"multicast.h"

int main(int argc)
{
	struct sockaddr_in addr;
	int addrlen, sock;
	struct ip_mreq group;

	/* set up socket */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(4321);
	addrlen = sizeof(addr);

	socket_set_reuseaddr(sock);
	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(1);
	}

	if (argc > 1)
		data_send(sock);
	else
		data_receive(sock);

	return 0;
}
