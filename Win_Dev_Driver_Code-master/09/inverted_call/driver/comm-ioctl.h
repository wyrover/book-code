// -*- Mode: c++ -*-
///////////////////////////////////////////////////////////////////////////////
//
//  (C) Copyright 1995 - 2002 OSR Open Systems Resources, Inc.
//  All Rights Reserved
//
//  This software is part of a licensed software product and may
//  only be used or copy in accordance with the terms of that license.
//
//  OSR Open Systems Resources, Inc.
//  105 Route 101A Suite 19
//  Amherst, NH 03031  (603) 595-6500 FAX: (603) 595-6503
//  email bugs to: bugs@osr.com
//
//
//  MODULE:
//
//    $Workfile: comm-ioctl.h $
//
//  ABSTRACT:
//
//    This module contains OSR private definitions used by the FSDK.
//
//  AUTHOR:
//
//    OSR Open Systems Resources, Inc.
// 
//  REVISION:   
//
//  $Revision: 1 $
//
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(__OSR_COMM_IOCTL_H__)
#define __OSR_COMM_IOCTL_H__ 1

//
// IOCTL types
//
#define OSR_COMM_DATA_TYPE 45935
#define OSR_COMM_CONTROL_TYPE 45936

//
// Device GUIDs
//
DEFINE_GUID(OSR_COMM_DATA_GUID, 0xAA319196, 0x8533, 0x439B, 0xBB, 0xFE, 0x76, 0x2C, 0xAF, 0xE6, 0x4A, 0x36);

DEFINE_GUID(OSR_COMM_CONTROL_GUID, 0x4C1FD3F5, 0x86BB, 0x4D5C, 0x9A, 0x80, 0x7B, 0xFD, 0x09, 0xB5, 0x10, 0x4E);

//
// Service IOCTLs to driver
//
#define OSR_COMM_CONTROL_GET_REQUEST CTL_CODE(OSR_COMM_CONTROL_TYPE, 3192, METHOD_BUFFERED, FILE_READ_ACCESS)
#define OSR_COMM_CONTROL_SEND_RESPONSE CTL_CODE(OSR_COMM_CONTROL_TYPE, 3193, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define OSR_COMM_CONTROL_GET_AND_SEND CTL_CODE(OSR_COMM_CONTROL_TYPE, 3194, METHOD_BUFFERED, FILE_READ_ACCESS|FILE_WRITE_ACCESS)

//
// Data structures used for communicating between service and driver
//

typedef struct _OSR_COMM_CONTROL_REQUEST {
  //
  // The request ID is used to match up the response to the original request
  //
  ULONG RequestID;

  //
  // The request type indicates the operation to be performed
  //
  ULONG RequestType;

  //
  // The data buffer allows the application to receive arbitrary data
  // Note that this is done OUT OF BOUNDS from the IOCTL.  Thus, the driver
  // is responsible for managing this.
  //
  PVOID RequestBuffer;

  //
  // This specifies the size of the request buffer
  //
  ULONG RequestBufferLength;


} OSR_COMM_CONTROL_REQUEST, *POSR_COMM_CONTROL_REQUEST;

#define OSR_COMM_READ_REQUEST 0x10
#define OSR_COMM_WRITE_REQUEST 0x20

typedef struct _OSR_COMM_CONTROL_RESPONSE {
  //
  // The request ID is used to match up this response to the original request
  //
  ULONG RequestID;

  //
  // The response type indicates the type of response information
  //
  ULONG ResponseType;

  //
  // The data buffer allows the application to return arbitrary data
  // Note that this is done OUT OF BOUNDS from the IOCTL.  Thus, the driver
  // should not trust this data field to be correct.
  //
  PVOID ResponseBuffer;

  ULONG ResponseBufferLength;

} OSR_COMM_CONTROL_RESPONSE, *POSR_COMM_CONTROL_RESPONSE;

#endif /* __OSR_COMM_IOCTL_H__ */

#define OSR_COMM_READ_RESPONSE 0x10
#define OSR_COMM_WRITE_RESPONSE 0x20
