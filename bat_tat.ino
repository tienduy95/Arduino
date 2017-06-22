#include <LiquidCrystal.h>
#include <Wire.h> 
#include <EEPROM.h> // Thư viện EEPROM để lưu giá trị khi mất điện.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
boolean ok; 
/* khai báo các biến thời gian */
int dayx[]= {6,7,8,9,10,11,12,13,14,15,16,17,18,19};// mình sẽ mặc định ban ngày là như thế.
int night[] = {20,21,22,23,0,1,2,3,4,5}; // Ban đêm mặc định.
int nextdayhour[100], nextdayminute[100];
int second, minute, hour, day, wday, month, year;
int addr = 1;
boolean stat, caculator;
int delayday = 30, delaynight=120, delaytime=3;
int count=0;
int time2=0;
int co1, co2,co3, co4, l, i;
int lasthour, lastminute, nexthour, nextminute;
int timer=0;
int led = 13;
int led1=10;
void setup()
{
  pinMode(led, OUTPUT);
  pinMode(led1,OUTPUT);
  Wire.begin();
    /* cài đặt thời gian cho module */
  setTime(12, 30, 45, 1, 15, 10, 16); // 12:30:45 CN 08-02-2015
  Serial.begin(9600);
  lcd.begin(16, 2);
  caculator =true;
  
  //setTime(14,11, 0, 1, 20, 6, 16);
  
}
 
void loop(){  
      /* Đọc dữ liệu của DS1307 */
    readDS1307();
     if(hour == 12 && minute == 31 && second == 0)
           {
            digitalWrite(led, HIGH);
            delay(5000);
            digitalWrite(led, LOW);
            }
      else{
        digitalWrite(led,HIGH);
      }
    /* Hiển thị thời gian ra Serial monitor */
    digitalClockDisplay();
    delay(1000);

}

void readDS1307()
{
        Wire.beginTransmission(DS1307);
        Wire.write((byte)0x00);
        Wire.endTransmission();
        Wire.requestFrom(DS1307, NumberOfFields);
        
        second = bcd2dec(Wire.read() & 0x7f);
        minute = bcd2dec(Wire.read() );
        hour   = bcd2dec(Wire.read() & 0x3f);// chế độ 24h.
        wday   = bcd2dec(Wire.read() );
        day    = bcd2dec(Wire.read() );
        month  = bcd2dec(Wire.read() );
        year   = bcd2dec(Wire.read() );
        year += 2000;    
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}
 
void digitalClockDisplay(){
    // digital clock display of the time
    Serial.print(hour);
    printDigits(minute);
    printDigits(second);
    Serial.print(" ");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(month);
    Serial.print(" ");
    Serial.print(year); 
    Serial.println(); 
}
void printDigits(int digits){
      // các thành phần thời gian được ngăn chách bằng dấu :
      Serial.print(":");
          
      if(digits < 10)
          Serial.print('0');
      Serial.print(digits);
  }
/* cài đặt thời gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00)); // đặt lại pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}
