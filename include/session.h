#ifndef _INCLUDE_SESSION_H_
#define _INCLUDE_SESSION_H_

struct session {
	int cli_fd;
	struct sockaddr_in cli_addr;
	unsigned int cli_state;
	struct buffer cli_buffer;

	int srv_fd;
	struct sockaddr_in srv_addr;
	unsigned int srv_state;
	struct buffer srv_buffer;
};

#endif
