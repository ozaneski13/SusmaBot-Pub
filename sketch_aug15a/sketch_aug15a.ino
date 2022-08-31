#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String networkID = "";
String networkPass = "";

int rxPin = 10;
int txPin = 11;

String thingSpeakIPAddress = "184.106.153.149";

SoftwareSerial esp(rxPin, txPin);

int resval = 0;
int respin = A3;

unsigned long prevMillis = 0;
const long interval = 1000;

void setup()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }

  display.clearDisplay();
  draw(false);

  esp.begin(115200);
  esp.println("AT");

  while (!esp.find("OK"))
  {
    esp.println("AT");
  }

  esp.println("AT+CWMODE=1");

  while (!esp.find("OK"))
  {
    esp.println("AT+CWMODE=1");
  }

  esp.println("AT+CWJAP=\"" + networkID + "\",\"" + networkPass + "\"");

  while (!esp.find("OK"));

  delay(1000);
}

void loop()
{
  draw(true);
  
  esp.println("AT+CIPSTART=\"TCP\",\"" + thingSpeakIPAddress + "\",80");

  String thingSpeakData = "";

  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= interval)
  {
    prevMillis = currentMillis;

    resval = analogRead(respin);
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    display.print("Res value is: ");
    display.println(resval);
    display.display();

    thingSpeakData += String(resval);
    thingSpeakData += "\r\n\r\n";

    esp.print("AT+CIPSEND=");
    esp.println(thingSpeakData.length() + 2);
    delay(2000);

    if (esp.find(">"))
    {
      esp.print(thingSpeakData);
      delay(1000);
    }

    esp.println("AT+CIPCLOSE");
    delay(1000);
  }
}

void draw(bool status)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);

  if (status)
    display.write("Detection is ON!");
  else
    display.write("Detection is OFF!");
    
  display.display();
}
