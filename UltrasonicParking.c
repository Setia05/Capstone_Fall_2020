#include <iobb.h> // Library to access GPIO of Beaglebone
#include <time.h>  //Time Library   
#include <stdio.h> // Standard IO library
#include <unistd.h> //defines miscellaneous symbolic constants and types, and declares miscellaneous functions
#include <stdlib.h> //Includes some Standard C Libraries used for trigerring Command line
#include <string.h> // For using String Functions

long int start, stop, count; // Variables to store values

clock_t init, final, difference; // To store Time of UltraSonic Sensor


int stab = 0; // to stabilize multiple false detections of sensor1
int stab1 = 0; // to stabilize multiple false detections of sensor


int main(void)  
{  
  iolib_init();  // Initialize the IOBB Library
  iolib_setdir(8, 10, DigitalOut);  // Setting Pin 10 as Output for Sensor1
  iolib_setdir(8, 9, DigitalIn);    // Setting Pin 9 as Inputfor Sensor1
  
  iolib_setdir(8, 12, DigitalOut);  // Setting Pin 12 as Output for Sensor2
  iolib_setdir(8, 11, DigitalIn);    // Setting Pin 11 as Input for Sensor2


  
  printf("STARTED \n");
  iolib_delay_ms(500); // Small Delay

  
 
  
  while(1)  //Infinite loop
  {  
    pin_high(8, 10);  // Trigger Ultrasonic Sensor
    usleep(10); 
    pin_low(8, 10); 
    
    //iolib_delay_ms(1);
    while(is_low(8, 9))  // Wait until Echo Pulse starts
    {		
			count+= 1;
			init = clock();
			
			if(count > 5000)
			{
				printf("Kindly Check Wiring\n");
				count = 0;
				break;
			}
			
			
	}
	
	while(is_high(8, 9)) // Wait and store Echo pulse time
	{	
		count += 0;
		final = clock();
		if(count > 5000)
			{
				printf("Kindly Check Wiring\n");
				count = 0;
				break;
			}
			
	}
    
    difference = final - init; // Get Absolute time of Echo Pulse
    
    //printf("%d    ",init);
    //printf("%d    ",final);
    //printf("%d    Distance: ",difference); //Print time taken by ultasonic to travel
    //difference = difference/30; // inches to cm
    //printf("%d cm. \n",difference);
    
    
    if(difference < 15 && difference > 0 && stab == 1) // If  Ultrasonic value less than 15, (Car sensed )
    {
		
		stab = 0; // 0 so that message shouldn't print continuously
		printf("Slot -1 Occupied \n");
		iolib_delay_ms(500); // Small Delay
	}
	if(difference > 15 && stab == 0) // When car passes and no more in front of sensor
    {
		stab = 1; // Make flag as 1 means ready to sense another car
		printf("Slot -1 Empty \n");
		iolib_delay_ms(500); // Small Delay
		
	}
    
    
    pin_low(8, 10); // Make sure trigger pin is low
    iolib_delay_ms(60);  // Small delay to settle trigger and Echo
    start = 0;

    
    
    ///////////////////// SLOT - 2//////////////////////////////
    
    pin_high(8, 12);  // Trigger Ultrasonic Sensor-2
    usleep(10); 
    pin_low(8, 12); 
    
    //iolib_delay_ms(1);
    while(is_low(8, 11))  // Wait until Echo Pulse starts
    {		
			count+= 1;
			init = clock();
			
			if(count > 5000)
			{
				printf("Kindly Check Wiring\n");
				count = 0;
				break;
			}
			
			
	}
	
	while(is_high(8, 11)) // Wait and store Echo pulse time
	{	
		count += 0;
		final = clock();
		if(count > 5000)
			{
				printf("Kindly Check Wiring\n");
				count = 0;
				break;
			}
			
	}
    
    difference = final - init; // Get Absolute time of Echo Pulse
    
    //printf("%d    ",init);
    //printf("%d    ",final);
    //printf("%d    Distance: ",difference); //Print time taken by ultasonic to travel
    //difference = difference/30;
    //printf("%d cm. \n",difference);
    
    
    if(difference < 15 && difference >0 && stab1 == 1) // If  Ultrasonic value less than 15, (Car sensed )
    {

		stab1 = 0; // Make Flag 0 so that message shouldn't print continuously
		printf(" Slot-2 Occupied \n");
		iolib_delay_ms(500); // Small Delay
	}
	if(difference > 15 && stab1 == 0) // When Car passes and no more in front of sensor
    {
		stab1 = 1; // Make flag as 1 means ready to sense another Car
		printf(" Slot-2 Empty \n");
		iolib_delay_ms(500); // Small Delay
	}
    
    
    pin_low(8, 12); // Make sure trigger pin is low
    iolib_delay_ms(60);  // Small delay to settle trigger and Echo
    start = 0;
    
	 
  iolib_delay_ms(900);
     
   
    
      
  }  
  iolib_free();  
  return(0);  
  
  
}  


