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
SmartWAV2 simple polyphonic demo: be sure to previously load into the 
microSD card inserted in SmartWAV2 the .wav files called "t1.wav" up to "t5.wav",
those files will be played in this example. Follow the code while listening
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
  unsigned long currTime=0;

  /**************Demo Part 1****************/ 
  /* In this first part, 5 tracks are loaded into smartWAV2 channels,
   * then all the tracks are triggered together, being mixed and 
   * creating a melody.
   */
  //Mount tracks 1 to 5 in channels 0 to 4
  sw.mount("t1",     SW_CH0);
  sw.mount("t2.wav", SW_CH1);           //Name can contain .wav or no extension(.wav assumed)
  sw.mount("t3",     SW_CH2);
  sw.mount("t4",     SW_CH3);
  sw.mount("t5.WAV", SW_CH4);           //Name can contain .WAV or no extension(.wav assumed)

  sw.play(SW_CHALL);                    //Trigger all channels at the same time

  delay(8000);                          //Delay/wait 8 seconds
  sw.pause(SW_CHALL);                   //Pause all channels

  /**************Demo Part 2****************/
  /* In this second part, the 5 tracks loaded into smartWAV2 channels,
   * are stopped and then triggered one by one, this shows how to mix tracks or instruments.
   */
  delay(3000);                          //Delay 3 seconds
  sw.loop(1,SW_CHALL);                  //Enable loop A to B points play to all channels(default loop A and B points are the beginning and end of the track)

  sw.advanceTo(0, SW_CH0);              //Rewind channel 0 by advancing to millisecond 0
  sw.play(SW_CH0);                      //Play channel 0
  delay(8000);                          //Delay

  sw.getCurrentPoint(&currTime,SW_CH0); //Obtain current playing point(time) of channel 0 to match song time
  sw.advanceTo(currTime,SW_CH1);        //Advance channel 1 to obtained time
  sw.play(SW_CH1);                      //Play channel 1
  delay(8000);                          //Delay

  sw.getCurrentPoint(&currTime,SW_CH0); //Obtain current playing point(time) of channel 0 to match song time
  sw.advanceTo(currTime,SW_CH2);        //Advance channel 2 to obtained time
  sw.play(SW_CH2);                      //Play channel 2
  delay(8000);                          //Delay

  sw.getCurrentPoint(&currTime,SW_CH0); //Obtain current playing point(time) of channel 0 to match song time
  sw.advanceTo(currTime,SW_CH3);        //Advance channel 3 to obtained time
  sw.play(SW_CH3);                      //Play channel 3
  delay(8000);                          //Delay

  sw.getCurrentPoint(&currTime,SW_CH0); //Obtain current playing point(time) of channel 0 to match song time
  sw.advanceTo(currTime,SW_CH4);        //Advance channel 4 to obtained time
  sw.play(SW_CH4);                      //Play channel 4

  //Track files will be playing in a default loop points forever...

  while(1){}                  //loop forever
}
