/*********************************************************
VIZIC TECHNOLOGIES. COPYRIGHT 2020.
THE DATASHEETS, SOFTWARE AND LIBRARIES ARE PROVIDED "AS IS." 
VIZIC EXPRESSLY DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
OR NONINFRINGEMENT. IN NO EVENT SHALL VIZIC BE LIABLE FOR 
ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF 
PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, 
ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO 
ANY DEFENCE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
OR OTHER SIMILAR COSTS.
*********************************************************/

/********************************************************
 ARDUINO SMARTGPU2 LIBRARY VERSION V1.0
 - Library supports the SmartWAV2 via any TX and RX pin using software serial(default commented #define SW_USE_HARDWARESERIAL in SMARTWAV2.h)
 - Library supports SmartWAV2 via SmartSHIELD board by uncommenting #define SW_USE_HARDWARESERIAL in SMARTWAV2.h
 IMPORTANT : This library was created for the Arduino 1.8.9 Software IDE or newer
********************************************************/

#ifndef SMARTWAV2_h
#define SMARTWAV2_h

#include <inttypes.h>
#include <SoftwareSerial.h>

/********************************************************
 GENERAL USER DEFINITIONS - SOFTWARE OR HARDWARE SERIAL DEFINITIONS
********************************************************/
//Uncomment next line to allow SMARTWAV2 communicate via hardware serial ports
//#define SW_USE_HARDWARESERIAL

//IFs
#ifdef SW_USE_HARDWARESERIAL    //If the above line SW_USE_HARDWARESERIAL is uncommented:
	#define SW_SERIALPORT Serial  //Use hardware serial(modify to Serial, Serial2, Serial3, etc)
#else                           //USE_SOFTWARESERIAL, the next defined TX_PIN and RX_PIN will be used for SmartWAV2 communication
	#define SW_SERIALPORT softSerial //Use software serial(don't modify this define)
	//TX and RX pin definitions: those are valid only if SW_USE_HARDWARESERIAL is NOT defined!
	#define SW_TX_PIN     2       //Software serial UART Transmit pin - Connect this pin to SMARTWAV2 RX pin
	#define SW_RX_PIN     3       //Software serial UART Receive pin - Connect this pin to SMARTWAV2 TX pin
#endif

//Pin to reset smartWAV2 definition
#define SW_RESET_PIN    5       //Connect this arduino pin number to the SMARTWAV2 reset pin


/*******************************************************/
/****************END OF USER MODIFABLE******************/
/**************DON'T MODIFY DOWN FROM HERE**************/
/*******************************************************/

#define ARDUINOREFERENCE  DEFAULT   //Define the AREF pin - Internal Analog reference as DEFAULT

#define SW_CH0           0         //Channel 0 definition
#define SW_CH1           1         //Channel 1 definition
#define SW_CH2           2         //Channel 2 definition
#define SW_CH3           3         //Channel 3 definition
#define SW_CH4           4         //Channel 4 definition
#define SW_CH5           5         //Channel 5 definition
#define SW_CH6           6         //Channel 6 definition
#define SW_CH7           7         //Channel 7 definition
#define SW_CH8           8         //Channel 8 definition
#define SW_CH9           9         //Channel 9 definition
#define SW_CH10         10         //Channel 10 definition
#define SW_CH11         11         //Channel 11 definition
#define SW_CH12         12         //Channel 12 definition
#define SW_CH13         13         //Channel 13 definition
#define SW_CHALL        0xFF       //All Audio channels definition

//General command reply enum
typedef enum {
	SW_R_OK   = 'O',  //Command successfully executed
	SW_R_FAIL = 'F'   //Command Fail
} SW_REPLY;           //to get SmartWAV2 command ACK/NAK states

typedef enum {
	SW_READONLY = 1, //1
	SW_WRITEONLY,    //2
	SW_READWRITE     //3
} SW_OPENMODE;       //to set the file access open mode

//SmartWAV2 FAT functions replies
typedef enum {
	SW_F_OK = 0,				/* (0) Succeeded */
	SW_F_DISK_ERR,				/* (1) A hard error occurred in the low level disk I/O layer */
	SW_F_INT_ERR,				/* (2) Assertion failed */
	SW_F_NOT_READY,			/* (3) The physical drive cannot work */
	SW_F_NO_FILE,				/* (4) Could not find the file */
	SW_F_NO_PATH,				/* (5) Could not find the path */
	SW_F_INVALID_NAME,			/* (6) The path name format is invalid */
	SW_F_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	SW_F_EXIST,				/* (8) Access denied due to prohibited access */
	SW_F_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	SW_F_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	SW_F_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	SW_F_NOT_ENABLED,			/* (12) The volume has no work area */
	SW_F_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	SW_F_MKFS_ABORTED,			/* (14) The f_mkfs() aborted due to any parameter error */
	SW_F_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	SW_F_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	SW_F_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	SW_F_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
	SW_F_INVALID_PARAMETER		/* (19) Given parameter is invalid */
} SW_FILERESULT;               //Gets all FAT functions related responses

