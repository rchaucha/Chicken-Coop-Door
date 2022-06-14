#include <LowPower.h>

const int photocellPin = A3;
const int mot_up = 7;
const int mot_down = 8;

const int power_L298N = 11;
const int power_photocell = 13;

const int topSwitchPin = 2;
const int bottomSwitchPin = 3;

const int night_threshold = 30;
const int day_threshold = 60;

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

  if (photocell_value < night_threshold && !digitalRead(bottomSwitchPin))
  {
    digitalWrite(power_L298N, HIGH);
    quick_wait();
  
    digitalWrite(mot_down, HIGH);
    digitalWrite(mot_up, LOW);
    
    while (!digitalRead(bottomSwitchPin)) {}
    
    digitalWrite(mot_down, LOW);
    digitalWrite(mot_up, LOW);

    digitalWrite(power_L298N, LOW);

    deep_sleep(7*60); // Wait 7 hours
  }
  else if (photocell_value > day_threshold && !digitalRead(topSwitchPin))
  {
    digitalWrite(power_L298N, HIGH);
    quick_wait();
    
    digitalWrite(mot_down, LOW);
    digitalWrite(mot_up, HIGH);
    
    while (!digitalRead(topSwitchPin)) {}
    
    digitalWrite(mot_down, LOW);
    digitalWrite(mot_up, LOW);

    digitalWrite(power_L298N, LOW);

    deep_sleep(7*60); // Wait 7 hours
  }
  
  deep_sleep(10); // Wait 10 minutes
}
