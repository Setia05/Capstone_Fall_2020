#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <string.h>
#include <iobb.h>   //GPIO Library
#include <time.h>  //Time Library  
 
/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

char buf[180];
char sendbuf[180];
char receive1[180];
char receive4[180];
int fd1;  // File descriptor
int fd4; 
int n;
int sendbytes;


int us1_trig = 7;
int us1_echo = 8;

int us2_trig = 9;
int us2_echo = 10;

int us3_trig = 11;
int us3_echo = 12;

int us4_trig = 13;
int us4_echo = 14;

int us5_trig = 15;
int us5_echo = 16;

int servoPin1 = 17;
int servoPin2 = 18;

int buzzerPin1 = 25;
int buzzerPin2 = 30;


int threshold = 20;

int before, end;
int time_taken;

int distance;
int slots_available = 0;


char *num1 = "xxxxxxxxx";
char *num2 = "xxxxxxxxx";
char *num3 = "xxxxxxxxx";

char *card1 = "71216196";
char *card2 = "217179198153";
char *card3 = "121242187153";


int search=0;
int position=0;
int uploadCount = 0;
int readerNum = -1;


int open_port1(void);
int open_port4(void);
int readBack1(void);
int readBack4(void);
int sendSMS(char *num,int s_available);
int sendSMSentry(char *num);
int sendSMSexit(char *num);
int getus1(); // For Gate 1
int getus2(); // For Slot-1
int getus3(); // For Slot-2
int getus4(); // For Slot-3
int getus5(); // For Gate 2
void servo_angle1(int angle); //Defining a function
void servo_angle2(int angle); //Defining a function

  

