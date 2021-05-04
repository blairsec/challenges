#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define DEBUG 0
#define CLOG 0
struct response {	
	unsigned long checksum;
	char payload;
	char id[13];
};
struct cli {
	struct response cur;
	int curoff;
	unsigned int v;
	int flagoff;
	char id[13];
};
unsigned long gchecksum(unsigned long v) {
	srand(v ^ 12345678);
	int rn = 0;
	for (int d = 0; d < 8; d++) {
		rn *= 10;
		rn += rand() % 10;
	}
	return rn * v;
}
int main() {
	struct addrinfo a;
	struct addrinfo* b;
	int yes = 1;
	char badb = 13;
	char goodb = 37;
	struct cli connections[65536];
	char idxor[13] = {0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xca, 0xfe, 0x13, 0x37, 0xab, 0xcd, 0xef};
	memset(&a, 0, sizeof a);
	a.ai_family = AF_UNSPEC;
	a.ai_socktype = SOCK_STREAM;
	a.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, "21450", &a, &b)) {
		printf("getaddrinfo error\n");
		exit(1);
	}
	int serverfd = socket(b->ai_family, b->ai_socktype, b->ai_protocol);
	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes)) {
		printf("setsockopt error\n");
		exit(1);
	}
	if (bind(serverfd, b->ai_addr, b->ai_addrlen)) {
		printf("bind error\n");
		exit(1);
	}
	if (listen(serverfd, 16)) {
		printf("listen error\n");
		exit(1);
	}
	int fdmax = serverfd;
	fd_set fds;
	fd_set temp;
	fd_set errs;
	fd_set kill;
	FD_ZERO(&fds);
	FD_SET(serverfd, &fds);
	FILE *urandom = fopen("/dev/urandom", "r");
	char flag[64];
	FILE *ff = fopen("flag.txt", "r");
	fgets(flag, 64, ff);
	int nconns = 0;
	for (;;) {
		#if CLOG
		printf("%d conns\n", nconns);
		#endif
		temp = fds;
		errs = fds;
		if (select(fdmax+1, &temp, NULL, &errs, NULL) == -1) {
			printf("select error\n");
			exit(1);
		}
		for (int i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &errs)) {
				#if DEBUG
				printf("error on connection %d\n", i);
				#endif
				FD_CLR(i, &fds);
				nconns--;
				continue;
			} else if (FD_ISSET(i, &temp)) {
				if (i == serverfd) {
					int client = accept(serverfd, NULL, NULL);
					char buf;
					if (client == -1) {
						printf("accept error\n");
						continue;
					}
					FD_SET(client, &fds);
					fdmax = fdmax < client ? client : fdmax;
					#if DEBUG
					printf("new connection established %d\n", client);
					#endif
					nconns++;
					for (int j = 0; j < 13; j++) {
						connections[client].id[j] = getc(urandom);
						buf = connections[client].id[j] ^ idxor[j];
						send(client, &buf, 1, 0);
					}
					connections[client].curoff = 0;
					connections[client].v = 0x13371337;
					connections[client].flagoff = 0;
				} else {
					char buf[256];
					int size = recv(i, buf, sizeof buf, 0);
					if (size == -1) {
						printf("recv error %s\n", strerror(errno));
						FD_CLR(i, &fds);
						nconns--;
						continue;
					} else if (size == 0) {
						#if DEBUG
						printf("connection has died %d\n", i);
						#endif
						nconns--;
						FD_CLR(i, &fds);
					} else {
						for (int j = 0; j < size; j++) {
							*((char*) &(connections[i].cur) + connections[i].curoff++) = buf[j];
							if (connections[i].curoff == 22) {
								int good = 1;
								for (int k = 0; k < 13; k++) {
									if (connections[i].cur.id[k] != connections[i].id[k]) {
										#if DEBUG
										printf("bad id byte %d for conn %d - got %d wanted %d\n", k, i, connections[i].cur.id[k], connections[i].id[k]);
										#endif
										good = 0;
									}
								}
								if (connections[i].cur.checksum != gchecksum(connections[i].v)) {
									#if DEBUG
									printf("bad cksum for conn %d - got %lx wanted %lx\n", i, connections[i].cur.checksum, gchecksum(connections[i].v));
									#endif
									good = 0;
								}
								if (connections[i].cur.payload != flag[connections[i].flagoff]) {
									#if DEBUG
									printf("bad flag char for conn %d - got %c wanted %c\n", i, connections[i].cur.payload, flag[connections[i].flagoff]);
									#endif
									send(i, &badb, 1, 0);
									connections[i].curoff = 0;
									continue;
								}
								if (good) {
									#if DEBUG
									printf("conn %d sent char successfully\n", i);
									#endif
									connections[i].flagoff++;
									connections[i].curoff = 0;
									connections[i].v = 0;
									unsigned char vbuf;
									send(i, &goodb, 1, 0);
									for (int k = 0; k < 4; k++) {
										vbuf = rand() % 256;
										send(i, &vbuf, 1, 0);
										connections[i].v <<= 8;
										connections[i].v += vbuf;
									}
								} else {
									#if DEBUG
									printf("killed %d\n", i);
									#endif
									close(i);
									nconns--;
									FD_CLR(i, &fds);
								}
							}
						}
					}
				}
			}
		}
	}
	freeaddrinfo(b);
	
}
