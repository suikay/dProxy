#ifndef _INCLUDE_LOG_H_
#define _INCLUDE_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int warn(const char* fmt, ...)
{
	static char buffer[256];
	int buf_size = 255, sz;
	va_list ap;
	va_start(ap, fmt);
	sz = snprintf(buf, buf_size, "warn: %s", fmt);
	snprintf(buf+sz, buf_size-sz, buf, ap);
	va_end(ap); 

	return 0;
}

int error(const char* fmt, ...)
{
	static char buffer[256];
	int buf_size = 255, sz;
	va_list ap;
	va_start(ap, fmt);
	sz = snprintf(buf, buf_size, "error: %s", fmt);
	snprintf(buf+sz, buf_size-sz, buf, ap);
	va_end(ap); 

	return 0;
}

#endif
