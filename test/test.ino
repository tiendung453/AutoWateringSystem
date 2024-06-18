#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

char ssid[] = "Okee"; // Tên của mạng WiFi
char pass[] = "04052005";  // Mật khẩu của mạng WiFi

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // Cấu hình NTP client, 7*3600 để lấy thời gian tại múi giờ số 7

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_ROWS    2

#define DHTPIN 15     // Chân kết nối cảm biến DHT11
#define DHTTYPE DHT11 // Loại cảm biến là DHT11

#define moistureSensorPin 35 // Chân analog input kết nối với cảm biến độ ẩm đất
#define buttonPin 5           // Chân kết nối nút nhấn
#define rainSensorPin 4       // Chân kết nối với cảm biến mưa 

#define soilWet 500 // khai báo ngưỡng đất ẩm 
#define soilDry 75  // khai báo ngưỡng đất khô
#define humiMax 70  // độ ẩm không khí cho phép max
#define humiMin 50  // độ ẩm không khí cho phép 
#define tempMax 24  // nhiệt độ cao nhất cho phép
#define tempMin 18  // nhiệt độ thấp nhất cho phép

#define timePumpAm 6 // thời gian tự động bơm vào buổi sáng
#define timePumpPm 18 // thời gian tự động bơm vào buổi chiều 

char command; // tạo biến đọc dữ liệu từ serial

DHT dht(DHTPIN, DHTTYPE); // Khởi tạo đối tượng DHT

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS); // Khởi tạo đối tượng LCD

int buttonState = HIGH;   // trạng thái nút nhấn lúc đầu
int lastButtonState = HIGH; // trạng thái nút nhấn lúc sau
unsigned long lastDebounceTime = 0; // thời gian chạy
unsigned long debounceDelay = 50;  // thời gian trì trệ
int dem = 0; // khai báo biến đếm để lấy giá trị số lần ấn nút

// khai báo các chân 
const int motorPin1 = 13;  // kết nối với IN1 trên L298N
const int motorPin2 = 12;  // kết nối với IN2 trên L298N
const int enablePinA = 14; // kết nối với ENA trên L298N
const int pumpPin3 = 27;   // kết nối với IN3 trên L298N
const int pumpPin4 = 25;   // kết nối với IN4 trên L298N
const int enablePinB = 33; // kết nối với ENB trên L298N

bool autoModeActive;
bool cancelAutoModeActive;
bool openRoof;
bool closeRoof;
bool onPumpM;
bool offPumpM;

unsigned long previousMillis = 0;
       
unsigned long openStartTime = 0; 
unsigned long closeStartTime = 0;
unsigned long moisTime = 0;  
bool roofOpening = false; // Thêm biến trạng thái để kiểm tra nếu mái che đang mở
bool roofClosing = false;

float temp; // đọc nhiệt độ
float humidity; // đọc độ ẩm 
double moistureValue; // đọc giá trị độ ẩm đất

void setup() 
{
  Serial.begin(115200);
  dht.begin(); // Khởi động cảm biến DHT11
  pinMode(moistureSensorPin, INPUT);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) // nếu trang thái wifi chưa kết nối thì in ra ....
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  timeClient.begin();  // khởi tạo đối tượng timeClient
  pinMode(buttonPin, INPUT_PULLUP); // chân nút nhấn là chế độ input_pullup
  pinMode(rainSensorPin, INPUT);

  // cấu hình các chân bơm và chân motor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePinA, OUTPUT);
  pinMode(pumpPin3, OUTPUT);
  pinMode(pumpPin4, OUTPUT);
  pinMode(enablePinB, OUTPUT);

  lcd.init(); // khởi tạo đối tượng lcd
  lcd.backlight(); // bật đèn nền 
  lcd.setCursor(0, 0); // bắt đầu tại vị trí (0 0);
  lcd.print("Press button to");
  lcd.setCursor(0, 1);
  lcd.print("get data");
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    command = Serial.read();
    if(command == '1')
    {
      autoMode();
    }
    if (command == '0')
    {
      stopRoofDoor();
    }
    if (command == '2')
    {
      openRoofDoor();
    }
    if (command == '3')
    {
      closeRoofDoor();
    }
    if (command == '4')
    {
      onPump();
    }
    if (command == '5')
    {
      offPump();
    }
  }

  if (roofOpening && (millis() - openStartTime >= 10000)) 
  {
    stopRoofDoor();
    roofOpening = false;
  }
  if (roofClosing && (millis() - closeStartTime >= 10000)) 
  {
    stopRoofDoor();
    roofClosing = false;
  }
  if (millis() - moisTime >= 3000)
  {
    temp = dht.readTemperature(); // đọc nhiệt độ
    humidity = dht.readHumidity(); // đọc độ ẩm 
    moistureValue = readMoisture(); // đọc giá trị độ ẩm đất
    Serial.println(moistureValue);
    displayInQt();
    moisTime = millis();
  }
  //int currentHour = timeClient.getHours(); // lấy về thời gian cụ thể là giờ

  timeClient.update(); // Cập nhật thời gian từ NTP server
  int currentHour = timeClient.getHours(); // lấy giờ từ ntpclient

  selectModeButtonLcd();
}

