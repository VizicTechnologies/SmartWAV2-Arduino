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

/*********************************************************
SmartWAV2 Drum Machine demo: be sure to previously load into the 
microSD card inserted in SmartWAV2 the .wav files of microSDcardContents,
those files will be played in this example. This demo simulates an electronic
drum machine.
*********************************************************/

#include "SMARTWAV2.h"

//defines to trigger drum sounds easy without too much text...
#define BD  sw.rewindPlay(SW_CH0)
#define SN  sw.rewindPlay(SW_CH1)
#define TD2 sw.rewindPlay(SW_CH2)
#define HH  sw.rewindPlay(SW_CH3)
#define RR  sw.rewindPlay(SW_CH4)
#define CR1 sw.rewindPlay(SW_CH5)

#define SPEED 300 //The less the delay, the faster the rythm

//create object
SMARTWAV2 sw;

/************************/
void setup() {
  //Initialize SmartWAV2
  sw.begin();
}

/************************/
/************************/
void loop() {
  unsigned char compass = 0;

  //Mount drum tracks
  sw.mount("BD",  SW_CH0); //Mounts an audio .wav file called "BD.wav"  in channel 0
  sw.mount("SD",  SW_CH1); //Mounts an audio .wav file called "SD.wav"  in channel 1
  sw.mount("TD2", SW_CH2); //Mounts an audio .wav file called "TD2.wav" in channel 2
  sw.mount("HH",  SW_CH3); //Mounts an audio .wav file called "HH.wav"  in channel 3
  sw.mount("RR",  SW_CH4); //Mounts an audio .wav file called "RR.wav"  in channel 4
  sw.mount("CR1", SW_CH5); //Mounts an audio .wav file called "CR1.wav" in channel 5

  while(1){ //loop forever
    switch(compass){ //Each case : or compass must delay SPEED*4 in total time
      case 0:
        BD;  HH; CR1; delay(SPEED);
        SN;  HH;      delay(SPEED);
        BD;  HH;      delay(SPEED/2);
        BD;           delay(SPEED/2);
        SN;  HH;      delay(SPEED);
      break;
      case 1:
        BD;  HH;      delay(SPEED);
        SN;  HH;      delay(SPEED);
        BD;  HH;      delay(SPEED/2);
        TD2;          delay(SPEED/2);
        SN;  HH;      delay(SPEED/2);
        RR;           delay(SPEED/2);
      break;
      case 2:
        BD;  HH; CR1; delay(SPEED);
        SN;  HH;      delay(SPEED);
        BD;  HH;      delay(SPEED/2);
        BD;           delay(SPEED/2);
        SN;  HH;      delay(SPEED);
      break;
      case 3:
        BD;  HH;      delay(SPEED);
        SN;  HH;      delay(SPEED);
        BD;  HH;      delay(SPEED/2);
        BD;           delay(SPEED/2);
        SN;  HH;      delay(SPEED/2);
        SN;           delay(SPEED/2);
      break;
      default:
      break;
    }
    compass++;
    if(compass >= 4) compass = 0;
  }
}
