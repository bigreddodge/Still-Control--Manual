#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS            A3    // Pin D2
#define NUM_SENSORS             5     // For verifying hardware operation
#define TEMPERATURE_PRECISION   12    // Temperature precision

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tSensor[5] = {{0x28, 0x52, 0x09, 0x3A, 0x1A, 0x13, 0x01, 0x39},
                            {0x28, 0xAA, 0xD7, 0x12, 0x12, 0x13, 0x02, 0xBD},
                            {0x28, 0xB7, 0xB7, 0x5D, 0x1A, 0x13, 0x01, 0x00},
                            {0x28, 0xFF, 0x92, 0xBC, 0x83, 0x16, 0x03, 0xDD},
                            {0x28, 0xFF, 0xD1, 0x3E, 0x84, 0x16, 0x04, 0x5D}};

void setup(){
  Serial.begin(115200);
  InitSensors(NUM_SENSORS, TEMPERATURE_PRECISION);
}

void loop(){
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures();

  //print readings
  Serial.print("\n");
  for (int i = 0; i < 5; i++){
    Serial.print(formatTemp(tSensor[i]));
    Serial.print("\t\t");
  }
  delay(5000);

}

// InitSensors(int) returns false if number of sensors found != expected
bool InitSensors(int expected_device_count, int device_precision){
  sensors.begin();
  if (sensors.getDeviceCount() != expected_device_count){
    return false;
  }
  Serial.print("\n");
  for (int i = 0; i < 5; i++){
    sensors.setResolution(tSensor[i], device_precision);
    Serial.print(formatAddress(tSensor[i]));
    Serial.print("\t");
  }
}

String formatTemp(DeviceAddress deviceAddress){
  float tempC = sensors.getTempC(deviceAddress);
  return ((String(DallasTemperature::toFahrenheit(tempC))) + "ºF");
}


String formatAddress(DeviceAddress deviceAddress){
  String tempAddr = "0x";
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16){
      tempAddr += "0";
    }
    tempAddr += String(deviceAddress[i], HEX);
  }
  return tempAddr;
}
