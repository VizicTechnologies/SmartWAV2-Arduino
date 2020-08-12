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
SmartWAV2 simplest demo: be sure to previously load into the 
microSD card inserted in SmartWAV2 a .wav file called "track.wav",
this file will be played in this example.
*********************************************************/

#include "SMARTWAV2.h"

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

  //Mounts an audio .wav file called "track.wav" in channel 0 and immediately plays it
  sw.playImme("track",SW_CH0);

  //Track file will play until its end...

  //loop forever
  while(1){}
}
