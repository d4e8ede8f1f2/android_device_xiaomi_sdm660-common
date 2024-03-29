
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <log_util.h>
#include "loc_target.h"
#include "loc_log.h"
#include <loc_pla.h>

#define APQ8064_ID_1 "109"
#define APQ8064_ID_2 "153"
#define MPQ8064_ID_1 "130"
#define MSM8930_ID_1 "142"
#define MSM8930_ID_2 "116"
#define APQ8030_ID_1 "157"
#define APQ8074_ID_1 "184"

#define LINE_LEN 100
#define STR_LIQUID      "Liquid"
#define STR_SURF        "Surf"
#define STR_MTP         "MTP"
#define STR_APQ         "apq"
#define STR_SDC         "sdc"  // alternative string for APQ targets
#define STR_QCS         "qcs"  // string for Gen9 APQ targets
#define STR_MSM         "msm"
#define STR_SDM         "sdm"  // alternative string for MSM targets
#define STR_APQ_NO_WGR  "baseband_apq_nowgr"
#define STR_AUTO        "auto"
#define IS_STR_END(c) ((c) == '\0' || (c) == '\n' || (c) == '\r')
#define LENGTH(s) (sizeof(s) - 1)
#define GPS_CHECK_NO_ERROR 0
#define GPS_CHECK_NO_GPS_HW 1

static unsigned int gTarget = (unsigned int)-1;

static int read_a_line(const char * file_path, char * line, int line_size)
{
    FILE *fp;
    int result = 0;

    * line = '\0';
    fp = fopen(file_path, "r" );
    if( fp == NULL ) {
        LOC_LOGE("open failed: %s: %s\n", file_path, strerror(errno));
        result = -1;
    } else {
        int len;
        fgets(line, line_size, fp);
        len = strlen(line);
        len = len < line_size - 1? len : line_size - 1;
        line[len] = '\0';
        LOC_LOGD("cat %s: %s", file_path, line);
        fclose(fp);
    }
    return result;
}

/*The character array passed to this function should have length
  of atleast PROPERTY_VALUE_MAX*/
void loc_get_target_baseband(char *baseband, int array_length)
{
    if(baseband && (array_length >= PROPERTY_VALUE_MAX)) {
        property_get("ro.baseband", baseband, "");
        LOC_LOGD("%s:%d]: Baseband: %s\n", __func__, __LINE__, baseband);
    }
    else {
        LOC_LOGE("%s:%d]: NULL parameter or array length less than PROPERTY_VALUE_MAX\n",
                 __func__, __LINE__);
    }
}

/*The character array passed to this function should have length
  of atleast PROPERTY_VALUE_MAX*/
void loc_get_platform_name(char *platform_name, int array_length)
{
    if(platform_name && (array_length >= PROPERTY_VALUE_MAX)) {
        property_get("ro.board.platform", platform_name, "");
        LOC_LOGD("%s:%d]: Target name: %s\n", __func__, __LINE__, platform_name);
    }
    else {
        LOC_LOGE("%s:%d]: Null parameter or array length less than PROPERTY_VALUE_MAX\n",
                 __func__, __LINE__);
    }
}

/*The character array passed to this function should have length
  of atleast PROPERTY_VALUE_MAX*/
void loc_get_auto_platform_name(char *platform_name, int array_length)
{
    if(platform_name && (array_length >= PROPERTY_VALUE_MAX)) {
        property_get("ro.hardware.type", platform_name, "");
        LOC_LOGD("%s:%d]: Autoplatform name: %s\n", __func__, __LINE__, platform_name);
    }
    else {
        LOC_LOGE("%s:%d]: Null parameter or array length less than PROPERTY_VALUE_MAX\n",
                 __func__, __LINE__);
    }
}