int main()
{

  iolib_init();  

  //printf("IOBB library Initialized\n");

  iolib_setdir(8, us1_trig, DigitalOut);  
  iolib_setdir(8, us1_echo, DigitalIn); 
  //printf("Sensor 1 Pin Configured\n");


  iolib_setdir(8, us2_trig, DigitalOut);  
  iolib_setdir(8, us2_echo, DigitalIn); 
  //printf("Sensor 2 Pin Configured\n");

  iolib_setdir(8, us3_trig, DigitalOut);  
  iolib_setdir(8, us3_echo, DigitalIn); 
  //printf("Sensor 3 Pin Configured\n");

  iolib_setdir(8, us4_trig, DigitalOut);  
  iolib_setdir(8, us4_echo, DigitalIn); 
  //printf("Sensor 4 Pin Configured\n");

  iolib_setdir(8, us5_trig, DigitalOut);  
  iolib_setdir(8, us5_echo, DigitalIn);  
  //printf("Sensor 5 Pin Configured\n"); 


  iolib_setdir(8, servoPin1, DigitalOut);  
  iolib_setdir(8, servoPin2, DigitalOut);  

  iolib_setdir(9, buzzerPin1, DigitalOut);  
  iolib_setdir(9, buzzerPin2, DigitalOut);  

  pin_high(9,buzzerPin1); 
  servo_angle1(0);
  pin_low(9,buzzerPin1); 

  pin_high(9,buzzerPin2); 
  servo_angle2(0);
  pin_low(9,buzzerPin2); 

  printf("READY\n"); 

//Open UART 1 Port and Setting it
  fd1 = open_port1(); //For GSM
 
  // Read the configureation of the port 
 
  struct termios options;
  tcgetattr( fd1, &options );
 
  /* SEt Baud Rate */
  cfsetispeed( &options, B9600 );
  cfsetospeed( &options, B9600 );
  
  //I don't know what this is exactly 
  options.c_cflag |= ( CLOCAL | CREAD );
  // Set the Charactor size
  options.c_cflag &= ~CSIZE; /* Mask the character size bits */
  options.c_cflag |= CS8;    /* Select 8 data bits */
  // Set parity - No Parity (8N1)
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
 
  // Disable Hardware flowcontrol
 
  //  options.c_cflag &= ~CNEW_RTSCTS;  -- not supported
 
  // Enable Raw Input
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  // Disable Software Flow control
  options.c_iflag &= ~(IXON | IXOFF | IXANY);
  // Chose raw (not processed) output
  options.c_oflag &= ~OPOST;

 
  if ( tcsetattr( fd1, TCSANOW, &options ) == -1 )
    printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
  else
    printf ( "%s\n", "tcsetattr succeed-1" );
 
  fcntl(fd1, F_SETFL, FNDELAY);

///////////////////////////////////////////////////////////////////////

//Opening UART 4 and setting it
  fd4 = open_port4();   //For ESP
  // Read the configureation of the port 
 
  struct termios options4;
  tcgetattr( fd4, &options4 );
 
  /* SEt Baud Rate */
  cfsetispeed( &options4, B9600 );
  cfsetospeed( &options4, B9600 );
  
  //I don't know what this is exactly 
  options4.c_cflag |= ( CLOCAL | CREAD );
  // Set the Charactor size
  options4.c_cflag &= ~CSIZE; /* Mask the character size bits */
  options4.c_cflag |= CS8;    /* Select 8 data bits */
  // Set parity - No Parity (8N1)
  options4.c_cflag &= ~PARENB;
  options4.c_cflag &= ~CSTOPB;
  options4.c_cflag &= ~CSIZE;
  options4.c_cflag |= CS8;
 
  // Disable Hardware flowcontrol
 
  //  options.c_cflag &= ~CNEW_RTSCTS;  -- not supported
 
  // Enable Raw Input
  options4.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  // Disable Software Flow control
  options4.c_iflag &= ~(IXON | IXOFF | IXANY);
  // Chose raw (not processed) output
  options4.c_oflag &= ~OPOST;

 
  if ( tcsetattr( fd4, TCSANOW, &options ) == -1 )
    printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
  else
    printf ( "%s\n", "tcsetattr succeed-4" );
 
  fcntl(fd4, F_SETFL, FNDELAY);

////////////////////////////////////////////////////////////////////  

  memset(sendbuf, 0, sizeof(sendbuf)); 
  strcpy(sendbuf, "AT\r\n"); // Copy a string in buf char array
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  n = write(fd1,sendbuf, sendbytes);

  if (n < 0)
    fputs("write() of 4 bytes failed!\n", stderr);
  else
    printf ("Write 1 succeed n = %i\n", n );

  sleep(1);
  readBack1();
  sleep(1);

  // memset(sendbuf, 0, sizeof(sendbuf)); 
  // strcpy(sendbuf, "AT+CMGF=1\r\n"); // Copy a string in buf char array
  // sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  // n = write(fd1,sendbuf, sendbytes);

  // if (n < 0)
  //   fputs("write() of 4 bytes failed!\n", stderr);
  // else
  //   printf ("Write 1 succeed n = %i\n", n );

  // sleep(1);
  // readBack1();
  // sleep(1);



while(1)
{
  
  iolib_delay_ms(100);
  int gate1 = getus1(); // Entry Gate
  if(gate1 > 0)
  {
    printf("Car Arrived at Gate-1. \n");
  }

  iolib_delay_ms(100);
  int gate2 = getus5(); //Exit Gate
  if(gate2 > 0)
  {
    printf("Car Arrived at Gate-2. \n");
  }


  slots_available = 0; // To store the number of empty slots

  iolib_delay_ms(100);  
  int slot1 = getus2(); //Slot 1
  if(slot1 == 0)
  {
     slots_available++;
  }

  iolib_delay_ms(100);  
  int slot2 = getus3(); //Slot 2
  if(slot2 == 0)
  {
     slots_available++;
  }

  iolib_delay_ms(100);  
  int slot3 = getus4(); //Slot 3
  if(slot3 == 0)
  {
     slots_available++;
  }

  printf("Number of Slots Available: %d.\n",slots_available);

  readBack1();
  sleep(0.1);
  readBack4();

  
//////////// CHECKING SMS ////////////////

  search = (int)strstr(receive1, num1);
  position = search-(int)receive1;
  if(position >= 0)
  {
    printf("Got number 1 at : %d \n",position);
    printf("Sending SMS \n");
    sendSMS(num1,slots_available);
    uploadCount += 10;
    position = -1;
  }


  search = (int)strstr(receive1, num2);
  position = search-(int)receive1;
  if(position >= 0)
  {
    printf("Got number 2 at : %d \n",position);
    printf("Sending SMS \n");
    sendSMS(num2,slots_available);
    uploadCount += 10;
    position = -1;
  }

  search = (int)strstr(receive1, num3);
  position = search-(int)receive1;
  if(position >= 0)
  {
    printf("Got number 3 at : %d \n",position);
    printf("Sending SMS \n");
    sendSMS(num3,slots_available);
    uploadCount += 10;
    position = -1;
  }

//////////// CHECKING CARD ////////////////

  // printf("Receive 4 %s \n",receive4);

  /////////// GET READER NUMBER //////////////
  int reader = (int)strstr(receive4, "A");
  // printf("Reader 1 Found at %d\n",reader);
  if(reader > 0)
  {
    readerNum = 1;
    printf("GOT DATA FROM READER 1\n");
  }

  reader = (int)strstr(receive4, "B");
  // printf("Reader 2 Found at %d\n",reader);
  if(reader > 0)
  {
    readerNum = 2;
    printf("GOT DATA FROM READER 2\n");
  }

  
  //////////// GET CARD NUMBER - 1/////////////

  search =  (int)strstr(receive4, card1);
  // printf("Card 1 Position: ",position);
  if(search > 0)
  {
    printf("Got Card 1 at : %d \n",search);
    if(readerNum == 1)
    {
      printf("From Reader 0\n");

      if(slots_available > 0)
      {
        servo_angle1(100); //Opening the Door
        sleep(3);          //Waiting for 3 seconds
        servo_angle1(0);   //Closing the door
        sendSMSentry(num1); //Sending Entry Sms to owner
        uploadCount += 10;
      }
      else //If slots are not available
      {
        printf("SLOTS NOT AVAILABLE\n");
        sendSMS(num1,slots_available); //Send sms that slots not available
        uploadCount += 10;
      }
    }
    if(readerNum == 2)
    {
      printf("From Reader 1\n");
      servo_angle2(100); //Open Exit Door
      sleep(3);
      servo_angle2(0);
      sendSMSexit(num1);  //Sms for exit to owner
      uploadCount += 10;
    }
    // printf("Sending SMS \n");
    // sendSMS(num1,slots_available);
    search = -1;
  }

  //////////// GET CARD NUMBER - 2 /////////////

  search = (int)strstr(receive4, card2);
  position = search-(int)receive1;
  if(search > 0)
  {
    printf("Got Card 2 at : %d \n",search);
    if(readerNum == 1)
    {
      if(slots_available > 0)
      {
        servo_angle1(100);
        sleep(3);
        servo_angle1(0);
        sendSMSentry(num2);
        uploadCount += 10;
      }
      else
      {
        printf("SLOTS NOT AVAILABLE\n");
        sendSMS(num2,slots_available);
        uploadCount += 10;
      }
    }
    if(readerNum == 2)
    {
      printf("From Reader 1\n");
      servo_angle2(100);
      sleep(3);
      servo_angle2(0);
      sendSMSexit(num2);
      uploadCount += 10;
    }
    // printf("Sending SMS \n");
    // sendSMS(num2,slots_available);
    search = -1;
  }

    //////////// GET CARD NUMBER - 3/////////////

    search = (int)strstr(receive4, card3);
    position = search-(int)receive1;


     if(search > 0)
    {

    printf("Got Card 3 at : %d \n",search);
    if(readerNum == 1)
    {
      printf("From Reader 0\n");
      if(slots_available > 0)
      {
        servo_angle1(100);
        sleep(3);
        servo_angle1(0);
        sendSMSentry(num3);
        uploadCount += 10;
      }
      else
      {
        printf("SLOTS NOT AVAILABLE\n");
        sendSMS(num3,slots_available);
        uploadCount += 10;
      }
    }
    if(readerNum == 2)
    {
      printf("From Reader 1\n");
      servo_angle2(100);
      sleep(3);
      servo_angle2(0);
      sendSMSexit(num3);
      uploadCount += 10;
    }
    // printf("Sending SMS \n");
    // sendSMS(num3,slots_available);
    search = -1;
  }

  memset(receive4, 0, sizeof(receive4)); 
  memset(receive1, 0, sizeof(receive1)); 


    //////////// SENDING VALUES FOR ONLINE UPLOAD/////////////

  uploadCount++;

  if(uploadCount > 15)
  {
    printf("Online Upload \n");
    uploadCount = 0;

    tcflush(fd4, TCIFLUSH);

    if(slots_available == 0)
    {
      memset(sendbuf, 0, sizeof(sendbuf)); 
      strcpy(sendbuf, "0"); // Copy a string in buf char array
      sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
      printf("Writing Command: %s\n",sendbuf);
      n = write(fd4,sendbuf, sendbytes);
    }

    if(slots_available == 1)
    {
      memset(sendbuf, 0, sizeof(sendbuf)); 
      strcpy(sendbuf, "1"); // Copy a string in buf char array
      sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
      printf("Writing Command: %s\n",sendbuf);
      n = write(fd4,sendbuf, sendbytes);
    }

    if(slots_available == 2)
    {
      memset(sendbuf, 0, sizeof(sendbuf)); 
      strcpy(sendbuf, "2"); // Copy a string in buf char array
      sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
      printf("Writing Command: %s\n",sendbuf);
      n = write(fd4,sendbuf, sendbytes);
    }

    if(slots_available == 3)
    {
      memset(sendbuf, 0, sizeof(sendbuf)); 
      strcpy(sendbuf, "3"); // Copy a string in buf char array
      sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
      printf("Writing Command: %s\n",sendbuf);
      n = write(fd4,sendbuf, sendbytes);
    }



  }
  
}
 
 
  close(fd1); //CLOSING FILE1
  close(fd4); //CLOSING FILE 4
  iolib_free();  
  return(0);
 
}
/////////////////////FUNCTION DECLARATIONS///////////////////////////////////    
 
