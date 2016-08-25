//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1992 - 1996  Microsoft Corporation.  All Rights Reserved.
//
//==========================================================================;

//
// This file handles all adapter property sets
//


#include "strmini.h"
#include "ksmedia.h"
#include "capmain.h"
#include "capdebug.h"
#include "capxfer.h"

// -------------------------------------------------------------------
// A few notes about property set handling
//  
// Property sets used in Testcap are of two varieties, those that have
// default values, ranges, and stepping, such as VideoProcAmp and CameraControl,
// and those which don't have defaults and ranges, such as TVTuner and
// Crossbar.
// 
// Default values and stepping are established by tables in capprop.h,
// no code is required to implement this other than initally creating the tables.
// 
// Many of the property sets require the ability to modify a number
// of input parameters.  Since KS doesn't allow this inherently, you'll
// note that some property sets require copying the provided input parameters
// to the ouput parameter list, effectively creating a "read, modify, write"
// capability.  For this reason, the input and output parameter lists
// use identical structures.
//
// On an SRB_GET_DEVICE_PROPERTY, read-only input data to the driver is provided as:
//      pSrb->CommandData.PropertyInfo
//
// ... while the output data pointer is:
//      pSrb->CommandData.PropertyInfo.PropertyInfo
// 
// -------------------------------------------------------------------



// -------------------------------------------------------------------
// VideoProcAmp functions
// -------------------------------------------------------------------

