#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

char ssid[] = "Okee"; // Tên của mạng WiFi
char pass[] = "04052005";  // Mật khẩu của mạng WiFi

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // Cấu hình NTP client , 7*3600 để lấy thời gian tại múi giờ số 7

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_ROWS    2

#define DHTPIN 15     // Chân kết nối cảm biến DHT11
#define DHTTYPE DHT11 // Loại cảm biến là DHT11

#define moistureSensorPin 26 // Chân analog input kết nối với cảm biến độ ẩm đất
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
char command1;

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

bool openState = false;        
bool commandProcessed = false;   
unsigned long openStartTime = 0; 

void setup() 
{
  Serial.begin(115200);
  pinMode(moistureSensorPin, INPUT);
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

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) // nếu trang thái wifi chưa kết nối thì in ra ....
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  timeClient.begin();  // khởi tạo đối tượng timeClient
}

void loop() 
{
  timeClient.update(); // Cập nhật thời gian từ NTP server
  int currentHour = timeClient.getHours(); // lấy giờ từ ntpclient
 
  selectModeButtonLcd();
  displayInQt();

  checkSerial();
  //Serial.println(command);
  if(autoModeActive)
  {
    autoMode();   
  }
  if(cancelAutoModeActive)
  {
    autoModeActive = false;
    stopRoofDoor();
  }
  if(openRoof)
  {
    openRoofDoor();
  }
  if(closeRoof)
  {
    closeRoofDoor();
  }
  if(onPumpM)
  {
    onPump();
  }
  if(offPumpM)
  {
    offPump();
  }
}

void threeSecond()
{
    if (openState && millis() - openStartTime >= 2000) 
  {
    stopRoofDoor();
    commandProcessed = false; 
  }
}

void checkSerial()
{
    if (Serial.available() > 0) 
    {
      command = Serial.read();
      if(command == '1')
      {
        autoModeActive = true;
        cancelAutoModeActive = false;
        openRoof = false;
        closeRoof = false;
        onPumpM = false;
        offPumpM = false;
      }
      if (command == '0')
      {
        autoModeActive = false;
        cancelAutoModeActive = true;
        openRoof = false;
        closeRoof = false;
        onPumpM = false;
        offPumpM = false;
      }
      if (command == '2')
      {
        autoModeActive = false;
        cancelAutoModeActive = false;
        openRoof = true;
        closeRoof = false;
        onPumpM = false;
        offPumpM = false;
      }
      if (command == '3')
      {
        autoModeActive = false;
        cancelAutoModeActive = false;
        openRoof = false;
        closeRoof = true;
        onPumpM = false;
        offPumpM = false;
      }
      if (command == '4')
      {
        autoModeActive = false;
        cancelAutoModeActive = false;
        openRoof = false;
        closeRoof = false;
        onPumpM = true;
        offPumpM = false;
      }
      if (command == '5')
      {
        autoModeActive = false;
        cancelAutoModeActive = false;
        openRoof = false;
        closeRoof = false;
        onPumpM = false;
        offPumpM = true;
      }
      
    }
       
}



void displayLcd()
{
  lcd.setCursor(0, 0); // bắt đầu tại vị trí (0 0);
  lcd.print("Press button to");
  lcd.setCursor(0, 1);
  lcd.print("get data");
}

void displayTempAndHumidity() 
{
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print(" C  ");
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(dht.readHumidity());
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
  lcd.print(analogRead(moistureSensorPin));
  lcd.setCursor(0, 1);
  lcd.print("                      ");
}

// mở mái che
void openRoofDoor() 
{
  analogWrite(enablePinA, 200);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

// đóng mái che
void closeRoofDoor() 
{
  analogWrite(enablePinA, 200);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

// dừng mái che
void stopRoofDoor() 
{
  analogWrite(enablePinA, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
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
  float temp = dht.readTemperature(); // đọc nhiệt độ
  float humidity = dht.readHumidity(); // đọc độ ẩm 
  int moistureValue = analogRead(moistureSensorPin); // đọc giá trị độ ẩm đất
  int currentHour = timeClient.getHours(); // lấy về thời gian cụ thể là giờ
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
  int moistureValue = analogRead(moistureSensorPin); // đọc giá trị độ ẩm đất
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
  if(sensorValue == 1)
  {
    return true;
  }
  if(sensorValue == 0)
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
        dem = dem+1;
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
