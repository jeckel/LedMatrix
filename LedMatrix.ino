#include <TimerOne.h>

/**
 * Using ShiftOut IC :
 * http://arduino.cc/en/Tutorial/ShiftOut
 *
 * And Demultiplexer : SN74S138N
 * http://pdf1.alldatasheet.com/datasheet-pdf/view/27992/TI/SN74S138N.html
 */

byte addrY1 = 3;
byte addrY2 = 4;
byte addrY3 = 5;

//Pin connected to ST_CP of 74HC595
byte latchPin = 8;
//Pin connected to SH_CP of 74HC595
byte clockPin = 12;
////Pin connected to DS of 74HC595
byte dataPin = 11;

#define addrXPin 6

byte matrix[] = {
    0, 0, 0, 0, 0, 0, 0, 0};

byte row = 0;

long previousMillis = 0;
long interval = 30; 

#define RANDOM_EFFECT  0
#define GO_UP_EFFECT   1
#define ROTATOR_EFFECT 2

#define NB_EFFECTS     3

byte current_effect = RANDOM_EFFECT;

void setup() 
{
    //  Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    //  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

    Timer1.initialize(1000);
    Timer1.attachInterrupt( timeredDisplay ); // attach the service routine here


    pinMode(addrY1, OUTPUT);
    pinMode(addrY2, OUTPUT);
    pinMode(addrY3, OUTPUT);

    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    
    pinMode(13, OUTPUT);  
    randomSeed(analogRead(0));
    for(int i = 0; i <= 7; i++) {
        matrix[i] = random(0, 256);
        //    matrix[i] = 1 << i;
    }
}

void loop()
{
    switch(current_effect)
    {
        case RANDOM_EFFECT  : randomEffect();  break;
        case GO_UP_EFFECT   : goUpEffect();    break;
        case ROTATOR_EFFECT : rotatorEffect(); break;
    }
}

int getNewInterval()
{
    /*
  // read the input on analog pin 0:
     int sensorValue = analogRead(A0);
     // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
     float voltage = sensorValue * (5.0 / 1023.0);
     */
//    return (1023 - analogRead(A0)) / 4;
    return 20;
}

/**
 * Display the led Matrix
 */
void timeredDisplay()
{
    unsigned long currentMillis = millis();
    int interval_between_effects = 5 * 1000;         // 5 seconds
    
    // First, set all pins off to avoid unwanted persistance
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);  
    digitalWrite(latchPin, HIGH);

    // Update address of next row on Demux
    digitalWrite(addrY1, (row & 1) ? HIGH : LOW);
    digitalWrite(addrY2, (row & 2) ? HIGH : LOW);
    digitalWrite(addrY3, (row & 4) ? HIGH : LOW);

    // Set all pin to the wanted value
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, matrix[row]);  
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    
    row = (row + 1) % 8;

    // Change effect
    if(currentMillis - previousMillis > interval_between_effects) {
        // save the last time you blinked the LED 
        previousMillis = currentMillis;
        current_effect = random(0, NB_EFFECTS);
    }
}

/**
 * Randomly turn on and off leds
 */
void randomEffect()
{
    matrix[random(0, 8)] = random(0, 256);
    delay(getNewInterval());
}

/**
 * Make a line that go from bottom to up
 */
void goUpEffect()
{
    for(int i = 0; i < 4; i++)
    {
        matrix[3 - i] = 255;
        matrix[i + 4] = 255;
        matrix[3 - (i + 3) % 4] = 0;
        matrix[((i + 3) % 4) + 4] = 0;
        delay(getNewInterval() * 4);
    }
}

/**
 * Two small lines rotating around the head
 */
void rotatorEffect()
{
    for(int i = 0; i < 8; i++)
    {
        for(byte x = 0; x < 8; x++)
        {
            matrix[x] = (x < 4) ? 1 << i : 1 << 8-i;
        }
        delay(getNewInterval() * 2);
    }
}