int open_port1(void)
{
  int fd; /* File descriptor for the port */
 
  fd = open("/dev/ttyO1", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
    {
      perror("open_port: Unable to open /dev/O1 - ");
    }
  else
    fcntl(fd, F_SETFL, FNDELAY);
 
  printf ( "In Open port fd = %i\n", fd);
 
  return (fd);
}

int readBack1(void)
{
  char buff;
  n = read( fd1, &receive1, 80 );

  // printf("Received Bytes Port 1: %d \n",n);
  int i = 0;
  if(n > 0)
  {
    for(i =0; i < n;i++)
    {
      //printf ( "Buf %d = %c\n",i, buf[i] );
      printf("%c",receive1[i]);
    }
  }
  printf("\n");
  return 1;
}


////////////////////////////////////////////////////////////////////

int open_port4(void)
{
  int fd4; /* File descriptor for the port */
 
  fd4 = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd4 == -1)
    {
      perror("open_port: Unable to open /dev/O1 - ");
    }
  else
    fcntl(fd4, F_SETFL, FNDELAY);
 
  printf ( "In Open port fd = %i\n", fd4);
 
  return (fd4);
}
////////////////////////////////////////////////////////////////////
int readBack4(void)
{
 char buff;
  n = read( fd4, &receive4, 80 );

  // printf("Received Bytes Port 4: %d \n",n);
  int i = 0;
  if(n > 0)
  {
    printf("ESP: ");
    for(i =0; i < n;i++)
    {
      //printf ( "Buf %d = %c\n",i, buf[i] );
      printf("%c",receive4[i]);
    }
  }
  printf("\n");
  return 1;
}

