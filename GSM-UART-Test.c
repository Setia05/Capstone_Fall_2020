#include <stdio.h> //Standard C input Output Library
#include <unistd.h> //defines miscellaneous symbolic constants and types, and declares miscellaneous functions
#include <string.h> //C Library for various String Operations
#include <termios.h> // Contains the definitions used by the terminal I/O interfaces 
#include <sys/stat.h> //header defines the structure of the data returned by the functions fstat(), lstat(), and stat(), give file size
#include <fcntl.h> // FIle control, Read Write




 int main (void) // Main function
{
    int file, i; // Variable integers

    char receive[100]; // declare a char array for receiving data
    char buf[140]; // A buffer char array to store temporary data for sending data
    
    int nbytes; //To store number of incoming bytes
    int bytes_written; // To store number of Outgoing bytes
	
	// /dev/ttyO1 is linked with UART1 Port of Beaglebone 
    if ((file = open("/dev/ttyO1", O_RDWR))<0) // Try opening UART 1 file in Read Write mode
    {
		
        printf("UART 1: Failed to open the file.\n"); 
        return 0;
    }
    
    else
    {
		printf("WELCOME TO THIS PARKING\n"); 
		
	}

    
    struct termios options; // using termios to get the atrributes of UART 
    tcgetattr(file, &options); // Reading the attributes from file(UART1 file) and storing in options

    // Set up the communications options:
    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL; //Setting Baud Rate and other Parameters
    options.c_iflag = IGNPAR | ICRNL; 
    tcflush(file, TCIFLUSH); // discard file information not transmitted
    tcsetattr(file, TCSANOW, &options); // apply the changes



	sleep(1); 
	memset(buf, 0, sizeof(buf)); // Clearing any data if available in buf variable
	strcpy(buf, "AT\n"); // Copy a string in buf char array
	nbytes = strlen(buf); // Store size of buf array in nbytes

	bytes_written = write(file, buf, nbytes); // Sending message to GSM Module

	sleep(3);

	int bytes_read = 0;
	
	bytes_read = read(file,&receive,100); // Read the incoming Message from GSM Module
	// Read the file and store the data in receive , read 100 bytes max
	
	//printf("\n\nBytes Received - %d",bytes_read); // Print how many bytes was received
	//printf("\n");
	
	if(bytes_read > 0) //If no. of bytes are read is more than 1
	{
		for(i=0;i<bytes_read;i++) //a for loop to print data byte by byte
		{
			printf("%c",receive[i]); //print a byte of message from GSM Module
		}
		printf("\nCITCO GEDI ROUTE\n\n"); //General Print

        sleep(2);
	}
	
	sleep(1);
	
	
	memset(buf, 0, sizeof(buf));  //Clearing any previous data in buf
	strcpy(buf, "AT+CMGF=1\n"); // AT command to enable Text mode of Module

	nbytes = strlen(buf); // Store size of buf array in nbytes
	bytes_written = write(file, buf, nbytes); // Sending message to GSM Module

	sleep(2);

	
	bytes_read = read(file,&receive,100); // Read the incoming Message from GSM Module
	//Read the file and store the data in receive , read 100 bytes max

	
	if(bytes_read > 1) //If no. of bytes are read is more than 1
	{
		for(i=0;i<bytes_read;i++) //a for loop to print data byte by byte
		{
			printf("%c",receive[i]); //print a byte of message from GSM Module
		}
		printf("\n\n"); //General Print

        sleep(1);
	}
	
	
	sleep(2);
	
	
	
	
	memset(buf, 0, sizeof(buf)); 
	strcpy(buf, "AT+CMGS=\"+16476750147\"\n"); // AT command to mention mobile number to which we want to send SMS
	nbytes = strlen(buf); // Store size of buf array in nbytes

	bytes_written = write(file, buf, nbytes); // Sending message to GSM Module

	sleep(2);

	
	//bytes_read = read(file,&receive,100); // Read the incoming Message from GSM Module
	// Read the file and store the data in receive , read 100 bytes max
	
	printf("\n\nBytes Received - %d",bytes_read); // Print how many bytes was received
	printf("\n");
	
	if(bytes_read > 1) //If no. of bytes are read is more than 1
	{
		for(i=0;i<bytes_read;i++) //a for loop to print data byte by byte
		{
			printf("%c",receive[i]); //print a byte of message from GSM Module
		}
		printf("\n\n\n"); //General Print

        sleep(1);
	}
	
	
	sleep(2);
	
	
	
	memset(buf, 0, sizeof(buf)); 
	strcpy(buf, "PARKING IS AVAILABLE"); // Message which we want to send in SMS (under 140 char)

	nbytes = strlen(buf); // Store size of buf array in nbytes
	bytes_written = write(file, buf, nbytes); // Sending message to GSM Module

	sleep(2);
    
	
	//bytes_read = read(file,&receive,100); // Read the incoming Message from GSM Module
	// Read the file and store the data in receive , read 100 bytes max
	
	printf("\n\nBytes Received - %d",bytes_read); // Print how many bytes was received
	printf("\n");
	
	if(bytes_read > 1) //If no. of bytes are read is more than 1
	{
		for(i=0;i<bytes_read;i++) //a for loop to print data byte by byte
		{
			printf("%c",receive[i]); //print a byte of message from GSM Module
		}
		printf("\n"); //General Print

        sleep(1);
	}
	
	
	//strcpy(buf, char(26)); // Copy a string in buf char array
	//nbytes = strlen(buf); // Store size of buf array in nbytes
	
	sleep(5);
	
	
	
	
	memset(buf, 0, sizeof(buf)); 
	//buf[0] = '\032';
	buf[0] = 26; // Submitting - ASCII code 26 
	nbytes = 1;

	bytes_written = write(file, buf, nbytes); // Sending message to GSM Module

	sleep(2);
    
	bytes_read = read(file,&receive,100); // Read the incoming Message from GSM Module
	// Read the file and store the data in receive , read 100 bytes max
	
	printf("\n\nBytes Received - %d",bytes_read); // Print how many bytes was received
	printf("\n");
	
	if(bytes_read > 1) //If no. of bytes are read is more than 1
	{
		for(i=0;i<bytes_read;i++) //a for loop to print data byte by byte
		{
			printf("%c",receive[i]); //print a byte of message from GSM Module
		}
		printf("\n\n"); //General Print

        sleep(1);
	}
	
  
    close(file); 
}
