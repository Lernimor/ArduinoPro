#include <Wire.h>
#include "U8glib.h"
#include "IR_Thermometer_Sensor_MLX90614.h"

IR_Thermometer_Sensor_MLX90614 MLX90614 = IR_Thermometer_Sensor_MLX90614();

//  MLX90614.GetAmbientTemp_Celsius() 环境温度 摄氏度C'
//  MLX90614.GetObjectTemp_Celsius()  目标温度 摄氏度C'
//  MLX90614.GetAmbientTemp_Fahrenheit()) 环境温度 华氏度F'
//  MLX90614.GetObjectTemp_Fahrenheit() 目标温度 华氏度F'

U8GLIB_SSD1309_128X64 u8g(13, 11, 10, 3);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 3

static const unsigned char wendu[3][32] PROGMEM={
  {0x10,0x02,0x10,0x02,0x10,0x02,0x08,0x02,0xE8,0x3F,0x0C,0x02,0x0C,0x07,0x0A,0x07,
    0x89,0x0A,0x88,0x0A,0x48,0x12,0xA8,0x2F,0x18,0x42,0x08,0x02,0x08,0x02,0x08,0x02},/*"体",0*/

  {0x40,0x00,0x80,0x00,0xFE,0x7F,0x02,0x40,0x01,0x20,0xFC,0x1F,0x20,0x00,0x10,0x04,
    0xF8,0x0F,0x80,0x08,0x80,0x00,0xFC,0x1F,0x80,0x00,0x80,0x00,0xFF,0x7F,0x00,0x00},/*"室",1*/
  
  {0x00,0x00,0xC4,0x1F,0x48,0x10,0x48,0x10,0xC1,0x1F,0x42,0x10,0x42,0x10,0xC8,0x1F,
    0x08,0x00,0xE4,0x3F,0x27,0x25,0x24,0x25,0x24,0x25,0x24,0x25,0xF4,0x7F,0x00,0x00}/*"温",2*/
};

int in = 2;

void setup() {
    MLX90614.begin(); 
    pinMode(in, INPUT_PULLUP);
    drawStart();
}

void loop() {
  if(digitalRead(in) == LOW){
    showTemperature();
    delay(2000);
  }
}

void showTemperature(){
  int times = 0;
  float ambient = 0;
  const int len = 15;
  float temps[len];
  int i = 0;
    do  {
      ambient += MLX90614.GetAmbientTemp_Celsius();
      temps[i] = MLX90614.GetObjectTemp_Celsius();
      i++;
      delay(100);
    } while (i < len);
    
    float max=temps[0], min=temps[0],  sum=temps[0];
    double temp;
    for(int a=1; a<len; a++)
    {
       if(temps[a]>max)
       {
          max=temps[a];
       }
       else if(temps[a]<min)
       {
          min=temps[a];
       }
       sum+=temps[a];
    }

    temp=TranslateTemp(1.0*(sum-max-min)/(len-2));
    u8g.firstPage();  
    do {
      draw(ambient/len, temp);
    } while( u8g.nextPage() );
}


void drawStart(){
    u8g.firstPage();  
    do {
      u8g.setFont(u8g_font_osb18r); 
      u8g.setPrintPos(27, 40); 
      u8g.print("Ready!");
    } while(u8g.nextPage());
}

void draw(float ambient, float temp) {
  u8g.setFont(u8g_font_osb18r);
  /*室温-汉字*/
  u8g.drawXBMP(0, 10, 16, 16, wendu[1]);
  u8g.drawXBMP(16, 10, 16, 16, wendu[2]);

  u8g.setPrintPos(34, 25); 
  u8g.print(" ");
  u8g.print(ambient);
  u8g.print("C");
  
 /*体温-汉字*/
  u8g.drawXBMP(0, 40, 16, 16, wendu[0]);
  u8g.drawXBMP(16, 40, 16, 16, wendu[2]);
  
  u8g.setPrintPos(34, 57); 
  u8g.print(" ");
  u8g.print(temp);
  u8g.print("C");
 
}