/*
** AdapterSetVideoProcAmpProperty ()
**
**    Handles Set operations on the VideoProcAmp property set.
**      Testcap uses this for demo purposes only.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID 
STREAMAPI
AdapterSetVideoProcAmpProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    ULONG Id = pSPD->Property->Id;              // index of the property
    PKSPROPERTY_VIDEOPROCAMP_S pS = (PKSPROPERTY_VIDEOPROCAMP_S) pSPD->PropertyInfo;

    ASSERT (pSPD->PropertyInputSize >= sizeof (KSPROPERTY_VIDEOPROCAMP_S));

    switch (Id) {

    case KSPROPERTY_VIDEOPROCAMP_BRIGHTNESS:
        pHwDevExt->Brightness = pS->Value;
        pHwDevExt->BrightnessFlags = pS->Flags;
        break;
        
    case KSPROPERTY_VIDEOPROCAMP_CONTRAST:
        pHwDevExt->Contrast = pS->Value;
        pHwDevExt->ContrastFlags = pS->Flags;
        break;

    case KSPROPERTY_VIDEOPROCAMP_COLORENABLE:
        pHwDevExt->ColorEnable = pS->Value;
        pHwDevExt->ColorEnableFlags = pS->Flags;
        break;

    default:
        TRAP;
        break;
    }
}

/*
** AdapterGetVideoProcAmpProperty ()
**
**    Handles Get operations on the VideoProcAmp property set.
**      Testcap uses this for demo purposes only.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID 
STREAMAPI
AdapterGetVideoProcAmpProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    ULONG Id = pSPD->Property->Id;              // index of the property
    PKSPROPERTY_VIDEOPROCAMP_S pS = (PKSPROPERTY_VIDEOPROCAMP_S) pSPD->PropertyInfo;  

    ASSERT (pSPD->PropertyOutputSize >= sizeof (KSPROPERTY_VIDEOPROCAMP_S));

    switch (Id) {

    case KSPROPERTY_VIDEOPROCAMP_BRIGHTNESS:
        pS->Value = pHwDevExt->Brightness;
        pS->Flags = pHwDevExt->BrightnessFlags;
        pS->Capabilities = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL | 
                           KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
        break;
        
    case KSPROPERTY_VIDEOPROCAMP_CONTRAST:
        pS->Value = pHwDevExt->Contrast;
        pS->Flags = pHwDevExt->ContrastFlags;
        pS->Capabilities = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL | 
                           KSPROPERTY_VIDEOPROCAMP_FLAGS_AUTO;
        break;

    case KSPROPERTY_VIDEOPROCAMP_COLORENABLE:
        pS->Value = pHwDevExt->ColorEnable;
        pS->Flags = pHwDevExt->ColorEnableFlags;
        pS->Capabilities = KSPROPERTY_VIDEOPROCAMP_FLAGS_MANUAL;
        break;

    default:
        TRAP;
        break;
    }
    pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOPROCAMP_S);
}

// -------------------------------------------------------------------
// CameraControl functions
// -------------------------------------------------------------------

/*
** AdapterSetCameraControlProperty ()
**
**    Handles Set operations on the CameraControl property set.
**      Testcap uses this for demo purposes only.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID 
STREAMAPI
AdapterSetCameraControlProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    ULONG Id = pSPD->Property->Id;              // index of the property
    PKSPROPERTY_CAMERACONTROL_S pS = (PKSPROPERTY_CAMERACONTROL_S) pSPD->PropertyInfo;

    ASSERT (pSPD->PropertyInputSize >= sizeof (KSPROPERTY_CAMERACONTROL_S));

    switch (Id) {

    case KSPROPERTY_CAMERACONTROL_ZOOM:
        pHwDevExt->Zoom = pS->Value;
        pHwDevExt->ZoomFlags = pS->Flags;
        break;
        
    case KSPROPERTY_CAMERACONTROL_FOCUS:
        pHwDevExt->Focus = pS->Value;
        pHwDevExt->FocusFlags = pS->Flags;
        break;

    default:
        TRAP;
        break;
    }
}

/*
** AdapterGetCameraControlProperty ()
**
**    Handles Get operations on the CameraControl property set.
**      Testcap uses this for demo purposes only.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID 
STREAMAPI
AdapterGetCameraControlProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    ULONG Id = pSPD->Property->Id;              // index of the property
    PKSPROPERTY_CAMERACONTROL_S pS = (PKSPROPERTY_CAMERACONTROL_S) pSPD->PropertyInfo;    // pointer to the output data

    ASSERT (pSPD->PropertyOutputSize >= sizeof (KSPROPERTY_CAMERACONTROL_S));

    switch (Id) {

    case KSPROPERTY_CAMERACONTROL_ZOOM:
        pS->Value = pHwDevExt->Zoom;
        pS->Flags = pHwDevExt->ZoomFlags;
        pS->Capabilities = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL | 
                           KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
        break;
        
    case KSPROPERTY_CAMERACONTROL_FOCUS:
        pS->Value = pHwDevExt->Focus;
        pS->Flags = pHwDevExt->FocusFlags;
        pS->Capabilities = KSPROPERTY_CAMERACONTROL_FLAGS_MANUAL | 
                           KSPROPERTY_CAMERACONTROL_FLAGS_AUTO;
        break;

    default:
        TRAP;
        break;
    }
    pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_CAMERACONTROL_S);
}

// -------------------------------------------------------------------
// VideoControl functions
// -------------------------------------------------------------------

/*
** AdapterSetVideoControlProperty ()
**
**    Handles Set operations on the VideoControl property set.
**      Testcap uses this for demo purposes only.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID 
STREAMAPI
AdapterSetVideoControlProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    ULONG Id = pSPD->Property->Id;              // index of the property
    PKSPROPERTY_VIDEOCONTROL_MODE_S pS = (PKSPROPERTY_VIDEOCONTROL_MODE_S) pSPD->PropertyInfo;
    PSTREAMEX pStrmEx;
    ULONG StreamIndex;
    ULONG *pVideoControlMode;

    // For this property set, the StreamIndex is always in the same place
    // for each property
    StreamIndex = ((PKSPROPERTY_VIDEOCONTROL_CAPS_S) pSPD->Property)->StreamIndex;

    ASSERT (StreamIndex < MAX_TESTCAP_STREAMS);

    // Verify the stream index is valid
    if (StreamIndex >= MAX_TESTCAP_STREAMS) {
        pSrb->Status = STATUS_INVALID_PARAMETER;
        return;
    }

    pStrmEx = (PSTREAMEX) pHwDevExt->pStrmEx[StreamIndex];

    // If the stream is not opened when this property set is used,
    // store the values in the HwDevExt

    if (pStrmEx) {
        pVideoControlMode = &pStrmEx->VideoControlMode;
    }
    else {
        pVideoControlMode = &pHwDevExt->VideoControlMode;
    }

    ASSERT (pSPD->PropertyInputSize >= sizeof (KSPROPERTY_VIDEOCONTROL_MODE_S));
    
    switch (Id) {

    case KSPROPERTY_VIDEOCONTROL_MODE:
    {
        *pVideoControlMode = pS->Mode;
    }
    break;

    default:
        TRAP;
        break;
    }
}

/*
** AdapterGetVideoControlProperty ()
**
**    Handles Get operations on the VideoControl property set.
**      Testcap uses this for demo purposes only.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID 
STREAMAPI
AdapterGetVideoControlProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    ULONG Id = pSPD->Property->Id;              // index of the property
    PSTREAMEX pStrmEx;
    ULONG StreamIndex;
    ULONG *pVideoControlMode;

    // For this property set, the StreamIndex is always in the same place
    // for each property
    StreamIndex = ((PKSPROPERTY_VIDEOCONTROL_CAPS_S) pSPD->Property)->StreamIndex;

    ASSERT (StreamIndex < MAX_TESTCAP_STREAMS);

    // Verify the stream index is valid
    if (StreamIndex >= MAX_TESTCAP_STREAMS) {
        pSrb->Status = STATUS_INVALID_PARAMETER;
        return;
    }

    pStrmEx = (PSTREAMEX) pHwDevExt->pStrmEx[StreamIndex];

    // If the stream is not opened when this property set is used,
    // store the values in the HwDevExt

    if (pStrmEx) {
        pVideoControlMode = &pStrmEx->VideoControlMode;
    }
    else {
        pVideoControlMode = &pHwDevExt->VideoControlMode;
    }

    switch (Id) {

    case KSPROPERTY_VIDEOCONTROL_CAPS:
    {
        PKSPROPERTY_VIDEOCONTROL_CAPS_S pS = (PKSPROPERTY_VIDEOCONTROL_CAPS_S) pSPD->PropertyInfo;    // pointer to the data

        ASSERT (pSPD->PropertyOutputSize >= sizeof (KSPROPERTY_VIDEOCONTROL_CAPS_S));
        
        pS->VideoControlCaps =    
              KS_VideoControlFlag_FlipHorizontal       
//            | KS_VideoControlFlag_FlipVertical         
//            | KS_VideoControlFlag_ExternalTriggerEnable
//            | KS_VideoControlFlag_Trigger              
            ;

        pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOCONTROL_CAPS_S);
    }
    break;
        
    case KSPROPERTY_VIDEOCONTROL_ACTUAL_FRAME_RATE:
    {
        PKSPROPERTY_VIDEOCONTROL_ACTUAL_FRAME_RATE_S pS = 
            (PKSPROPERTY_VIDEOCONTROL_ACTUAL_FRAME_RATE_S) pSPD->PropertyInfo;    // pointer to the data

        ASSERT (pSPD->PropertyOutputSize >= sizeof (KSPROPERTY_VIDEOCONTROL_ACTUAL_FRAME_RATE_S));

        pS->CurrentActualFrameRate = 15;        // TODO: Implement the right rates in shipping drivers.
        pS->CurrentMaxAvailableFrameRate = 15;  // TODO: Implement the right rates in shipping drivers.
        

        pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOCONTROL_ACTUAL_FRAME_RATE_S);
    }
    break;

    case KSPROPERTY_VIDEOCONTROL_FRAME_RATES:
    {
        // todo
    }
    break;

    case KSPROPERTY_VIDEOCONTROL_MODE:
    {
        PKSPROPERTY_VIDEOCONTROL_MODE_S pS = (PKSPROPERTY_VIDEOCONTROL_MODE_S) pSPD->PropertyInfo;    // pointer to the data

        ASSERT (pSPD->PropertyOutputSize >= sizeof (KSPROPERTY_VIDEOCONTROL_MODE_S));

        pS->Mode = *pVideoControlMode;

        pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOCONTROL_MODE_S);
    }
    break;

    default:
        TRAP;
        break;
    }
}


/*
** AdapterGetVideoCompressionProperty()
**
**    Gets compressor settings
**
** Arguments:
**
**    pSrb - pointer to the stream request block for properties
**
** Returns:
**
** Side Effects:  none
*/

