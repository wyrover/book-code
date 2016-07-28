// -*- Mode: c++ -*-
//
// (C) Copyright 2002 OSR Open Systems Resources, Inc.
// All Rights Reserved.
//

#define INITGUID 1 // initialize our GUIDs in this driver
#include <comm.h>

extern "C" {
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);
}

PDEVICE_OBJECT OsrCommDeviceObject;
PDEVICE_OBJECT OsrDataDeviceObject;

static LONG OsrRequestID = 0xFFFFFFFE; // checks for "roll-over" problems

typedef struct _OSR_COMM_DATA_REQUEST {
  //
  // This is used to thread the requests onto a data request queue
  //
  LIST_ENTRY ListEntry;

  //
  // This is used to thread the requests onto a service request queue
  //
  LIST_ENTRY ServiceListEntry;

  //
  // The request ID is used to match up the response.
  //
  ULONG RequestID;

  //
  // The IRP is the one associated with this particular operation.
  //
  PIRP Irp;

} OSR_COMM_DATA_REQUEST, *POSR_COMM_DATA_REQUEST;

#pragma alloc_text(INIT, DriverEntry)

//
// OsrCommCreate
//
//  This is the create entry point
//
// Inputs:
//  DeviceObject - this is the device object on which we are operating
//  Irp - this is the create IRP
//
// Outputs:
//  None.
//
// Returns:
//  SUCCESS - the operation was successful.
//
// Notes:
//  None.
//
NTSTATUS OsrCommCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
  POSR_COMM_DATA_DEVICE_EXTENSION dataExt;

  //
  // Tell the caller "yes"
  //
  Irp->IoStatus.Status = STATUS_SUCCESS;

  Irp->IoStatus.Information = FILE_OPENED;

  IoCompleteRequest(Irp, IO_NO_INCREMENT);

  //
  // If this was the control device, note that we now have
  // activated the data device.
  //
  if (OSR_COMM_CONTROL_TYPE == DeviceObject->DeviceType) {

    dataExt = (POSR_COMM_DATA_DEVICE_EXTENSION) OsrDataDeviceObject->DeviceExtension;

    dataExt->DeviceState = OSR_COMM_DATA_DEVICE_ACTIVE;

  }

  //
  // Done.
  //
  return STATUS_SUCCESS;

}



//
// CancelPendingRequestList
//
//   This routine will walk through a list of data requests
//   looking for requests that need to be cancelled.
//
// Inputs:
//   List - this is the list to traverse
//   ListLock - this is the fast mutex protecting the list
//   FileObject - this is the file object to match against the requests being cancelled.  If it is
//                zero, it indicates that all entries on the queue should be cancelled.
//
// Outputs:
//   None.
//
// Returns:
//   VOID function
//
// Notes:
//   This is a "helper" function for cleanup, not a general-purpose cancellation mechanism.
//
static VOID CancelPendingRequestList(PLIST_ENTRY List, PFAST_MUTEX ListLock, PFILE_OBJECT FileObject)
{
  PLIST_ENTRY listEntry;
  PLIST_ENTRY nextListEntry;
  POSR_COMM_DATA_REQUEST dataRequest;

  //
  // Lock the list
  //
  ExAcquireFastMutex(ListLock);

  //
  // Walk the list
  //
  for (listEntry = List->Flink;
       listEntry != List;
       listEntry = nextListEntry) {
    //
    // Set up the next list entry first.  Thus, even if we delete the
    // list entry from the queue, we can skip to the next entry
    //
    nextListEntry = listEntry->Flink;

    //
    // Extract the data request from the list entry
    //
    dataRequest = CONTAINING_RECORD(listEntry, OSR_COMM_DATA_REQUEST, ListEntry);

    //
    // If there is an IRP associated with this request and the file object in the IRP
    // stack location matches the file object passed to this call, then we need to
    // cancel this request.
    //
    // Note that if the file object is NULL we cancel ALL I/O operations on the queue.
    //
    if (dataRequest->Irp &&
        ((NULL == FileObject) ||
         (IoGetCurrentIrpStackLocation(dataRequest->Irp)->FileObject == FileObject))) {
      
      //
      // We need to remove this entry from the queue
      //
      RemoveEntryList(listEntry);

      //
      // Cancel this IRP
      //
      dataRequest->Irp->IoStatus.Status = STATUS_CANCELLED;

      dataRequest->Irp->IoStatus.Information = 0;

      IoCompleteRequest(dataRequest->Irp, IO_NO_INCREMENT);

      //
      // Free this data request
      //
      ExFreePool(dataRequest);

    }

  }

  //
  // Unlock the list
  //
  ExReleaseFastMutex(ListLock);

  //
  // Done!
  //
  return;
}




