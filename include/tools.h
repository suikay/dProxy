#ifndef _INCLUDE_TOOLS_H_
#define _INCLUDE_TOOLS_H_

#define MAX_BUFFER_LEN	(1 << 19)	// 512K for each side

#define MAX_HOST_LEN	512			// max domain name len

#define container_of(ptr_, type_, member_)  \
    ((type_ *)((char *)ptr_ - (size_t)&((type_ *)0)->member_))

struct buffer
{
	char content[MAX_BUFFER_LEN];
	size_t len;
};

int set_fd_nonblock(int fd)
{       
	int flags;       
	flags = fcntl(fd, F_GETFL);       
	if (flags < 0)               
		return flags;       
	flags |= O_NONBLOCK;       
	if (fcntl(fd, F_SETFL, flags) < 0)               
		return -1;       

	return 0;
}

int get_host_name(char* hostname, size_t &lhostname, char* str, size_t lstr)
{
	char *sta, *end;

	if ((sta = strcasestr(buf, "host:")) == NULL) return -1;
	if ((end = strcasestr(sta, "\r\n")) == NULL) return -1;
	if (end - sta >= lhostname) return -1;

	*lhostname = end - sta;
	strncpy(hostname, sta, lhostname);
	hostname[lhostname] = '\0'; 

	return 0;
}

int resolve(char* hostname, struct sockaddr_in *addr)
{
	struct hostent *host;
	char* mid_ptr;

	if ((mid_ptr = strstr(hostname, ":")) != NULL) {
		*mid_ptr = '\0';
		mid_ptr++;
		addr->sin_port = htons(atoi(mid_ptr));
	} else {
		addr->sin_port = htons(80);
	}
	if ((host = gethostbyname(hostname)) == NULL) { 
		// TODO warn
		return -1;
	}
	addr->sin_addr.s_addr = *((unsigned int *) host->h_addr_list[0]);
	if (mid_ptr != NULL) mid_ptr = ':';

	return 0;
}

int connect_to_remote(int *fd, struct sockaddr_in srv_addr)
{
	if ((*fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { 
		// TODO warn
		return -1;
	}
	// TODO this is a blocking function !!
	if (connect(*fd, &srv_addr, sizeof(srv_addr)) < 0) {
		// TODO warn
		return -1;
	}
	return 0;
}

#endif
