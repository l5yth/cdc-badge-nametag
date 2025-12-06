#include <Arduino.h>
#include <unity.h>


void test_led_state_high(void)
{
  digitalWrite(TEST_LED_PIN, HIGH);
  delay(1); // seems to need a short delay
  TEST_ASSERT_EQUAL(HIGH, digitalRead(TEST_LED_PIN));
}

void test_led_state_low(void)
{
  digitalWrite(TEST_LED_PIN, LOW);
  delay(1);
  TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_LED_PIN));
}

void test_pin_mode(void) {
  pinMode(TEST_LED_PIN, OUTPUT);
  TEST_ASSERT_TRUE(true);
}


void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  pinMode(TEST_LED_PIN, OUTPUT);

  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_pin_mode);
}

uint8_t i = 0;
uint8_t max_blinks = 5;

void loop()
{
  if (i < max_blinks)
  {
    RUN_TEST(test_led_state_high);
    delay(500);
    RUN_TEST(test_led_state_low);
    delay(500);
    i++;
  }
  else if (i == max_blinks)
  {
    UNITY_END(); // stop unit testing
  }
}

