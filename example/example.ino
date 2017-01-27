/* Railbot Project
 *  
 *  Reading encoder on the wheel.
 *  
 *  Max RPM motor = 3000 rpm = 50 Hz
 *  Min sampling frequency = 100 Hz (Nyquist)
 *  > enforce encoder reading every 10ms
 *  
 *  Vinicius Goecks
 *  Dec 15, 2016
 */
#include <Filters.h>

// Variables
double h = 0;               // step size
double omega = 0;           // angular velocity
double r = 0.105/2;               // wheel radius
const double pi = 3.14159;
double v = 0;

// A0 pin, voltage output (absolute encoder)
const int encoderPin = 0;
const int samplingInterval = 10; // ms

// Encoder readings
int oldReading = 0; // counts
int newReading = 0; // counts
int noise = 10; // counts
int numberRotations = 0; // counts

// Velocity readings
long firstTime[3];
long firstPos[3];

// Time variables
unsigned long startTime = 0;
unsigned long finalTime = 0;
unsigned long runTime = 0;

// Filter variables
float filterFrequency = 1; // low pass filter: changes faster than
                             // filterFrequency will be filtered

void setup() {
  // Initialize the serial
  Serial.begin(115200);

}

double CalculateVel() {
    
  // Create one pole RC filter
  FilterOnePole lowpassFilter( LOWPASS, filterFrequency );

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
    v = processVelocity(firstPos,firstTime);
    v = lowpassFilter.input(v);
    Serial.println(v);
  
    // Stop counting time
    finalTime = millis();
    runTime = finalTime - startTime;
  
    // Make sure samplingInterval will happen
    delay(samplingInterval - runTime); // ms

    return v

  }
}

double processVelocity(long firstPos[3], long firstTime[3]){
  h = (firstTime[2] - firstTime[0])/2.0;
  omega = ( (1.5)*firstPos[2] + (-2.0)*firstPos[1] + (0.5)*firstPos[0] )/h; // in counts/us
  omega = omega*2*pi/1024*1000000; // in radians/s

  v = (r*omega); // velocity in m/s
   
  return v;
}

void loop() {
  // Calculate, filter, and send velocity from the encoder
  v = CalculateVel();
}
