/* Encoder to Speed
 *
 *  Reading encoder on the wheel and converting to robot speed.
 *
 *  Vinicius Goecks
 *  Dec 15, 2016
 */

 class Encoder
 {
 public:
     Encoder(int pin, float WheelDiameter, int MaxMotorRotation);
     float CalculateVel();
 private:

 }
