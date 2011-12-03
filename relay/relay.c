#include <stdio.h> 
#include <getopt.h>

/*
 * TODO some tools to add
 * buffer, warn, exit
 */
int source_port, target_port;

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
					// TODO warn
					print_help(argv[0]);
					return 1;
				}
				break;
			case 't':
				target_port = atoi(optarg);
				if (target_port <= 0 || target_port > 65535) {
					// TODO warn
					print_help(argv[0]);
					return 1;
				}
				break;
			default:
				// TODO warn
				print_help(argv[0]);
				return 1;
		}
	}
	if (source_port < 0 || target_port < 0) {
		// TODO warn
		print_help(argv[0]);
		return -1;
	}
	if (source_port == target_port) {
		// TODO warn two ports should not be the same
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	if (init() < 0)
		return -1;

	if (parse_opt(argc, argv) < 0)
		return -1;

	return 0;
}
