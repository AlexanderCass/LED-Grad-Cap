// Alex Cass LED Graduation Cap Code
// Used May 10th 2024
// Final product: Skip to around 23:50 in this video: https://www.youtube.com/watch?v=F_e8GyRCm2M
// Hardware used: 32x8 Addressable RGB LED Matrix Panel + Arduino Uno + External USB Battery

// The FastLED library provides an easy interface to the addressable "Neopixel" LEDs 
#include "FastLED.h"
#define NUM_LEDS 256 // 32 * 8
#define LED_PIN 6 // Adruino Uno pin 6 has PWM capabilities and worked with a custom shield I made up
CRGB leds[NUM_LEDS]; 

/*
 * Note: Many of these global variables should actually be local, just worked out this way with the time crunch (made this in a couple hours)
 * In the future it is possible to move all of these constant variables to flash ROM and read them into RAM only when needed.
 * ...Maybe I'll figure that out by the tiem I graduate with my masters lol.
 */

// Custom font for the letters to go by: Each letter is 5 bytes and can be easily changed. 
// Letter locations in the fontData array are set in stone, order is a-zA-Z lowercase and uppercase.
const byte fontData[] = {0x02,0x15,0x15,0x15,0x0F,0xFF,0x11,0x11,0x11,0x0E,0x0E,0x11,0x11,0x11,0x0A,0x0E,0x11,0x11,0x11,0xFF,0x0E,0x15,0x15,0x15,0x0C,0x10,0x7F,0x90,0x80,0x40,0x09,0x15,0x15,0x15,0x0E,0xFF,0x10,0x10,0x10,0x0F,0x00,0x5E,0x01,0x00,0x00,0x00,0x02,0x01,0x5E,0x00,0xFF,0x0C,0x0A,0x11,0x00,0x00,0xFE,0x01,0x00,0x00,0x1F,0x10,0x0F,0x10,0x0F,0x10,0x0F,0x10,0x10,0x0F,0x0E,0x11,0x11,0x11,0x0E,0x1F,0x14,0x14,0x14,0x08,0x08,0x14,0x14,0x1F,0x01,0x1F,0x08,0x10,0x10,0x08,0x09,0x15,0x15,0x15,0x12,0x00,0x10,0xFE,0x11,0x00,0x1E,0x01,0x01,0x1E,0x01,0x1C,0x02,0x01,0x02,0x1C,0x1E,0x01,0x02,0x01,0x1E,0x11,0x0A,0x04,0x0A,0x11,0x18,0x05,0x05,0x05,0x1E,0x11,0x13,0x15,0x19,0x11,0x3F,0x50,0x90,0x50,0x3F,0xFF,0x91,0x91,0x71,0x0E,0x7E,0x81,0x81,0x81,0x42,0xFF,0x81,0x81,0x42,0x3C,0xFF,0x91,0x91,0x81,0x81,0xFF,0x90,0x90,0x90,0x80,0x7E,0x81,0x89,0x89,0x4E,0xFF,0x10,0x10,0x10,0xFF,0x00,0x81,0xFF,0x81,0x00,0x02,0x01,0x81,0x81,0xFE,0xFF,0x10,0x20,0x5C,0x83,0xFF,0x01,0x01,0x01,0x00,0xFF,0x40,0x30,0x40,0xFF,0xFF,0x40,0x20,0x10,0xFF,0x7E,0x81,0x81,0x81,0x7E,0xFF,0x90,0x90,0x90,0x60,0x7E,0x81,0x85,0x82,0x7D,0xFF,0x90,0x90,0x98,0x67,0x61,0x91,0x91,0x91,0x8E,0x80,0x80,0xFF,0x80,0x80,0xFE,0x01,0x01,0x01,0xFE,0xE0,0x1E,0x01,0x1E,0xE0,0xFE,0x01,0x0E,0x01,0xFE,0xC3,0x2C,0x10,0x2C,0xC3,0xC0,0x20,0x1F,0x20,0xC0,0x87,0x89,0x91,0xA1,0xC1,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00,0x05,0x06,0x00,0x00,0x00,0xFD,0x00,0x00,0x00,0x00,0xC3,0x00,0x00,0x00,0x00};

