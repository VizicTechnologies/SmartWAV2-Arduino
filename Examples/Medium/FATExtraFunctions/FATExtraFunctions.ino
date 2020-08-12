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
SmartWAV2 FAT extra functions demo: this demo creates a directory
called "Folder" and inside it creates a "text.txt" file, then
the text "This is an example of smartWAV2 FAT extra functions"is 
written to the file, this show some of the SDFxxx functions.
*********************************************************/

#include "SMARTWAV2.h"

//create object
SMARTWAV2 sw;

/************************/
void setup() {
  //Initialize SmartWAV2
  sw.begin();
  //Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

/************************/
/************************/
void loop() {
  unsigned int i;
  unsigned int bw, br;                              //written and read bytes variables
  char data[]="This is an example of smartWAV2 FAT extra functions";
  char buff[100];                                   //Working array
  SW_FILERESULT r;                                  //Variable to store the result of the SDFxxx functions


  sw.SDFnewDir("Folder",&r);                        //Create a new dir called "Folder" in current microSD 0:/ path
  sw.SDFopenDir("Folder",&r);                       //Open the dir called "Folder", current path will be 0:/Folder/

  sw.SDFnewFile("test.txt",&r);                     //Create a new file called "test.txt" 
  sw.SDFopenFile("test.txt",SW_READWRITE,SW_CH0,&r);//Open file called "test.txt" in read+write mode in channel/workspace 0

  //Write "This is an example of smartWAV2 FAT extra functions" to "test.txt" file
  sw.SDFwriteFile(data,sizeof(data),&bw,SW_CH0,&r); //Write data array to channel/workspace 0
  sw.SDFsaveFile(SW_CH0,&r);                        //Save changes to channel/workspace 0
  sw.SDFcloseFile(SW_CH0,&r);                       //Close channel/workspace 0

  delay(1000);                                      //Delay 1 second

  sw.SDFopenFile("test.txt",SW_READWRITE,SW_CH0,&r);//Open file called "test.txt" in read+write mode in channel/workspace 0
  //Read from file "text.txt" the size of data array
  sw.SDFreadFile(buff,sizeof(data),&br,SW_CH0,&r);  //Read data array bytes and store those in buff array from channel/workspace 0
  sw.SDFcloseFile(SW_CH0,&r);                       //Close channel/workspace 0

  delay(1000);                                      //Delay 1 second

  //Verify written vs read bytes with strcmp function
  if(strcmp(data,buff) == 0){                       //If contents are equal strcmp returns 0
    for(i=0; i<10; i++){                            //Turn LED on and off fast 10 times
      digitalWrite(LED_BUILTIN, HIGH);              //Turn the LED on
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);               //Turn the LED off
      delay(200);
    }
  }else{                                            //Contents not equal, some error occur
    while(1){                                       //Loop forever turning LED on and off slowly
      digitalWrite(LED_BUILTIN, HIGH);              //Turn the LED on
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);               //Turn the LED off
      delay(1000);
    }
  }

  //Erase created file and directory
  //sw.SDFeraseFile("test.txt", &r);                  //Erase file "test.txt"
  //sw.SDFopenDir("..",&r);                           //Go up one level in dirs, current path will be 0:/
  //sw.SDFeraseDir("Folder",&r);                      //Erase dir "Folder"

  /*
   * If the above lines SDFerase(), SDFopenDir(), and SDFeraseDir() are commented, the user can
   * remove the microSD card and verify contents, a "Folder" with a "test.txt" file must be present with
   * text contents: This is an example of smartWAV2 FAT extra functions
  */
  
  while(1){}                                   //loop forever
}
