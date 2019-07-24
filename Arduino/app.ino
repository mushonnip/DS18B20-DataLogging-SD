#include <Wire.h>
#include <LiquidCrystal_PCF8574.h> //memasukan library LCD
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>

#define ONE_WIRE_BUS1 36
#define ONE_WIRE_BUS2 38
#define ONE_WIRE_BUS3 40
#define chipSelect 53

OneWire oneWire1(ONE_WIRE_BUS1);
OneWire oneWire2(ONE_WIRE_BUS2);
OneWire oneWire3(ONE_WIRE_BUS3);

DallasTemperature sensorSuhu1(&oneWire1);
DallasTemperature sensorSuhu2(&oneWire2);
DallasTemperature sensorSuhu3(&oneWire3);

RTC_DS3231 rtc;
LiquidCrystal_PCF8574 lcd(0x3F);
File fileSuhu;
int time;
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long periode = 60000; //interval waktu menyimpan dalam milidetik
//const unsigned long periode = 600000; //interval waktu menyimpan dalam milidetik

void setup()
{
    Serial.begin(9600);
    //RTC (Real Time Clock)
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
            ;
    }
    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, lets set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        //rtc.adjust(DateTime(2019, 7, 12, 15, 31, 17));
    }

    //Sensor Suhu DS18B20
    sensorSuhu1.begin();
    sensorSuhu2.begin();
    sensorSuhu3.begin();

    //LCD (Liquid Crystal Display)
    lcd.begin(20, 4);
    lcd.setBacklight(255);

    //SD Card
    lcd.setCursor(0, 0);
    if (!SD.begin(53))
    {
        lcd.setCursor(00, 2);
        lcd.print("SD Card Error");
        while (1)
            ;
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("SD Card OK");
        lcd.setCursor(0, 1);
        lcd.print("Membuka TEMP.TXT");
        delay(1000);
    }
    lcd.clear();

    Serial.println("Waktu\t\t\t\tS1\tS2\tS3\tS4\tS5\tS6\tS7\tS8\tS9");

    startMillis = millis();

    DateTime now = rtc.now();
    sensorSuhu1.requestTemperatures();
    sensorSuhu2.requestTemperatures();
    sensorSuhu3.requestTemperatures();
    //SERIAL
    Serial.print(now.day(), DEC);
    Serial.print('-');
    Serial.print(now.month(), DEC);
    Serial.print('-');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print("\t\t");

    Serial.print(sensorSuhu1.getTempCByIndex(0));
    Serial.print("\t");
    Serial.print(sensorSuhu1.getTempCByIndex(1));
    Serial.print("\t");
    Serial.print(sensorSuhu1.getTempCByIndex(2));
    Serial.print("\t");
    Serial.print(sensorSuhu2.getTempCByIndex(0));
    Serial.print("\t");
    Serial.print(sensorSuhu2.getTempCByIndex(1));
    Serial.print("\t");
    Serial.print(sensorSuhu2.getTempCByIndex(2));
    Serial.print("\t");
    Serial.print(sensorSuhu3.getTempCByIndex(0));
    Serial.print("\t");
    Serial.print(sensorSuhu3.getTempCByIndex(1));
    Serial.print("\t");
    Serial.print(sensorSuhu3.getTempCByIndex(2));
    Serial.println();
}

