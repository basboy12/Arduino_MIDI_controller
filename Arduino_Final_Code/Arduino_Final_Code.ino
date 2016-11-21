#include <CapacitiveSensor.h>

#define numberOfInputs 12   // define the number of inputs. Do not choose more inputs than capacitive sensors connected to your Arduino. It will cause an error.
#define THRESSHOLD 260      // define the thresshold at which midi messages will be activated.
#define DEFAULT_OCTAVE 5    // Set the default octave when switching on your Arduino. 5 = note C5, 4 = note C4, etc.
#define RES 50

#define octaveUpBtn A2
#define octaveDownBtn A3
#define ldrSensor A0
#define ldrBtn A4
#define statusLed A5
#define sendPin A1          // define the send pin. 
#define firstRecievePin 2   // define the first receive pin.

boolean midiMode = false;   // if midiMode = false, the Arduino will send on and off messages via the serial monitor.
//Set the value to true if you want the Arduino to act as a Midi device

CapacitiveSensor * capSensor[numberOfInputs];
unsigned long previousMillis;
byte octaveValue = DEFAULT_OCTAVE;
boolean isActive[numberOfInputs];
boolean lastUpBtnState;
boolean lastDownBtnState;
boolean lastLdrBtnState;
boolean ldrIsActive;
boolean ledState;
int ldrMin = 200;
int ldrMax = 0;
int ldrValue;
int lastLdrValue;

//________________________________________________________________________________________________________________________________________
void setup() {
  if (midiMode) {
    Serial.begin(31250);
  }
  else {
    Serial.begin(9600);
  }

  pinMode(octaveUpBtn, INPUT);
  pinMode(octaveDownBtn, INPUT);
  pinMode(ldrSensor, INPUT);
  pinMode(ldrBtn, INPUT);
  pinMode(statusLed, OUTPUT);

  while (millis() < 5000) { // ldr calibration code, move your hand up and down during the 5 seconds that the led flashes, in order to calibrate the ldr sensor.
    ldrValue = analogRead(ldrSensor);
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 150) { // statusLed blinks with 150 milliseconds intervals to indicate the calibration is active.
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(statusLed, ledState);
    }

    if (ldrValue > ldrMax) {
      ldrMax = ldrValue;
    }

    if (ldrValue < ldrMin) {
      ldrMin = ldrValue;
    }
  }

  for (int i = 1; i <= numberOfInputs; i++) {
    capSensor[i - 1] =  new CapacitiveSensor(sendPin, numberOfInputs + firstRecievePin - i);
  }
}

//________________________________________________________________________________________________________________________________________
void loop() {
  digitalWrite(statusLed, HIGH); // This line ensures the status LED gets lit after each loop

  boolean upBtnState = digitalRead(octaveUpBtn);
  if (upBtnState == HIGH && upBtnState != lastUpBtnState) {
    octaveValue = min(octaveValue + 1, 10);
  }
  lastUpBtnState = upBtnState;

  boolean downBtnState = digitalRead(octaveDownBtn);
  if (downBtnState == HIGH && downBtnState != lastDownBtnState) {
    octaveValue = max(octaveValue - 1, 0);
  }
  lastDownBtnState = downBtnState;

  boolean ldrBtnState = digitalRead(ldrBtn);
  if (ldrBtnState == HIGH && ldrBtnState != lastLdrBtnState) {
    ldrIsActive = !ldrIsActive;
  }
  lastLdrBtnState = ldrBtnState;

  if (ldrIsActive) {
    ldrValue = constrain(map(analogRead(ldrSensor), ldrMin , ldrMax , 0, 127), 0, 127); // constrain the values to 0 - 127
    if (abs(ldrValue - lastLdrValue) >= 1) {                                            // limit the sensor output by using a thresshold
      sendMidi(0xB0, 0, ldrValue);
      lastLdrValue = ldrValue;
    }
  }

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

  if (midiMode) {
    Serial.write(statusbyte); //send noteOn, noteOff or Control Change command
    Serial.write(databyte1); //send pitch or channel data
    Serial.write(databyte2); //send velocity/ value data
  }

  else {
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
}