//
// 
//OsrCommCleanup
//  This is the cleanup entry point
//
// Inputs:
//  DeviceObject - this is the device object on which we are operating
//  Irp - this is the cleanup IRP
//
// Outputs:
//  None.
//
// Returns:
//  SUCCESS - the operation was successful.
//
// Notes:
//  None.
//
NTSTATUS OsrCommCleanup(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
  PLIST_ENTRY listEntry;
  PLIST_ENTRY nextListEntry;
  POSR_COMM_DATA_DEVICE_EXTENSION dataExt;
  POSR_COMM_CONTROL_DEVICE_EXTENSION controlExt;

  DbgPrint("OsrCommCleanup: entered. \n");

  //
  // If this is the control object, we need to deal with it closing,
  // in case there are any existing threads waiting for answers from
  // the control object.
  //
  if (OSR_COMM_CONTROL_TYPE == DeviceObject->DeviceType) {

    //
    // Need the data device
    //
    dataExt = (POSR_COMM_DATA_DEVICE_EXTENSION) OsrDataDeviceObject->DeviceExtension;
    controlExt = (POSR_COMM_CONTROL_DEVICE_EXTENSION) DeviceObject->DeviceExtension;

    //
    // Set the device state
    //
    dataExt->DeviceState = OSR_COMM_DATA_DEVICE_INACTIVE;


    //
    // In this case, we must cancel all pending requests on the queue
    //

    CancelPendingRequestList(&controlExt->ServiceQueue,
                             &controlExt->ServiceQueueLock,
                             NULL);

    CancelPendingRequestList(&controlExt->RequestQueue,
                             &controlExt->RequestQueueLock,
                             NULL);


    //
    // In this case, we must cancel all pending requests on the queue
    //
    CancelPendingRequestList(&dataExt->ReadRequestQueue,
                             &dataExt->ReadRequestQueueLock,
                             NULL);

    CancelPendingRequestList(&dataExt->WriteRequestQueue,
                             &dataExt->WriteRequestQueueLock,
                             NULL);

  }

  //
  // If this is the data object, we need to find any pending requests
  // and remove them from the queue
  //
  if (OSR_COMM_DATA_TYPE == DeviceObject->DeviceType) {

    //
    // We need to walk the list of pending requests and
    // remove any matching the file object in the cleanup request.
    //
    dataExt = (POSR_COMM_DATA_DEVICE_EXTENSION) DeviceObject->DeviceExtension;
    controlExt = (POSR_COMM_CONTROL_DEVICE_EXTENSION) OsrCommDeviceObject->DeviceExtension;

    //
    // In this case, we must cancel all pending requests on the queue
    //

    CancelPendingRequestList(&controlExt->ServiceQueue,
                             &controlExt->ServiceQueueLock,
                             IoGetCurrentIrpStackLocation(Irp)->FileObject);

    CancelPendingRequestList(&controlExt->RequestQueue,
                             &controlExt->RequestQueueLock,
                             IoGetCurrentIrpStackLocation(Irp)->FileObject);


    //
    // Clean up the pending read queue
    //
    CancelPendingRequestList(&dataExt->ReadRequestQueue,
                             &dataExt->ReadRequestQueueLock,
                             IoGetCurrentIrpStackLocation(Irp)->FileObject);

    //
    // Clean up the pending write queue
    //
    CancelPendingRequestList(&dataExt->WriteRequestQueue,
                             &dataExt->WriteRequestQueueLock,
                             IoGetCurrentIrpStackLocation(Irp)->FileObject);

  }

  //
  // Tell the caller "yes"
  //
  Irp->IoStatus.Status = STATUS_SUCCESS;

  Irp->IoStatus.Information = 0;

  IoCompleteRequest(Irp, IO_NO_INCREMENT);

  //
  // Done.
  //
  return STATUS_SUCCESS;

}

//
// Close
//
//  This is the close entry point
//
// Inputs:
//  DeviceObject - this is the device object on which we are operating
//  Irp - this is the close IRP
//
// Outputs:
//  None.
//
// Returns:
//  SUCCESS - the operation was successful.
//
// Notes:
//  None.
//
NTSTATUS OsrCommClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
  //
  // Tell the caller "yes"
  //
  Irp->IoStatus.Status = STATUS_SUCCESS;

  Irp->IoStatus.Information = 0;

  IoCompleteRequest(Irp, IO_NO_INCREMENT);

  //
  // Done.
  //
  return STATUS_SUCCESS;

}



//
// ProcessResponse
//
//  This routine is used to process a response
//
// Inputs:
//  Irp - this is the IRP containing a (validated) response
//
// Outputs:
//  None.
//
// Returns:
//  STATUS_SUCCESS - the operation completed successfully
//
// Notes:
//  This is a helper function for the device control logic.  It does NOT
//  complete the control request - that is the job of the caller!  It DOES
//  complete the data request (if it finds a matching entry).
//
static NTSTATUS ProcessResponse(PIRP Irp)

{
  POSR_COMM_CONTROL_RESPONSE response;
  PLIST_ENTRY queue;
  PFAST_MUTEX queueLock;
  PLIST_ENTRY listEntry;
  PLIST_ENTRY nextEntry;
  POSR_COMM_DATA_REQUEST dataRequest = NULL;
  NTSTATUS status = STATUS_SUCCESS;
  PVOID requestBuffer;
  PIO_STACK_LOCATION irpSp;
  ULONG bytesToCopy;
  POSR_COMM_DATA_DEVICE_EXTENSION dataExt =
    (POSR_COMM_DATA_DEVICE_EXTENSION) OsrDataDeviceObject->DeviceExtension;
  POSR_COMM_CONTROL_DEVICE_EXTENSION controlExt =
    (POSR_COMM_CONTROL_DEVICE_EXTENSION) OsrCommDeviceObject->DeviceExtension;

  DbgPrint("ProcessResponse: Entered. \n");

  //
  // Get the response packet
  //
  response = (POSR_COMM_CONTROL_RESPONSE) Irp->AssociatedIrp.SystemBuffer;

  //
  // Let's pick the right queue to process
  //
  if (OSR_COMM_READ_RESPONSE == response->ResponseType) {

    queue = &dataExt->ReadRequestQueue;
    queueLock = &dataExt->ReadRequestQueueLock;
    
  } else if (OSR_COMM_WRITE_RESPONSE == response->ResponseType) {

      queue = &dataExt->WriteRequestQueue;
      queueLock = &dataExt->WriteRequestQueueLock;

  } else {

    //
    // Invalid response
    //
    Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;

    Irp->IoStatus.Information = 0;

    //
    // Caller will handle completing the request
    //
    return STATUS_INVALID_PARAMETER;

  }
        
  ExAcquireFastMutex(queueLock);

  for (listEntry = queue->Flink;
       listEntry != queue;
       listEntry = listEntry->Flink) {

    //
    // Check to see if this response matches up
    //
    dataRequest = CONTAINING_RECORD(listEntry, OSR_COMM_DATA_REQUEST, ListEntry);

    if (dataRequest->RequestID == response->RequestID) {

      //
      // This is our request, process it:
      //  - Remove it from the list
      //  - Transfer the data
      //  - Indicate the results
      //  - Complete the IRP
      //
      RemoveEntryList(listEntry);

      requestBuffer = MmGetSystemAddressForMdlSafe(dataRequest->Irp->MdlAddress, NormalPagePriority);

      if (NULL == requestBuffer) {
        //
        // We were unable to obtain the system PTEs
        //
        dataRequest->Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;

        dataRequest->Irp->IoStatus.Information = 0;

        IoCompleteRequest(dataRequest->Irp, IO_NO_INCREMENT);

        status = STATUS_INSUFFICIENT_RESOURCES;

        ExFreePool(dataRequest);

        break; // from loop

      }

      //
      // Figure out how much data we are going to actually copy here.
      //
      irpSp = IoGetCurrentIrpStackLocation(dataRequest->Irp);
      
      if (response->ResponseBufferLength < irpSp->Parameters.Read.Length) {
        
        bytesToCopy = response->ResponseBufferLength;
        
      } else {
        
        bytesToCopy = irpSp->Parameters.Read.Length;
        
      }

      //
      // We run this in a try/except to protect against bogus pointers, the usual
      //
      __try { 

        RtlCopyMemory(requestBuffer, response->ResponseBuffer, bytesToCopy);

      } __except (EXCEPTION_EXECUTE_HANDLER) {

        status = GetExceptionCode();

      }

      dataRequest->Irp->IoStatus.Status = status;

      dataRequest->Irp->IoStatus.Information = NT_SUCCESS(status) ? bytesToCopy : 0;

      IoCompleteRequest(dataRequest->Irp, IO_NO_INCREMENT);

      ExFreePool(dataRequest);

      //
      // The control operation was successful in any case
      //
      status = STATUS_SUCCESS;

      //
      // And break from the loop, no sense looking any farther
      //
      break;

    }
    
  }

  ExReleaseFastMutex(queueLock);

  //
  // Return the results of the operation.
  //
  return status;
  
}

