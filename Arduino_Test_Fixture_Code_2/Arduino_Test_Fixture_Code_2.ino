#include <CapacitiveSensor.h>

#define numberOfInputs 12   // define the number of inputs. Do not choose more inputs than capacitive sensors connected to your Arduino. It will cause an error.
#define THRESSHOLD 260      // define the thresshold at which midi messages will be activated.
#define DEFAULT_OCTAVE 5    // Set the default octave when switching on your Arduino. 5 = note C5, 4 = note C4, etc.
#define RES 50


#define statusLed A5
#define sendPin A1          // define the send pin. 
#define firstRecievePin 2   // define the first receive pin.


CapacitiveSensor * capSensor[numberOfInputs];
unsigned long previousMillis;
byte octaveValue = DEFAULT_OCTAVE;
boolean isActive[numberOfInputs];

//________________________________________________________________________________________________________________________________________
void setup() {
  Serial.begin(9600);
  pinMode(statusLed, OUTPUT);

  for (int i = 1; i <= numberOfInputs; i++) {
    capSensor[i - 1] =  new CapacitiveSensor(sendPin, numberOfInputs + firstRecievePin - i);
  }
}

//________________________________________________________________________________________________________________________________________
void loop() {
  digitalWrite(statusLed, HIGH); // This line ensures the status LED gets lit after each loop

  for ( int i = 0; i < numberOfInputs; i++) {
    if (capSensor[i]->capacitiveSensor(RES) > THRESSHOLD && !isActive[i] ) {
      sendMidi(0x90, 12 * octaveValue + i, capSensor[i]->capacitiveSensor(RES));
      isActive[i] = true;
    }
    if (capSensor[i]->capacitiveSensor(RES) < THRESSHOLD && isActive[i]) {
      sendMidi(0x90, 12 * octaveValue + i, 0);
      isActive[i] = false;
    }
  }
}


//________________________________________________________________________________________________________________________________________
void sendMidi(int statusbyte, int databyte1, int databyte2) {
  digitalWrite(statusLed, LOW);

  if (databyte2 > 0) {
    Serial.print("Note ");
    Serial.print(databyte1);
    Serial.print(" is active:  ");
    Serial.println(databyte2);
  }
  else {
    Serial.print("Note ");
    Serial.print(databyte1);
    Serial.println(" is off");
  }
}
