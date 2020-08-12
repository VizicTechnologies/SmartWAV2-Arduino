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
SmartWAV2 advanced functions demo: be sure to previously load into the 
microSD card inserted in SmartWAV2 a .wav file called "track.wav",
this file will be played in this example. Follow the code while listening
to the SmartWAV2 audio output, hear how the audio is being controlled
by the commands.
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

  /**************Demo Part 1****************/
  sw.mount("track",SW_CH0);                 //Mounts an audio .wav file called "track.wav" in channel 0
  sw.advanceTo(11000,SW_CH0);               //Advance file mounted on channel 0 to point 11000 milliseconds(11 seconds), just before the distortion kicks in
  sw.play(SW_CH0);                          //Play channel 0 (starting from second 11 as it was previously advancedTo)
  
  delay(5000);                              //Delay 5 seconds
  sw.pause(SW_CH0);                         //Pause channel 0

  /**************Demo Part 2****************/
  delay(2000);                              //Delay 2 seconds
  //this next command just assigns file playing point A and point B markers, but does not moves the current file playing point
  sw.setABPoints(0, 10500, SW_CH0);         //set point A to 0 milliseconds(0 seconds) and point B to 10500 milliseconds(10.5 seconds)

  sw.advanceTo(0,SW_CH0);                   //Advance file mounted on channel 0 to point 0 milliseconds
  sw.play(SW_CH0);                          //Play channel 0

  delay(12000);                             //Delay 12 seconds
  //At this time the file in channel 0 must have finished playing and be paused as it reached point B, we set 
  //point A to 0 seconds and point B to 10.5 seconds, so 10.5 - 0 = 10.5 seconds and we have delayed 12 seconds

  /**************Demo Part 3****************/
  sw.loop(1,SW_CH0);                        //Enable loop to to channel 0
  sw.play(SW_CH0);                          //Play channel 0

  //File mounted on channel 0 will be playing forever in a loop from set point A to set point B...
  
  while(1){}                                //loop forever
}
