
#ifndef IPACM_LOG_H
#define IPACM_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <syslog.h>

#define MAX_BUF_LEN 256

#ifdef FEATURE_IPA_ANDROID
#define IPACMLOG_FILE "/dev/socket/ipacm_log_file"
#else/* defined(FEATURE_IPA_ANDROID) */
#define IPACMLOG_FILE "/etc/ipacm_log_file"
#endif /* defined(NOT FEATURE_IPA_ANDROID)*/

typedef struct ipacm_log_buffer_s {
	char	user_data[MAX_BUF_LEN];
} ipacm_log_buffer_t;

void ipacm_log_send( void * user_data);

static char buffer_send[MAX_BUF_LEN];
static char dmesg_cmd[MAX_BUF_LEN];

#define IPACMDBG_DMESG(fmt, ...) memset(buffer_send, 0, MAX_BUF_LEN);\
								 snprintf(buffer_send,MAX_BUF_LEN,"%s:%d %s: " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);\
								 ipacm_log_send (buffer_send);\
								 printf("%s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__); \
								 memset(dmesg_cmd, 0, MAX_BUF_LEN);\
								 snprintf(dmesg_cmd, MAX_BUF_LEN, "echo %s > /dev/kmsg", buffer_send);\
								 system(dmesg_cmd);
#ifdef DEBUG
#define PERROR(fmt)   memset(buffer_send, 0, MAX_BUF_LEN);\
					  snprintf(buffer_send,MAX_BUF_LEN,"%s:%d %s()", __FILE__, __LINE__, __FUNCTION__);\
					  ipacm_log_send (buffer_send); \
                      perror(fmt);
#define IPACMERR(fmt, ...)	memset(buffer_send, 0, MAX_BUF_LEN);\
							snprintf(buffer_send,MAX_BUF_LEN,"ERROR: %s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);\
							ipacm_log_send (buffer_send);\
							printf("ERROR: %s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define IPACMDBG_H(fmt, ...) memset(buffer_send, 0, MAX_BUF_LEN);\
							 snprintf(buffer_send,MAX_BUF_LEN,"%s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);\
							 ipacm_log_send (buffer_send);\
							 printf("%s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);
#else
#define PERROR(fmt)   perror(fmt)
#define IPACMERR(fmt, ...)   printf("ERR: %s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define IPACMDBG_H(fmt, ...) printf("%s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);
#endif
#define IPACMDBG(fmt, ...)	printf("%s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define IPACMLOG(fmt, ...)  printf(fmt, ##__VA_ARGS__);

#ifdef __cplusplus
}
#endif

#endif /* IPACM_LOG_H */