////////////////// SEND ENQUIRY SMS /////////////////////////////
int sendSMS(char *num, int s_available)
{
  
  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  strcpy(sendbuf, "AT\r\n"); // Copy a string in buf char array
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write 9 succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(1);
  readBack1();

  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  strcpy(sendbuf, "AT+CMGS=\""); // Copy a string in buf char array
  strcat(sendbuf,num);
  strcat(sendbuf,"\"\r\n");
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);

  printf ("Write 20 succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(2);

  printf("DATA TEST \n");
  printf("Available Slots: %d\n",s_available);
  sleep(1);

  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  sleep(1);
  strcpy(sendbuf, "PARKING SLOTS "); // Copy a string in buf char array
 

  if(s_available == 0)
  {
    strcat(sendbuf, "NOT AVAILABLE."); // Copy a string in buf char array 
  }
  else
  {
    strcat(sendbuf, "AVAILABLE."); // Copy a string in buf char array 
  }
  

  
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing 24 Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(2);

  // memset(sendbuf, 0, sizeof(sendbuf)); 
  // strcpy(sendbuf, '26'); // Copy a string in buf char array
  // sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  tcflush(fd1, TCIFLUSH);  
  sendbuf[0] = '\x1A';
  sendbuf[1] = '\r';
  sendbytes = 2;
  printf("Writing Command: %c\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write succeed n = %i\n", n );
  sleep(1);
  readBack1();

  sleep(2);

  // memset(sendbuf, 0, sizeof(sendbuf)); 
  // strcpy(sendbuf, '26'); // Copy a string in buf char array
  // sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  // tcflush(fd1, TCIFLUSH);  
  // sendbuf[0] = 26;
  // sendbuf[1] = '\r';
  // sendbuf[2] = '\n';
  // sendbytes = 3;
  // printf("Writing Command: %c\n",sendbuf);
  // n = write(fd1,sendbuf, sendbytes);
  // printf ("Write succeed n = %i\n", n );
  // sleep(1);
  // readBack1();


}

