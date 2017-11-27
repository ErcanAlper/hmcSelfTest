#include <Wire.h>
#include <HMC5883L.h>
HMC5883L compass;
unsigned long start;

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
  compass.setRange(HMC5883L_RANGE_0_88GA);

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
  while (millis() - start < 30000) {
    selfTest();
  }
}
void selfTest()
{
    Vector raw = compass.readNormalize();      

    float testX = raw.XAxis;         
    float testY = raw.YAxis;
    float testZ = raw.ZAxis;


  
    Serial.print(testX );
    Serial.print(",");
    Serial.print(testY);
    Serial.print(",");
    Serial.println(testZ);
    delay(64);
  }

 


