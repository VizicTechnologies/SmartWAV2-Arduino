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

#include <Arduino.h>
#include "SMARTWAV2.h"

//Object creation to be used when software serial is needed (not used if SW_USE_HARDWARESERIAL is defined)
#ifndef SW_USE_HARDWARESERIAL	
	SoftwareSerial SW_SERIALPORT(SW_RX_PIN, SW_TX_PIN);
#endif	

/****************************************************************/
//Communication Functions(PLATFORM DEPENDENT) - MODIFY THE NEXT 
//FUNCTIONS TO FIT YOUR PLATFORM IF DIFFERENT THAN ARDUINO
//Note: SMARTWAV2 board default baud rate is 57600bps
/****************************************************************/
//Sends a single character through the serial port(USART)
static void putcharTX(char data){
	SW_SERIALPORT.write(data);
}

//returns the available received chars in the RX pin
static unsigned char availableRX(void){
	return SW_SERIALPORT.available();	
}

//Returns a single character obtained from the serial port(USART)
static char getcharRX(void){
	while(availableRX() == 0);
	return SW_SERIALPORT.read();
}

//Changes/Sets a new Baudrate to the Host processor
static void setBaud(unsigned long newBaud){
	SW_SERIALPORT.begin(newBaud);
}

//Performs a Hardware Reset on smartWAV2 RESET pin
static void resetSmartWAV2(void){    //Reset the SMARTWAV2 board
	digitalWrite(SW_RESET_PIN, LOW);   //Set the pin to GND to reset 
	delay(500);
	digitalWrite(SW_RESET_PIN, HIGH);  //Set the pin to 5v to end reset
	delay(500);	
}

//Arduino Exclusive function - Configures the Hardware
static void initBoard(void){         //configure the arduino board for SMARTWAV2 board support
	//analogReference(ARDUINOREFERENCE); //Set AREF pin Analog Reference (avoids damage to arduino boards if using shields)
	setBaud(57600);                    //Default Arduino-SMARTWAV2 communication Baudrate is 57600
	pinMode(SW_RESET_PIN, OUTPUT);     //Configure pin as output	
}
 

/****************************************************************/
/****************************************************************/ 
/****************************************************************/
/*SmartWAV2 Library exclusive Functions - DO NOT MODIFY DOWN FROM HERE*/
/****************************************************************/
/****************************************************************/
/****************************************************************/

/****************************************************************/
//Arduino exclusive Functions
/****************************************************************/
SMARTWAV2::SMARTWAV2(void){
}


/****************************************************************/
//Master Functions
/****************************************************************/

