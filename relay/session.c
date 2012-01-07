#include "../include/session.h"

#include <event.h>
#include "../include/tools.h"

extern struct event_base *base;

void write_to_server_callback(evutil_socket_t fd, short what, void *arg)
{
	write_to_server((struct session* s) arg);
	if (s->cli_buffer.len) {
		add_event_to_base(base, s->srv_fd, EV_WRITE, 
				write_to_server_callback, (void*) s, NULL);
	}
}

void write_to_server(struct session* s)
{
	int sz;

	if (s->cli_state == ST_READ) {
		sz = write(s->srv_fd, s->cli_buffer.content, s->cli_buffer.len);
		if (sz < 0) {
			perror("write to server failed. try again later.");
			add_event_to_base(base, s->srv_fd, EV_WRITE, (void*) s);
			return ;
		}
		s->cli_buffer.len -= sz;
		memmove(s->cli_buffer.content, s->cli_buffer.content + sz, s->cli_buffer.len);
	}
}

void connect_to_server_callback(evutil_socket_t fd, short what, void *arg)
{
	struct session *s;
	size_t sz;

	s = (struct session*) arg;
	s->srv_state = ST_CONNECTED;
	write_to_server(s);
}

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
	if (connect_to_remote_asyn(&s->srv_fd, s->srv_addr) < 0)
		return -1;
	add_event_to_base(base, s->srv_fd, EV_WRITE, connect_to_server_callback, s, NULL);

	return 0;
}

void client_read_callback(evutil_socket_t fd, short what, void *arg)
{
	struct session *s = (struct session*) arg;

	if (s->cli_buffer.len >= MAX_BUFFER_LEN) {
		warn("client buffer full.");
		return ;
	}
	s->cli_buffer.len += read(s->cli_fd, s->cli_buffer.content, 
			MAX_BUFFER_LEN - s->cli_buffer.len - 1);

	if (s->srv_state == ST_NOT_CONECT) {
		connect_to_server(s);
		return ;
	}
	if (s->cli_state != ST_READ) {
	}
}

