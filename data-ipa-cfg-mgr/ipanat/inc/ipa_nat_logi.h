
#ifndef IPA_NAT_LOGI_H
#define IPA_NAT_LOGI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <syslog.h>

#define PERROR(fmt) printf("%s:%d %s()", __FILE__, __LINE__, __FUNCTION__);\
                    perror(fmt);

#define IPAERR(fmt, ...)  printf("ERR: %s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);

#ifdef DEBUG
#define IPADBG(fmt, ...) printf("%s:%d %s() " fmt, __FILE__,  __LINE__, __FUNCTION__, ##__VA_ARGS__);

#define IPADUMP(fmt, ...) printf(fmt, ##__VA_ARGS__);

#else
#define IPADBG(fmt, ...)
#define IPADUMP(fmt, ...)
#endif


#ifdef __cplusplus
}
#endif

#endif /* IPA_NAT_LOGI_H */
