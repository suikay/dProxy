#include "../include/session.h"

#include <event.h>

extern struct event_base *base;

int connect_to_server(struct session* s)
{
	char* buf = s->buffer.content;
	size_t len = s->buffer.len;
	char* sta, end;
	static char hostname[MAX_HOST_LEN];
	int lhostname;

	buf[len] = '\0';
	lhostname = MAX_BUFFER_LEN;
	if (get_host_name(hostname, &lhostname, buf, len) < 0)
		return -1;
	if (resolve(hostname, &s->srv_addr) < 0)
		return -1;
	if (connect_to_remote(&s->srv_fd, s->srv_addr) < 0)
		return -1;
	return 0;
}

void client_read_callback(evutil_socket_t fd, short what, void *arg)
{
	struct session *s = (struct session*) arg;

	if (s->cli_buffer.len >= MAX_BUFFER_LEN) {
		// TODO warn
		return ;
	}
	s->cli_buffer.len += read(s->cli_fd, s->cli_buffer.content, MAX_BUFFER_LEN - s->cli_buffer.len - 1);

	if (s->srv_state == ST_NOT_CONECT) {
		if (connect_to_server(s) < 0)
			return;
	}
	if (s->cli_state != ST_READ) {
	}
}
