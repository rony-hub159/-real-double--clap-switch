int relayPin = 13;
int sensorPin = 2; // Must be interrupt-capable (e.g., 2 or 3 on Arduino Uno)

volatile bool clapDetected = false;
volatile unsigned long lastClapTime = 0;

const unsigned int clapInterval = 800; // Max delay between two claps (in ms)

unsigned long firstClapTime = 0;
bool waitingForSecondClap = false;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay OFF if active LOW

  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), soundISR, FALLING);
}

void loop() {
  if (clapDetected) {
    clapDetected = false; // Reset flag after detecting

    unsigned long now = millis();

    if (waitingForSecondClap && (now - firstClapTime <= clapInterval)) {
      // Second clap in time — toggle relay
      digitalWrite(relayPin, digitalRead(relayPin) == HIGH ? LOW : HIGH);
      waitingForSecondClap = false;
    } else {
      // First clap — start timer
      waitingForSecondClap = true;
      firstClapTime = now;
    }
  }

  // Timeout if second clap doesn’t come soon enough
  if (waitingForSecondClap && (millis() - firstClapTime > clapInterval)) {
    waitingForSecondClap = false;
  }
}

// Interrupt Service Routine
void soundISR() {
  unsigned long now = millis();
  if (now - lastClapTime > 30) { // Simple debounce
    clapDetected = true;
    lastClapTime = now;
  }
}
