#define MAXPENDING	100
#include <stdio.h> 
#include <getopt.h>
#include <event.h>

#include "../include/tools.h"
/*
 * TODO some tools to add
 * session
 * buffer, warn, exit
 */
int source_port, target_port;
struct event_base *base = event_base_new();

int print_help(char* prog_name)
{
	printf("Usage: \t%s -s source_port -t target_port\n", prog_name);

	return 0;
}

int init()
{
	source_port = target_port = -1;

	return 0;
}

int parse_opt(int argc, char* argv[])
{
	char opt;

	while ((opt = getopt(argc, argv, "st:")) != -1) {
		switch (opt) {
			case 's':
				source_port = atoi(optarg);
				if (source_port <= 0 || source_port > 65535) {
					warn("invalid port number: %s .\n", optarg);
					print_help(argv[0]);
					return 1;
				}
				break;
			case 't':
				target_port = atoi(optarg);
				if (target_port <= 0 || target_port > 65535) {
					warn("invalid port number: %s .\n", optarg);
					print_help(argv[0]);
					return 1;
				}
				break;
			default:
				warn("unknown option: %c .\n", opt);
				print_help(argv[0]);
				return 1;
		}
	}
	if (source_port < 0 || target_port < 0) {
		warn("source_port or target_port not set.\n");
		print_help(argv[0]);
		return -1;
	}
	if (source_port == target_port) {
		warn("source_port and target_port is the same.\n");
		return -1;
	}

	return 0;
}

int create_server(char* hostname, int port) 
{
	int sock;
	struct sockaddr_in srv_addr;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("create socket() for server failed.");
		return -1;
	}
	memset(srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(port); 
	if (bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		perror("bind() for server failed.");
		return -1;
	}
	if (listen(sock, MAXPENDING) < 0) {
		perror("listen() for server failed.");
		return -1;
	}
	return sock;
}

void get_connected(evutil_socket_t fd, short what, void *arg)
{
	int client_fd;
	struct sockaddr_in cli_addr;
	struct session *s;

	unsigned int cli_len = sizeof(cli_addr);
	if ((client_fd = accept(fd, (struct sockaddr *)&cli_addr, &cli_len)) < 0) {
		perror("accept() failed.");
		return -1;
	}
	s = calloc(sizeof(struct session));	
	s->cli_fd = client_fd;
	s->cli_addr = client_fd;
	s->cli_state = ST_CONNECT;
	s->srv_state = ST_NOT_CONNECT;

	add_event_to_base(base, s->cli_fd, EV_READ | EV_PERSIST, client_read_callback, s, NULL);
}

int main(int argc, char* argv[])
{
	if (init() < 0)
		return -1;

	if (parse_opt(argc, argv) < 0)
		return -1;

	if ((sock = create_server(NULL, source_port)) < 0) {
		return -1;
	}
	add_event_to_base(base, sock, EV_READ | EV_PERSIST, get_connected, NULL, NULL);

	event_base_dispatch(base);
	return 0;
}
