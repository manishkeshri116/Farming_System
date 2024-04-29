/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/FirebaseJson
 *
 * Copyright (c) 2023 mobizt
 *
 */

#include<Wire.h>
// BME280 I2C address is 0x76(108)
#define Addr 0x76
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "Mk@12345"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyAKJQTSklffQyF-ced2Mp6qqQJ8aAMFDUE"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://farming-system-84ff5-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "manishkeshri116@gmail.com"
#define USER_PASSWORD "Oppo@123"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;
const int sensor_pin = A0;  

void setup()
{
     Wire.begin();
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Or use legacy authenticate method
    // config.database_url = DATABASE_URL;
    // config.signer.tokens.legacy_token = "<database secret>";

    Firebase.begin(&config, &auth);

    // Comment or pass false value when WiFi reconnection will control by your code or third party library
    Firebase.reconnectWiFi(true);
}

void loop()
{
	//bme sensor data 
	
	unsigned int b1[24];
  unsigned int data[8];
  unsigned int dig_H1 = 0;
  for(int i = 0; i < 24; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((136+i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);

    // Read 24 bytes of data
    if(Wire.available() == 1)
    {
      b1[i] = Wire.read();
    }
  }

  // Convert the data
  // temp coefficients
  unsigned int dig_T1 = (b1[0] & 0xff) + ((b1[1] & 0xff) * 256);
  int dig_T2 = b1[2] + (b1[3] * 256);
  int dig_T3 = b1[4] + (b1[5] * 256);
  
  // pressure coefficients
  unsigned int dig_P1 = (b1[6] & 0xff) + ((b1[7] & 0xff ) * 256);
  int dig_P2 = b1[8] + (b1[9] * 256);
  int dig_P3 = b1[10] + (b1[11] * 256);
  int dig_P4 = b1[12] + (b1[13] * 256);
  int dig_P5 = b1[14] + (b1[15] * 256);
  int dig_P6 = b1[16] + (b1[17] * 256);
  int dig_P7 = b1[18] + (b1[19] * 256);
  int dig_P8 = b1[20] + (b1[21] * 256);
  int dig_P9 = b1[22] + (b1[23] * 256);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(161);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(Addr, 1);
  
  // Read 1 byte of data
  if(Wire.available() == 1)
  {
    dig_H1 = Wire.read();
  }

  for(int i = 0; i < 7; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((225+i));
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);
    
    // Read 7 bytes of data
    if(Wire.available() == 1)
    {
      b1[i] = Wire.read();
    }
  }

  // Convert the data
  // humidity coefficients
  int dig_H2 = b1[0] + (b1[1] * 256);
  unsigned int dig_H3 = b1[2] & 0xFF ;
  int dig_H4 = (b1[3] * 16) + (b1[4] & 0xF);
  int dig_H5 = (b1[4] / 16) + (b1[5] * 16);
  int dig_H6 = b1[6];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control humidity register
  Wire.write(0xF2);
  // Humidity over sampling rate = 1
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control measurement register
  Wire.write(0xF4);
  // Normal mode, temp and pressure over sampling rate = 1
  Wire.write(0x27);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select config register
  Wire.write(0xF5);
  // Stand_by time = 1000ms
  Wire.write(0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  for(int i = 0; i < 8; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((247+i));
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);
    
    // Read 8 bytes of data
    if(Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }
  
  // Convert pressure and temperature data to 19-bits
  long adc_p = (((long)(data[0] & 0xFF) * 65536) + ((long)(data[1] & 0xFF) * 256) + (long)(data[2] & 0xF0)) / 16;
  long adc_t = (((long)(data[3] & 0xFF) * 65536) + ((long)(data[4] & 0xFF) * 256) + (long)(data[5] & 0xF0)) / 16;
  // Convert the humidity data
  long adc_h = ((long)(data[6] & 0xFF) * 256 + (long)(data[7] & 0xFF));
  
  // Temperature offset calculations
  double var1 = (((double)adc_t) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
  double var2 = ((((double)adc_t) / 131072.0 - ((double)dig_T1) / 8192.0) *
  (((double)adc_t)/131072.0 - ((double)dig_T1)/8192.0)) * ((double)dig_T3);
  double t_fine = (long)(var1 + var2);
  double cTemp = (var1 + var2) / 5120.0;
  double fTemp = cTemp * 1.8 + 32;
  
  // Pressure offset calculations
  var1 = ((double)t_fine / 2.0) - 64000.0;
  var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
  var2 = var2 + var1 * ((double)dig_P5) * 2.0;
  var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
  var1 = (((double) dig_P3) * var1 * var1 / 524288.0 + ((double) dig_P2) * var1) / 524288.0;
  var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
  double p = 1048576.0 - (double)adc_p;
  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
  var1 = ((double) dig_P9) * p * p / 2147483648.0;
  var2 = p * ((double) dig_P8) / 32768.0;
  double pressure = (p + (var1 + var2 + ((double)dig_P7)) / 16.0) / 100;
  
  // Humidity offset calculations
  double var_H = (((double)t_fine) - 76800.0);
  var_H = (adc_h - (dig_H4 * 64.0 + dig_H5 / 16384.0 * var_H)) * (dig_H2 / 65536.0 * (1.0 + dig_H6 / 67108864.0 * var_H * (1.0 + dig_H3 / 67108864.0 * var_H)));
  double humidity = var_H * (1.0 -  dig_H1 * var_H / 524288.0);
  if(humidity > 100.0)
  {
    humidity = 100.0;
  }
  else if(humidity < 0.0)
  {
    humidity = 0.0;
  }

if(fTemp > 50.0)
  {
    fTemp = 50.0;
  }
  else if(fTemp < 50.0)
  {
    fTemp = 50.0;
  }
  
    if(cTemp > 50.0)
  {
    cTemp = 50.0;
  }
  else if(cTemp < 25.0)
  {
    cTemp = 25.0;
  }
  
  
    if(pressure > 1010.0)
  {
    pressure = 1010.0;
  }
  else if(pressure< 150.0)
  {
    pressure = 150.0;
  }
float moisture_percentage;

  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );

   Serial.print("Temperature in Celsius : ");

  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit : ");
  Serial.print(fTemp);
  Serial.println(" F");
  Serial.print("Pressure : ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Relative Humidity : ");
  Serial.print(humidity);
  Serial.println(" RH");
  
	
    // Flash string (PROGMEM and  (FPSTR), String C/C++ string, const char, char array, string literal are supported
    // in all Firebase and FirebaseJson functions, unless F() macro is not supported.

    // Firebase.ready() should be called repeatedly to handle authentication tasks.

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

        //FirebaseJson json;
       // json.setDoubleDigits(3);
        //json.add("value", count);

        //Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "/test/json", &json) ? "ok" : fbdo.errorReason().c_str());

       // Serial.printf("Get json... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json") ? fbdo.to<FirebaseJson>().raw() : fbdo.errorReason().c_str());

       // FirebaseJson jVal;
       // Serial.printf("Get json ref... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json", &jVal) ? jVal.raw() : fbdo.errorReason().c_str());

        FirebaseJsonArray arr;
        arr.setFloatDigits(2);
        arr.setDoubleDigits(4);
        arr.add((double)cTemp, (double)pressure, (double)humidity, (double)moisture_percentage);

        Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "/test/array", &arr) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get array... %s\n", Firebase.RTDB.getArray(&fbdo, "/test/array") ? fbdo.to<FirebaseJsonArray>().raw() : fbdo.errorReason().c_str());

        // Serial.printf("Push json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/test/push", &json) ? "ok" : fbdo.errorReason().c_str());

        // json.set("value", count + 0.29745);
        // Serial.printf("Update json... %s\n\n", Firebase.RTDB.updateNode(&fbdo, "/test/push/" + fbdo.pushName(), &json) ? "ok" : fbdo.errorReason().c_str());


  
    count++;
		delay(1000);
    }
    // Output data to serial monitor
 
}