//Recommended(but not limited to) Arduino-SmartWAV2 Baud rate definitions
typedef enum{
	SW_BAUD0 = 9600,
	SW_BAUD1 = 19200,
	SW_BAUD2 = 57600,
	SW_BAUD3 = 115200,
	SW_BAUD4 = 256000
} SW_BAUDRATE;

//Files date and time struct
typedef struct {
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char day;
	unsigned char month;
	unsigned int  year;	
} SW_TIMESTAMP;    //to create a date-time info struct

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
class SMARTWAV2{
	
public:
/****************************************************************/
//Arduino exclusive Functions
/****************************************************************/

    //Constructor
	SMARTWAV2(void);

	
/****************************************************************/
//Master Functions
/****************************************************************/

	SW_REPLY begin(void); //Initialize SmartWAV2
	
	SW_REPLY reset(void); //Reset system
	
	SW_REPLY sleep(void); //Set system to sleep

	SW_REPLY standby(void); //Set system to power down standby

	SW_REPLY wake(void); //Wake system from sleep
	
	SW_REPLY masterGain(unsigned char gain); //Set system master gain

	SW_REPLY setDefaultMasterGain(unsigned char gain); //Save/set power-on startup master gain to flash
	
	SW_REPLY masterSampleRate(unsigned long sampleRate); //Set system master sample rate(per second)

	SW_REPLY keepSampleRate(unsigned char keep); //Keep or update master sample rate

	SW_REPLY baudChange(unsigned long baudrate); //Set system baudrate
	
	SW_REPLY setDefaultVoiceMsg(unsigned char setReset); //Save/set power-on voice messages for stand alone modes only
	
	SW_REPLY setDefaultMIDIRelease(unsigned char time); //Save/set power-on MIDI mode release time(0 - 127), for MIDI mode only

/****************************************************************/
//Channel Functions
/****************************************************************/	

	SW_REPLY mount(char trackName[], unsigned char channel); //Mounts a track

	SW_REPLY play(unsigned char channel); //Play/resume
	
	SW_REPLY pause(unsigned char channel); //Pause
	
	SW_REPLY playPause(unsigned char pausePlay, unsigned char channel); //Play/Pause
	
	SW_REPLY playImme(char trackName[], unsigned char channel); //Plays immediately a track	

	SW_REPLY volume(unsigned char volume, unsigned char channel); //Sets the volume
	
	SW_REPLY panning(char pan, unsigned char channel); //Sets the stereo panning

	SW_REPLY rewindPlay(unsigned char channel); //Rewinds and play

	SW_REPLY advanceTo(unsigned long millisecond, unsigned char channel); //Goes to received millisecond point

	SW_REPLY setABPoints(unsigned long A, unsigned long B, unsigned char channel); //Set A and B points in millisecond

	SW_REPLY loop(unsigned char enDisloop, unsigned char channel); //Enable/Disable loop from point A to B points

	SW_REPLY stop(unsigned char channel); //Stop and unmount track

	SW_REPLY getMount(unsigned char* mount, unsigned char channel); //Obtain current mount state

	SW_REPLY getPlayPause(unsigned char* play, unsigned char channel); //Obtain current play state

	SW_REPLY getVolume(unsigned char* volume, unsigned char channel); //Obtain current volume

	SW_REPLY getPanning(char* pan, unsigned char channel); //Obtain current panning

	SW_REPLY getLoop(unsigned char* loop, unsigned char channel); //Obtain curren loop state

	SW_REPLY getName(char trackName[], unsigned char channel); //Obtain mounted track name
	
	SW_REPLY getCurrentPoint(unsigned long* millisecond, unsigned char channel); //Obtain current point in millisecond

	SW_REPLY getABPoints(unsigned long* A, unsigned long* B, unsigned char channel); //Obtain A and B points in millisecond

	SW_REPLY getRemainTime(unsigned long* remainMilliseconds, unsigned char channel); //Obtain remaining track millisecond

	SW_REPLY getSizeTime(unsigned long* sizeMilliseconds, unsigned char channel); //Obtain track total size in millisecond

	SW_REPLY getMonoStereo(unsigned char* ch, unsigned char channel); //Obtain track channels: 1 for mono, 2 for stereo

	SW_REPLY getBitsPerSample(unsigned char* bps, unsigned char channel); //Obtain track bits per sample

