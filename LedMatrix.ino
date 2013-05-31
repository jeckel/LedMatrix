#include <TimerOne.h>

int addrY1 = 3;
int addrY2 = 4;
int addrY3 = 5;

int addrX1 = 6;
int addrX2 = 7;
int addrX3 = 8;
int addrX4 = 9;
int addrX5 = 10;
int addrX6 = 11;
int addrX7 = 12;
int addrX8 = 13;

#define addrXPin 6

int matrix[] = {
    0, 0, 0, 0, 0, 0, 0, 0};

int row = 0;

long previousMillis = 0;
long interval = 30; 



void setup() 
{
    // Initialize the digital pin as an output.
    // Pin 13 has an LED connected on most Arduino boards
    //  pinMode(13, OUTPUT);    

    //  Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    //  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

    Timer1.initialize(1000);
    Timer1.attachInterrupt( timeredDisplay ); // attach the service routine here


    pinMode(addrY1, OUTPUT);
    pinMode(addrY2, OUTPUT);
    pinMode(addrY3, OUTPUT);
    pinMode(addrX1, OUTPUT);
    pinMode(addrX2, OUTPUT);
    pinMode(addrX3, OUTPUT);
    pinMode(addrX4, OUTPUT);
    pinMode(addrX5, OUTPUT);
    pinMode(addrX6, OUTPUT);
    pinMode(addrX7, OUTPUT);
    pinMode(addrX8, OUTPUT);

    randomSeed(analogRead(0));
    for(int i = 0; i <= 7; i++) {
        matrix[i] = random(0, 256);
        //    matrix[i] = 1 << i;
    }
}

void loop()
{
    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis > interval) {
        // save the last time you blinked the LED 
        previousMillis = currentMillis;
        matrix[random(0, 8)] = random(0, 256);
        interval = getNewInterval();
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
    return 10;
}

/**
 * Display the led Matrix
 */
void timeredDisplay()
{
    // For each row
    //  for(int y = 0; y <= 7; y++) {
    // First, set all pins off to avoid unwanted persistance
    for(int x = 0; x <= 7; x++) {
        digitalWrite(addrXPin + x, LOW);
    }

    // Update address of next row on Demux
    digitalWrite(addrY1, (row & 1) ? HIGH : LOW);
    digitalWrite(addrY2, (row & 2) ? HIGH : LOW);
    digitalWrite(addrY3, (row & 4) ? HIGH : LOW);

    // Set all pin to the wanted value
    for(int x = 0; x <= 7; x++) {
        digitalWrite(addrXPin + x, ((matrix[row] >> x) % 2) ? HIGH : LOW);
    }

    // Small delay for persistance
    //    delay(1);
    //  }
    row = (row + 1) % 8;
}