float TranslateTemp(float temp){
    if (temp < 31)
      return temp;
    else if (31 <= temp && temp < 31.2)
      return temp * 33.4 / 31;  
    else if (31.2 <= temp && temp < 31.4)
      return temp * 33.6 / 31.2;  
    else if (31.4 <= temp && temp < 31.6)
      return temp * 33.8 / 31.4;  
    else if (31.6 <= temp && temp < 31.8)
      return temp * 34 / 31.6;    
    else if (31.8 <= temp && temp < 32)
      return temp * 34.2 / 31.8;  
      
    else if (32 <= temp && temp < 32.2)
      return temp * 34.4 / 32;  
    else if (32.2 <= temp && temp < 32.4)
      return temp * 34.6 / 32.2;  
    else if (32.4 <= temp && temp < 32.6)
      return temp * 34.8 / 32.4;  
    else if (32.6 <= temp && temp < 32.8)
      return temp * 35 / 32.6;    
    else if (32.8 <= temp && temp < 33)
      return temp * 35.1 / 32.8;  
      
    else if (33 <= temp && temp < 33.2)
      return temp * 35.3 / 33;  
    else if (33.2 <= temp && temp < 33.4)
      return temp * 35.5 / 33.2;  
    else if (33.4 <= temp && temp < 33.6)
      return temp * 35.7 / 33.4;  
    else if (33.6 <= temp && temp < 33.8)
      return temp * 35.9 / 33.6;    
    else if (33.8 <= temp && temp < 34)
      return temp * 36 / 33.8;  

    else if (34 <= temp && temp < 34.2)
      return temp * 36.1 / 34;  
    else if (34.2 <= temp && temp < 34.4)
      return temp * 36.4 / 34.2;  
    else if (34.4 <= temp && temp < 34.6)
      return temp * 36.5 / 34.4;  
    else if (34.6 <= temp && temp < 34.8)
      return temp * 36.7 / 34.6;    
    else if (34.8 <= temp && temp < 35)
      return temp * 36.9 / 34.8;  

    else if (35 <= temp && temp < 35.2)
      return temp * 37 / 35;  
    else if (35.2 <= temp && temp < 35.4)
      return temp * 37.2 / 35.2;  
    else if (35.4 <= temp && temp < 35.6)
      return temp * 37.3 / 35.4;  
    else if (35.6 <= temp && temp < 35.8)
      return temp * 37.5 / 35.6;    
    else if (35.8 <= temp && temp < 36)
      return temp * 37.7 / 35.8;

    else if (36 <= temp && temp < 36.2)
      return temp * 37.8 / 36;  
    else if (36.2 <= temp && temp < 36.4)
      return temp * 38 / 36.2;  
    else if (36.4 <= temp && temp < 36.6)
      return temp * 38.1 / 36.4;  
    else if (36.6 <= temp && temp < 36.8)
      return temp * 38.2 / 36.6;    
    else if (36.8 <= temp && temp < 37)
      return temp * 38.4 / 36.8;  

    else if (37 <= temp && temp < 37.2)
      return temp * 38.5 / 37;  
    else if (37.2 <= temp && temp < 37.4)
      return temp * 38.7 / 37.2;  
    else if (37.4 <= temp && temp < 37.6)
      return temp * 38.8 / 37.4;  
    else if (37.6 <= temp && temp < 37.8)
      return temp * 38.9 / 37.6;    
    else if (37.8 <= temp && temp < 38)
      return temp * 39.1 / 37.8;  

    else if (38 <= temp && temp < 38.2)
      return temp * 39.2 / 38;  
    else if (38.2 <= temp && temp < 38.4)
      return temp * 39.3 / 38.2;  
    else if (38.4 <= temp && temp < 38.6)
      return temp * 39.5 / 38.4;  
    else if (38.6 <= temp && temp < 38.8)
      return temp * 39.6 / 38.6;
    else if (38.8 <= temp && temp < 40)
      return temp * 39.7 / 38.8;

    else if (temp > 40)
      return temp;
}
