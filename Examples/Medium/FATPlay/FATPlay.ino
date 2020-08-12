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
SmartWAV2 FAT play demo: be sure to previously load into the 
microSD card inserted in SmartWAV2 any .wav files to be searched,
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
  unsigned int files;
  unsigned int i;
  unsigned char pl;
  char fname[50];
  SW_FILERESULT r; //Variable to store the result of the SDFxxx functions

  //Obtain list of files in the microSD card root 0:/ path
  sw.SDFgetListFiles(&files, &r);

  //Go through all found files
  for(i=0; i<files; i++){
    //Get name of file number i into the names array
    sw.SDFgetFileItemName(i,fname,&r);

    //Check if that obtained file name has .wav or .WAV extension
    if((strstr(fname,".wav") != 0) || (strstr(fname,".WAV") != 0)){
      //Play the obtained .wav file in channel 0
      sw.playImme(fname,SW_CH0);
      //loop while the file is being played
      do{
        delay(1000);
        sw.getPlayPause(&pl, SW_CH0);
      }while(pl == 1); //Do-while the play is 1 (means playing)

      //Once we reach here it means the file was ended (play is == 0)
      
    }//else no .wav or .WAV extension

    //Continue cycle for()
  }
 
  while(1){}               //loop forever
}
