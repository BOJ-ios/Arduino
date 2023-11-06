#include <WiFiUdp.h>
#include <TM1637Display.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>

const char *ssid = "healing 201";
const char *password = "0553213247";

int previousMin = 0;

WiFiUDP udp;
NTPClient timeClient(udp, "kr.pool.ntp.org", 32400, 3600000);
// 32400을 더해 kr 시간에 맞게 조정

#define CLK 2
#define DIO 8
TM1637Display display(CLK, DIO);

void setup()
{
    display.setBrightness(7, true);
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    timeClient.begin();
}

void loop()
{
    timeClient.update();
    int currentMin = timeClient.getMinutes();
    if (previousMin != currentMin)
    {
        previousMin = currentMin;
        int currentTime = timeClient.getHours() * 100 + currentMin;
    }

    Serial.println(timeClient.getFormattedTime());
    // display.showNumberDecEx(timeClient.getHours(), 0b11100000, false, 2, 0);
    // display.showNumberDecEx(timeClient.getMinutes(), 0b11100000, false, 2, 2);
    // 출력값, Dot표현위치 (왼쪽0x80, 0x40, 0x20, 0x10) 순으로 지정하면됨. 쉽죠??

    display.showNumberDecEx(timeClient.getHours(), 0x40, true, 2, 0);
    display.showNumberDecEx(timeClient.getMinutes(), 0, true, 2, 2);

    delay(1000);
}