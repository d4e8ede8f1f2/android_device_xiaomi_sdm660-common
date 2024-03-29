
#ifndef LOC_LOG_H
#define LOC_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <ctype.h>
#include <stdlib.h>
#include "loc_target.h"

typedef struct
{
   const char *name;
   long        val;
} loc_name_val_s_type;

#define NAME_VAL(x) {"" #x "", x }

#define UNKNOWN_STR "UNKNOWN"

#define CHECK_MASK(type, value, mask_var, mask) \
   (((mask_var) & (mask)) ? (type) (value) : (type) (-1))

#define LOC_TABLE_SIZE(table) (sizeof(table)/sizeof((table)[0]))

/* Get names from value */
const char* loc_get_name_from_mask(const loc_name_val_s_type table[], size_t table_size, long mask);
const char* loc_get_name_from_val(const loc_name_val_s_type table[], size_t table_size, long value);
const char* loc_get_msg_q_status(int status);
const char* loc_get_target_name(unsigned int target);

extern const char* log_succ_fail_string(int is_succ);

extern char *loc_get_time(char *time_string, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* LOC_LOG_H */
