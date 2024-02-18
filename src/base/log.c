#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <pthread.h>

#include "log.h"
#include "stdext.h"

static const char *logprefix[4] = {
	"ALL: ", "INFO: ", "WARINING: ", "ERROR: "
};

pthread_mutex_t logmutex;

static void logtofile(const char *restrict msg);
// Warning: overwrites 'msg'
static inline void appendTime(char *restrict msg);
// appends runtime info to dest
static void appendRTimeInfo(char *dest, const char *restrict file,
#ifndef NO_FUNC
		const char *restrict func,
#endif
		int line);

static enum LogLevel stdoutMask, fileMask;
static bool doNothing = false;
static FILE *logfile;

void loginit(enum LogLevel _stdoutMask, enum LogLevel _fileMask) {
	stdoutMask = _stdoutMask;
	fileMask = _fileMask;
	if(_stdoutMask == L_NONE && _fileMask == L_NONE)
		doNothing = true;
	logfile = fopen(LOGFILE_NAME, "a");
	if(!logfile) {
		fprintf(stderr, "ERROR: Couldn't open logfile, %s, %s\n", LOGFILE_NAME, strerror(errno));
	}
	pthread_mutex_init(&logmutex, 0);
}

void logdestroy() {
	pthread_mutex_lock(&logmutex);
	if(logfile) {
		fclose(logfile);
		logfile = NULL;
	}
	pthread_mutex_unlock(&logmutex);
	pthread_mutex_destroy(&logmutex);
}

void __log_f_(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *format, ...) {
	if(doNothing || level == L_NONE)
		return;
	va_list ap;
	va_start(ap, format);

	char message[512];
	appendTime(message);
	strncpy(message, logprefix[level], 9);
	{
		char tmp[256];
		vsnprintf(tmp, 256, format, ap);
		strcat(message, tmp);
	}
	va_end(ap);
	appendRTimeInfo(message, file,
#ifndef NO_FUNC
			func,
#endif
			line);
	if(stdoutMask != L_NONE && level >= stdoutMask)
		printf("%s\n", message);
	if(fileMask != L_NONE && level >= fileMask)
		logtofile(message);
}

void __log_(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *msg) {
	if(doNothing || level == L_NONE)
		return;
	char message[512];
	appendTime(message);
	strncat(message, logprefix[level], 9);
	strncat(message, msg, 256);

	appendRTimeInfo(message, file,
#ifndef NO_FUNC
			func,
#endif
			line);

	// outputing
	if(stdoutMask != L_NONE && level >= stdoutMask)
		printf("%s\n", message);
	if(fileMask != L_NONE && level >= fileMask)
		logtofile(message);
}

static void appendRTimeInfo(char *dest, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line) {
	// not ASSERT because it would call log
#ifdef _NTR_ASSERT_H_
	__assert_m(dest != NULL, "Dest was NULL, while appending runtime info", __FILE__,
# ifndef NO_FUNC
			__func__,
# endif
			__LINE__, false);
#endif

	// ignore warning because, it thinks that message can be 512 long, but it at most can be 265
	snprintf(dest+strnlen(dest, 265), 512, "\n\t in file: '%s', "
#ifndef NO_FUNC
			"in function: %s, "
#endif

			"at line: %d, errno: %s\n",
			file,
#ifndef NO_FUNC
			func,
#endif
			line, strerror(errno));
}

static void logtofile(const char *msg) {
	pthread_mutex_lock(&logmutex);
	if(!logfile)
		return;
	// Error already printed in loginit
	fprintf(logfile, "%s", msg);
	//async_write_str(fileno(logfile), (char *)msg, 512);
	pthread_mutex_unlock(&logmutex);
}

static inline void appendTime(char *restrict msg) {
	const time_t current_time = time(NULL);
	const struct tm *tinfo = localtime(&current_time);
	char tstr[24];

	if(strftime(tstr, sizeof(tstr), "%Y.%m.%d %H:%M:%S%n", tinfo) == 0) {
		fprintf(stderr, "UNLOGGED ERROR: strftime returned 0\n");
		strcpy(tstr, "");
	}
	strcpy(msg, tstr);
}

