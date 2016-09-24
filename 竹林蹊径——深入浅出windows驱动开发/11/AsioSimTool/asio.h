
#ifndef __ASIO_H
#define __ASIO_H

// force 4 byte alignment
#if defined(_MSC_VER) && !defined(__MWERKS__) 
#pragma pack(push,4)
#elif PRAGMA_ALIGN_SUPPORTED
#pragma options align = native
#endif


//- - - - - - - - - - - - - - - - - - - - - - - - -
// Type definitions
//- - - - - - - - - - - - - - - - - - - - - - - - -

// number of samples data type is 64 bit integer
#if NATIVE_INT64
	typedef long long int ASIOSamples;
#else
	typedef struct ASIOSamples {
		unsigned long hi;
		unsigned long lo;
	} ASIOSamples;
#endif

// Timestamp data type is 64 bit integer,
// Time format is Nanoseconds.
#if NATIVE_INT64
	typedef long long int ASIOTimeStamp ;
#else
	typedef struct ASIOTimeStamp {
		unsigned long hi;
		unsigned long lo;
	} ASIOTimeStamp;
#endif

// Samplerates are expressed in IEEE 754 64 bit double float,
// native format as host computer
#if IEEE754_64FLOAT
	typedef double ASIOSampleRate;
#else
	typedef struct ASIOSampleRate {
		char ieee[8];
	} ASIOSampleRate;
#endif

// Boolean values are expressed as long
typedef long ASIOBool;
enum {
	ASIOFalse = 0,
	ASIOTrue = 1
};

// Sample Types are expressed as long
typedef long ASIOSampleType;
enum {
	ASIOSTInt16MSB   = 0,
	ASIOSTInt24MSB   = 1,		// used for 20 bits as well
	ASIOSTInt32MSB   = 2,
	ASIOSTFloat32MSB = 3,		// IEEE 754 32 bit float
	ASIOSTFloat64MSB = 4,		// IEEE 754 64 bit double float

	// these are used for 32 bit data buffer, with different alignment of the data inside
	// 32 bit PCI bus systems can be more easily used with these

	ASIOSTInt32MSB16 = 8,		// 32 bit data with 18 bit alignment
	ASIOSTInt32MSB18 = 9,		// 32 bit data with 18 bit alignment
	ASIOSTInt32MSB20 = 10,		// 32 bit data with 20 bit alignment
	ASIOSTInt32MSB24 = 11,		// 32 bit data with 24 bit alignment
	ASIOSTInt16LSB   = 16,
	ASIOSTInt24LSB   = 17,		// used for 20 bits as well
	ASIOSTInt32LSB   = 18,
	ASIOSTFloat32LSB = 19,		// IEEE 754 32 bit float, as found on Intel x86 architecture
	ASIOSTFloat64LSB = 20, 		// IEEE 754 64 bit double float, as found on Intel x86 architecture

	// these are used for 32 bit data buffer, with different alignment of the data inside
	// 32 bit PCI bus systems can more easily used with these

	ASIOSTInt32LSB16 = 24,		// 32 bit data with 18 bit alignment
	ASIOSTInt32LSB18 = 25,		// 32 bit data with 18 bit alignment
	ASIOSTInt32LSB20 = 26,		// 32 bit data with 20 bit alignment
	ASIOSTInt32LSB24 = 27		// 32 bit data with 24 bit alignment
};


//- - - - - - - - - - - - - - - - - - - - - - - - -
// Error codes
//- - - - - - - - - - - - - - - - - - - - - - - - -
typedef long ASIOError;
enum {
	ASE_OK = 0,             // This value will be returned whenever the call succeeded
	ASE_SUCCESS = 0x3f4847a0,	// unique success return value for ASIOFuture calls
	ASE_NotPresent = -1000, // hardware input or output is not present or available
	ASE_HWMalfunction,      // hardware is malfunctioning (can be returned by any ASIO function)
	ASE_InvalidParameter,   // input parameter invalid
	ASE_InvalidMode,        // hardware is in a bad mode or used in a bad mode
	ASE_SPNotAdvancing,     // hardware is not running when sample position is inquired
	ASE_NoClock,            // sample clock or rate cannot be determined or is not present
	ASE_NoMemory            // not enough memory for completing the request
};


