/* Encoder to Speed
 *
 *  Reading encoder on the wheel and converting to robot speed.
 *
 *  Vinicius Goecks
 *  Dec 15, 2016
 */

 #include "Arduino.h"
 #include "Encoder.h"
 #include <Filters.h>

 Encoder::Encoder(int pin, float WheelDiameter, int MaxMotorRotation)
 {
     _pin = pin;
     _WheelDiameter = WheelDiameter;
     _MaxMotorRotation = MaxMotorRotation;
 }
 
 double Encoder::CalculateVel()
 {
  // Create one pole RC filter
  FilterOnePole Filters::lowpassFilter( LOWPASS, filterFrequency );

  while (true) {
    // Start counting time
    startTime = millis();

    // Reset number of rotations
    numberRotations = 0;

    for(int i = 0; i < 3; i++){
      // Read sensor
      oldReading = newReading - noise;  // Correct for noise
      newReading = analogRead(encoderPin);
      firstTime[i] = micros();

       // Check if completed one more rotation
      if (newReading < oldReading) {
        numberRotations += 1;
        // Serial.println(numberRotations);
      }

      // Our first measurement is always numberRotations = 0
      if (i == 0) {
        numberRotations = 0;
      }

      firstPos[i] = (newReading + 1024*numberRotations); // in counts

      delay(1);     // delay 1 milliseconds between each reading
    }

    // Calculate velocity and filter it
    v = Encoder::processVelocity(firstPos,firstTime);
    v = lowpassFilter.input(v);
    Serial.println(v);

    // Stop counting time
    finalTime = millis();
    runTime = finalTime - startTime;

    // Make sure samplingInterval will happen
    delay(samplingInterval - runTime); // ms

    return v;
 }

 double Encoder::processVelocity(long firstPos[3], long firstTime[3])
 {
      h = (firstTime[2] - firstTime[0])/2.0;
      omega = ( (1.5)*firstPos[2] + (-2.0)*firstPos[1] + (0.5)*firstPos[0] )/h; // in counts/us
      omega = omega*2*pi/1024*1000000; // in radians/s

      v = (r*omega); // velocity in m/s

      return v;
 }
