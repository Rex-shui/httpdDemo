#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static FILE *logFp = NULL;
int global_debug_lvl = DEBUG_INFO;

char debuglvl_to_str[][16] = 
{
	"DEBUG_ERROR",
	"DEBUG_INFO",
};

void log_start()
{
	logFp = fopen(LOG_PATH, "a");
	if(NULL == logFp)
	{
		printf("create log file failed\n");
		return;
	}
}

char *intToStr(int num)
{
	char *ret = NULL;
	int ret_idx = 0;
	ret = (char *)malloc(32);

	while(num != 0)
	{
		ret[ret_idx++] = num % 10 + '0';
		num = num / 10;
	}

	ret[ret_idx] = '\0';

	char t;
	for(int i=0; i<(ret_idx)/2; i++)
	{
		t = ret[i];
		ret[i] = ret[ret_idx-1-i];
		ret[ret_idx-1-i] = t;
	}

	return ret;
}

void log_write(int debugLevel, char *filename, int line, char *format, ...)
{
	if(debugLevel > global_debug_lvl)
	{
		return;
	}

	if(NULL == logFp)
	{
		log_start();
	}

	va_list ap;
	va_start(ap, format);

	char finalStr[2048] = {0};
	int finalStr_idx = 0;

	for(int i=0; format[i]!='\0'; i++)
	{
		if(format[i] != '%')
		{
			finalStr[finalStr_idx++] = format[i];
			continue;
		}

		switch(format[i+1])
		{
			case 'd':
				{
					int num = va_arg(ap, int);
					char *numStr = intToStr(num);
					strcat(finalStr, numStr);
					finalStr_idx += strlen(numStr);
					free(numStr);
					break;
				}
			case 's':
				{
					char *str = va_arg(ap, char *);
					strcat(finalStr, str);
					finalStr_idx += strlen(str);
					break;
				}
			case 'c':
				{
					int ch = va_arg(ap, int);
					finalStr[finalStr_idx++] = format[i+1];
					break;
				}
			default:
				{
					finalStr[finalStr_idx++] = format[i];
					finalStr[finalStr_idx++] = format[i+1];
					break;
				}
		}
		i++;
	}

	va_end(ap);

	time_t sec = time(NULL);
	struct tm *ct = localtime(&sec);
	
	char logMsg[4096] = {0};
	sprintf(logMsg, "[%04d-%02d-%02dT%02d:%02d:%02d][%s %d][%s] %s",\
		ct->tm_year+1900, ct->tm_mon+1, ct->tm_mday, ct->tm_hour, ct->tm_min, ct->tm_sec,\
		filename, line, debuglvl_to_str[debugLevel], finalStr);

	fwrite(logMsg, strlen(logMsg), 1, logFp);
	fflush(logFp);
	
}

void log_end()
{
	fclose(logFp);
}