//
// ProcessControlRequest
//
//  This routine is used to either satisfy the control request or enqueue it
//
// Inputs:
//  Irp - this is the IRP that we are processing
//  ControlRequest - this is the control request (from the IRP, actually)
//
// Outputs:
//  None.
//
// Returns:
//  SUCCESS - there's data going back up to the application
//  PENDING - the IRP will block and wait 'til it is time...
//
//
static NTSTATUS ProcessControlRequest(PIRP Irp)
{
  POSR_COMM_CONTROL_DEVICE_EXTENSION controlExt =
    (POSR_COMM_CONTROL_DEVICE_EXTENSION) OsrCommDeviceObject->DeviceExtension;
  PLIST_ENTRY listEntry = NULL;
  NTSTATUS status = STATUS_UNSUCCESSFUL;
  POSR_COMM_CONTROL_REQUEST controlRequest;
  PIRP dataIrp;
  POSR_COMM_DATA_REQUEST dataRequest;
  PIO_STACK_LOCATION irpSp;
  PVOID dataBuffer;
  ULONG bytesToCopy;

  DbgPrint("ProcessControlRequest: Entered. \n");

  //
  // First, we need to lock the control queue before we do anything else
  //
  ExAcquireFastMutex(&controlExt->ServiceQueueLock);

  ExAcquireFastMutex(&controlExt->RequestQueueLock);

  //
  // Check request queue
  //
  if (!IsListEmpty(&controlExt->RequestQueue)) {

    listEntry = RemoveHeadList(&controlExt->RequestQueue);

    status = STATUS_SUCCESS;

  } else {

    POSR_COMM_CONTROL_REQUEST controlRequest = (POSR_COMM_CONTROL_REQUEST) Irp->AssociatedIrp.SystemBuffer;
    irpSp = IoGetCurrentIrpStackLocation(Irp);

    if(!controlRequest || irpSp->Parameters.DeviceIoControl.OutputBufferLength < sizeof(OSR_COMM_CONTROL_REQUEST)) {

        status = STATUS_INVALID_PARAMETER;

    } else {

        //
        // We have to insert the control IRP into the queue
        //
        IoMarkIrpPending(Irp);

        DbgPrint("ProcessControlRequest:  Irp %x, RequestBuffer %x RequestBufferLength %x\n.",
                 Irp,
                 controlRequest->RequestBuffer,
                 controlRequest->RequestBufferLength);

        InsertTailList(&controlExt->ServiceQueue, &Irp->Tail.Overlay.ListEntry);

        status = STATUS_PENDING;

    }

  }

  //
  // OK.  At this point we can drop both locks
  //
  ExReleaseFastMutex(&controlExt->RequestQueueLock);

  ExReleaseFastMutex(&controlExt->ServiceQueueLock);

  //
  // If we found an entry to process, we need to return the information to
  // the caller here.
  //
  if (listEntry) {

    //
    // This is the request we removed from the queue.
    //
    controlRequest = (POSR_COMM_CONTROL_REQUEST) Irp->AssociatedIrp.SystemBuffer;

    dataRequest = CONTAINING_RECORD(listEntry, OSR_COMM_DATA_REQUEST, ServiceListEntry);

    irpSp = IoGetCurrentIrpStackLocation(dataRequest->Irp);

    //
    // We are going to use the request ID to match the response
    //
    controlRequest->RequestID = dataRequest->RequestID;

    //
    // Is this a read or write?
    //
    if (IRP_MJ_WRITE == irpSp->MajorFunction) {

      controlRequest->RequestType = OSR_COMM_WRITE_REQUEST; // : OSR_COMM_READ_REQUEST;

      //
      // We must copy the data from the user's address space to the control application's
      // address space.
      //
      dataBuffer = MmGetSystemAddressForMdlSafe(dataRequest->Irp->MdlAddress, NormalPagePriority);

      if (NULL == dataBuffer) {
        //
        // It failed.
        //
        dataRequest->Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;

        dataRequest->Irp->IoStatus.Information = 0;

        IoCompleteRequest(dataRequest->Irp, IO_NO_INCREMENT);

        ExFreePool(dataRequest);

        return status;

      }

      //
      // Figure out how much data we are going to move.  Allow control app to set its
      // own MAX size here...
      //
      if (irpSp->Parameters.Write.Length < controlRequest->RequestBufferLength) {
        
        bytesToCopy = irpSp->Parameters.Write.Length;

      } else {

        bytesToCopy = controlRequest->RequestBufferLength;

      }

      //
      // Since the control application's address space is "naked" here we must protect our
      // data copy.
      //
      __try {

        RtlCopyMemory(controlRequest->RequestBuffer, dataBuffer, bytesToCopy);

        controlRequest->RequestBufferLength = bytesToCopy;

        status = STATUS_SUCCESS;

      } __except(EXCEPTION_EXECUTE_HANDLER) {

        status = GetExceptionCode();

      }

      //
      // We return the final results below...
      //

    } else {

      //
      // This is a read operation
      //

      controlRequest->RequestType = OSR_COMM_READ_REQUEST;

      //
      // For a READ operation, we must lob data into the user's address space
      //
      controlRequest->RequestBuffer = NULL;

      controlRequest->RequestBufferLength = IoGetCurrentIrpStackLocation(dataRequest->Irp)->Parameters.Read.Length;


      //
      // We've finished processing the request to this point.  Dispatch to the control application
      // for further processing.
      //

      status = STATUS_SUCCESS;
    }

  }

  if(status == STATUS_SUCCESS) {

      Irp->IoStatus.Information = sizeof(OSR_COMM_CONTROL_REQUEST);

  }

  return status;
}