// Messages to be displayed by the ScrollText function. Ideally this should have just passed a pointer, but again I'm just happy it worked with the time crunch.
const char msg1[] = "   THANK YOU to my FAMILY... MOM  DAD  WILL  AMANDA  OLIVIA  NANA  PAPA ... LOVE YOU !!!     ";
const char msg2[] = "   THANK YOU to my FRIENDS... CALVIN  DAVID  GABBY  JACOB  JARED  JENI  JEREMY  JULIA  JUNIOR  MATT  NICK  SANTI  YUAN        ";
const char msg3[] = "   ALEX CASS COMPUTER SCIENCE     ";

// The "CASS" image, hardcoded into a byte array.
const byte cass[] = {/*
  S*/10,11,12,15,21,20,26,29,37,34,44,45,55,52,51,50,56,57,58,59,60,61,62,63,/*
  s*/71,70,69,68,67,66,65,64,74,75,76,79,85,84,90,93,101,98,108,109,119,116,115,114,120,121,122,123,124,125,126,127,/*
  a*/135,134,133,132,131,130,129,128,136,154,155,158,159,165,164,161,160,183,184,185,186,187,188,189,190,191,/*
  c*/199,198,197,196,195,194,193,192,200,203,204,207,212,211,218,219,220,221,229,228,227,226,247,240,248,249,250,251,252,253,254,255};
  
// The two frames of animation for the space invaders.
byte up[]={25,26,27,28,36,35,34,32,40,42,43,44,45,46,54,53,51,50,58,59,60,61,69,68,67,66,74,75,76,77,86,85,83,82,88,90,91,92,93,93,94,100,99,98,96,105,106,107,108,150,149,148,147,155,156,157,159,167,165,164,163,162,161,169,170,172,173,181,180,179,178,186,187,188,189,197,196,195,194,201,202,204,205,215,213,212,211,210,209,219,220,221,223,230,229,228,227};
byte down[]={28,29,30,36,35,40,42,43,44,45,46,54,53,51,50,48,58,59,60,61,63,69,68,67,66,74,75,76,77,79,86,85,83,82,80,88,90,91,92,93,94,100,99,108,109,110,147,146,145,155,156,167,165,164,163,162,161,169,170,172,173,175,181,180,179,178,176,186,187,188,189,197,196,195,194,192,201,202,204,205,207,215,213,212,211,210,209,219,220,227,226,225};

// Binghamton University B "2024" image. First array is displayed in white, second is displayed in green.
const byte bing[]={9,10,11,12,13,14,19,25,26,28,36,35,53,49,57,59,62,70,67,65,74,77,78,90,91,92,93,102,97,105,110,117,116,115,114,133,129,137,139,142,150,147,145,154,157,158,183,182,184,185,186,188,189,199,197,196,195,194,193,200,203,206,215,214,213,212,209,216,218,219,220,222,231,228,227,226,225,237,238,241};
const byte bug[]={169,181,187,179,178,190,192,207,208,223,224,239};



// The "setup" function runs once when the arduino board starts.
void setup() {
  // FastLED library setup stuff. LEDs used are technically "WS2811" but are branded as "Neopixel".
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
  
  // Initialize the matrix to all black for a moment. Read somewhere a long time ago that this is good to do for the longevity of the board and to not freak out your power supply.
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(300);
}

// This "loop" sequences everything and runs forever.
void loop() {
  BU();
  ScrollText(1, 93);
  Cass();
  ScrollText(2, 123);
  SpaceInvaders();
  ScrollText(3, 34);
}



