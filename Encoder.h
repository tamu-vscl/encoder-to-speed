/* Encoder to Speed
 *
 *  Reading encoder on the wheel and converting to robot speed.
 *
 *  Vinicius Goecks
 *  Dec 15, 2016
 */

 #ifndef Encoder_h
 #define Encoder_h

 #include "Arduino.h"

 class Encoder
 {
 public:
     Encoder(int pin, float WheelDiameter, int MaxMotorRotation);
     double CalculateVel();
 private:
     double processVelocity(long firstPos[3], long firstTime[3]);
     int _pin;
     float _WheelDiameter;
     int _MaxMotorRotation
 };

 #endif
