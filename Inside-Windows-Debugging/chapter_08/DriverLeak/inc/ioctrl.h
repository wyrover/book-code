#pragma once

//
// IO Control Codes for the test driver device
//
#define IOCTL_DRV_TEST_API \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0xa01, METHOD_BUFFERED, FILE_READ_ACCESS)

#define DRV_CONTROL_DEVICE_NAME L"DrvCtrl"
