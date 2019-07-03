
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/pps.h>

#ifndef _TIMEPPS_H
#define _TIMEPPS_H

#ifdef __cplusplus
extern "C" {
#endif
/* time of assert event */
typedef struct  timespec pps_info;
/* represents pps source */
typedef int pps_handle;

 /* Open the PPS source */
static __inline int pps_create(int source, pps_handle *handle)
{
   int ret;
   struct pps_kparams dummy;

   if (!handle)
   {
      errno = EINVAL;
      return -1;
   }
   /* check if current device is valid pps */
   ret = ioctl(source, PPS_GETPARAMS, &dummy);
   if (ret)
   {
      errno = EOPNOTSUPP;
      return -1;
   }
   *handle = source;

   return 0;
}
/* close the pps source */
static __inline int pps_destroy(pps_handle handle)
{
   return close(handle);
}
/*reads timestamp from pps device*/
static __inline int pps_fetch(pps_handle handle, const int tsformat,
                              pps_info *ppsinfobuf,
                              const struct timespec *timeout)
{
   struct pps_fdata fdata;
   int ret;

   if (tsformat != PPS_TSFMT_TSPEC)
   {
      errno = EINVAL;
      return -1;
   }
   if (timeout)
   {
      fdata.timeout.sec = timeout->tv_sec;
      fdata.timeout.nsec = timeout->tv_nsec;
      fdata.timeout.flags = ~PPS_TIME_INVALID;
   }
   else
   {
      fdata.timeout.flags = PPS_TIME_INVALID;
   }
   ret = ioctl(handle, PPS_FETCH, &fdata);

   ppsinfobuf->tv_sec = fdata.info.assert_tu.sec;
   ppsinfobuf->tv_nsec = fdata.info.assert_tu.nsec;

   return ret;
}

#ifdef __cplusplus
}
#endif
#endif