//////////////////// SEND SMS ENTRY ////////////////////////
int sendSMSentry(char *num)
{
  
  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  strcpy(sendbuf, "AT\r\n"); // Copy a string in buf char array
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write 9 succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(1);
  readBack1();

  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  strcpy(sendbuf, "AT+CMGS=\""); // Copy a string in buf char array
  strcat(sendbuf,num);
  strcat(sendbuf,"\"\r\n");
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);

  printf ("Write 20 succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(2);

  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  sleep(1);
  strcpy(sendbuf, "CAR ENTERED IN PARKING"); // Copy a string in buf char array
  
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing 24 Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(2);

  // memset(sendbuf, 0, sizeof(sendbuf)); 
  // strcpy(sendbuf, '26'); // Copy a string in buf char array
  // sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  tcflush(fd1, TCIFLUSH);  
  sendbuf[0] = '\x1A';
  sendbuf[1] = '\r';
  sendbytes = 2;
  printf("Writing Command: %c\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write succeed n = %i\n", n );
  sleep(1);
  readBack1();

  sleep(2);

}


//////////////////// SEND SMS ENTRY ////////////////////////
int sendSMSexit(char *num)
{
  

  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  strcpy(sendbuf, "AT\r\n"); // Copy a string in buf char array
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write 9 succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(1);
  readBack1();

  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  strcpy(sendbuf, "AT+CMGS=\""); // Copy a string in buf char array
  strcat(sendbuf,num);
  strcat(sendbuf,"\"\r\n");
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);

  printf ("Write 20 succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(2);


  tcflush(fd1, TCIFLUSH);
  memset(sendbuf, 0, sizeof(sendbuf)); 
  sleep(1);
  strcpy(sendbuf, "CAR EXITED PARKING"); // Copy a string in buf char array
  
  sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  printf("Writing 24 Command: %s\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write succeed n = %i\n", n );
  sleep(1);
  readBack1();
  sleep(2);

  // memset(sendbuf, 0, sizeof(sendbuf)); 
  // strcpy(sendbuf, '26'); // Copy a string in buf char array
  // sendbytes = strlen(sendbuf); // Store size of buf array in nbytes
  tcflush(fd1, TCIFLUSH);  
  sendbuf[0] = '\x1A';
  sendbuf[1] = '\r';
  sendbytes = 2;
  printf("Writing Command: %c\n",sendbuf);
  n = write(fd1,sendbuf, sendbytes);
  printf ("Write succeed n = %i\n", n );
  sleep(1);
  readBack1();

  sleep(2);

}

