#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED1_PIN 0  // PB0
#define LED2_PIN 1  // PB1

volatile unsigned long wakeCount = 0; // Changed 'wdtTriggered' to a single counter

const unsigned long ON_SECONDS = 6L * 60L * 60L;     // 21600 seconds
const unsigned long OFF_SECONDS = 18L * 60L * 60L;   // 64800 seconds
const unsigned long WDT_INTERVAL_S = 8;              // 8-second WDT interval

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
    delay(30);
  }

  // Stay on for 6 hours using IDLE sleep to keep PWM active
  sleepForSeconds(ON_SECONDS, true);

  // Fade out to 0
  for (int i = 64; i >= 0; i--) {
    analogWrite(LED1_PIN, i);
    analogWrite(LED2_PIN, i);
    delay(30);
  }

  // Fully off
  digitalWrite(LED1_PIN, LOW); // Use digitalWrite for a guaranteed off state
  digitalWrite(LED2_PIN, LOW);

  // Sleep for 18 hours using POWER DOWN sleep for maximum power saving
  sleepForSeconds(OFF_SECONDS, false);
}

void sleepForSeconds(unsigned long seconds, bool isIdle) {
  wakeCount = 0; // Reset the wake-up counter
  unsigned long targetWakeCount = seconds / WDT_INTERVAL_S;

  // Set the appropriate sleep mode
  if (isIdle) {
    set_sleep_mode(SLEEP_MODE_IDLE); // Keeps Timers (and PWM) running
  } else {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Shuts everything down for max savings
  }

  while (wakeCount < targetWakeCount) {
    sleep_enable();
    sleep_mode();
    // MCU sleeps here until WDT interrupt
    sleep_disable();
  }
}

void setupWatchdog() {
  cli(); // Disable interrupts
  MCUSR = 0; // Clear reset flags
  
  // Enable watchdog timer changes
  WDTCR |= (1 << WDCE) | (1 << WDE);

  // ***FIXED***: Set watchdog to Interrupt-Only mode with an 8-second interval
  WDTCR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // WDP3 + WDP0 = 1024K cycles = 8 seconds
  
  sei(); // Enable interrupts
}

// Watchdog Interrupt Service Routine
ISR(WDT_vect) {
  wakeCount++; // Increment the wake-up counter
}
