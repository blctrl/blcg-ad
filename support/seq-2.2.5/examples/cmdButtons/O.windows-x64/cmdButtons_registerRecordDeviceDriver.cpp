/* THIS IS A GENERATED FILE. DO NOT EDIT! */
/* Generated from ../O.Common/cmdButtons.dbd */

#include <string.h>

#include "epicsStdlib.h"
#include "iocsh.h"
#include "iocshRegisterCommon.h"
#include "registryCommon.h"

extern "C" {

epicsShareExtern rset *pvar_rset_boRSET;
epicsShareExtern int (*pvar_func_boRecordSizeOffset)(dbRecordType *pdbRecordType);
epicsShareExtern rset *pvar_rset_stringoutRSET;
epicsShareExtern int (*pvar_func_stringoutRecordSizeOffset)(dbRecordType *pdbRecordType);

static const char * const recordTypeNames[2] = {
    "bo",
    "stringout"
};

static const recordTypeLocation rtl[2] = {
    {pvar_rset_boRSET, pvar_func_boRecordSizeOffset},
    {pvar_rset_stringoutRSET, pvar_func_stringoutRecordSizeOffset}
};

epicsShareExtern dset *pvar_dset_devBoSoft;
epicsShareExtern dset *pvar_dset_devSoSoft;

static const char * const deviceSupportNames[2] = {
    "devBoSoft",
    "devSoSoft"
};

static const dset * const devsl[2] = {
    pvar_dset_devBoSoft,
    pvar_dset_devSoSoft
};

epicsShareExtern void (*pvar_func_seqCmdBtnsRegistrar)(void);

int cmdButtons_registerRecordDeviceDriver(DBBASE *pbase)
{
    const char *bldTop = "D:/epics/ad/seq-2.2.5";
    const char *envTop = getenv("TOP");

    if (envTop && strcmp(envTop, bldTop)) {
        printf("Warning: IOC is booting with TOP = \"%s\"\n"
               "          but was built with TOP = \"%s\"\n",
               envTop, bldTop);
    }

    if (!pbase) {
        printf("pdbbase is NULL; you must load a DBD file first.\n");
        return -1;
    }

    registerRecordTypes(pbase, 2, recordTypeNames, rtl);
    registerDevices(pbase, 2, deviceSupportNames, devsl);
    (*pvar_func_seqCmdBtnsRegistrar)();
    return 0;
}

/* registerRecordDeviceDriver */
static const iocshArg registerRecordDeviceDriverArg0 =
                                            {"pdbbase",iocshArgPdbbase};
static const iocshArg *registerRecordDeviceDriverArgs[1] =
                                            {&registerRecordDeviceDriverArg0};
static const iocshFuncDef registerRecordDeviceDriverFuncDef =
                {"cmdButtons_registerRecordDeviceDriver",1,registerRecordDeviceDriverArgs};
static void registerRecordDeviceDriverCallFunc(const iocshArgBuf *)
{
    cmdButtons_registerRecordDeviceDriver(*iocshPpdbbase);
}

} // extern "C"
/*
 * Register commands on application startup
 */
static int Registration() {
    iocshRegisterCommon();
    iocshRegister(&registerRecordDeviceDriverFuncDef,
        registerRecordDeviceDriverCallFunc);
    return 0;
}

static int done = Registration();