////////////////// ULTRASONIC 1 - GATE 1 //////////////////////////////////////////

int getus1()
{
   int count = 0;

  pin_high(8, us1_trig);   //Generate Ultrasonic Pulse
  usleep(10); 
  pin_low(8, us1_trig); 

  before = clock(); //Store current clock time
  while(is_low(8, us1_echo)) //Wait for Echo pin to get ready
  {   
    count+= 1; //Kep loop count for checking possible error
    before = clock(); //Store current clock time
    
    if(count > 5000) //If no response for long time
    {
      printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
      count = 0;
      return -1;
      break;
    }  
  }
  
  while(is_high(8, us1_echo)) //Wait for Echo Pin to go Low - means signal received
  { 
    end = clock();  //Store Current Clock
    count+= 1; //Kep loop count for checking possible error
    if(count > 5000) //If no response for long time
      {
        printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
        count = 0;
        return -1;
        break;
      }
  }
    
  time_taken = end - before;
  distance = time_taken/30; //Calculate distance
  //printf("%d  %d  %d ",before,end,time_taken);
  //printf("%d cm. \n",distance);
  iolib_delay_ms(100);  
  if(distance > threshold) return 0;
  else return 1;
}

////////////////// ULTRASONIC 2 //////////////////////////////////////////
int getus2()
{
  int count = 0;

  pin_high(8, us2_trig);   //Generate Ultrasonic Pulse
  usleep(10); 
  pin_low(8, us2_trig); 

  before = clock(); //Store current clock time
  while(is_low(8, us2_echo)) //Wait for Echo pin to get ready
  {   
    count+= 1; //Kep loop count for checking possible error
    before = clock(); //Store current clock time
    
    if(count > 5000) //If no response for long time
    {
      printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
      count = 0;
      return -1;
      break;
    }  
  }
  
  while(is_high(8, us2_echo)) //Wait for Echo Pin to go Low - means signal received
  { 
    end = clock();  //Store Current Clock
    count+= 1; //Kep loop count for checking possible error
    if(count > 5000) //If no response for long time
      {
        printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
        count = 0;
        return -1;
        break;
      }
  }
    
  time_taken = end - before;
  distance = time_taken/30; //Calculate distance
  //printf("%d  %d  %d ",before,end,time_taken);
  //printf("%d cm. \n",distance);
  iolib_delay_ms(100);  
  if(distance > threshold) return 0;
  else return 1;
}

////////////////// ULTRASONIC 3 //////////////////////////////////////////
int getus3()
{
   int count = 0;

  pin_high(8, us3_trig);   //Generate Ultrasonic Pulse
  usleep(10); 
  pin_low(8, us3_trig); 

  before = clock(); //Store current clock time
  while(is_low(8, us3_echo)) //Wait for Echo pin to get ready
  {   
    count+= 1; //Kep loop count for checking possible error
    before = clock(); //Store current clock time
    
    if(count > 5000) //If no response for long time
    {
      printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
      count = 0;
      return -1;
      break;
    }  
  }
  
  while(is_high(8, us3_echo)) //Wait for Echo Pin to go Low - means signal received
  { 
    end = clock();  //Store Current Clock
    count+= 1; //Kep loop count for checking possible error
    if(count > 5000) //If no response for long time
      {
        printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
        count = 0;
        return -1;
        break;
      }
  }
    
  time_taken = end - before;
  distance = time_taken/30; //Calculate distance
  //printf("%d  %d  %d ",before,end,time_taken);
  //printf("%d cm. \n",distance);
  iolib_delay_ms(100);  
  if(distance > threshold) return 0;
  else return 1;
}

