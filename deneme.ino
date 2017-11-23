#include <Wire.h>
#include <HMC5883L.h>
HMC5883L compass;
unsigned long start;
boolean passX, passY, passZ;
void setup() {
   Serial.begin(9600);

  // Initialize Initialize HMC5883L
  Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_4_7GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_15HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);
  delay(6); 
}
void loop() {
  while (millis() - start < 5000) {
    selfTest();
  }
}
void selfTest()
{
  float averageX = 0;
  float averageY = 0;
  float averageZ = 0;


  for (int i = 0; i <= 7; i++)      // take and average 8 measurements
  {
    Vector raw = compass.readRaw();      // read 6 bytes, 3 16bit 2's compliment

    float testX = raw.XAxis;             // read raw values each axis
    float testY = raw.YAxis;
    float testZ = raw.ZAxis;

    averageX = averageX + testX;   // add test values to be averaged later
    averageY = averageY + testY;
    averageZ = averageZ + testZ;
    Serial.print(i + 1);
    Serial.print(". Raw:\t");        // print the test values
    Serial.print(testX );
    Serial.print("   ");
    Serial.print(testY);
    Serial.print("   ");
    Serial.println(testZ);
    delay(67);
  }

  // check limits...

  averageX = averageX / 8;          // calc averages for 8 samples
  averageY = averageY / 8;
  averageZ = averageZ / 8;

  Serial.print("\nCalculated Averages:\t");
  Serial.print(averageX);
  Serial.print("   ");
  Serial.print(averageY);
  Serial.print("   ");
  Serial.println(averageZ);

  if (abs(averageX) >= 243 && abs(averageX) <= 575) // work with absolute values
  { 
    Serial.println("\nX within limits");
    passX = true;
  }
  else
  {
    Serial.println("\nX not within limits");
    passX = false;
  }

  if (abs(averageY) >= 243 && abs(averageY) <= 575)
  {
    Serial.println("\nY within limits");
    passY = true;
  }
  else
  {
    Serial.println("\nY not within limits");
    passY = false;
  }

  if (abs(averageZ) >= 243 && abs(averageZ) <= 575)
  {
    Serial.println("\nZ within limits");
    passZ = true;
  }
  else
  {
    Serial.println("\nZ not within limits");
    passZ = false;
  }

  if (passX == true && passY == true && passZ == true)  // if all axes passed
  {
    compass.setMeasurementMode(HMC5883L_CONTINOUS);
    compass.setDataRate(HMC5883L_DATARATE_15HZ);
    compass.setSamples(HMC5883L_SAMPLES_8);
     
      delay(10);
    Serial.println("\n\nSelf Test Passed - Moving On...\n");
  }
  else
  {
    Serial.println("\n\nSelf Test Failed - Moving On...\n");
  }
}
