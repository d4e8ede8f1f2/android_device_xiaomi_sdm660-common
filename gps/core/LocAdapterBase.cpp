
#define LOG_NDEBUG 0
#define LOG_TAG "LocSvc_LocAdapterBase"

#include <dlfcn.h>
#include <LocAdapterBase.h>
#include <loc_target.h>
#include <log_util.h>
#include <LocAdapterProxyBase.h>

namespace loc_core {

// This is the top level class, so the constructor will
// always gets called. Here we prepare for the default.
// But if getLocApi(targetEnumType target) is overriden,
// the right locApi should get created.
LocAdapterBase::LocAdapterBase(const LOC_API_ADAPTER_EVENT_MASK_T mask,
                               ContextBase* context, LocAdapterProxyBase *adapterProxyBase) :
    mEvtMask(mask), mContext(context),
    mLocApi(context->getLocApi()), mLocAdapterProxyBase(adapterProxyBase),
    mMsgTask(context->getMsgTask())
{
    mLocApi->addAdapter(this);
}

uint32_t LocAdapterBase::mSessionIdCounter(1);

uint32_t LocAdapterBase::generateSessionId()
{
    if (++mSessionIdCounter == 0xFFFFFFFF)
        mSessionIdCounter = 1;

     return mSessionIdCounter;
}

void LocAdapterBase::handleEngineUpEvent()
{
    if (mLocAdapterProxyBase) {
        mLocAdapterProxyBase->handleEngineUpEvent();
    }
}

void LocAdapterBase::handleEngineDownEvent()
{
    if (mLocAdapterProxyBase) {
        mLocAdapterProxyBase->handleEngineDownEvent();
    }
}

void LocAdapterBase::
    reportPositionEvent(const UlpLocation& location,
                        const GpsLocationExtended& locationExtended,
                        enum loc_sess_status status,
                        LocPosTechMask loc_technology_mask,
                        bool /*fromUlp*/) {
    if (mLocAdapterProxyBase != NULL) {
        mLocAdapterProxyBase->reportPositionEvent((UlpLocation&)location,
                                                   (GpsLocationExtended&)locationExtended,
                                                   status,
                                                   loc_technology_mask);
    } else {
        DEFAULT_IMPL()
    }
}

void LocAdapterBase::
    reportSvEvent(const GnssSvNotification& /*svNotify*/, bool /*fromUlp*/)
DEFAULT_IMPL()

void LocAdapterBase::
    reportSvMeasurementEvent(GnssSvMeasurementSet &/*svMeasurementSet*/)
DEFAULT_IMPL()

void LocAdapterBase::
    reportSvPolynomialEvent(GnssSvPolynomial &/*svPolynomial*/)
DEFAULT_IMPL()

void LocAdapterBase::
    reportStatus(LocGpsStatusValue /*status*/)
DEFAULT_IMPL()


void LocAdapterBase::
    reportNmeaEvent(const char* /*nmea*/, size_t /*length*/, bool /*fromUlp*/)
DEFAULT_IMPL()

bool LocAdapterBase::
    reportXtraServer(const char* /*url1*/, const char* /*url2*/,
                     const char* /*url3*/, const int /*maxlength*/)
DEFAULT_IMPL(false)

bool LocAdapterBase::
    requestXtraData()
DEFAULT_IMPL(false)

bool LocAdapterBase::
    requestTime()
DEFAULT_IMPL(false)

bool LocAdapterBase::
    requestLocation()
DEFAULT_IMPL(false)

bool LocAdapterBase::
    requestATL(int /*connHandle*/, LocAGpsType /*agps_type*/)
DEFAULT_IMPL(false)

bool LocAdapterBase::
    releaseATL(int /*connHandle*/)
DEFAULT_IMPL(false)

bool LocAdapterBase::
    requestSuplES(int /*connHandle*/)
DEFAULT_IMPL(false)

bool LocAdapterBase::
    reportDataCallOpened()
DEFAULT_IMPL(false)

bool LocAdapterBase::
    reportDataCallClosed()
DEFAULT_IMPL(false)

bool LocAdapterBase::
    requestNiNotifyEvent(const GnssNiNotification &/*notify*/, const void* /*data*/)
DEFAULT_IMPL(false)

void LocAdapterBase::
    reportGnssMeasurementDataEvent(const GnssMeasurementsNotification& /*measurements*/,
                                   int /*msInWeek*/)
DEFAULT_IMPL()

bool LocAdapterBase::
    reportWwanZppFix(LocGpsLocation &/*zppLoc*/)
DEFAULT_IMPL(false)

bool LocAdapterBase::
    reportOdcpiRequestEvent(OdcpiRequestInfo& /*request*/)
DEFAULT_IMPL(false)

} // namespace loc_core