//
// OsrCommDeviceControl
//
//  This is the device control entry point
//
// Inputs:
//  DeviceObject - this is the device object on which we are operating
//  Irp - this is the device control IRP
//
// Outputs:
//  None.
//
// Returns:
//  SUCCESS - the operation was successful.
//
// Notes:
//  None.
//
NTSTATUS OsrCommDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
  POSR_COMM_CONTROL_RESPONSE controlResponse;
  POSR_COMM_CONTROL_REQUEST  controlRequest;
  PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
  BOOLEAN sendResponse;
  BOOLEAN getRequest;
  NTSTATUS status;

  DbgPrint("OsrCommDeviceControl: Entered. \n");

  //
  // This is only supported for the control device
  //
  if (OSR_COMM_CONTROL_TYPE != DeviceObject->DeviceType) {

    Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;

    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    //
    // Note that we don't do this...
    //
    return STATUS_INVALID_DEVICE_REQUEST;

  }

  //
  // Case on the control code
  //
  switch (irpSp->Parameters.DeviceIoControl.IoControlCode) {
    
    case OSR_COMM_CONTROL_GET_AND_SEND:
    DbgPrint("OsrCommDeviceControl: GET_AND_SEND received.\n");
    sendResponse = TRUE;
    getRequest = TRUE;
    break;

    case OSR_COMM_CONTROL_GET_REQUEST:
    DbgPrint("OsrCommDeviceControl: GET received.\n");
    sendResponse = FALSE;
    getRequest = TRUE;
    break;

    case OSR_COMM_CONTROL_SEND_RESPONSE:
    DbgPrint("OsrCommDeviceControl: SEND received.\n");
    sendResponse = TRUE;
    getRequest = FALSE;
    break;

    default:
    //
    // What IS this thing?
    //
    sendResponse = FALSE;
    getRequest = FALSE;
    break;
  }

  if (!sendResponse && !getRequest) {
    //
    // Invalid request operation
    //
    Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;

    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_INVALID_DEVICE_REQUEST;

  }

  //
  // Parameter validation...
  //

  if (sendResponse) {
    //
    // Validate response parameters
    //
    if (irpSp->Parameters.DeviceIoControl.InputBufferLength < sizeof(OSR_COMM_CONTROL_RESPONSE)) {

      Irp->IoStatus.Status = STATUS_BUFFER_TOO_SMALL;

      Irp->IoStatus.Information = sizeof(OSR_COMM_CONTROL_RESPONSE);

      IoCompleteRequest(Irp, IO_NO_INCREMENT);

      return STATUS_BUFFER_TOO_SMALL;

    }

  }

  if (getRequest) {

    if (irpSp->Parameters.DeviceIoControl.OutputBufferLength < sizeof(OSR_COMM_CONTROL_REQUEST)) {
      
      Irp->IoStatus.Status = STATUS_BUFFER_OVERFLOW;
      
      Irp->IoStatus.Information = sizeof(OSR_COMM_CONTROL_REQUEST);
      
      IoCompleteRequest(Irp, IO_NO_INCREMENT);
      
      return STATUS_BUFFER_OVERFLOW;
      
    }

  }

  //
  // Parameters are OK at this point.  Let's process the request/response stuff
  //
  if (sendResponse) {

    //
    // Have to handle response first.
    //
    status = ProcessResponse(Irp);

    //
    // If there was an error, the error is reported back
    // immediately, even if this was also a get request call
    //
    if (!NT_SUCCESS(status)) {

      return status;

    }

  }

  if (getRequest) {

    //
    // Now process the request.  This IRP may be queued
    // as part of the processing here.
    //
    status = ProcessControlRequest(Irp);

  }

  //
  // If the request was not pending, we complete it here.  Note that
  // we assume the subroutines have set up the IRP for completion.  We're
  // just the only point where we can safely complete it (convergence for
  // both branches above.)
  //
  if (STATUS_PENDING != status) {

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

  }

  //
  // Regardless, at this point we return the status back to the caller.
  //
  return status;

}