void loop()
{
    DateTime now = rtc.now();
    currentMillis = millis();

    //SENSOR SUHU
    sensorSuhu1.requestTemperatures();
    sensorSuhu2.requestTemperatures();
    sensorSuhu3.requestTemperatures();

    //LCD
    lcd.setCursor(0, 0);
    lcd.print(now.day(), DEC); //Tampilkan tanggal
    lcd.print("-");

    lcd.setCursor(3, 00);
    int bulan = now.month();
    if (bulan <= 9)
    {
        lcd.print(0);
    }
    lcd.print(now.month(), DEC); //Tampilkan bulan

    lcd.print("-");
    lcd.setCursor(6, 0);
    lcd.print(now.year(), DEC); //Tampilkan tanggal

    lcd.setCursor(12, 0);
    if (now.hour() < 10)
    {
        lcd.print(0);
    }
    lcd.print(now.hour(), DEC); //Tampilkan jam
    lcd.print(':');
    lcd.setCursor(15, 0);
    if (now.minute() < 10)
    {
        lcd.print(0);
    }
    lcd.print(now.minute(), DEC); //Tampilkan menit

    //Menampilkan temperatur ke LCD
    lcd.setCursor(0, 1);
    if (int(sensorSuhu1.getTempCByIndex(0)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu1.getTempCByIndex(0));
    }

    lcd.setCursor(8, 1);
    if (int(sensorSuhu1.getTempCByIndex(1)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu1.getTempCByIndex(1));
    }

    lcd.setCursor(15, 1);
    if (int(sensorSuhu1.getTempCByIndex(2)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu1.getTempCByIndex(2));
    }

    lcd.setCursor(0, 2);
    if (int(sensorSuhu2.getTempCByIndex(0)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu2.getTempCByIndex(0));
    }

    lcd.setCursor(8, 2);
    if (int(sensorSuhu2.getTempCByIndex(1)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu2.getTempCByIndex(1));
    }

    lcd.setCursor(15, 2);
    if (int(sensorSuhu2.getTempCByIndex(2)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu2.getTempCByIndex(2));
    }

    lcd.setCursor(0, 3);
    if (int(sensorSuhu3.getTempCByIndex(0)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu3.getTempCByIndex(0));
    }

    lcd.setCursor(8, 3);
    if (int(sensorSuhu3.getTempCByIndex(1)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu3.getTempCByIndex(1));
    }

    lcd.setCursor(15, 3);
    if (int(sensorSuhu3.getTempCByIndex(2)) == -127)
    {
        lcd.print("ERROR");
    }
    else
    {
        lcd.print(sensorSuhu3.getTempCByIndex(2));
    }

    // //SERIAL
    // Serial.print(now.day(), DEC);
    // Serial.print('-');
    // Serial.print(now.month(), DEC);
    // Serial.print('-');
    // Serial.print(now.year(), DEC);
    // Serial.print(' ');
    // Serial.print(now.hour(), DEC);
    // Serial.print(':');
    // Serial.print(now.minute(), DEC);
    // Serial.print(':');
    // Serial.print(now.second(), DEC);
    // Serial.print("\t\t");

    // Serial.print(sensorSuhu1.getTempCByIndex(0));
    // Serial.print("\t");
    // Serial.print(sensorSuhu1.getTempCByIndex(1));
    // Serial.print("\t");
    // Serial.print(sensorSuhu1.getTempCByIndex(2));
    // Serial.print("\t");
    // Serial.print(sensorSuhu2.getTempCByIndex(0));
    // Serial.print("\t");
    // Serial.print(sensorSuhu2.getTempCByIndex(1));
    // Serial.print("\t");
    // Serial.print(sensorSuhu2.getTempCByIndex(2));
    // Serial.print("\t");
    // Serial.print(sensorSuhu3.getTempCByIndex(0));
    // Serial.print("\t");
    // Serial.print(sensorSuhu3.getTempCByIndex(1));
    // Serial.print("\t");
    // Serial.print(sensorSuhu3.getTempCByIndex(2));
    // Serial.println();

    //Menyimpan ke SD
    if (currentMillis - startMillis >= periode)
    {
        fileSuhu = SD.open("TEMP.txt", FILE_WRITE);
        if (fileSuhu)
        {
            fileSuhu.print(now.day(), DEC);
            fileSuhu.print('-');
            fileSuhu.print(now.month(), DEC);
            fileSuhu.print('-');
            fileSuhu.print(now.year(), DEC);
            fileSuhu.print(' ');
            fileSuhu.print(now.hour(), DEC);
            fileSuhu.print(':');
            fileSuhu.print(now.minute(), DEC);
            fileSuhu.print(':');
            fileSuhu.print(now.second(), DEC);
            fileSuhu.print("\t\t");

            fileSuhu.print(sensorSuhu1.getTempCByIndex(0));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu1.getTempCByIndex(1));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu1.getTempCByIndex(2));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu2.getTempCByIndex(0));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu2.getTempCByIndex(1));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu2.getTempCByIndex(2));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu3.getTempCByIndex(0));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu3.getTempCByIndex(1));
            fileSuhu.print("\t");
            fileSuhu.print(sensorSuhu3.getTempCByIndex(2));
            fileSuhu.println();

            Serial.println("Tersimpan");
            fileSuhu.close();
            time += 1;

            //SERIAL
            Serial.print(now.day(), DEC);
            Serial.print('-');
            Serial.print(now.month(), DEC);
            Serial.print('-');
            Serial.print(now.year(), DEC);
            Serial.print(' ');
            Serial.print(now.hour(), DEC);
            Serial.print(':');
            Serial.print(now.minute(), DEC);
            Serial.print(':');
            Serial.print(now.second(), DEC);
            Serial.print("\t\t");

            Serial.print(sensorSuhu1.getTempCByIndex(0));
            Serial.print("\t");
            Serial.print(sensorSuhu1.getTempCByIndex(1));
            Serial.print("\t");
            Serial.print(sensorSuhu1.getTempCByIndex(2));
            Serial.print("\t");
            Serial.print(sensorSuhu2.getTempCByIndex(0));
            Serial.print("\t");
            Serial.print(sensorSuhu2.getTempCByIndex(1));
            Serial.print("\t");
            Serial.print(sensorSuhu2.getTempCByIndex(2));
            Serial.print("\t");
            Serial.print(sensorSuhu3.getTempCByIndex(0));
            Serial.print("\t");
            Serial.print(sensorSuhu3.getTempCByIndex(1));
            Serial.print("\t");
            Serial.print(sensorSuhu3.getTempCByIndex(2));
            Serial.println();
        }
        else
        {
            Serial.println("Gagal menyimpan");
        }
        startMillis = currentMillis;
    }

    if (time < 10)
    {
        lcd.setCursor(19, 0);
        lcd.print(time);
    }
    else
    {
        lcd.setCursor(18, 0);
        lcd.print(time);
    }

} //end of loop