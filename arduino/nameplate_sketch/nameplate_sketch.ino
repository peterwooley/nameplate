/* LED Nameplate by Jeff Hogan & Peter Wooley */
/* Code originally written by dataTeam in August 2011 */
/* Pong game for LED matrix 16x32 by sure electronics */

#define HT1632_ID_CMD 4		/* ID = 100 - Commands */
#define HT1632_ID_RD  6		/* ID = 110 - Read RAM */
#define HT1632_ID_WR  5		/* ID = 101 - Write RAM */

#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSON  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-0xxx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x18	/* CMD= 0001-10xx-x Use on-chip clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM    0xA0	/* CMD= 101x-PPPP-x PWM duty cycle */

#define X_MAX 31 // 0 based X
#define Y_MAX 15 // 0 based Y
#define CHIP_MAX 4 // Number of HT1632C Chips

/*
 * Set these constants to the values of the pins connected to the SureElectronics Module
 */
static const byte data = 6;    // Data pin (pin 7)
static const byte clk = 9;     // Data pin (pin 2)
static const byte wrclk = 7;   // Write clock pin (pin 5)
static const byte cs = 8;      // Chip Select (pin 1) 
// The should also be a common GND (pins .
// The module with all LEDs like draws about 200mA,
//  which makes it PROBABLY powerable via Arduino +5V


const int bits_l = 64;
const int numChip = 4;

byte pattern[bits_l][numChip];

String cmd = "";

void setup()
{
  Serial.begin(9600);
  
  for(int i=0; i<bits_l; i++) {
    for(int j=0; j<numChip; j++) {
      pattern[i][j] = 0;
    }
  }
  
  pinMode(cs, OUTPUT);
  pinMode(wrclk, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(clk, OUTPUT);
  
  
  for (int i=1; i<=CHIP_MAX; i++) {
    sendcmd(i, HT1632_CMD_SYSDIS); // Disable system
    sendcmd(i, HT1632_CMD_COMS00); // 16*32, PMOS drivers
    sendcmd(i, HT1632_CMD_MSTMD);  // MASTER MODE - Use on-chip clock
    sendcmd(i, HT1632_CMD_SYSON);  // System on - Enable system oscil
    sendcmd(i, HT1632_CMD_LEDON);  // LEDs on 
  }
  
  
  Clear(); // Clear the display
}

void loop ()
{
  //Clear();
  
  //char IMG [] = {0,1,2,3,4}; // Off, Transparent, Green, Red, Orange
  //char DRAWING [] = {4,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,4,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,4,4,0,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,4,4,0,4,4,0,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,4,4,4,0,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,4,4,4,4,4,0,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,4,4,4,4,4,0,4,4,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,4,4,4,4,4,0,4,4,4,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,};
  //drawImage(DRAWING, 32, 16, 0, 0);
  cmd = readline();
  //oSerial.println(cmd);        
  if(cmd.length() > 0) {
    Serial.println("Got cmd: " + cmd);
    Clear();
    char IMG[] = {stringToNumber(cmd)};
    drawImage(IMG, 1, 1, 2, 2); 
  }
  
  delay(1000);
}

void drawImage(const char * image, int height, int width, int x, int y) {
  int pixel;
  for(int a=0; a<width; a++) {
   for(int b=0; b<height; b++) {
     pixel = image[a*height+b];
     switch(pixel) {
       //case 0:
         //Plot(x+b, y+a, 0);
         //break;
       case 2:
         Plot(x+b, y+a, 0);
         break;
       case 3:
         Plot(x+b, y+a, 1);
         break;
       case 4:
         Plot(x+b, y+a, 0);
         Plot(x+b, y+a, 1);
     }
   }
 }
  
}

String readline() {
  String readString = "";
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }

  return readString;
}

int stringToNumber(String thisString) {
  int i, value, length;
  length = thisString.length();
  char blah[(length+1)];
  for(i=0; i<length; i++) {
    blah[i] = thisString.charAt(i);
  }
  blah[i]=0;
  value = atoi(blah);
  return value;
}

