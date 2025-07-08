#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED1_PIN 0  // PB0
#define LED2_PIN 1  // PB1

volatile bool wdtTriggered = false;
unsigned long wakeCount = 0;

const unsigned long ON_SECONDS = 6L * 60L * 60L;
const unsigned long OFF_SECONDS = 18L * 60L * 60L;
const unsigned long WDT_INTERVAL = 8;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  setupWatchdog();
}

void loop() {
  // Fade in to 25% PWM brightness (64/255)
  for (int i = 0; i <= 64; i++) {
    analogWrite(LED1_PIN, i);
    analogWrite(LED2_PIN, i);
    delay(30); // Adjust fade speed
  }

  // Stay on for 6 hours
  sleepForSeconds(ON_SECONDS);

  // Fade out to 0
  for (int i = 64; i >= 0; i--) {
    analogWrite(LED1_PIN, i);
    analogWrite(LED2_PIN, i);
    delay(30);
  }

  // Fully off
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);

  // Sleep for 18 hours
  sleepForSeconds(OFF_SECONDS);
}

void sleepForSeconds(unsigned long seconds) {
  wakeCount = 0;
  unsigned long target = seconds / WDT_INTERVAL;
  while (wakeCount < target) {
    enterSleep();
  }
}

void enterSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

void setupWatchdog() {
  cli();
  MCUSR = 0;
  WDTCR |= (1 << WDCE) | (1 << WDE);
  WDTCR = (1 << WDIE) | (1 << WDP3); // 8s
  sei();
}

ISR(WDT_vect) {
  wdtTriggered = true;
  wakeCount++;
}