	SW_REPLY getSampleRate(unsigned long* sampleRate, unsigned char channel); //Obtain track sample rate(per second)

/****************************************************************/
//MASTER FAT/FAT32 Data Management Functions
//Those next Functions return FAT ACK/NAK List execution status in the SW_FILERESULT parameter
/****************************************************************/

	SW_REPLY SDFmount(unsigned char mount, SW_FILERESULT* result); //Mounts or unmounts SD card FAT system
	
	SW_REPLY SDFgetListDirs(unsigned int* dirs, SW_FILERESULT* result); //get number of dirs inside the current directory path

	SW_REPLY SDFgetListFiles(unsigned int* files, SW_FILERESULT* result); //get number of files inside the current directory path
	
	SW_REPLY SDFgetDirItemName(unsigned int dirNo, char dirName[], SW_FILERESULT* result); //Searches for the "itemNumber" dir name

	SW_REPLY SDFgetFileItemName(unsigned int fileNo, char fileName[], SW_FILERESULT* result); //Searches for the "itemNumber" file name
	
	SW_REPLY SDFopenDir(char dirName[], SW_FILERESULT* result); //opens an existing directory inside the current directory path

	SW_REPLY SDFgetPath(char path[], SW_FILERESULT* result); //obtains current dir path

	SW_REPLY SDFnewDir(char dirName[], SW_FILERESULT* result); //creates a new directory

	SW_REPLY SDFnewFile(char fileName[], SW_FILERESULT* result); //create a new File

	SW_REPLY SDFeraseDir(char dirName[], SW_FILERESULT* result); //Erases an existing directory
	
	SW_REPLY SDFeraseFile(char fileName[], SW_FILERESULT* result); //Erases an existing file

	SW_REPLY SDFgetFileSize(char fileName[], unsigned long* size, SW_FILERESULT* result); //Get size of an existing file

	SW_REPLY SDFgetFATAttribute(char fileName[], unsigned char*attribute, SW_FILERESULT* result); //Get FAT attribute of file

	SW_REPLY SDFgetDateTime(SW_TIMESTAMP* timeDate, char fileName[], SW_FILERESULT* result); //Get date and time of an existing file
	
	SW_REPLY SDFsetDateTime(SW_TIMESTAMP* timeDate, char fileName[], SW_FILERESULT* result); //Set date and time to an existing file

	SW_REPLY SDFrenameDir(char oldName[], char newName[], SW_FILERESULT* result); //renames or/and moves an existing directory

	SW_REPLY SDFrenameFile(char oldName[], char newName[], SW_FILERESULT* result); //renames or/and moves an existing file

	SW_REPLY SDFgetFreeTotalSpace(unsigned long* free,unsigned long* total, SW_FILERESULT* result); //Get free and total space in KBytes of the microSD card drive

/****************************************************************/
//WORKSPACE FAT/FAT32 Data Management Functions - THOSE FUNCTIONS NEED A WORKSPACE#
//Those next Functions return FAT ACK/NAK List execution status in the SW_FILERESULT parameter
/****************************************************************/

	SW_REPLY SDFopenFile(char fileName[], SW_OPENMODE mode, unsigned char workspace, SW_FILERESULT* result); //Opens an existing file in READONLY, WRITEONLY or READWRITE mode 

	SW_REPLY SDFreadFile(char buffer[], unsigned int btr, unsigned int* srb, unsigned char workspace, SW_FILERESULT* result); //Bytes to Read, Succesfully Read Bytes
 
	SW_REPLY SDFwriteFile(char buffer[], unsigned int btw, unsigned int* swb, unsigned char workspace, SW_FILERESULT* result); //Bytes to Write, Succesfully Written Bytes

	SW_REPLY SDFgetPointer(unsigned long* position, unsigned char workspace, SW_FILERESULT* result); //Get file pointer of file

	SW_REPLY SDFsetPointer(unsigned long position, unsigned char workspace, SW_FILERESULT* result); //Set/move file pointer of file

	SW_REPLY SDFsaveFile(unsigned char workspace, SW_FILERESULT* result); //Sync/save file

	SW_REPLY SDFtruncateFile(unsigned char workspace, SW_FILERESULT* result);  //Truncates the file size to the current file read/write pointer position

	SW_REPLY SDFtestErrorFile(unsigned char workspace, SW_FILERESULT* result);  //Test for an error on file

	SW_REPLY SDFtestEndFile(unsigned char workspace, SW_FILERESULT* result);  //Test for end of file

	SW_REPLY SDFcloseFile(unsigned char workspace, SW_FILERESULT* result); //Close and ave file

private:

};

#endif