//**************************************************************************************************
//Function Name: chipselect
//Function Feature: choose the chipset in which work
//Input Argument: number of the chip
//Output Argument: void
//**************************************************************************************************
void chipselect(int chip){

  if(chip<0){
    digitalWrite(cs, LOW); 
    for(int tmp=0; tmp<CHIP_MAX; tmp++) {
      digitalWrite(clk, HIGH);
      digitalWrite(clk, LOW);
    }
  }
  else{

    digitalWrite(cs, HIGH);
    for(int tmp=0; tmp<CHIP_MAX; tmp++) {
      digitalWrite(clk, HIGH);
      digitalWrite(clk, LOW);
    }

    if(chip>0){
      digitalWrite(cs, LOW);
      digitalWrite(clk, HIGH);
      digitalWrite(clk, LOW);
      digitalWrite(cs, HIGH);
      for(int tmp=1 ; tmp<chip; tmp++) {
        digitalWrite(clk, HIGH);
        digitalWrite(clk, LOW);
      }
    }
  }
    
}

//**************************************************************************************************
//Function Name: clear
//Function Feature: clear display
//Input Argument: void
//Output Argument: void
//**************************************************************************************************
void Clear()
{
  char i;

  for (int j=1;j<=CHIP_MAX;j++) {
    chipselect(j);
    writebits(HT1632_ID_WR, 2);  // send ID: WRITE to RAM
    writebits(0, 6); // Send address
    for (i=0; i<64/2; i++) // Clear entire display
      writebits(0, 7); // send 8 bits of data
    chipselect(0);

  }
  for(int i=0; i<bits_l; i++) {
    for(int j=0; j<numChip; j++) {
      pattern[i][j] = 0;
    }
  }
}

//**************************************************************************************************
//Function Name: writebits
//Function Feature: Write bits (up to 8) to h1632 on pins data, wrclk
//                  Chip is assumed to already be chip-selected
//                  Bits are shifted out from MSB to LSB
//Input Argument: bits: bits to send
//	        length: length of the bits to send
//Output Argument: void
//**************************************************************************************************
void writebits (byte bits, int length)
{
  
  for (int i=length; i>=0; i--) {
    digitalWrite(wrclk, LOW);
    if (bits & 1<<i) {
      digitalWrite(data, HIGH);
    }
    else {
	digitalWrite(data, LOW);
    }
    digitalWrite(wrclk, HIGH);
  }
}


//**************************************************************************************************
//Function Name: sendcmd
//Function Feature: Send a command to the ht1632 chip.
//                  Select 1 0 0 c7 c6 c5 c4 c3 c2 c1 c0 xx Free
//Input Argument: chipNo: the chip you want to send data
//               command: consists of a 3-bit "CMD" ID, an 8bit command, and
//                        one "don't care bit".
//Output Argument: void
//**************************************************************************************************
void sendcmd (byte chipNo, byte command)
{
  chipselect(chipNo);
  writebits(HT1632_ID_CMD, 2);  // send 3 bits of id: COMMMAND
  writebits(command, 7);        // send the actual command
  writebits(0, 1); 	        /* one extra dont-care bit in commands. */
  chipselect(0);
}

byte address=0;

void Plot (int X, int Y){
  Plot(X,Y,0);
}

void Plot (int X, int Y, int color)
{
 // color:
 // 0 -> green
 // 1 -> red

 // check the inputs value
 // in case of error return nothing
 if(color > 1 || color < 0)
    return;
 if(X<0 || X>31 || Y<0 || Y>15)  
    return;      

  // variables declaration
  int x = 0;
  int y = 0;
  int nChip = 0;
   
  

  // transform the X/Y coordinates to the number of the microchip
  // that controls the region (1,2,3 or 4) the LED you want to
  // light up
  nChip = 1 + X/16 + (Y>7?2:0);
  chipselect(nChip); // call the function chipselect to send the
				 // information to the matrix
  
  // after the selection of the chip we need just the coordinate
  // for 1 of them, so we need to change the coordinate system
  x = X%16; // columns
  y = Y%8;  // rows
  
  

  // from X/Y to address
  address = ((x*2+y/4)) + color*32;

  // from Y to pattern
  pattern[address][nChip] = pattern[address][nChip] | ((1<<(3-y%4)));      
  
  
  writebits(HT1632_ID_WR, 2);  		   // send ID: WRITE to RAM
  writebits(address, 6);       		   // Send address
  writebits(pattern[address][nChip], 3);   // send 4 bits of data
  
}


