/*
  Author : Hak Se Engineer
  Date : 04/11/2026
  
  Ultrasonic Flappy - Arduino Nano + HC-SR04
  Wiring:  VCC -> 5V | GND -> GND | TRIG -> D9 | ECHO -> D10
  Output:  "D:23.4\n" (filtered distance in cm) about 33 times per second @ 115200 baud

  Filtering pipeline (3 stages):
   1. Range gate  - echoes outside 2..200 cm or timeouts are discarded
   2. Spike gate  - a sudden jump > MAX_JUMP is ignored for up to 3 readings;
                    if it persists it is accepted as real movement
   3. Median(5) then EMA - median kills isolated glitches, EMA smooths jitter
*/
#define TRIG 9
#define ECHO 10

const float MIN_CM = 2.0, MAX_CM = 200.0;
const float MAX_JUMP = 10.0;   // cm per reading considered suspicious
const float ALPHA = 0.45;      // EMA weight (higher = faster, noisier)
const uint8_t N = 5;           // median window
const uint16_t PERIOD = 30;    // ms between pings (HC-SR04 needs >= 60ms between *echoes*, 30ms is fine at short range)

float win[N];
uint8_t cnt = 0, head = 0, spikes = 0;
float filt = 0;
bool started = false;
unsigned long lastPing = 0;

float ping() {
  digitalWrite(TRIG, LOW);  delayMicroseconds(3);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long us = pulseIn(ECHO, HIGH, 12000UL);  // ~2 m timeout
  if (us == 0) return -1;
  return us * 0.01715f;                              // 343 m/s, round trip
}

float median() {
  float t[N];
  for (uint8_t i = 0; i < cnt; i++) t[i] = win[i];
  for (uint8_t i = 1; i < cnt; i++) {                // insertion sort
    float k = t[i]; int8_t j = i - 1;
    while (j >= 0 && t[j] > k) { t[j + 1] = t[j]; j--; }
    t[j + 1] = k;
  }
  return t[cnt / 2];
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(115200);
}

void loop() {
  if (millis() - lastPing < PERIOD) return;
  lastPing = millis();

  float r = ping();
  bool ok = (r >= MIN_CM && r <= MAX_CM);                       // stage 1
  if (ok && started && fabs(r - filt) > MAX_JUMP && spikes < 5) { // stage 2
    spikes++;
    ok = false;
  } else if (ok) {
    spikes = 0;
  }

  if (ok) {                                                     // stage 3
    win[head] = r;
    head = (head + 1) % N;
    if (cnt < N) cnt++;
    float m = median();
    filt = started ? filt + ALPHA * (m - filt) : m;
    started = true;
  }
  if (started) {
    Serial.print("D:");
    Serial.println(filt, 1);
  }
}