/****************************************************************/	
SW_REPLY SMARTWAV2::begin(void){ //Initialize SmartWAV2
	initBoard();
	resetSmartWAV2();
	//Send commands
	putcharTX('U'); //Start communication
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::reset(void){ //Reset system
	//This function performs a hardware reset
	begin();
	return SW_R_OK;
}

/****************************************************************/
SW_REPLY SMARTWAV2::sleep(void){ //Set system to sleep
	unsigned char reply;
	//Send commands
	putcharTX('Z');
	putcharTX(0x00); //sleep
	//Get reply
	reply = getcharRX();
	delay(100); //wait for system to enter sleep mode
	return (SW_REPLY)reply;
}

/****************************************************************/
SW_REPLY SMARTWAV2::wake(void){ //Wake system from sleep
	//Send commands
	putcharTX(0x00); //wake from sleep
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
//Warning, once the SmartWAV2 enter standby mode, the only way to wake is by perfoming a reset() call
SW_REPLY SMARTWAV2::standby(void){ //Set system to power down standby
	unsigned char reply;
	//Send commands
	putcharTX('Z');
	putcharTX(0x01); //standby/power off
	//Get reply
	reply = getcharRX();
	delay(100); //wait for system to enter standby mode
	return (SW_REPLY)reply;
}

/****************************************************************/
SW_REPLY SMARTWAV2::masterGain(unsigned char gain){ //Set system master gain
	//Send commands
	putcharTX('G');
	putcharTX(gain); //Gain
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::setDefaultMasterGain(unsigned char gain){ //Save/set power-on startup master gain to flash
	//Send commands
	putcharTX('H');
	putcharTX(gain); //Gain
	//Get reply
	return (SW_REPLY)getcharRX();	
}

/****************************************************************/
SW_REPLY SMARTWAV2::masterSampleRate(unsigned long sampleRate){ //Set system master sample rate(per second)
	//Send commands
	putcharTX('T');
	putcharTX(sampleRate>>24);
	putcharTX(sampleRate>>16);
	putcharTX(sampleRate>>8);
	putcharTX(sampleRate);
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::keepSampleRate(unsigned char keep){ //Keep or update master sample rate
	//Send commands
	putcharTX('K');
	putcharTX(keep); //Keep or update
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::baudChange(unsigned long baudrate){ //Set system baudrate
	//Send commands
	putcharTX('X');
	putcharTX(baudrate>>24);
	putcharTX(baudrate>>16);
	putcharTX(baudrate>>8);
	putcharTX(baudrate);
	//Get first reply
	if( (SW_REPLY)getcharRX() == SW_R_OK){ //If command accepted
		//change baud
		delay(100);
		setBaud(baudrate);
		delay(500);
		return (SW_REPLY)getcharRX();
	}else{                                  //Command fail
		return SW_R_FAIL;
	}
}

/****************************************************************/	
SW_REPLY setDefaultVoiceMsg(unsigned char setReset){ //Save/set power-on voice messages for stand alone modes only
	//Send commands
	putcharTX('J');
	putcharTX(setReset); //1 for set, 0 for reset
	//Get reply
	return (SW_REPLY)getcharRX();	
}

/****************************************************************/	
SW_REPLY setDefaultMIDIRelease(unsigned char time){ //Save/set power-on MIDI mode release time(0 - 127), for MIDI mode only
	//Send commands
	putcharTX('D');
	putcharTX(time); //MIDI release time(0 - 127)
	//Get reply
	return (SW_REPLY)getcharRX();
}


/****************************************************************/
//Channel Functions
/****************************************************************/

/****************************************************************/
SW_REPLY SMARTWAV2::mount(char trackName[], unsigned char channel){ //Mounts a track
	unsigned char i=0;
	//Send commands
	putcharTX('M');
	putcharTX(channel);
	while(1){
		putcharTX(trackName[i]);
		if(trackName[i] == 0x00) break;
		i++;
	}
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::play(unsigned char channel){ //Play/resume
	//Send commands
	putcharTX('P');
	putcharTX(channel);
	putcharTX(0x01); //Set play
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::pause(unsigned char channel){ //Pause
	//Send commands
	putcharTX('P');
	putcharTX(channel);
	putcharTX(0x00); //Clear play
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY playPause(unsigned char pausePlay, unsigned char channel){ //Play/Pause
	//Send commands
	putcharTX('P');
	putcharTX(channel);
	putcharTX(playPause); //Set/Reset play
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::playImme(char trackName[], unsigned char channel){ //Plays immediately a track
	unsigned char i=0;
	//Send commands
	putcharTX('I');
	putcharTX(channel);
	while(1){
		putcharTX(trackName[i]);
		if(trackName[i] == 0x00) break;
		i++;
	}
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::volume(unsigned char volume, unsigned char channel){ //Sets the volume
	//Send commands
	putcharTX('V');
	putcharTX(channel);
	putcharTX(volume); //Clear pause
	//Get reply
	return (SW_REPLY)getcharRX();
}
	
/****************************************************************/
SW_REPLY SMARTWAV2::panning(char pan, unsigned char channel){ //Sets the stereo panning
	//Send commands
	putcharTX('A');
	putcharTX(channel);
	putcharTX(pan); //Clear pause
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::rewindPlay(unsigned char channel){ //Rewinds and play
	//Send commands
	putcharTX('R');
	putcharTX(channel);
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::advanceTo(unsigned long millisecond, unsigned char channel){ //Goes to received millisecond point
	//Send commands
	putcharTX('E');
	putcharTX(channel);
	putcharTX(millisecond>>24);
	putcharTX(millisecond>>16);
	putcharTX(millisecond>>8);
	putcharTX(millisecond);
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::setABPoints(unsigned long A, unsigned long B, unsigned char channel){  //Set A and B points in millisecond
	//Send commands
	putcharTX('B');
	putcharTX(channel);
	putcharTX(A>>24);
	putcharTX(A>>16);
	putcharTX(A>>8);
	putcharTX(A);
	putcharTX(B>>24);
	putcharTX(B>>16);
	putcharTX(B>>8);
	putcharTX(B);
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::loop(unsigned char enDisLoop, unsigned char channel){ //Enable/Disable loop from point A to B points
	//Send commands
	putcharTX('L');
	putcharTX(channel);
	putcharTX(enDisLoop); //Enable/Disable loop
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::stop(unsigned char channel){ //Stop and unmount track
	//Send commands
	putcharTX('S');
	putcharTX(channel);
	//Get reply
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getMount(unsigned char* mount, unsigned char channel){ //Obtain current mount state
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('M');
	//Get reply
	*mount = getcharRX();
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getPlayPause(unsigned char* play, unsigned char channel){ //Obtain current play state
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('P');
	//Get reply
	*play = getcharRX();
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getVolume(unsigned char* volume, unsigned char channel){ //Obtain current volume
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('V');
	//Get reply
	*volume = getcharRX();
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getPanning(char* pan, unsigned char channel){ //Obtain current panning
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('A');
	//Get reply
	*pan = getcharRX();
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getLoop(unsigned char* loop, unsigned char channel){ //Obtain curren loop state
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('L');
	//Get reply
	*loop = getcharRX();
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getName(char trackName[], unsigned char channel){ //Obtain mounted track name
	unsigned char i=0;
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('N');
	//Get reply
	while(1){
		trackName[i]=getcharRX();
		if(trackName[i] == 0x00) break;
		i++;
	}
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getCurrentPoint(unsigned long* millisecond, unsigned char channel){ //Obtain current point in millisecond
	unsigned long cSec=0;
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('O');
	//Get reply
	((unsigned char *) &cSec)[3]=getcharRX();
	((unsigned char *) &cSec)[2]=getcharRX();
	((unsigned char *) &cSec)[1]=getcharRX();
	((unsigned char *) &cSec)[0]=getcharRX();
	*millisecond = cSec;
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getABPoints(unsigned long* A, unsigned long* B, unsigned char channel){ //Obtain A and B points in millisecond
	unsigned long Aaux=0, Baux=0;
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('B');
	//Get reply
	((unsigned char *) &Aaux)[3]=getcharRX();
	((unsigned char *) &Aaux)[2]=getcharRX();
	((unsigned char *) &Aaux)[1]=getcharRX();
	((unsigned char *) &Aaux)[0]=getcharRX();
	((unsigned char *) &Baux)[3]=getcharRX();
	((unsigned char *) &Baux)[2]=getcharRX();
	((unsigned char *) &Baux)[1]=getcharRX();
	((unsigned char *) &Baux)[0]=getcharRX();
	*A = Aaux;
	*B   = Baux;
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getRemainTime(unsigned long* remainMilliseconds, unsigned char channel){ //Obtain remaining track millisecond
	unsigned long cSec=0;
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('R');
	//Get reply
	((unsigned char *) &cSec)[3]=getcharRX();
	((unsigned char *) &cSec)[2]=getcharRX();
	((unsigned char *) &cSec)[1]=getcharRX();
	((unsigned char *) &cSec)[0]=getcharRX();
	*remainMilliseconds = cSec;
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getSizeTime(unsigned long* sizeMilliseconds, unsigned char channel){ //Obtain track total size in millisecond
	unsigned long cSec=0;
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('S');
	//Get reply
	((unsigned char *) &cSec)[3]=getcharRX();
	((unsigned char *) &cSec)[2]=getcharRX();
	((unsigned char *) &cSec)[1]=getcharRX();
	((unsigned char *) &cSec)[0]=getcharRX();
	*sizeMilliseconds = cSec;
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getMonoStereo(unsigned char* ch, unsigned char channel){ //Obtain track channels: 1 for mono, 2 for stereo
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('C');
	//Get reply
	*ch = getcharRX();
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getBitsPerSample(unsigned char* bps, unsigned char channel){ //Obtain track bits per sample
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('I');
	//Get reply
	*bps = getcharRX();
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::getSampleRate(unsigned long* sampleRate, unsigned char channel){ //Obtain track sample rate(per second)
	unsigned long sr=0;
	//Send commands
	putcharTX('O');
	putcharTX(channel);
	putcharTX('T');
	//Get reply
	((unsigned char *) &sr)[3]=getcharRX();
	((unsigned char *) &sr)[2]=getcharRX();
	((unsigned char *) &sr)[1]=getcharRX();
	((unsigned char *) &sr)[0]=getcharRX();
	*sampleRate = sr;
	return (SW_REPLY)getcharRX();
}


/****************************************************************/
//MASTER FAT/FAT32 Data Management Functions
//Those next Functions return FAT ACK/NAK List execution status in the SW_FILERESULT parameter
/****************************************************************/

/****************************************************************/
SW_REPLY SMARTWAV2::SDFmount(unsigned char mount, SW_FILERESULT* result){ //Mounts or unmounts SD card FAT system
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('X');           //Mount/Unmount SD card
	putcharTX(mount);         //State
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();	
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetListDirs(unsigned int* dirs, SW_FILERESULT* result){ //get number of dirs inside the current directory path
	unsigned int dr=0, fs=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('L');           //List/count
	putcharTX('D');           //Directories
	//Get reply
	((unsigned char *) &dr)[1]= getcharRX(); //Get Upper part
	((unsigned char *) &dr)[0]= getcharRX(); //Get Lower part
	*dirs=dr;  
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetListFiles(unsigned int* files, SW_FILERESULT* result){ //get number of files inside the current directory path
	unsigned int dr=0, fs=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('L');           //List/count
	putcharTX('F');           //Files
	//Get reply
	((unsigned char *) &fs)[1]= getcharRX(); //Get Upper part
	((unsigned char *) &fs)[0]= getcharRX(); //Get Lower part
	*files=fs;
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetDirItemName(unsigned int dirNo, char dirName[], SW_FILERESULT* result){ //searches for the "itemNumber" dir name
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('G');           //Get name of given item number
	putcharTX('D');           //Directory
	putcharTX(dirNo>>8);      //Send Upper part of dirNo
	putcharTX(dirNo);         //Send Lower part of dirNo
	//Get reply
	while(1){
		dirName[i]=getcharRX(); 
		if(dirName[i]==0x00){ //if we find NULL character, means end of name
			break;
		}
		i++;
	}
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();  
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetFileItemName(unsigned int fileNo, char fileName[], SW_FILERESULT* result){ //Searches for the "itemNumber" file name
	unsigned int i=0;
	//Send commands
	putcharTX('F');            //File function - memory card file management
	putcharTX('G');            //Get name of given item number
	putcharTX('F');            //File  
	putcharTX(fileNo>>8);      //Send Upper part of fileNo
	putcharTX(fileNo);         //Send Lower part of fileNo
	//Get reply
	while(1){
		fileName[i]=getcharRX(); 
		if(fileName[i]==0x00){ //if we find NULL character, means end of name
			break;   
		}
		i++;
	}
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFopenDir(char dirName[], SW_FILERESULT* result){ //opens an existing directory inside the current directory path
	unsigned int i=0;
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('D');       //Open Dir
	while(1){
	putcharTX(dirName[i]);
		if(dirName[i]==0x00){
			break;
		}	
		i++;
	}
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetPath(char path[], SW_FILERESULT* result){ //obtains current dir path
	unsigned int i=0;
	//Send commands
	putcharTX('F');        //File function - memory card file management
	putcharTX('H');        //Get current Dir Path
	//Get reply
	while(1){
		path[i]=getcharRX(); 
		if(path[i]==0x00){ //if we find NULL character, means end of path
			break;   
		}
		i++;
	}
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFnewDir(char dirName[], SW_FILERESULT* result){ //create a new directory
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('N');           //New
	putcharTX('D');           //Directory/Folder  
	while(1){
		putcharTX(dirName[i]);
		if(dirName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFnewFile(char fileName[], SW_FILERESULT* result){ //create a new file
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('N');           //New
	putcharTX('F');           //File
	while(1){
		putcharTX(fileName[i]);
		if(fileName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFeraseDir(char dirName[], SW_FILERESULT* result){ //Erases an existing dir/folder
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('E');           //Erase Dir
	putcharTX('O');           //Unlock Erase Protection
	while(1){
		putcharTX(dirName[i]);
		if(dirName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFeraseFile(char fileName[], SW_FILERESULT* result){ //Erases an existing file
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('E');           //Erase File
	putcharTX('O');           //Unlock Erase Protection
	while(1){
		putcharTX(fileName[i]);
		if(fileName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetFileSize(char fileName[], unsigned long* size, SW_FILERESULT* result){ //Get size of an existing file
	unsigned int i=0, sz=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('I');           //Info
	putcharTX('S');           //Size
	while(1){
		putcharTX(fileName[i]);
		if(fileName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	((unsigned char *) &sz)[3]=getcharRX();
	((unsigned char *) &sz)[2]=getcharRX();
	((unsigned char *) &sz)[1]=getcharRX();  
	((unsigned char *) &sz)[0]=getcharRX(); 
	*size=sz;
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetFATAttribute(char fileName[], unsigned char*attribute, SW_FILERESULT* result){ //Get FAT attribute of file
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('I');           //Info
	putcharTX('F');           //FAT attribute
	while(1){
		putcharTX(fileName[i]);
		if(fileName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	*attribute=getcharRX();
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetDateTime(SW_TIMESTAMP* timeDate, char fileName[], SW_FILERESULT* result){ //Get date and time of an existing file
	unsigned int i=0, yr=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('T');           //Time/Date
	putcharTX('G');           //Get
	while(1){
		putcharTX(fileName[i]);
		if(fileName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	timeDate->hour   = getcharRX();         //hour
	timeDate->minute = getcharRX();         //min
	timeDate->second = getcharRX();         //sec
	timeDate->day    = getcharRX();         //day
	timeDate->month  = getcharRX();         //month
	((unsigned char*)&yr)[1] = getcharRX();	//year upper
	((unsigned char*)&yr)[0] = getcharRX();	//year lower
	timeDate->year = yr;		            //year
	*result = (SW_FILERESULT)getcharRX();      //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFsetDateTime(SW_TIMESTAMP* timeDate, char fileName[], SW_FILERESULT* result){ //Set date and time to an existing file
	unsigned int i=0;
	//Send commands
	putcharTX('F');              //File function - memory card file management
	putcharTX('T');              //Time/Date
	putcharTX('S');              //Set
	putcharTX(timeDate->hour);   //hour
	putcharTX(timeDate->minute); //min
	putcharTX(timeDate->second); //sec
	putcharTX(timeDate->day);	 //day
	putcharTX(timeDate->month);  //month
	putcharTX(timeDate->year>>8);//year upper
	putcharTX(timeDate->year);	 //year lower	
	while(1){
		putcharTX(fileName[i]);
		if(fileName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFrenameDir(char oldName[], char newName[], SW_FILERESULT* result){ //renames or moves an existing directory
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('M');           //Rename / Move
	while(1){
		putcharTX(oldName[i]);
		if(oldName[i]==0x00){
		break;
		}	
		i++;
	}
	i=0;
	while(1){
		putcharTX(newName[i]);
		if(newName[i]==0x00){
			break;
		}	
		i++;
	}	
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFrenameFile(char oldName[], char newName[], SW_FILERESULT* result){ //renames or moves an existing file
	unsigned int i=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('M');           //Rename / Move
	while(1){
		putcharTX(oldName[i]);
		if(oldName[i]==0x00){
		break;
		}	
		i++;
	}
	i=0;
	while(1){
		putcharTX(newName[i]);
		if(newName[i]==0x00){
			break;
		}	
		i++;
	}	
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetFreeTotalSpace(unsigned long* free,unsigned long* total, SW_FILERESULT* result){ //Get free and total space in KBytes of the microSD card drive
	unsigned long fs=0, ts=0;
	//Send commands
	putcharTX('F');           //File function - memory card file management
	putcharTX('F');           //Free/Total space
	//Get reply
	((unsigned char *) &fs)[3]=getcharRX();
	((unsigned char *) &fs)[2]=getcharRX();
	((unsigned char *) &fs)[1]=getcharRX();
	((unsigned char *) &fs)[0]=getcharRX();
	((unsigned char *) &ts)[3]=getcharRX();
	((unsigned char *) &ts)[2]=getcharRX();
	((unsigned char *) &ts)[1]=getcharRX();
	((unsigned char *) &ts)[0]=getcharRX();
	*free=fs;
	*total=ts;
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}


/****************************************************************/
//WORKSPACE FAT/FAT32 Data Management Functions - THOSE FUNCTIONS NEED A WORKSPACE#
//Those next Functions return FAT ACK/NAK List execution status in the SW_FILERESULT parameter
/****************************************************************/

/****************************************************************/
SW_REPLY SMARTWAV2::SDFopenFile(char fileName[], SW_OPENMODE mode, unsigned char workspace, SW_FILERESULT* result){ //Opens an existing file in SW_READONLY, SW_WRITEONLY or SW_READWRITE mode 
	unsigned int i=0;
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('O');       //Open file
	putcharTX(workspace); //object workspace number to allocate open file
	putcharTX(mode);      //Mode - SW_READONLY,SW_WRITEONLY,SW_READWRITE
	while(1){
		putcharTX(fileName[i]);
		if(fileName[i]==0x00){
			break;
		}
		i++;
	}
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFreadFile(char buffer[], unsigned int btr, unsigned int* srb, unsigned char workspace, SW_FILERESULT* result){ //Bytes to Read, Succesfully Read Bytes
	unsigned int i=0, rb=0;
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('R');       //Read file
	putcharTX(workspace); //object workspace number to read bytes
	putcharTX(btr>>8);
	putcharTX(btr);
	//Get reply
	for(i=0;i<btr;i++){
		buffer[i]=getcharRX();
	}
	((unsigned char*)&rb)[1]=getcharRX();
	((unsigned char*)&rb)[0]=getcharRX();
	*srb = rb;            //store succesfully read bytes
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFwriteFile(char buffer[], unsigned int btw, unsigned int* swb, unsigned char workspace, SW_FILERESULT* result){ //Bytes to Write, Succesfully Written Bytes
	unsigned int i=0, wb=0;
	putcharTX('F');       //File function - memory card file management
	putcharTX('W');       //Write file
	putcharTX(workspace); //object workspace number to write bytes
	putcharTX(btw>>8);
	putcharTX(btw);    
	for(i=0;i<btw;i++){
		putcharTX(buffer[i]);
	}
	//Get reply
	((unsigned char*)&wb)[1]=getcharRX();
	((unsigned char*)&wb)[0]=getcharRX();
	*swb = wb;            //store succesfully written bytes
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFgetPointer(unsigned long* position, unsigned char workspace, SW_FILERESULT* result){ //Get file pointer of file
	unsigned long pos=0;
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('P');       //Pointer position
	putcharTX(workspace); //object workspace number to get pointer
	putcharTX('G');       //Get  
	//Get reply
	((unsigned char *) &pos)[3]=getcharRX();
	((unsigned char *) &pos)[2]=getcharRX();
	((unsigned char *) &pos)[1]=getcharRX(); 
	((unsigned char *) &pos)[0]=getcharRX();
	*position = pos;      //Assign received position
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFsetPointer(unsigned long position, unsigned char workspace, SW_FILERESULT* result){ //Set/move file pointer of file
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('P');       //Pointer position
	putcharTX(workspace); //object workspace number to set pointer
	putcharTX('S');       //Set
	putcharTX(position>>24);
	putcharTX(position>>16);
	putcharTX(position>>8);
	putcharTX(position);
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFsaveFile(unsigned char workspace, SW_FILERESULT* result){ //Sync/save file
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('S');       //Save/Sync file - Save changes on file
	putcharTX(workspace); //object workspace number to save changes
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFtruncateFile(unsigned char workspace, SW_FILERESULT* result){  //Truncates the file size to the current file read/write pointer position
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('V');       //Truncate contents of file starting from current pointer position
	putcharTX(workspace); //object workspace number to truncate
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFtestErrorFile(unsigned char workspace, SW_FILERESULT* result){  //Test for an error on file
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('Q');       //Test file
	putcharTX(workspace); //object workspace number to test
	putcharTX('R');       //Test file for error
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFtestEndFile(unsigned char workspace, SW_FILERESULT* result){  //Test for end of file
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('Q');       //Test file
	putcharTX(workspace); //object workspace number to test
	putcharTX('E');       //Test for end of file
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/****************************************************************/
SW_REPLY SMARTWAV2::SDFcloseFile(unsigned char workspace, SW_FILERESULT* result){ //Close and ave file
	//Send commands
	putcharTX('F');       //File function - memory card file management
	putcharTX('C');       //Close File
	putcharTX(workspace); //object workspace number to close
	//Get reply
	*result = (SW_FILERESULT)getcharRX(); //Get SD file execution status 
	return (SW_REPLY)getcharRX();
}

/***************************END OF FILE**************************/