unsigned int loc_get_target(void)
{
    if (gTarget != (unsigned int)-1)
        return gTarget;

    static const char hw_platform[]      = "/sys/devices/soc0/hw_platform";
    static const char id[]               = "/sys/devices/soc0/soc_id";
    static const char hw_platform_dep[]  =
        "/sys/devices/system/soc/soc0/hw_platform";
    static const char id_dep[]           = "/sys/devices/system/soc/soc0/id";
    static const char mdm[]              = "/target"; // mdm target we are using

    char rd_hw_platform[LINE_LEN];
    char rd_id[LINE_LEN];
    char rd_mdm[LINE_LEN];
    char baseband[LINE_LEN];
    char rd_auto_platform[LINE_LEN];

    loc_get_target_baseband(baseband, sizeof(baseband));

    if (!access(hw_platform, F_OK)) {
        read_a_line(hw_platform, rd_hw_platform, LINE_LEN);
    } else {
        read_a_line(hw_platform_dep, rd_hw_platform, LINE_LEN);
    }
    if (!access(id, F_OK)) {
        read_a_line(id, rd_id, LINE_LEN);
    } else {
        read_a_line(id_dep, rd_id, LINE_LEN);
    }

    /*check automotive platform*/
    loc_get_auto_platform_name(rd_auto_platform, sizeof(rd_auto_platform));
    if( !memcmp(rd_auto_platform, STR_AUTO, LENGTH(STR_AUTO)) )
    {
          gTarget = TARGET_AUTO;
          goto detected;
    }

    if( !memcmp(baseband, STR_APQ_NO_WGR, LENGTH(STR_APQ_NO_WGR)) ){

        gTarget = TARGET_NO_GNSS;
        goto detected;
    }

    if( !memcmp(baseband, STR_APQ, LENGTH(STR_APQ)) ||
        !memcmp(baseband, STR_SDC, LENGTH(STR_SDC)) ||
        !memcmp(baseband, STR_QCS, LENGTH(STR_QCS)) ) {

        if( !memcmp(rd_id, MPQ8064_ID_1, LENGTH(MPQ8064_ID_1))
            && IS_STR_END(rd_id[LENGTH(MPQ8064_ID_1)]) )
            gTarget = TARGET_NO_GNSS;
        else
            gTarget = TARGET_APQ_SA;
    } else if (((!memcmp(rd_hw_platform, STR_LIQUID, LENGTH(STR_LIQUID))
                 && IS_STR_END(rd_hw_platform[LENGTH(STR_LIQUID)])) ||
                (!memcmp(rd_hw_platform, STR_SURF,   LENGTH(STR_SURF))
                 && IS_STR_END(rd_hw_platform[LENGTH(STR_SURF)])) ||
                (!memcmp(rd_hw_platform, STR_MTP,   LENGTH(STR_MTP))
                 && IS_STR_END(rd_hw_platform[LENGTH(STR_MTP)]))) &&
               !read_a_line( mdm, rd_mdm, LINE_LEN)) {
        gTarget = TARGET_MDM;
    } else if( (!memcmp(rd_id, MSM8930_ID_1, LENGTH(MSM8930_ID_1))
                && IS_STR_END(rd_id[LENGTH(MSM8930_ID_1)])) ||
               (!memcmp(rd_id, MSM8930_ID_2, LENGTH(MSM8930_ID_2))
                && IS_STR_END(rd_id[LENGTH(MSM8930_ID_2)])) ) {
        gTarget = TARGET_MSM_NO_SSC;
    } else if ( !memcmp(baseband, STR_MSM, LENGTH(STR_MSM)) ||
                !memcmp(baseband, STR_SDM, LENGTH(STR_SDM)) ) {
        gTarget = TARGET_DEFAULT;
    } else {
        gTarget = TARGET_UNKNOWN;
    }

detected:
    LOC_LOGW("HAL: %s returned %d", __FUNCTION__, gTarget);
    return gTarget;
}

int loc_read_device_soc_id()
{
    char buf[10] = {0};
    int soc_id = 0;
    int fd = open("/sys/devices/soc0/soc_id", O_RDONLY);
    if (fd >= 0)
    {
      if (read(fd, buf, sizeof(buf) - 1) == -1)
      {
        LOC_LOGI ("%s: unable to read soc_id", __FUNCTION__);
      }
      else
      {
        soc_id = atoi(buf);
        LOC_LOGI ("%s: Soc ID :%d", __FUNCTION__, soc_id);
      }
      close(fd);
    }
    return soc_id;
}
