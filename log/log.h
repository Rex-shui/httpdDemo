#ifndef LOG_H
#define LOG_H

#define LOG_PATH "test.log"

enum DEBUG_LEVEL
{
	DEBUG_ERROR = 0,
	DEBUG_INFO,
};

#define DEBUG_ERR(format, ...) log_write(DEBUG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define DEBUG_INFO(format, ...) log_write(DEBUG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

void log_start();
void log_write(int debugLevel, char *filename, int line, char *format, ...);
void log_end();


#endif