VOID
STREAMAPI
AdapterGetVideoCompressionProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAMEX pStrmEx;
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    ULONG Id = pSPD->Property->Id;              // index of the property
    ULONG StreamIndex;
    PCOMPRESSION_SETTINGS pCompressionSettings;

    // For this property set, the StreamIndex is always in the same place
    // for each property
    StreamIndex = ((PKSPROPERTY_VIDEOCOMPRESSION_S) pSPD->Property)->StreamIndex;

    ASSERT (StreamIndex < MAX_TESTCAP_STREAMS);

    // Verify the stream index is valid
    if (StreamIndex >= MAX_TESTCAP_STREAMS) {
        pSrb->Status = STATUS_INVALID_PARAMETER;
        return;
    }

    pStrmEx = (PSTREAMEX) pHwDevExt->pStrmEx[StreamIndex];

    // If the stream is not opened when this property set is used,
    // store the values in the HwDevExt

    if (pStrmEx) {
        pCompressionSettings = &pStrmEx->CompressionSettings;
    }
    else {
        pCompressionSettings = &pHwDevExt->CompressionSettings;
    }


    switch (Id) {

    case KSPROPERTY_VIDEOCOMPRESSION_GETINFO:
        {
            PKSPROPERTY_VIDEOCOMPRESSION_GETINFO_S pS = 
                (PKSPROPERTY_VIDEOCOMPRESSION_GETINFO_S) pSPD->PropertyInfo;

            pS->DefaultKeyFrameRate = 15;    // Key frame rate
            pS->DefaultPFrameRate = 3;       // Predeicted frames per Key frame
            pS->DefaultQuality = 5000;       // 0 to 10000
            pS->Capabilities = 
                       KS_CompressionCaps_CanQuality  |
                       KS_CompressionCaps_CanKeyFrame |
                       KS_CompressionCaps_CanBFrame   ;
            
            pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOCOMPRESSION_GETINFO_S);
        }
        break;

    case KSPROPERTY_VIDEOCOMPRESSION_KEYFRAME_RATE:
        {
            PKSPROPERTY_VIDEOCOMPRESSION_S pS = 
                (PKSPROPERTY_VIDEOCOMPRESSION_S) pSPD->PropertyInfo;

            pS->Value = pCompressionSettings->CompressionKeyFrameRate;
                
            pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOCOMPRESSION_S);
        }
        break;
    
    case KSPROPERTY_VIDEOCOMPRESSION_PFRAMES_PER_KEYFRAME:
        {
            PKSPROPERTY_VIDEOCOMPRESSION_S pS = 
                (PKSPROPERTY_VIDEOCOMPRESSION_S) pSPD->PropertyInfo;

            pS->Value = pCompressionSettings->CompressionPFramesPerKeyFrame;
                
            pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOCOMPRESSION_S);
        }
        break;
    
    case KSPROPERTY_VIDEOCOMPRESSION_QUALITY:
        {
            PKSPROPERTY_VIDEOCOMPRESSION_S pS = 
                (PKSPROPERTY_VIDEOCOMPRESSION_S) pSPD->PropertyInfo;

            pS->Value = pCompressionSettings->CompressionQuality;
                
            pSrb->ActualBytesTransferred = sizeof (KSPROPERTY_VIDEOCOMPRESSION_S);
        }
        break;
    
    default:
        TRAP;
        break;
    }
}

