#include <LowPower.h>

const int NIGHT_THRESHOLD = 3;
const int DAY_THRESHOLD = 30;

const int photocell_input_pin = A3;
const int mot_up_pin = 7;
const int mot_down_pin = 8;

const int power_L298N_pin = 11;
const int power_photocell_pin = 13;

const int top_switch_pin = 2;
const int bottom_switch_pin = 3;

enum DOOR_OPERATION
{
  OPEN,
  CLOSE
};


///////////////////// FUNCTIONS /////////////////////

void quick_wait()
{
  LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
}

void deep_sleep(int nb_minutes)
{
  int nb_seconds = 60 * nb_minutes;
  for (int s = 0 ; s < nb_seconds; s += 8) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

int get_light_level()
{
  digitalWrite(power_photocell_pin, HIGH);

  quick_wait();
  int photocell_value = analogRead(photocell_input_pin);

  digitalWrite(power_photocell_pin, LOW);

  return photocell_value;
}

void power_L298N()
{
  digitalWrite(power_L298N_pin, HIGH);
  quick_wait();
}

void stop_motor()
{
  // Stop the motor
  digitalWrite(mot_down_pin, LOW);
  digitalWrite(mot_up_pin, LOW);

  // Turn off L298N
  digitalWrite(power_L298N_pin, LOW);
}

void activate_door(const DOOR_OPERATION op)
{
  power_L298N();

  int switch_pin = 0;

  switch (op)
  {
    case OPEN :
      digitalWrite(mot_up_pin, HIGH);
      digitalWrite(mot_down_pin, LOW);
      switch_pin = top_switch_pin;
      break;
    case CLOSE :
      digitalWrite(mot_up_pin, LOW);
      digitalWrite(mot_down_pin, HIGH);
      switch_pin = bottom_switch_pin;
      break;
  }

  // Continue until the end switch is reached
  while (!digitalRead(switch_pin)) {}

  stop_motor();

  // Wait 7 hours
  deep_sleep(7 * 60);
}


///////////////////// PROGRAM /////////////////////

void setup()
{
  pinMode(mot_down_pin, OUTPUT);
  pinMode(mot_up_pin, OUTPUT);

  pinMode(power_L298N_pin, OUTPUT);
  pinMode(power_photocell_pin, OUTPUT);

  pinMode(top_switch_pin, INPUT_PULLUP);
  pinMode(bottom_switch_pin, INPUT_PULLUP);
}

void loop()
{
  int photocell_value = get_light_level();

  // If the door is not opened and the light is above the day threshold
  if (photocell_value > DAY_THRESHOLD && !digitalRead(top_switch_pin))
  {
    activate_door(DOOR_OPERATION::OPEN);
  }
  // If the door is not closed and the light is below the night threshold
  else if (photocell_value < NIGHT_THRESHOLD && !digitalRead(bottom_switch_pin))
  {
    activate_door(DOOR_OPERATION::CLOSE);
  }

  // Wait 10 minutes
  deep_sleep(10);
}
