# Chicken-Coop-Door
Automatic chicken coop door that opens and closes depending on the sun.\
Works on battery on an Arduino Pro Mini 3.3V.

Everything has been done to maximize the battery life :
* The L298N driver board and the photocell are only powered when needed
* The Arduino Pro Mini is put into deep sleep mode almost all the time

Here is the daily routine (beginning with closed door):
1. Every 10 minutes, wake up the Arduino to check if the ambiant light is above a certain threshold (`day_threshold`).
2. Open the door and deep sleep for 7 hours.
3. Wake up the Arduino every 10 minutes to check if the light is below another threshold (`night_threshold`).
4. Close the door and deep sleep for 7 hours.
5. Repeat.

## Dependency
You have to install the Low Power library : https://github.com/rocketscream/Low-Power.

## Schematics
![schematics](https://github.com/rchaucha/Chicken-Coop-Door/blob/main/schematics.png) 