//
// OsrCommReadWrite
//
//  This is the read/write entry point
//
// Inputs:
//  DeviceObject - this is the device object on which we are operating
//  Irp - this is the read IRP
//
// Outputs:
//  None.
//
// Returns:
//  SUCCESS - the operation was successful.
//
// Notes:
//  The operation is common for both read/write, except that it uses the "correct" queue
//  for each operation.
//
NTSTATUS OsrCommReadWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
  NTSTATUS status = STATUS_SUCCESS;
  ULONG information = 0;
  POSR_COMM_DATA_REQUEST dataRequest;
  POSR_COMM_DATA_DEVICE_EXTENSION dataExt =
    (POSR_COMM_DATA_DEVICE_EXTENSION) DeviceObject->DeviceExtension;
  POSR_COMM_CONTROL_DEVICE_EXTENSION controlExt =
    (POSR_COMM_CONTROL_DEVICE_EXTENSION) OsrCommDeviceObject->DeviceExtension;
  PLIST_ENTRY listEntry;
  PIRP controlIrp;
  POSR_COMM_CONTROL_REQUEST controlRequest;
  BOOLEAN writeOp = IRP_MJ_WRITE == IoGetCurrentIrpStackLocation(Irp)->MajorFunction;
  PFAST_MUTEX queueLock;
  PLIST_ENTRY queue;
  PMDL mdl;
  PVOID dataBuffer, controlBuffer;
  ULONG bytesToCopy;

  if (OSR_COMM_CONTROL_TYPE == DeviceObject->DeviceType) {

    //
    // Control device does not support read operations
    //
    status = STATUS_INVALID_DEVICE_REQUEST;

  }

  if (OSR_COMM_DATA_TYPE == DeviceObject->DeviceType) {

    //
    // Set the queue to use appropriately
    //
    if (writeOp) {

        DbgPrint("OsrCommReadWrite: Write Request Received.\n");

        queue = &dataExt->WriteRequestQueue;

        queueLock = &dataExt->WriteRequestQueueLock;

    } else {

        DbgPrint("OsrCommReadWrite: Read Request Received.\n");

        queue = &dataExt->ReadRequestQueue;

        queueLock = &dataExt->ReadRequestQueueLock;

    }

    //
    // If the device is enabled, enqueue the request
    //
    switch (dataExt->DeviceState) {

        case OSR_COMM_DATA_DEVICE_ACTIVE:
          //
          // Data device read must be satisfied by queuing request
          // off to the service.
          //
          dataRequest = (POSR_COMM_DATA_REQUEST) ExAllocatePoolWithTag(PagedPool, sizeof(OSR_COMM_DATA_REQUEST), 'rdCO');

          if (!dataRequest) {

            //
            // Complete the request, indicating that the operation failed
            //
            status = STATUS_INSUFFICIENT_RESOURCES;
        
            break;
          }
      
          RtlZeroMemory(dataRequest, sizeof(OSR_COMM_DATA_REQUEST));
      
          dataRequest->RequestID = (ULONG) InterlockedIncrement(&OsrRequestID);
      
          dataRequest->Irp = Irp;
      
          //
          // Since we are enqueuing the IRP, mark it pending
          //
          IoMarkIrpPending(Irp);
      
          status = STATUS_PENDING;

          //
          // Insert the request into the appropriate queue here
          //
          ExAcquireFastMutex(queueLock);
        
          InsertTailList(queue, &dataRequest->ListEntry);
        
          ExReleaseFastMutex(queueLock);
        
          //
          // Now, let's try to dispatch this to a service thread (really an IRP)
          // and if we cannot do so, we need to enqueue it for later processing
          // when a thread becomes available.
          //
          ExAcquireFastMutex(&controlExt->ServiceQueueLock);
      
          if (IsListEmpty(&controlExt->ServiceQueue)) {

            //
            // No waiting threads.  We need to insert this into the service request queue
            //
            ExAcquireFastMutex(&controlExt->RequestQueueLock);

            InsertTailList(&controlExt->RequestQueue, &dataRequest->ServiceListEntry);

            ExReleaseFastMutex(&controlExt->RequestQueueLock);
        
            //
            // Release the service queue lock
            //
            ExReleaseFastMutex(&controlExt->ServiceQueueLock);

          } else {

            //
            // A service thread is available right now.  Remove the service thread
            // from the queue.
            //
            listEntry = RemoveHeadList(&controlExt->ServiceQueue);
        
            controlIrp = CONTAINING_RECORD(listEntry, IRP, Tail.Overlay.ListEntry);

            //
            // Now build the request packet here.
            //
            controlRequest = (POSR_COMM_CONTROL_REQUEST) controlIrp->AssociatedIrp.SystemBuffer;

            controlRequest->RequestID = dataRequest->RequestID;

            DbgPrint("OsrCommReadWrite:  Irp %x, RequestBuffer %x RequestBufferLength %x\n.",
                     controlIrp,
                     controlRequest->RequestBuffer,
                     controlRequest->RequestBufferLength);


            if (writeOp) {

              controlRequest->RequestType = OSR_COMM_WRITE_REQUEST;

              //
              // Our problem here is that the control buffer is in a different
              // address space.  So, we need to reach over into that address space and
              // grab it.
              //
              mdl = IoAllocateMdl(controlRequest->RequestBuffer,
                                  controlRequest->RequestBufferLength,
                                  FALSE, // should not be any other MDLs associated with control IRP
                                  FALSE, // no quota charged
                                  controlIrp); // track the MDL in the control IRP...

              if (NULL == mdl) {
                //
                // We failed to get an MDL.  What a pain.
                //
                InsertTailList(&controlExt->ServiceQueue, listEntry);

                //
                // Complete the data request - this falls through and completes below.
                //
                status = STATUS_INSUFFICIENT_RESOURCES;

                //
                // Release the service queue lock
                //
                ExReleaseFastMutex(&controlExt->ServiceQueueLock);

                Irp->IoStatus.Status = status;
    
                Irp->IoStatus.Information = 0;
    
                IoCompleteRequest(Irp, IO_NO_INCREMENT);

                status = STATUS_PENDING;

                break;
            
              }

              __try {
                //
                // Probe and lock the pages
                //
                MmProbeAndLockProcessPages(mdl,
                                           IoGetRequestorProcess(controlIrp),
                                           UserMode,
                                           IoWriteAccess);

              } __except(EXCEPTION_EXECUTE_HANDLER) {

                //
                // Access probe failed
                //
                status = GetExceptionCode();

                //
                // Cleanup what we were doing....
                //

                IoFreeMdl(mdl);

                InsertTailList(&controlExt->ServiceQueue, listEntry);

                //
                // Release the service queue lock
                //
                ExReleaseFastMutex(&controlExt->ServiceQueueLock);

                Irp->IoStatus.Status = status;
    
                Irp->IoStatus.Information = 0;
    
                IoCompleteRequest(Irp, IO_NO_INCREMENT);

                status = STATUS_PENDING;

                break;

              }

              //
              // We now have an MDL we can use
              //
              dataBuffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);

              controlBuffer = MmGetSystemAddressForMdlSafe(mdl, NormalPagePriority);

              if ((NULL == dataBuffer) || (NULL == controlBuffer)) {

                //
                // Not enough PTEs, obviously. Since we've modified the control IRP we need
                // to complete it here so we don't leave junk lying around.
                //
                controlIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;

                controlIrp->IoStatus.Information = 0;

                IoCompleteRequest(controlIrp, IO_NO_INCREMENT);

                status = STATUS_INSUFFICIENT_RESOURCES;

                //
                // Release the service queue lock
                //
                ExReleaseFastMutex(&controlExt->ServiceQueueLock);

                //
                // Handle data irp failure below
                //
                break;

              }

              bytesToCopy = IoGetCurrentIrpStackLocation(Irp)->Parameters.Write.Length;

              //
              // Cannot copy more data than there is room to copy it...
              //
              if (controlRequest->RequestBufferLength < bytesToCopy) {
            
                bytesToCopy = controlRequest->RequestBufferLength;

              }

              //
              // OK.  We can copy data!  Yeah!
              //
              RtlCopyMemory(controlBuffer, dataBuffer, bytesToCopy);

              //
              // Complete the control IRP
              //
              controlRequest->RequestBufferLength = bytesToCopy;

              controlIrp->IoStatus.Status = STATUS_SUCCESS;

              controlIrp->IoStatus.Information = sizeof(OSR_COMM_CONTROL_REQUEST);

              IoCompleteRequest(controlIrp, IO_NO_INCREMENT);

              status = STATUS_PENDING;

            } else {

              controlRequest->RequestType = OSR_COMM_READ_REQUEST;

              controlRequest->RequestBuffer = Irp->AssociatedIrp.SystemBuffer;
          
              //
              // Note that length is in the same location for both read and write
              //
              controlRequest->RequestBufferLength = IoGetCurrentIrpStackLocation(Irp)->Parameters.Read.Length;
          
              //
              // And complete the control request
              //
              controlIrp->IoStatus.Status = STATUS_SUCCESS;
          
              controlIrp->IoStatus.Information = sizeof(OSR_COMM_CONTROL_REQUEST);
          
              IoCompleteRequest(controlIrp, IO_NO_INCREMENT);
          
              status = STATUS_PENDING;

            }
        
            //
            // Release the service queue lock
            //
            ExReleaseFastMutex(&controlExt->ServiceQueueLock);
        
          }
          break;
        
        default:
          status = STATUS_INVALID_DEVICE_REQUEST;
      
    }

  }

  //
  // If the status is not STATUS_PENDING, complete the request
  //
  if (STATUS_PENDING != status) {
    
    //
    // Set status
    //
    Irp->IoStatus.Status = status;
    
    Irp->IoStatus.Information = 0;
    
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    
  }

  //
  // Done.
  //
  return status;

}



