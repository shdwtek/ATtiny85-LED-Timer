#include <avr/sleep.h>
#include <avr/wdt.h>

#define LED1 PB0
#define LED2 PB1

volatile bool wdtFlag = false;
unsigned long wakeCount = 0; // Count watchdog wakeups (1 every 8 sec)

// Constants
const unsigned long SECONDS_ON = 6L * 60L * 60L;
const unsigned long SECONDS_OFF = 18L * 60L * 60L;
const unsigned long WDT_INTERVAL = 8; // WDT interval in seconds

// Main setup for assigning LED pins
void setup() {
  pinMode(LED1, OUTPUT); // Pin 5
  pinMode(LED2, OUTPUT); // Pin 6
  setupWatchdog();
}

// Main loop for 6 hours on and 18 hours off cycle
void loop() {
  // 6h ON cycle
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  countSleepFor(SECONDS_ON);

  // 18h OFF cycle
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  countSleepFor(SECONDS_OFF);
}

void countSleepFor(unsigned long seconds) {
  wakeCount = 0;
  unsigned long targetWakeups = seconds / WDT_INTERVAL;
  while (wakeCount < targetWakeups) {
    systemSleep();
  }
}
// Function for controlling system sleep
void systemSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode(); // Wait for WDT interrupt
  sleep_disable();
}

void setupWatchdog() {
  cli(); // Disable interrupts
  MCUSR = 0;
  WDTCR |= (1 << WDCE) | (1 << WDE);
  WDTCR = (1 << WDIE) | (1 << WDP3); // 8s timeout
  sei(); // Enable interrupts
}

ISR(WDT_vect) {
  wdtFlag = true;
  wakeCount++;
}
