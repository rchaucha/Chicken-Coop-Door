#include <LowPower.h>

const int night_threshold = 3;
const int day_threshold = 50;

const int photocellPin = A3;
const int mot_up = 7;
const int mot_down = 8;

const int power_L298N = 11;
const int power_photocell = 13;

const int topSwitchPin = 2;
const int bottomSwitchPin = 3;

void quick_wait(){
  LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
}

void deep_sleep(int nb_minutes) {
  int nb_seconds = 60 * nb_minutes;
  for (int s = 0 ; s < nb_seconds; s+=8){
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

int get_light_level()
{
  digitalWrite(power_photocell, HIGH);

  quick_wait();
  int photocell_value = analogRead(photocellPin);
  
  digitalWrite(power_photocell, LOW);

  return photocell_value;
}

void power_L298N()
{
    digitalWrite(power_L298N, HIGH);
    quick_wait();
}

void stop_motor()
{
    // Stop the motor
    digitalWrite(mot_down, LOW);
    digitalWrite(mot_up, LOW);

    // Turn off L298N
    digitalWrite(power_L298N, LOW);
}

void setup() {  
  pinMode(mot_down, OUTPUT);
  pinMode(mot_up, OUTPUT);
  
  pinMode(power_L298N, OUTPUT);
  pinMode(power_photocell, OUTPUT);

  pinMode(topSwitchPin, INPUT_PULLUP);
  pinMode(bottomSwitchPin, INPUT_PULLUP);
}

void loop()
{
  int photocell_value = get_light_level();
  
  // If the door is not closed and the light is below the night threshold
  if (photocell_value < night_threshold && !digitalRead(bottomSwitchPin))
  {
    power_L298N();
  
    // Activate the motor to close the door
    digitalWrite(mot_down, HIGH);
    digitalWrite(mot_up, LOW);
    
    // Close the door until the bottom is reached
    while (!digitalRead(bottomSwitchPin)) {}
    
    stop_motor();
    
    // Wait 7 hours
    deep_sleep(7*60);
  }
  // If the door is not opened and the light is above the day threshold
  else if (photocell_value > day_threshold && !digitalRead(topSwitchPin))
  {
    power_L298N();
    
    // Activate the motor to open the door
    digitalWrite(mot_down, LOW);
    digitalWrite(mot_up, HIGH);
    
    // Close the door until the top is reached
    while (!digitalRead(topSwitchPin)) {}
    
    stop_motor();

    // Wait 7 hours
    deep_sleep(7*60);
  }
  
  // Wait 10 minutes
  deep_sleep(10);
}
