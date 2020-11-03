#include <iobb.h> // Library to access GPIO
#include <time.h>  //Time Library   
#include <stdio.h> // Standard IO library
#include <unistd.h> //defines miscellaneous symbolic constants and types, and declares miscellaneous functions
#include <stdlib.h> //Includes some Standard C Libraries used for trigerring Command line
#include <string.h> // For using String Functions

long int start, stop, count; // Variables to store values

clock_t before, end, difference; // To store Time of UltraSonic Sensor


//Exceution Command
//gcc -o ultrasonicparking UltrasonicParking.c -liobb
//./ultrasonicparking

//Sensor 1 Connection:  Trig - P8 10  , Echo - P8 9
//Sensor 2 Connection:  Trig - P8 12  , Echo - P8 11


int cflag = 1; // Flag to stabilize multiple false detections
int cflag1 = 1; // Flag to stabilize multiple false detections


int main(void)  
{  
  iolib_init();  // Initialize the IOBB Library
  iolib_setdir(8, 10, DigitalOut);  // Setting Pin 10 or Port 8 as Output for Sensor -1
  iolib_setdir(8, 9, DigitalIn);    // Setting Pin 9 or Port 8 as Inputfor Sensor -1
  
  iolib_setdir(8, 12, DigitalOut);  // Setting Pin 12 or Port 8 as Output for Sensor -2
  iolib_setdir(8, 11, DigitalIn);    // Setting Pin 11 or Port 8 as Input for Sensor -2
  
  printf("STARTED \n");
  iolib_delay_ms(500); // Small Delay
  
 
  
  while(1)  
  {  
    pin_high(8, 10);  // Trigger Ultrasonic Sensor
    usleep(10); 
    pin_low(8, 10); 
    
    //iolib_delay_ms(1);
    while(is_low(8, 9))  // Wait until Echo Pulse starts
    {		
			count+= 1;
			before = clock();
			
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
		end = clock();
		if(count > 5000)
			{
				printf("Kindly Check Wiring\n");
				count = 0;
				break;
			}
			
	}
    
    difference = end - before; // Get Absolute time of Echo Pulse
    
    printf("%d    ",before);
    printf("%d    ",end);
    printf("%d    Distance: ",difference); //Print time taken by ultasonic to travel
    difference = difference/30; // Get a rough value in centimeters
    printf("%d cm. \n",difference);
    
    
    if(difference < 15 difference > 0 && cflag == 1) // If  Ultrasonic value less than 15, (Car sensed )
    {
		
		cflag = 0; // Make Flag 0 so that 1 car is not counted as many
		printf("One Car Arrived at Entry Gate \n");
		iolib_delay_ms(500); // Small Delay
	}
	if(difference > 10 && cflag == 0) // When car passes and no more in front of sensor
    {
		cflag = 1; // Make flag as 1 means ready to sense another car
	}
    
    
    pin_low(8, 10); // Make sure trigger pin is low
    iolib_delay_ms(60);  // Small delay to settle trigger and Echo
    start = 0;

    
    
    ///////////////////// OUT GOING PERSON//////////////////////////////
    
    pin_high(8, 12);  // Trigger Ultrasonic Sensor-2
    usleep(10); 
    pin_low(8, 12); 
    
    //iolib_delay_ms(1);
    while(is_low(8, 11))  // Wait until Echo Pulse starts
    {		
			count+= 1;
			before = clock();
			
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
		end = clock();
		if(count > 5000)
			{
				printf("Kindly Check Wiring\n");
				count = 0;
				break;
			}
			
	}
    
    difference = end - before; // Get Absolute time of Echo Pulse
    
    printf("%d    ",before);
    printf("%d    ",end);
    printf("%d    Distance: ",difference); //Print time taken by ultasonic to travel
    difference = difference/30; // Get a rough value in centimeters
    printf("%d cm. \n",difference);
    
    
    if(difference < 15 && difference >0 && cflag1 == 1) // If  Ultrasonic value less than 15, (Car sensed )
    {

		cflag1 = 0; // Make Flag 0 so that 1 Car is not counted as many
		printf("One Car Arrived at Exit Gate \n");
		iolib_delay_ms(500); // Small Delay
	}
	if(difference > 10 && cflag1 == 0) // When Car passes and no more in front of sensor
    {
		cflag1 = 1; // Make flag as 1 means ready to sense another Car
	}
    
    
    pin_low(8, 12); // Make sure trigger pin is low
    iolib_delay_ms(60);  // Small delay to settle trigger and Echo
    start = 0;
    
  
     
     //////////////////////////////////////////////////////////////////////
    
      
  }  
  iolib_free();  
  return(0);  
  
  
}  