//
// Unload
//
//  This is the unload function
//
// Inputs:
//   DriverObject
//
// Outputs:
//   None.
//
// Returns:
//   None - void function
//
// Notes:
//
VOID Unload(PDRIVER_OBJECT DriverObject)
{
  POSR_COMM_CONTROL_DEVICE_EXTENSION commExt;
  POSR_COMM_DATA_DEVICE_EXTENSION dataExt;
  NTSTATUS status;

  //
  // If we have a device object, free up elements within the device.
  //
  if (OsrCommDeviceObject) {

    commExt = (POSR_COMM_CONTROL_DEVICE_EXTENSION) OsrCommDeviceObject->DeviceExtension;
    
    ASSERT(OSR_COMM_CONTROL_EXTENSION_MAGIC_NUMBER == commExt->MagicNumber);

    //
    // Registry path buffer was allocated
    //
    if (commExt->RegistryPath.Buffer) {

      ExFreePool(commExt->RegistryPath.Buffer);

      commExt->RegistryPath.Buffer = 0;

    }

    //
    // Driver name buffer was allocated
    //
    if (commExt->DriverName.Buffer) {

      ExFreePool(commExt->DriverName.Buffer);

      commExt->DriverName.Buffer = 0;

    }

    if (commExt->SymbolicLinkName.Buffer) {
      //
      // Delete the symbolic link - ignore errors
      //
      (void) IoDeleteSymbolicLink(&commExt->SymbolicLinkName);

      //
      // Free the string buffer
      //
      ExFreePool(commExt->SymbolicLinkName.Buffer);

      commExt->SymbolicLinkName.Buffer = 0;

    }
      
    //
    // Done with the device object
    //
    (void) IoDeleteDevice(OsrCommDeviceObject);

    //
    // Reset it
    //
    OsrCommDeviceObject = 0;

  }

  //
  // If there is a data device object, we need to clean it up here
  //
  if (OsrDataDeviceObject) {
    
    dataExt = (POSR_COMM_DATA_DEVICE_EXTENSION) OsrDataDeviceObject->DeviceExtension;

    if (dataExt->SymbolicLinkName.Buffer) {
      //
      // Delete the symbolic link - ignore errors
      //
      (void) IoDeleteSymbolicLink(&dataExt->SymbolicLinkName);

      //
      // Free the string buffer
      //
      ExFreePool(dataExt->SymbolicLinkName.Buffer);

      dataExt->SymbolicLinkName.Buffer = 0;

    }
      
    //
    // Done with the device object
    //
    (void) IoDeleteDevice(OsrDataDeviceObject);

    //
    // Reset it
    //
    OsrDataDeviceObject = 0;
  }

  //
  // No additional cleanup required
  //
  return;
}



//
// DriverEntry
//
//  This is the standard entry-point function for this driver
//
// Inputs:
//  
NTSTATUS __stdcall DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
  UNICODE_STRING driverName;
  UNICODE_STRING deviceName;
  UNICODE_STRING tempString;
  NTSTATUS status;
  USHORT index;
  POSR_COMM_CONTROL_DEVICE_EXTENSION controlExt;
  POSR_COMM_DATA_DEVICE_EXTENSION dataExt;

  //
  // Let's figure out what our driver name is, compute our device name from that and
  // create a control device.  We'll store our valuable data within the device extension
  // of our control object.
  //

  DbgPrint("Comm Driver DeviceEntry: Entered.\n");

