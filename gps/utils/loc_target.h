
#ifndef LOC_TARGET_H
#define LOC_TARGET_H
#define TARGET_SET(gnss,ssc) ( (gnss<<1)|ssc )
#define TARGET_DEFAULT       TARGET_SET(GNSS_MSM, HAS_SSC)
#define TARGET_MDM           TARGET_SET(GNSS_MDM, HAS_SSC)
#define TARGET_APQ_SA        TARGET_SET(GNSS_GSS, NO_SSC)
#define TARGET_NO_GNSS       TARGET_SET(GNSS_NONE, NO_SSC)
#define TARGET_MSM_NO_SSC    TARGET_SET(GNSS_MSM, NO_SSC)
#define TARGET_AUTO          TARGET_SET(GNSS_AUTO, NO_SSC)
#define TARGET_UNKNOWN       TARGET_SET(GNSS_UNKNOWN, NO_SSC)
#define getTargetGnssType(target)  (target>>1)

#ifdef __cplusplus
extern "C"
{
#endif

unsigned int loc_get_target(void);

/*The character array passed to this function should have length
  of atleast PROPERTY_VALUE_MAX*/
void loc_get_target_baseband(char *baseband, int array_length);
/*The character array passed to this function should have length
  of atleast PROPERTY_VALUE_MAX*/
void loc_get_platform_name(char *platform_name, int array_length);
/*The character array passed to this function should have length
  of atleast PROPERTY_VALUE_MAX*/
void loc_get_auto_platform_name(char *platform_name, int array_length);
int loc_read_device_soc_id();

/* Please remember to update 'target_name' in loc_log.cpp,
   if do any changes to this enum. */
typedef enum {
    GNSS_NONE = 0,
    GNSS_MSM,
    GNSS_GSS,
    GNSS_MDM,
    GNSS_AUTO,
    GNSS_UNKNOWN
}GNSS_TARGET;

typedef enum {
    NO_SSC = 0,
    HAS_SSC
}SSC_TYPE;

#ifdef __cplusplus
}
#endif

#endif /*LOC_TARGET_H*/