////////////////// ULTRASONIC 4 //////////////////////////////////////////
int getus4()
{
   int count = 0;

  pin_high(8, us4_trig);   //Generate Ultrasonic Pulse
  usleep(10); 
  pin_low(8, us4_trig); 

  before = clock(); //Store current clock time
  while(is_low(8, us4_echo)) //Wait for Echo pin to get ready
  {   
    count+= 1; //Kep loop count for checking possible error
    before = clock(); //Store current clock time
    
    if(count > 5000) //If no response for long time
    {
      printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
      count = 0;
      return -1;
      break;
    }  
  }
  
  while(is_high(8, us4_echo)) //Wait for Echo Pin to go Low - means signal received
  { 
    end = clock();  //Store Current Clock
    count+= 1; //Kep loop count for checking possible error
    if(count > 5000) //If no response for long time
      {
        printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
        count = 0;
        return -1;
        break;
      }
  }
    
  time_taken = end - before;
  distance = time_taken/30; //Calculate distance
  //printf("%d  %d  %d ",before,end,time_taken);
  //printf("%d cm. \n",distance);
  iolib_delay_ms(100);  
  if(distance > threshold) return 0;
  else return 1;
}

////////////////// ULTRASONIC 5 - GATE 2 //////////////////////////////////////////
int getus5()
{
   int count = 0;

  pin_high(8, us5_trig);   //Generate Ultrasonic Pulse
  usleep(10); 
  pin_low(8, us5_trig); 

  before = clock(); //Store current clock time
  while(is_low(8, us5_echo)) //Wait for Echo pin to get ready
  {   
    count+= 1; //Kep loop count for checking possible error
    before = clock(); //Store current clock time
    
    if(count > 5000) //If no response for long time
    {
      printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
      count = 0;
      return -1;
      break;
    }  
  }
  
  while(is_high(8, us5_echo)) //Wait for Echo Pin to go Low - means signal received
  { 
    end = clock();  //Store Current Clock
    count+= 1; //Kep loop count for checking possible error
    if(count > 5000) //If no response for long time
      {
        printf("Kindly Check Sensor - 1 Wiring\n"); //Show error message
        count = 0;
        return -1;
        break;
      }
  }
    
  time_taken = end - before;
  distance = time_taken/30; //Calculate distance
  //printf("%d  %d  %d ",before,end,time_taken);
  //printf("%d cm. \n",distance);
  iolib_delay_ms(100);  
  if(distance > threshold) return 0;
  else return 1;
}

////////////////////// SERVO MOTOR 1 ///////////////////////////
void servo_angle1(int angle) // Function for moving servo to angle
{

int delay = 0; // An integer to store delays

pin_low(9, buzzerPin1);   //Generate Ultrasonic Pulse  

delay =( (5.55 * angle)+1000); // A formula to calculate delay for Servo motor
// We got the delay and other details from Servo motor (SG90) datasheet

printf("Delay1 %d \n",delay); // Print the calculated delay
for(int i=0;i<50;i++) // A for loop running 50 times for setting angle of Servo
{
pin_high(8,servoPin1); // Making the servo connected pin high
usleep(delay); //Delay in microseconds which we calculated above
pin_low(8,servoPin1); // Making the servo connected pin low
usleep(20000-delay); //Delay in microseconds which we calculated above subtracted by 20000

}

iolib_delay_ms(10); // Small delay

pin_low(9, buzzerPin1);   //Generate Ultrasonic Pulse  


}

///////////////////// SERVO MOTOR 2 ///////////////////////////////////////////////
void servo_angle2(int angle) // Function for moving servo to angle
{

int delay = 0; // An integer to store delays

pin_high(9, buzzerPin2);   //Generate Ultrasonic Pulse  

delay =( (5.55 * angle)+1000); // A formula to calculate delay for Servo motor
// We got the delay and other details from Servo motor (SG90) datasheet

printf("Delay2 %d \n",delay); // Print the calculated delay

for(int i=0;i<50;i++) // A for loop running 50 times for setting angle of Servo
{
pin_high(8,servoPin2); // Making the servo connected pin high
usleep(delay); //Delay in microseconds which we calculated above
pin_low(8,servoPin2); // Making the servo connected pin low
usleep(20000-delay); //Delay in microseconds which we calculated above subtracted by 20000

}

iolib_delay_ms(10); // Small delay
pin_low(9, buzzerPin2); 


}
