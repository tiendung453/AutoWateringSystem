#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

char ssid[] = "Phong 601"; // Tên của mạng WiFi
char pass[] = "94648089";  // Mật khẩu của mạng WiFi

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // Cấu hình NTP client , 7*3600 để lấy thời gian tại múi giờ số 7

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_ROWS    2

#define DHTPIN 2     // Chân kết nối cảm biến DHT11
#define DHTTYPE DHT11 // Loại cảm biến là DHT11

#define moistureSensorPin 26 // Chân analog input kết nối với cảm biến độ ẩm đất
#define buttonPin 5           // Chân kết nối nút nhấn
#define rainSensorPin 4       // Chân kết nối với cảm biến mưa 
#define autoPumpButtonPin 35  // khai báo nút bơm ngay lập tức 
#define ledPin 15             // Chân kết nối đèn LED báo trạng thái

#define soilWet 500 // khai báo ngưỡng đất ẩm 
#define soilDry 75  // khai báo ngưỡng đất khô
#define humiMax 70  // độ ẩm không khí cho phép max
#define humiMin 50  // độ ẩm không khí cho phép 
#define tempMax 24  // nhiệt độ cao nhất cho phép
#define tempMin 18  // nhiệt độ thấp nhất cho phép

#define timePumpAm 6 // thời gian tự động bơm vào buổi sáng
#define timePumpPm 18 // thời gian tự động bơm vào buổi chiều 

String command; // tạo biến đọc dữ liệu từ serial

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



void setup() 
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // chân nút nhấn là chế độ input_pullup
  pinMode(autoPumpButtonPin, INPUT_PULLUP); // chân nút bơm là chế độ input_pullup
  pinMode(ledPin, OUTPUT); // chân led là output
  pinMode(2, OUTPUT);

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
  
  displayLcd(); 
  selectModeButtonLcd();


  if (Serial.available() > 0) 
  {
    char command = Serial.read();
    if (command == '1') 
    {
      autoMode();
    }
    if (command != '1')
    {
      normalMode();
    }
  }
}

void displayLcd()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press button to change value");
  lcd.setCursor(0, 1);
  lcd.print("1 2 3");

}

void displayTempAndHumidity() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(dht.readHumidity());
  lcd.print(" %");
}

void displayRealTime() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(timeClient.getFormattedTime());
}

void displayMoistureValue()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(readMoistureSensor());
}

int readMoistureSensor()
{
  int moistureValue = analogRead(moistureSensorPin); // đọc giá trị cảm biến độ ẩm đất
  return moistureValue;
}

// mở mái che
void openRoofDoor() 
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  ledcWrite(0, 200); // Sử dụng kênh PWM 0 cho enablePinA với giá trị 200
}

// đóng mái che
void closeRoofDoor() 
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  ledcWrite(0, 200); // Sử dụng kênh PWM 0 cho enablePinA với giá trị 200
}

// dừng mái che
void stopRoofDoor() 
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  ledcWrite(0, 0); // Dừng kênh PWM 0
}

// bật bơm
void onPump() 
{
  digitalWrite(pumpPin3, HIGH);
  digitalWrite(pumpPin4, LOW);
  ledcWrite(1, 200); // Sử dụng kênh PWM 1 cho enablePinB với giá trị 200

}

// tắt bơm
void offPump() 
{
  digitalWrite(pumpPin3, LOW);
  digitalWrite(pumpPin4, LOW);
  ledcWrite(1, 0); // Dừng kênh PWM 1
}

void buttonPump() 
{
  int reading = digitalRead(autoPumpButtonPin); // đọc tín hiệu chân nút autoPump
  if (reading != lastButtonState) 
  {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    if (reading != buttonState) 
    {
      buttonState = reading;
      if (buttonState == LOW) 
      {
        onPump();
      }
    }
  }
  lastButtonState = reading;
}

void autoMode() 
{
  digitalWrite(2, HIGH);
  // Đọc giá trị cảm biến và gửi qua Serial
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
    Serial.println(moistureValue); // gửi độ ẩm đất vào serial
  }

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
void normalMode()
{
    digitalWrite(2, LOW);
  // Đọc giá trị cảm biến và gửi qua Serial
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int moistureValue = analogRead(moistureSensorPin);

  if (!isnan(temp) && !isnan(humidity)) 
  {
    Serial.print(temp);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.println(moistureValue);
  }
  char command = Serial.read(); // đọc giá trị ở bộ đệm serial
  if(command == '2')
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
      }
    }
  }


  lastButtonState = NowbuttonState;

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
