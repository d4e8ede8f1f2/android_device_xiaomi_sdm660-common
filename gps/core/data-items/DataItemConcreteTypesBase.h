
#ifndef __DATAITEMCONCRETEBASETYPES__
#define __DATAITEMCONCRETEBASETYPES__

#include <string>
#include <cstring>
#include <DataItemId.h>
#include <IDataItemCore.h>

#define MAC_ADDRESS_LENGTH    6
// MAC address length in bytes
// QMI_LOC_SRN_MAC_ADDR_LENGTH_V02
#define SRN_MAC_ADDRESS_LENGTH    6
#define WIFI_SUPPLICANT_DEFAULT_STATE    0

namespace loc_core
{
using namespace std;

class AirplaneModeDataItemBase : public IDataItemCore  {
public:
    AirplaneModeDataItemBase(bool mode):
        mMode(mode),
        mId(AIRPLANEMODE_DATA_ITEM_ID) {}
    virtual ~AirplaneModeDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mMode;

protected:
    DataItemId mId;
};

class ENHDataItemBase : public IDataItemCore {
public:
    ENHDataItemBase(bool enabled) :
        mEnabled(enabled),
        mId(ENH_DATA_ITEM_ID) {}
    virtual ~ENHDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mEnabled;
protected:
    DataItemId mId;
};

class GPSStateDataItemBase : public IDataItemCore {
public:
    GPSStateDataItemBase(bool enabled) :
        mEnabled(enabled),
        mId(GPSSTATE_DATA_ITEM_ID) {}
    virtual ~GPSStateDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mEnabled;
protected:
    DataItemId mId;
};

class NLPStatusDataItemBase : public IDataItemCore {
public:
    NLPStatusDataItemBase(bool enabled) :
        mEnabled(enabled),
        mId(NLPSTATUS_DATA_ITEM_ID) {}
    virtual ~NLPStatusDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mEnabled;
protected:
    DataItemId mId;
};

class WifiHardwareStateDataItemBase : public IDataItemCore {
public:
    WifiHardwareStateDataItemBase(bool enabled) :
        mEnabled(enabled),
        mId(WIFIHARDWARESTATE_DATA_ITEM_ID) {}
    virtual ~WifiHardwareStateDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mEnabled;
protected:
    DataItemId mId;
};

class ScreenStateDataItemBase : public IDataItemCore {
public:
    ScreenStateDataItemBase(bool state) :
        mState(state),
        mId(SCREEN_STATE_DATA_ITEM_ID) {}
    virtual ~ScreenStateDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mState;
protected:
    DataItemId mId;
};

class PowerConnectStateDataItemBase : public IDataItemCore {
public:
    PowerConnectStateDataItemBase(bool state) :
        mState(state),
        mId(POWER_CONNECTED_STATE_DATA_ITEM_ID) {}
    virtual ~PowerConnectStateDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mState;
protected:
    DataItemId mId;
};

class TimeZoneChangeDataItemBase : public IDataItemCore {
public:
    TimeZoneChangeDataItemBase(int64_t currTimeMillis, int32_t rawOffset, int32_t dstOffset) :
        mCurrTimeMillis (currTimeMillis),
        mRawOffsetTZ (rawOffset),
        mDstOffsetTZ (dstOffset),
        mId(TIMEZONE_CHANGE_DATA_ITEM_ID) {}
    virtual ~TimeZoneChangeDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    int64_t mCurrTimeMillis;
    int32_t mRawOffsetTZ;
    int32_t mDstOffsetTZ;
protected:
    DataItemId mId;
};

class TimeChangeDataItemBase : public IDataItemCore {
public:
    TimeChangeDataItemBase(int64_t currTimeMillis, int32_t rawOffset, int32_t dstOffset) :
        mCurrTimeMillis (currTimeMillis),
        mRawOffsetTZ (rawOffset),
        mDstOffsetTZ (dstOffset),
        mId(TIME_CHANGE_DATA_ITEM_ID) {}
    virtual ~TimeChangeDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    int64_t mCurrTimeMillis;
    int32_t mRawOffsetTZ;
    int32_t mDstOffsetTZ;
protected:
    DataItemId mId;
};

class ShutdownStateDataItemBase : public IDataItemCore {
public:
    ShutdownStateDataItemBase(bool state) :
        mState (state),
        mId(SHUTDOWN_STATE_DATA_ITEM_ID) {}
    virtual ~ShutdownStateDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mState;
protected:
    DataItemId mId;
};

class AssistedGpsDataItemBase : public IDataItemCore {
public:
    AssistedGpsDataItemBase(bool enabled) :
        mEnabled(enabled),
        mId(ASSISTED_GPS_DATA_ITEM_ID) {}
    virtual ~AssistedGpsDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    bool mEnabled;
protected:
    DataItemId mId;
};

class NetworkInfoDataItemBase : public IDataItemCore {
public:
    enum NetworkType {
        TYPE_MOBILE,
        TYPE_WIFI,
        TYPE_ETHERNET,
        TYPE_BLUETOOTH,
        TYPE_MMS,
        TYPE_SUPL,
        TYPE_DUN,
        TYPE_HIPRI,
        TYPE_WIMAX,
        TYPE_UNKNOWN,
    };
    NetworkInfoDataItemBase(
    NetworkType initialType, int32_t type, string typeName, string subTypeName,
    bool available, bool connected, bool roaming ):
            mAllTypes(typeToAllTypes(initialType)),
            mType(type),
            mTypeName(typeName),
            mSubTypeName(subTypeName),
            mAvailable(available),
            mConnected(connected),
            mRoaming(roaming),
            mId(NETWORKINFO_DATA_ITEM_ID) {}
    virtual ~NetworkInfoDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
    inline virtual NetworkType getType(void) const {
        return (NetworkType)mType;
    }
    inline uint64_t getAllTypes() { return mAllTypes; }
    // Data members
    uint64_t mAllTypes;
    int32_t mType;
    string mTypeName;
    string mSubTypeName;
    bool mAvailable;
    bool mConnected;
    bool mRoaming;
protected:
    DataItemId mId;
    inline uint64_t typeToAllTypes(NetworkType type) {
        return (type >= TYPE_UNKNOWN || type < TYPE_MOBILE) ?  0 : (1<<type);
    }
};

class ServiceStatusDataItemBase : public IDataItemCore {
public:
    ServiceStatusDataItemBase(int32_t serviceState) :
        mServiceState (serviceState),
        mId(SERVICESTATUS_DATA_ITEM_ID) {}
    virtual ~ServiceStatusDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    int32_t mServiceState;
protected:
    DataItemId mId;
};

class ModelDataItemBase : public IDataItemCore {
public:
    ModelDataItemBase(const string & name) :
        mModel (name),
        mId(MODEL_DATA_ITEM_ID) {}
    virtual ~ModelDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    string mModel;
protected:
    DataItemId mId;
};

class ManufacturerDataItemBase : public IDataItemCore {
public:
    ManufacturerDataItemBase(const string & name) :
        mManufacturer (name),
        mId(MANUFACTURER_DATA_ITEM_ID) {}
    virtual ~ManufacturerDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    string mManufacturer;
protected:
    DataItemId mId;
};

class RilServiceInfoDataItemBase : public IDataItemCore {
public:
    inline RilServiceInfoDataItemBase() :
            mData(nullptr), mId(RILSERVICEINFO_DATA_ITEM_ID) {}
    inline virtual ~RilServiceInfoDataItemBase() { if (nullptr != mData) free(mData); }
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
    inline RilServiceInfoDataItemBase(const RilServiceInfoDataItemBase& peer) :
            RilServiceInfoDataItemBase() {
        peer.setPeerData(*this);
    }
    inline virtual bool operator==(const RilServiceInfoDataItemBase& other) const {
        return other.mData == mData;
    }
    inline virtual void setPeerData(RilServiceInfoDataItemBase& /*peer*/) const {}
    void* mData;
protected:
    DataItemId mId;
};

class RilCellInfoDataItemBase : public IDataItemCore {
public:
    inline RilCellInfoDataItemBase() :
            mData(nullptr), mId(RILCELLINFO_DATA_ITEM_ID) {}
    inline virtual ~RilCellInfoDataItemBase() { if (nullptr != mData) free(mData); }
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
    inline RilCellInfoDataItemBase(const RilCellInfoDataItemBase& peer) :
            RilCellInfoDataItemBase() {
        peer.setPeerData(*this);
    }
    inline virtual bool operator==(const RilCellInfoDataItemBase& other) const {
        return other.mData == mData;
    }
    inline virtual void setPeerData(RilCellInfoDataItemBase& /*peer*/) const {}
    void* mData;
protected:
    DataItemId mId;
};

class WifiSupplicantStatusDataItemBase : public IDataItemCore {
public:
    WifiSupplicantStatusDataItemBase() :
        mState((WifiSupplicantState)WIFI_SUPPLICANT_DEFAULT_STATE),
        mApMacAddressValid(false),
        mWifiApSsidValid(false),
        mId(WIFI_SUPPLICANT_STATUS_DATA_ITEM_ID) {
            memset (&mApMacAddress, 0, sizeof (mApMacAddress));
            mWifiApSsid.clear();
        }
    virtual ~WifiSupplicantStatusDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
    // Data members
    typedef enum WifiSupplicantState {
        DISCONNECTED,
        INTERFACE_DISABLED,
        INACTIVE,
        SCANNING,
        AUTHENTICATING,
        ASSOCIATING,
        ASSOCIATED,
        FOUR_WAY_HANDSHAKE,
        GROUP_HANDSHAKE,
        COMPLETED,
        DORMANT,
        UNINITIALIZED,
        INVALID
    } WifiSupplicantState;
    /* Represents whether access point attach state*/
    WifiSupplicantState mState;
    /* Represents info on whether ap mac address is valid */
    bool mApMacAddressValid;
    /* Represents mac address of the wifi access point*/
    uint8_t mApMacAddress[MAC_ADDRESS_LENGTH];
    /* Represents info on whether ap SSID is valid */
    bool mWifiApSsidValid;
    /* Represents Wifi SSID string*/
    string mWifiApSsid;
protected:
    DataItemId mId;
};

class TacDataItemBase : public IDataItemCore {
public:
    TacDataItemBase(const string & name) :
        mValue (name),
        mId(TAC_DATA_ITEM_ID) {}
    virtual ~TacDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    string mValue;
protected:
    DataItemId mId;
};

class MccmncDataItemBase : public IDataItemCore {
public:
    MccmncDataItemBase(const string & name) :
        mValue(name),
        mId(MCCMNC_DATA_ITEM_ID) {}
    virtual ~MccmncDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
// Data members
    string mValue;
protected:
    DataItemId mId;
};

class SrnDeviceScanDetailsDataItemBase : public IDataItemCore {
public:
    SrnDeviceScanDetailsDataItemBase(DataItemId Id) :
        mValidSrnData(false),
        mApSrnRssi(-1),
        mApSrnTimestamp(0),
        mRequestTimestamp(0),
        mReceiveTimestamp(0),
        mErrorCause(-1),
        mId(Id) {}
    virtual ~SrnDeviceScanDetailsDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
    // Data members common to all SRN tech types
    /* Represents info on whether SRN data is valid (no error)*/
    bool mValidSrnData;
    /* SRN device RSSI reported */
    int32_t mApSrnRssi;
    /* MAC adress of SRN device */
    uint8_t mApSrnMacAddress[SRN_MAC_ADDRESS_LENGTH];
    /* UTC timestamp at which the scan was requested.for this SRN device*/
    int64_t mApSrnTimestamp;
    /* UTC timestamp at which the scan was started. */
    int64_t mRequestTimestamp;
    /* UTC timestamp at which the scan was received.*/
    int64_t mReceiveTimestamp;
    /* Reason for the error/failure if SRN details are not valid */
    int32_t mErrorCause;
protected:
    DataItemId mId;
};

class BtDeviceScanDetailsDataItemBase : public SrnDeviceScanDetailsDataItemBase {

public:
    BtDeviceScanDetailsDataItemBase() :
        SrnDeviceScanDetailsDataItemBase(BT_SCAN_DATA_ITEM_ID) {}
    virtual ~BtDeviceScanDetailsDataItemBase() {}
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
};

class BtLeDeviceScanDetailsDataItemBase : public SrnDeviceScanDetailsDataItemBase {

public:
    BtLeDeviceScanDetailsDataItemBase() :
        SrnDeviceScanDetailsDataItemBase(BTLE_SCAN_DATA_ITEM_ID) {}
    virtual ~BtLeDeviceScanDetailsDataItemBase() {}
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copy(IDataItemCore* /*src*/, bool* /*dataItemCopied = NULL*/) {return 1;}
};

class BatteryLevelDataItemBase : public IDataItemCore {
public:
    inline BatteryLevelDataItemBase(uint8_t batteryPct) :
            mBatteryPct(batteryPct), mId(BATTERY_LEVEL_DATA_ITEM_ID) {}
    inline ~BatteryLevelDataItemBase() {}
    inline virtual DataItemId getId() { return mId; }
// Data members
    uint8_t mBatteryPct;
protected:
    DataItemId mId;
};

} // namespace loc_core

#endif //__DATAITEMCONCRETEBASETYPES__
