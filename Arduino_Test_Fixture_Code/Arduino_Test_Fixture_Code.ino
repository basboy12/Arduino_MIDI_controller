#define octaveUpBtn A2
#define octaveDownBtn A3
#define ldrSensor A0
#define ldrBtn A4
#define statusLed A5


void setup() {
  pinMode(octaveUpBtn, INPUT);
  pinMode(octaveDownBtn, INPUT);
  pinMode(ldrSensor, INPUT);
  pinMode(ldrBtn, INPUT);
  pinMode(statusLed, OUTPUT);

  Serial.begin(9600);
  Serial.println("Initiating test fixture..");

  Serial.println();
  Serial.print("Press the OctaveUpButton");
  while (digitalRead(octaveUpBtn) == LOW) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("OctaveUpButton OK");

  Serial.println();
  Serial.print("Press the OctaveDownButton");
  while (digitalRead(octaveDownBtn) == LOW) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("OctaveDownButton OK");

  Serial.println();
  Serial.print("Press the ldrButton");
  while (digitalRead(ldrBtn) == LOW) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("ldrButton OK");

  int timer = millis();
  while (millis() - timer < 2500) {
    Serial.print("Testing ldr, move your hand up and down  |  Recieved value: ");
    Serial.println(analogRead(ldrSensor));
  }

  Serial.println("");
  Serial.println("Swichting on LED");
  Serial.println("");
  digitalWrite(statusLed, HIGH);
  Serial.print("Test fixture complete: If the LED is on, all connections succeeded!");
}

void loop() {
  // Nothing to see here...
}