//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//- - - - - - - - - - - - - - - - - - - - - - - - -
// Time Info support
//- - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct ASIOTimeCode
{       
	double          speed;                  // speed relation (fraction of nominal speed)
	                                        // optional; set to 0. or 1. if not supported
	ASIOSamples     timeCodeSamples;        // time in samples
	unsigned long   flags;                  // some information flags (see below)
	char future[64];
} ASIOTimeCode;

typedef enum ASIOTimeCodeFlags
{
	kTcValid                = 1,
	kTcRunning              = 1 << 1,
	kTcReverse              = 1 << 2,
	kTcOnspeed              = 1 << 3,
	kTcStill                = 1 << 4,
	kTcSpeedValid           = 1 << 8
}  ASIOTimeCodeFlags;

typedef struct AsioTimeInfo
{
	double          speed;                  // absolute speed (1. = nominal)
	ASIOTimeStamp   systemTime;             // system time related to samplePosition, in nanoseconds
	                                        // on mac, must be derived from Microseconds() (not UpTime()!)
	                                        // on windows, must be derived from timeGetTime()
	ASIOSamples     samplePosition;
	ASIOSampleRate  sampleRate;             // current rate
	unsigned long flags;                    // (see below)
	char reserved[12];
} AsioTimeInfo;

typedef enum AsioTimeInfoFlags
{
	kSystemTimeValid        = 1,            // must always be valid
	kSamplePositionValid    = 1 << 1,       // must always be valid
	kSampleRateValid        = 1 << 2,
	kSpeedValid             = 1 << 3,
	kSampleRateChanged      = 1 << 4,
	kClockSourceChanged     = 1 << 5
} AsioTimeInfoFlags;

typedef struct ASIOTime                          // both input/output
{
	long reserved[4];                       // must be 0
	struct AsioTimeInfo     timeInfo;       // required
	struct ASIOTimeCode     timeCode;       // optional, evaluated if (timeCode.flags & kTcValid)
} ASIOTime;


//- - - - - - - - - - - - - - - - - - - - - - - - -
// application's audio stream handler callbacks
//- - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct ASIOCallbacks
{
	void (*bufferSwitch) (long doubleBufferIndex, ASIOBool directProcess);
	void (*sampleRateDidChange) (ASIOSampleRate sRate);
	long (*asioMessage) (long selector, long value, void* message, double* opt);
	ASIOTime* (*bufferSwitchTimeInfo) (ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);
} ASIOCallbacks;



// asioMessage selectors
enum
{
	kAsioSelectorSupported = 1,	// selector in <value>, returns 1L if supported,
								// 0 otherwise
    kAsioEngineVersion,			// returns engine (host) asio implementation version,
								// 2 or higher
	kAsioResetRequest,			// request driver reset. if accepted, this
								// will close the driver (ASIO_Exit() ) and
								// re-open it again (ASIO_Init() etc). some
								// drivers need to reconfigure for instance
								// when the sample rate changes, or some basic
								// changes have been made in ASIO_ControlPanel().
								// returns 1L; note the request is merely passed
								// to the application, there is no way to determine
								// if it gets accepted at this time (but it usually
								// will be).
	kAsioBufferSizeChange,		// not yet supported, will currently always return 0L.
								// for now, use kAsioResetRequest instead.
								// once implemented, the new buffer size is expected
								// in <value>, and on success returns 1L
	kAsioResyncRequest,			// the driver went out of sync, such that
								// the timestamp is no longer valid. this
								// is a request to re-start the engine and
								// slave devices (sequencer). returns 1 for ok,
								// 0 if not supported.
	kAsioLatenciesChanged, 		// the drivers latencies have changed. The engine
								// will refetch the latencies.
	kAsioSupportsTimeInfo,		// if host returns true here, it will expect the
								// callback bufferSwitchTimeInfo to be called instead
								// of bufferSwitch
	kAsioSupportsTimeCode,		// supports time code reading/writing
	kAsioSupportsInputMonitor,	// supports input monitoring
	kAsioNumMessageSelectors
};