//  DbgBreakPoint();
  
  //
  // First, figure out how big the driver name is for this driver.
  //
  index = RegistryPath->Length / sizeof(WCHAR);

  do {
    index--;

  } while (index && (RegistryPath->Buffer[index] !=  OBJ_NAME_PATH_SEPARATOR));

  //
  // This entry should point to the slash before the last component of the name
  //
  if (RegistryPath->Buffer[index] == OBJ_NAME_PATH_SEPARATOR) {

    index++;

  }
  
  //
  // Now we know the length of the string
  //
  driverName.Length = RegistryPath->Length - (index * sizeof(WCHAR));

  //
  // Allocate enough for the NULL at the end...
  //
  driverName.MaximumLength = driverName.Length + sizeof(WCHAR);

  //
  // Allocate storage for the driver name: (name + NULL) * 2
  //
  driverName.Buffer = (PWCHAR) ExAllocatePoolWithTag(PagedPool, driverName.MaximumLength, 'ndCO');

  if (NULL == driverName.Buffer) {

    //
    // Allocation failed
    //
    return STATUS_INSUFFICIENT_RESOURCES;

  }

  RtlZeroMemory(driverName.Buffer, driverName.MaximumLength);

  RtlCopyMemory(driverName.Buffer, &RegistryPath->Buffer[index], driverName.Length);

  //
  // OK.  Now let's create a device name; we have the middle and we need a
  // prefix and a suffix.
  //
  deviceName.MaximumLength = driverName.MaximumLength +
    sizeof(OSR_COMM_CONTROL_DEVICE_NAME_PREFIX) +
    sizeof(OSR_COMM_CONTROL_DEVICE_NAME_SUFFIX);

  deviceName.Buffer = (PWSTR) ExAllocatePoolWithTag(PagedPool, deviceName.MaximumLength, 'NDFF');

  if (NULL == deviceName.Buffer) {

    //
    // Allocation failed
    //
    ExFreePool(driverName.Buffer);

    return STATUS_INSUFFICIENT_RESOURCES;

  }

  //
  // Build the control object name - do not include the trailing null size in this
  // computation.
  //
  deviceName.Length = sizeof(OSR_COMM_CONTROL_DEVICE_NAME_PREFIX) - sizeof(WCHAR);

  RtlCopyMemory(deviceName.Buffer, OSR_COMM_CONTROL_DEVICE_NAME_PREFIX, deviceName.Length);

  status = RtlAppendUnicodeStringToString(&deviceName, &driverName);

  if (!NT_SUCCESS(status)) {

    //
    // Append failed.
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    return status;
    
  }

  RtlInitUnicodeString(&tempString, OSR_COMM_CONTROL_DEVICE_NAME_SUFFIX);

  status = RtlAppendUnicodeStringToString(&deviceName, &tempString);

  if (!NT_SUCCESS(status)) {

    //
    // Append failed.
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    return status;
    
  }

  DbgPrint("Create device %wZ\n", &deviceName);

  //
  // Now we can create the control device object
  //
  status = IoCreateDevice(DriverObject,
                          sizeof(OSR_COMM_CONTROL_DEVICE_EXTENSION),
                          &deviceName,
                          OSR_COMM_CONTROL_TYPE,
                          FILE_DEVICE_SECURE_OPEN, // characteristics
                          TRUE, // exclusive - only one control process
                          &OsrCommDeviceObject);

  //
  // Did the create device work?
  //
  if (!NT_SUCCESS(status)) {
    //
    // The inevitable clean-up
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    return status;

  }

  //
  // Set up the device extension
  //
  controlExt = (POSR_COMM_CONTROL_DEVICE_EXTENSION) OsrCommDeviceObject->DeviceExtension;

  controlExt->MagicNumber = OSR_COMM_CONTROL_EXTENSION_MAGIC_NUMBER;

  controlExt->DriverName = driverName;

  InitializeListHead(&controlExt->ServiceQueue);

  ExInitializeFastMutex(&controlExt->ServiceQueueLock);

  InitializeListHead(&controlExt->RequestQueue);

  ExInitializeFastMutex(&controlExt->RequestQueueLock);


  //
  // Now, store away the registry path for future use
  //
  controlExt->RegistryPath.Buffer = (PWSTR) ExAllocatePoolWithTag(PagedPool,
                                                           RegistryPath->Length + sizeof(WCHAR),
                                                           'prCO');

  if (NULL == controlExt->RegistryPath.Buffer) {

    //
    // Allocation failed
    //
    IoDeleteDevice(OsrCommDeviceObject);

    ExFreePool(driverName.Buffer);
    ExFreePool(deviceName.Buffer);

    return STATUS_INSUFFICIENT_RESOURCES;

  }

  controlExt->RegistryPath.MaximumLength = RegistryPath->Length + sizeof(WCHAR);

  RtlCopyUnicodeString(&controlExt->RegistryPath, RegistryPath);

  //
  // We need to make the control object visible to Win32
  //
  controlExt->SymbolicLinkName.MaximumLength = driverName.MaximumLength +
    sizeof(OSR_COMM_DOSDEVICE_PATH) +
    sizeof(OSR_COMM_CONTROL_DEVICE_NAME_SUFFIX);
  

  controlExt->SymbolicLinkName.Buffer =
    (PWSTR) ExAllocatePoolWithTag(PagedPool,
                                  controlExt->SymbolicLinkName.MaximumLength,
                                  'lSCO');
  
  if (NULL == controlExt->SymbolicLinkName.Buffer) {

    //
    // Allocation failed
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(deviceName.Buffer);

    Unload(DriverObject);

  }

  //
  // Build the symlink name up
  //
  controlExt->SymbolicLinkName.Length = sizeof(OSR_COMM_DOSDEVICE_PATH) - sizeof(WCHAR);

  RtlCopyMemory(controlExt->SymbolicLinkName.Buffer,
                OSR_COMM_DOSDEVICE_PATH,
                controlExt->SymbolicLinkName.Length);

  status = RtlAppendUnicodeStringToString(&controlExt->SymbolicLinkName, &driverName);

  if (!NT_SUCCESS(status)) {

    //
    // Append failed
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return status;

  }

  RtlInitUnicodeString(&tempString, OSR_COMM_CONTROL_DEVICE_NAME_SUFFIX);

  status = RtlAppendUnicodeStringToString(&controlExt->SymbolicLinkName, &tempString);

  if (!NT_SUCCESS(status)) {

    //
    // Append failed
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return status;

  }

  //
  // Now we can create the symbolic link
  //
  status = IoCreateSymbolicLink(&controlExt->SymbolicLinkName, &deviceName);

  //
  // No matter what, we're done with the device name
  //
  ExFreePool(deviceName.Buffer);

  if (!NT_SUCCESS(status)) {

    //
    // The symbolic link creation failed.
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return status;

  }

  //
  // Now let's create the data device object
  //
  deviceName.MaximumLength = driverName.MaximumLength +
    sizeof(OSR_COMM_DATA_DEVICE_NAME_PREFIX) +
    sizeof(OSR_COMM_DATA_DEVICE_NAME_SUFFIX);

  deviceName.Buffer = (PWSTR) ExAllocatePoolWithTag(PagedPool, deviceName.MaximumLength, 'NDCO');

  if (NULL == deviceName.Buffer) {

    //
    // Allocation failed
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return STATUS_INSUFFICIENT_RESOURCES;
  }

  deviceName.Length = sizeof(OSR_COMM_DATA_DEVICE_NAME_PREFIX) - sizeof(WCHAR);

  RtlCopyMemory(deviceName.Buffer, OSR_COMM_DATA_DEVICE_NAME_PREFIX, deviceName.Length);

  status = RtlAppendUnicodeStringToString(&deviceName, &driverName);

  if (!NT_SUCCESS(status)) {

    //
    // Clean up, return error
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return status;

  }

  //
  // Append the suffix
  //
  RtlInitUnicodeString(&tempString, OSR_COMM_DATA_DEVICE_NAME_SUFFIX);

  status = RtlAppendUnicodeStringToString(&deviceName, &tempString);

  if (!NT_SUCCESS(status)) {

    //
    // Clean up, return error
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);
  }

  DbgPrint("Create device %wZ\n", &deviceName);

  //
  // Create the data device object
  //
  status = IoCreateDevice(DriverObject,
                          sizeof(OSR_COMM_DATA_DEVICE_EXTENSION),
                          &deviceName,
                          OSR_COMM_DATA_TYPE,
                          FILE_DEVICE_SECURE_OPEN,
                          FALSE,
                          &OsrDataDeviceObject);

  //
  // Did the create device work?
  //
  if (!NT_SUCCESS(status)) {

    //
    // Cleanup, bail out
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return status;

  }

  //
  // Note that this device uses DIRECT I/O
  //
  OsrDataDeviceObject->Flags |= DO_DIRECT_IO;

  //
  // Set up the data device extension
  //
  dataExt = (POSR_COMM_DATA_DEVICE_EXTENSION) OsrDataDeviceObject->DeviceExtension;

  dataExt->MagicNumber = OSR_COMM_DATA_DEVICE_EXTENSION_MAGIC_NUMBER;
  InitializeListHead(&dataExt->ReadRequestQueue);
  ExInitializeFastMutex(&dataExt->ReadRequestQueueLock);
  InitializeListHead(&dataExt->WriteRequestQueue);
  ExInitializeFastMutex(&dataExt->WriteRequestQueueLock);

  //
  // Create a symbolic link so the driver is visible to Win32 applications
  //
  dataExt->SymbolicLinkName.MaximumLength = driverName.MaximumLength + 
    sizeof(OSR_COMM_DOSDEVICE_PATH) +
    sizeof(OSR_COMM_DATA_DEVICE_NAME_SUFFIX);

  dataExt->SymbolicLinkName.Buffer =
    (PWSTR) ExAllocatePoolWithTag(PagedPool,
                                  dataExt->SymbolicLinkName.MaximumLength,
                                  'LSCO');

  if (NULL == dataExt->SymbolicLinkName.Buffer) {

    //
    // Allocation failed
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return STATUS_INSUFFICIENT_RESOURCES;

  }

  //
  // Build the symlink
  //
  dataExt->SymbolicLinkName.Length = sizeof(OSR_COMM_DOSDEVICE_PATH) - sizeof(WCHAR);

  RtlCopyMemory(dataExt->SymbolicLinkName.Buffer,
                OSR_COMM_DOSDEVICE_PATH,
                dataExt->SymbolicLinkName.Length);

  status = RtlAppendUnicodeStringToString(&dataExt->SymbolicLinkName, &driverName);

  if (!NT_SUCCESS(status)) {

    //
    // Append failed
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return status;

  }

  RtlInitUnicodeString(&tempString, OSR_COMM_DATA_DEVICE_NAME_SUFFIX);

  status = RtlAppendUnicodeStringToString(&dataExt->SymbolicLinkName, &tempString);

  if (!NT_SUCCESS(status)) {

    //
    // Append failed
    //
    ExFreePool(deviceName.Buffer);
    ExFreePool(driverName.Buffer);

    Unload(DriverObject);

    return status;
  }

  //
  // Now we can create the symbolic link
  //
  status = IoCreateSymbolicLink(&dataExt->SymbolicLinkName,
                                &deviceName);

  //
  // No matter what, we don't need the device name anymore
  //
  ExFreePool(deviceName.Buffer);
  ExFreePool(driverName.Buffer);

  if (!NT_SUCCESS(status)) {

    //
    // The symbolic link creation failed.
    //
    Unload(DriverObject);

    return status;

  }

  //
  // Now, set up entry points
  //

  DriverObject->DriverUnload = Unload;
  DriverObject->MajorFunction[IRP_MJ_CREATE] = OsrCommCreate;
  DriverObject->MajorFunction[IRP_MJ_CLEANUP] = OsrCommCleanup;
  DriverObject->MajorFunction[IRP_MJ_CLOSE] = OsrCommClose;
  DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OsrCommDeviceControl;
  DriverObject->MajorFunction[IRP_MJ_READ] = 
  DriverObject->MajorFunction[IRP_MJ_WRITE] = OsrCommReadWrite;
  

  //
  // We are done at this point
  //
  return STATUS_SUCCESS;
}











