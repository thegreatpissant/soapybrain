#include "OVR_CAPI.h"

#include <iostream>
#include <unistd.h>

void odDumpInfo( ovrHmdDesc &hmdDesc )
{
    std::cout << "-------------------------------\n";
    std::cout << "-- Oculus Device INFORMATION --\n";
    std::cout << "Type: ";
    switch ( hmdDesc.Type )
    {
    case ovrHmd_DK1:
        std::cout << "DK1.\n";
        break;
    case ovrHmd_DK2:
        std::cout << "DK2.\n";
        break;
    default:
        std::cout << "Unknown device.\n";
    };
    std::cout << "Product Name: ";
    std::cout << hmdDesc.ProductName << std::endl;

    std::cout << "Manufacturer Name: ";
    std::cout << hmdDesc.Manufacturer << std::endl;

    std::cout << "HMD capability bits (hex): ";
    std::cout << std::hex << hmdDesc.HmdCaps << std::endl;

    std::cout << "Sensor capability bits (hex): ";
    std::cout << std::hex << hmdDesc.SensorCaps << std::endl;

    std::cout << "Distortion capability: ";
    std::cout << std::hex << hmdDesc.DistortionCaps << std::endl;

    std::cout << "Resolution (Both eyes in pixels w,h): ";
    std::cout << hmdDesc.Resolution.w << ", " << hmdDesc.Resolution.h << std::endl;

    std::cout << "Monitor Window on screen (x,y): ";
    std::cout << hmdDesc.WindowsPos.x << ", " << hmdDesc.WindowsPos.y << std::endl;

    for ( auto i = 0; i < ovrEye_Count; ++i )
    {
        std::cout << "Eye # " << i + 1 << " " << std::endl;

        switch ( hmdDesc.EyeRenderOrder[i] )
        {
        case ovrEye_Left:
            std::cout << "Left Eye" << std::endl;
            break;
        case ovrEye_Right:
            std::cout << "Right Eye" << std::endl;
            break;
        default:
            std::cout << "Unknown Eye" << std::endl;
        }

        std::cout << "Default Recomended field of view:\n";
        std::cout << "\t UpTan: " << hmdDesc.DefaultEyeFov[i].UpTan << std::endl;
        std::cout << "\t DownTan: " << hmdDesc.DefaultEyeFov[i].DownTan << std::endl;
        std::cout << "\t LeftTan: " << hmdDesc.DefaultEyeFov[i].LeftTan << std::endl;
        std::cout << "\t RightTan: " << hmdDesc.DefaultEyeFov[i].RightTan << std::endl;

        std::cout << "Max practical optical field:\n";
        std::cout << "\t UpTan: " << hmdDesc.MaxEyeFov[i].UpTan << std::endl;
        std::cout << "\t DownTan: " << hmdDesc.MaxEyeFov[i].DownTan << std::endl;
        std::cout << "\t LeftTan: " << hmdDesc.MaxEyeFov[i].LeftTan << std::endl;
        std::cout << "\t RightTan: " << hmdDesc.MaxEyeFov[i].RightTan << std::endl;
    }
    std::cout << "Display Device Name: ";
    std::cout << hmdDesc.DisplayDeviceName << std::endl;

    std::cout << "-------------------------\n";
}

void odTestPollSensorFusion( ovrHmd &hmd, unsigned int supportedSensorCaps,
                             unsigned int requiredSensorCaps )
{
    if ( false == ovrHmd_StartSensor( hmd, supportedSensorCaps, requiredSensorCaps ) )
    {
        std::cerr << "Required Sensor Capabilities not available.\n";
        return;
    }
    int queries = 5000;
    int frameIndex = 0;
    std::cout << "Quering sensor " << queries << " times" << std::endl;
    for ( auto i = 0; i < queries; ++i )
    {
        ovrFrameTiming frameTiming = ovrHmd_BeginFrameTiming (hmd, frameIndex);
        ovrSensorState ss = ovrHmd_GetSensorState( hmd, frameTiming.ScanoutMidpointSeconds );
        //SetFrameHMDData (frameIndex, ss.Predicted.Pose);
        if ( ss.StatusFlags & ( ovrStatus_OrientationTracked ) )
        {
            ovrPosef pose = ss.Predicted.Pose;
            float yaw, eyePitch, eyeRoll;
            //pose.Orientation.GetEulerAngles< Axis_Y, Axis_X, Axis_Z> (&yaw, &eyePitch, &eyeRoll);
            yaw = pose.Orientation.y;
            eyePitch = pose.Orientation.x;
            eyeRoll = pose.Orientation.z;
            std::cout << "yaw: " << yaw << " eyePitch: " << eyePitch << " eyeRoll: " << eyeRoll << std::endl;
        }
        else
        {
            std::cerr << "Error getting sensor information.\n";
        }
        sleep (1);
        ovrHmd_EndFrameTiming(hmd);
    }
}

int main( )
{
    ovr_Initialize( );
    int num_devices = ovrHmd_Detect( );
    if ( num_devices > 0 )
    {
        std::cout << "Found " << num_devices << std::endl;
    }
    else
        std::cout << "No devices found." << std::endl;
    ovrHmdDesc hmdDesc[num_devices];
    ovrHmd hmd[num_devices];


    for ( auto i = 0; i < num_devices; ++i )
    {
        hmd[i] = ovrHmd_Create( i );
        if ( hmd[i] )
        {
            ovrHmd_GetDesc( hmd[i], &hmdDesc[i] );
            odDumpInfo( hmdDesc[i] );
            //odTestPollSensorFusion( hmd[i], hmdDesc[i].SensorCaps, ovrSensorCap_Orientation /* |ovrSensorCap_Position*/ );
            odTestPollSensorFusion( hmd[i], ovrSensorCap_Orientation,  ovrSensorCap_Orientation /* |ovrSensorCap_Position*/ );
        }
        else
        {
            std::cerr << " Could not enumerate device #" << i << std::endl;
        }
    }

    for ( auto i = 0; i < num_devices; ++i )
    {
        ovrHmd_Destroy( hmd[i] );
    }
    ovr_Shutdown( );

}
