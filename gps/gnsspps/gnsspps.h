
#ifndef _GNSSPPS_H
#define _GNSSPPS_H

#ifdef __cplusplus
extern "C" {
#endif

/*  opens the device and fetches from PPS source */
int initPPS(char *devname);
/* updates the fine time stamp */
int getPPS(struct timespec *current_ts, struct timespec *current_boottime, struct timespec *last_boottime);
/* stops fetching and closes the device */
void deInitPPS();

#ifdef __cplusplus
}
#endif
#endif