//- - - - - - - - - - - - - - - - - - - - - - - - -
// (De-)Construction
//- - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct ASIODriverInfo
{
	long asioVersion;		// currently, 2
	long driverVersion;		// driver specific
	char name[32];
	char errorMessage[124];
	void *sysRef;			// on input: system reference
							// (Windows: application main window handle, Mac & SGI: 0)
} ASIODriverInfo;


typedef struct ASIOClockSource
{
	long index;					// as used for ASIOSetClockSource()
	long associatedChannel;		// for instance, S/PDIF or AES/EBU
	long associatedGroup;		// see channel groups (ASIOGetChannelInfo())
	ASIOBool isCurrentSource;	// ASIOTrue if this is the current clock source
	char name[32];				// for user selection
} ASIOClockSource;


typedef struct ASIOChannelInfo
{
	long channel;			// on input, channel index
	ASIOBool isInput;		// on input
	ASIOBool isActive;		// on exit
	long channelGroup;		// dto
	ASIOSampleType type;	// dto
	char name[32];			// dto
} ASIOChannelInfo;

typedef struct ASIOBufferInfo
{
	ASIOBool isInput;			// on input:  ASIOTrue: input, else output
	long channelNum;			// on input:  channel index
	void *buffers[2];			// on output: double buffer addresses
} ASIOBufferInfo;

enum
{
	kAsioEnableTimeCodeRead = 1,	// no arguments
	kAsioDisableTimeCodeRead,		// no arguments
	kAsioSetInputMonitor,			// ASIOInputMonitor* in params
	kAsioTransport,					// ASIOTransportParameters* in params
	kAsioSetInputGain,				// ASIOChannelControls* in params, apply gain
	kAsioGetInputMeter,				// ASIOChannelControls* in params, fill meter
	kAsioSetOutputGain,				// ASIOChannelControls* in params, apply gain
	kAsioGetOutputMeter,			// ASIOChannelControls* in params, fill meter
	kAsioCanInputMonitor,			// no arguments for kAsioCanXXX selectors
	kAsioCanTimeInfo,
	kAsioCanTimeCode,
	kAsioCanTransport,
	kAsioCanInputGain,
	kAsioCanInputMeter,
	kAsioCanOutputGain,
	kAsioCanOutputMeter,
	kSpinCycleHandshake
};

typedef struct ASIOInputMonitor
{
	long input;		// this input was set to monitor (or off), -1: all
	long output;	// suggested output for monitoring the input (if so)
	long gain;		// suggested gain, ranging 0 - 0x7fffffffL (-inf to +12 dB)
	ASIOBool state;	// ASIOTrue => on, ASIOFalse => off
	long pan;		// suggested pan, 0 => all left, 0x7fffffff => right
} ASIOInputMonitor;

typedef struct ASIOChannelControls
{
	long channel;			// on input, channel index
	ASIOBool isInput;		// on input
	long gain;				// on input,  ranges 0 thru 0x7fffffff
	long meter;				// on return, ranges 0 thru 0x7fffffff
	char future[32];
} ASIOChannelControls;

typedef struct ASIOTransportParameters
{
	long command;		// see enum below
	ASIOSamples samplePosition;
	long track;
	long trackSwitches[16];		// 512 tracks on/off
	char future[64];
} ASIOTransportParameters;

enum
{
	kTransStart = 1,
	kTransStop,
	kTransLocate,		// to samplePosition
	kTransPunchIn,
	kTransPunchOut,
	kTransArmOn,		// track
	kTransArmOff,		// track
	kTransMonitorOn,	// track
	kTransMonitorOff,	// track
	kTransArm,			// trackSwitches
	kTransMonitor		// trackSwitches
};

// restore old alignment
#if defined(_MSC_VER) && !defined(__MWERKS__) 
#pragma pack(pop)
#elif PRAGMA_ALIGN_SUPPORTED
#pragma options align = reset
#endif

#endif
