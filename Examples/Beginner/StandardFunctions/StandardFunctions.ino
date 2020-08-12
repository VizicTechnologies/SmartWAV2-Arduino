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
SmartWAV2 standard functions demo: be sure to previously load into the 
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

  sw.mount("track",SW_CH0);//Mounts an audio .wav file called "track.wav" in channel 0
  sw.play(SW_CH0);         //Play channel 0
  
  delay(2000);             //Delay 2 seconds
  sw.pause(SW_CH0);        //Pause channel 0
  
  delay(2000);             //Delay 2 seconds
  sw.play(SW_CH0);         //Play channel 0

  delay(2000);             //Delay 2 seconds
  sw.volume(50,SW_CH0);    //Set channel 0 volume at 50%

  delay(2000);             //Delay 2 seconds
  sw.volume(100,SW_CH0);   //Set channel 0 volume at 100%
  sw.panning(-100,SW_CH0); //Set channel 0 panning at 100% left

  delay(3000);             //Delay 3 seconds
  sw.panning(100,SW_CH0);  //Set channel 0 panning at 100% right

  delay(3000);             //Delay 3 seconds
  sw.panning(0,SW_CH0);    //Set channel 0 panning at center (0%)

  delay(3000);             //Delay 3 seconds
  sw.rewindPlay(SW_CH0);   //Rewind and play channel 0

  delay(5000);             //Delay 5 seconds
  sw.stop(SW_CH0);         //Stop channel 0

  sw.sleep();              //Set smartWAV2 to sleep mode

  while(1){}               //loop forever
}