// ScrollText function, takes in a message number which it looks up, and the message length.
void ScrollText(int messageNum, int messageLength){

  int msgIndex = 0;

  for(int z=0; z<messageLength; z++){
  
    // get char from string at index
    char temp;
    // Really shoulda just used a pointer whoops
    switch (messageNum) {
      case 1:
        temp = msg1[msgIndex];
      break;
      case 2:
        temp = msg2[msgIndex];
      break;
      case 3:
        temp = msg3[msgIndex];
      break;
      default: 
        temp = ' ';
      break;
    }
  
    bool notFound = true;
    //convert ascii number to fontData number
    
    //handle capital letters
    if(temp>=65&&temp<=90){
      temp = temp-39;
      notFound = false;
    }
  
    //handle lowercase letters
    if(temp>=61&&temp<=122){
        temp = temp-97;
        notFound = false;
    }
    
    //switch statement for special chars... Mostly untested lol
    if(notFound){
      switch(temp){
        case 46:
          temp=53;
          break;
        case 44:
          temp=54;
          break;
        case 33:
          temp=55;
          break;
        case 58:
          temp=56;
          break;
        case 63:
          temp=0;
          break;
        default: //default anything else to a space
          temp=52;
          break;
      }
    }

    // This is currently using the fixed width approach, where every displayed letter is 5 pixels wide.
    // Hopefully this can be easily adapted to a variable width model in the future.
    for(int k=0; k<6; k++){
      
      // Draw background
      allblack();
      
      // Mask each bit one at a time and draw them
      byte mask = 1;
      for(int i=0;i<8;i++){
        if(fontData[k+(5*temp)] & mask && k!=5) // Blank out the last pixel, treating it as a space between letters
          leds[i] = CRGB::White;
          //leds[i]=CHSV(32*i,255,255); // Rainbow effect. I didn't use it because it made it harder to read from a distance
        mask = mask << 1;
      }
     
      // Display and shift
      FastLED.show();
      tickershift();
      delay(30); // Controls the speed of the text moving across the matrix
    }
    
    msgIndex++;
    if(msgIndex>messageLength)
      msgIndex=0;

  }
}

// Displays the BU B and 2024 screen.
void BU()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for(int i=0;i<80;i++){leds[bing[i]]=CRGB::White;};
  for(int i=0;i<12;i++){leds[bug[i]]=CRGB::Green;};
  FastLED.show();
  delay(5000);
}

// Flashes and displays the CASS image
void Cass()
{
  for(int j=0; j<4; j++){
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(500);

    // It takes fewer bytes to store the black pixels of the "CASS" text than the white ones, so this is kinda backwards
    fill_solid(leds, NUM_LEDS, CRGB::White); // You can actually change this 'background' color to anything and it works
    for(int i=0;i<114;i++){leds[cass[i]]=CRGB::Black;};
    allblack();
    FastLED.show();
    delay(500);
  }
  delay(5000);
}

// Display two frames of space invader animation
void SpaceInvaders()
{
  for(int j=0; j<5; j++){
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for(int i=0;i<97;i++){leds[up[i]]=CRGB::Cyan;};
    FastLED.show();
    delay(500);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for(int i=0;i<92;i++){leds[down[i]]=CRGB::Cyan;};
    FastLED.show();
    delay(500);
  }
}

// The LED Matrix panel I own is wired up in a snaking pattern
// Came up with this routine in highschool and it still works so I haven't touched it haha
// Basically just a really weird shift register in an array
void tickershift()
{
  for(int i=255; i>0; i=i-16){
    leds[i]=leds[i-15];
    leds[i-1]=leds[i-14];
    leds[i-2]=leds[i-13];
    leds[i-3]=leds[i-12];
    leds[i-4]=leds[i-11];
    leds[i-5]=leds[i-10];
    leds[i-6]=leds[i-9];
    leds[i-7]=leds[i-8];

    leds[i-15]=leds[i-16];
    leds[i-14]=leds[i-17];
    leds[i-13]=leds[i-18];
    leds[i-12]=leds[i-19];
    leds[i-11]=leds[i-20];
    leds[i-10]=leds[i-21];
    leds[i-9]=leds[i-22];
    leds[i-8]=leds[i-23];
  }
}

// Helper function to make the rightmost column of pixels black.
void allblack()
{
  for(int i=0;i<8;i++)
    leds[i]=CRGB::Black;
}

// Helper function to make the rightmost column of pixels white.
void allwhite()
{
  for(int i=0;i<8;i++)
    leds[i]=CRGB::White;
}
