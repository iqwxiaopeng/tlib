#ifndef _LOG_H_
#define _LOG_H_

#include <sys/types.h>

#if __GNUC__ == 2 && __GNUC_MINOR__ < 96
#define __builtin_expect(x, expected_value) (x)
#endif

#ifndef likely
#define likely(x)  __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)  __builtin_expect(!!(x), 0)
#endif

#define APP_FATAL		1  /* critical conditions              */
#define APP_ERROR		2  /* error conditions                 */
#define APP_WARNING		3  /* warning conditions               */
#define APP_INFO		4  /* informational                    */
#define APP_DEBUG		5  /* debug-level messages             */
#define APP_TRACE		6  /* trace-level messages             */

#define DETAIL(level, fmt, args...) \
	write_log (level, "[%s][%d][%s]: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args) 
	
#define SIMPLY(level, fmt, args...) write_log(level, fmt "\n", ##args)

#define LOG_FATAL(fmt, args...)		DETAIL(APP_FATAL, fmt, ##args)
#define LOG_ERROR(fmt, args...)		DETAIL(APP_ERROR, fmt, ##args)
#define LOG_WARN(fmt, args...)		DETAIL(APP_WARNING, fmt, ##args)
#define LOG_INFO(fmt, args...)		DETAIL(APP_INFO, fmt, ##args)
#define LOG_DEBUG(fmt, args...)		DETAIL(APP_DEBUG, fmt, ##args)
#define LOG_TRACE(fmt,args...)		DETAIL(APP_TRACE, fmt, ##args)

void write_log (int lvl, const char* fmt, ...);
int log_init (const char* dir, int lvl, const char* pre_name);
void log_close (void);

#endif //#ifndef LOG_H