/*
** AdapterSetVideoCompressionProperty()
**
**    Sets compressor settings
**
** Arguments:
**
**    pSrb - pointer to the stream request block for properties
**
** Returns:
**
** Side Effects:  none
*/

VOID
STREAMAPI
AdapterSetVideoCompressionProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )
{
    PHW_DEVICE_EXTENSION pHwDevExt = ((PHW_DEVICE_EXTENSION)pSrb->HwDeviceExtension);
    PSTREAMEX pStrmEx;
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;
    PKSPROPERTY_VIDEOCOMPRESSION_S pS = (PKSPROPERTY_VIDEOCOMPRESSION_S) pSPD->Property;
    ULONG Id = pSPD->Property->Id;              // index of the property
    ULONG StreamIndex;
    PCOMPRESSION_SETTINGS pCompressionSettings;

    // For this property set, the StreamIndex is always in the same place
    // for each property
    StreamIndex = ((PKSPROPERTY_VIDEOCOMPRESSION_S) pSPD->Property)->StreamIndex;

    ASSERT (StreamIndex < MAX_TESTCAP_STREAMS);

    // Verify the stream index is valid
    if (StreamIndex >= MAX_TESTCAP_STREAMS) {
        pSrb->Status = STATUS_INVALID_PARAMETER;
        return;
    }

    pStrmEx = (PSTREAMEX) pHwDevExt->pStrmEx[StreamIndex];

    // If the stream is not opened when this property set is used,
    // store the values in the HwDevExt

    if (pStrmEx) {
        pCompressionSettings = &pStrmEx->CompressionSettings;
    }
    else {
        pCompressionSettings = &pHwDevExt->CompressionSettings;
    }

    switch (Id) {

    case KSPROPERTY_VIDEOCOMPRESSION_KEYFRAME_RATE:
        {
            pCompressionSettings->CompressionKeyFrameRate = pS->Value;
        }
        break;
    
    case KSPROPERTY_VIDEOCOMPRESSION_PFRAMES_PER_KEYFRAME:
        {
            pCompressionSettings->CompressionPFramesPerKeyFrame = pS->Value;
        }
        break;
    
    case KSPROPERTY_VIDEOCOMPRESSION_QUALITY:
        {
            pCompressionSettings->CompressionQuality = pS->Value;
        }
        break;

    default:
        TRAP;
        break;
    }
}