double readMoisture() 
{
  int rawValue = analogRead(moistureSensorPin);
  double adcValue = (double(rawValue) / 4095) * 100;

  adcValue = round(adcValue * 100) / 100.0;
  return adcValue;
}

void displayLcd()
{
  lcd.setCursor(0, 0); // bắt đầu tại vị trí (0 0);
  lcd.print("Press button to  ");
  lcd.setCursor(0, 1);
  lcd.print("get data");
}

void displayTempAndHumidity() 
{
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C      ");
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humidity);
  lcd.print(" %");
}

void displayRealTime() 
{
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(timeClient.getFormattedTime());
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("                      ");
}

void displayMoistureValue()
{
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moistureValue);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("                      ");
}

// mở mái che
void openRoofDoor() 
{
  openStartTime = millis();
  analogWrite(enablePinA, 255);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  roofOpening = true;
}

// đóng mái che
void closeRoofDoor() 
{
  closeStartTime = millis();
  analogWrite(enablePinA, 255);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  roofClosing = true;
}

// dừng mái che
void stopRoofDoor() 
{
  analogWrite(enablePinA, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  roofOpening = false;
}

// bật bơm
void onPump() 
{
  analogWrite(enablePinB, 255);
  digitalWrite(pumpPin3, HIGH);
  digitalWrite(pumpPin4, LOW);
}

// tắt bơm
void offPump() 
{
  analogWrite(enablePinB, 0);
  digitalWrite(pumpPin3, LOW);
  digitalWrite(pumpPin4, LOW);
}

void autoMode() 
{
  if ((temp > tempMax) || (humidity < humiMin) || (moistureValue > soilDry))
  {
    onPump();
  }
  if (checkRain())
  {
    offPump();
    if(moistureValue < soilWet)
    {
      closeRoofDoor();
    }
    else
    {
      openRoofDoor();
    }
  }
}

void displayInQt()
{
  //Đọc giá trị cảm biến và gửi qua Serial
  float temp = dht.readTemperature(); // đọc nhiệt độ
  float humidity = dht.readHumidity(); // đọc độ ẩm 
  double moistureValue = readMoisture(); // đọc giá trị độ ẩm đất
  int currentHour = timeClient.getHours(); // lấy về thời gian cụ thể là giờ

  if (!isnan(temp) && !isnan(humidity)) 
  {
    Serial.print(temp); // gửi nhiệt độ vào serial
    Serial.print(",");
    Serial.print(humidity); // gửi độ ẩm vào serail
    Serial.print(",");
    Serial.print(moistureValue); // gửi độ ẩm đất vào serial
    Serial.print(",");
    Serial.println(currentHour); // gửi độ ẩm đất vào serial

  }
}

bool checkRain()
{
  int sensorValue = digitalRead(rainSensorPin); // Đọc giá trị từ cảm biến mưa;
  if(sensorValue == 0)
  {
    return true;
  }
  if(sensorValue == 1)
  {
    return false;
  }
}

void selectModeButtonLcd()
{
  int NowbuttonState = digitalRead(buttonPin);
  
  if (NowbuttonState != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    if (NowbuttonState != buttonState) 
    {
      buttonState = NowbuttonState;
      if (buttonState == LOW) {
        dem = dem + 1;
        if(dem == 4)
        {
          dem = 0;
        }
        Serial.println(dem);
      }
    }
  }

  lastButtonState = NowbuttonState;
  if(dem == 0)
  {
    displayLcd();
  }
  if(dem == 1)
  {
    displayTempAndHumidity();
  }
  if(dem == 2)
  {
    displayRealTime();
  }
  if(dem == 3)
  {
    displayMoistureValue();
  }
}