// -------------------------------------------------------------------
// General entry point for all get/set adapter properties
// -------------------------------------------------------------------

/*
** AdapterSetProperty ()
**
**    Handles Set operations for all adapter properties.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID
STREAMAPI 
AdapterSetProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )

{
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;

	DbgLogInfo(("TestCap: Enter AdapterSetProperty\n"));	 

    if (IsEqualGUID(&PROPSETID_VIDCAP_VIDEOPROCAMP, &pSPD->Property->Set)) {
        AdapterSetVideoProcAmpProperty (pSrb);
    }
    else if (IsEqualGUID(&PROPSETID_VIDCAP_CAMERACONTROL, &pSPD->Property->Set)) {
        AdapterSetCameraControlProperty (pSrb);
    }
    else if (IsEqualGUID(&PROPSETID_VIDCAP_VIDEOCONTROL, &pSPD->Property->Set)) {
        AdapterSetVideoControlProperty (pSrb);
    }
    else if (IsEqualGUID (&PROPSETID_VIDCAP_VIDEOCOMPRESSION, &pSPD->Property->Set)) {
        AdapterSetVideoCompressionProperty (pSrb);
    }
    else {
        //
        // We should never get here
        //

        TRAP;
        pSrb->Status = STATUS_NOT_IMPLEMENTED;
    }
}

/*
** AdapterGetProperty ()
**
**    Handles Get operations for all adapter properties.
**
** Arguments:
**
**      pSRB -
**          Pointer to the HW_STREAM_REQUEST_BLOCK 
**
** Returns:
**
** Side Effects:  none
*/

VOID
STREAMAPI 
AdapterGetProperty(
    PHW_STREAM_REQUEST_BLOCK pSrb
    )

{
    PSTREAM_PROPERTY_DESCRIPTOR pSPD = pSrb->CommandData.PropertyInfo;

	DbgLogInfo(("TestCap: Enter AdapterGetProperty\n"));	 

    if (IsEqualGUID(&PROPSETID_VIDCAP_VIDEOPROCAMP, &pSPD->Property->Set)) {
        AdapterGetVideoProcAmpProperty (pSrb);
    }
    else if (IsEqualGUID(&PROPSETID_VIDCAP_CAMERACONTROL, &pSPD->Property->Set)) {
        AdapterGetCameraControlProperty (pSrb);
    }
    else if (IsEqualGUID(&PROPSETID_VIDCAP_VIDEOCONTROL, &pSPD->Property->Set)) {
        AdapterGetVideoControlProperty (pSrb);
    }
    else if (IsEqualGUID (&PROPSETID_VIDCAP_VIDEOCOMPRESSION, &pSPD->Property->Set)) {
        AdapterGetVideoCompressionProperty (pSrb);
    }
    else {
        //
        // We should never get here
        //

        TRAP;
        pSrb->Status = STATUS_NOT_IMPLEMENTED;
    }
}

