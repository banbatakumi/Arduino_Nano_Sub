#include <Arduino.h>
#include <Pixy2I2C.h>

Pixy2I2C pixy;

#define RED_PIN 9
#define BLUE_PIN 6
#define GREEN_PIN 5

void rgb_led(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0);

int currentTime, LEDTimer, count;

void setup() {
      for (uint8_t count = 0; count < 15; count++) {
            rgb_led(0, 255, 0);
            delay(100);
            rgb_led(0, 0, 0);
            delay(100);
      }
      Serial.begin(19200);
      pixy.init();
}
void (*resetFunc)(void) = 0;

void loop() {
      char get_command = 0;
      int16_t yellow_angle = 0, blue_angle = 0, yellow_wide = 0, blue_wide = 0, tmp_yellow_wide = 0, tmp_blue_wide = 0, old_yellow_wide = 0, old_blue_wide = 0;

      pixy.ccc.getBlocks();
      if (pixy.ccc.numBlocks) {
            for (int count = 0; count < pixy.ccc.numBlocks; count++) {
                  if (pixy.ccc.blocks[count].m_signature == 1 && pixy.ccc.blocks[count].m_y > 100) {
                        old_yellow_wide = tmp_yellow_wide;
                        tmp_yellow_wide = pixy.ccc.blocks[count].m_width;
                        if (tmp_yellow_wide > old_yellow_wide){
                              yellow_angle = pixy.ccc.blocks[count].m_x / 1.5;
                              yellow_wide = tmp_yellow_wide / 5;
                        }
                  }
                  if (pixy.ccc.blocks[count].m_signature == 2 && pixy.ccc.blocks[count].m_y > 100) {
                        old_blue_wide = tmp_blue_wide;
                        tmp_blue_wide = pixy.ccc.blocks[count].m_width;
                        if (tmp_blue_wide > old_blue_wide){
                              blue_angle = pixy.ccc.blocks[count].m_x / 1.5;
                              blue_wide = tmp_blue_wide / 5;
                        } 
                  }
            }
      }
      Serial.write('a');
      Serial.write(yellow_angle);
      Serial.write(blue_angle);
      Serial.write(yellow_wide);
      Serial.write(blue_wide);
      Serial.flush();

      if (Serial.available()) get_command = Serial.read();
      if (get_command == 'a') {
            resetFunc();   // call reset
      } else if (get_command == 'b') {
            for (uint8_t count = 0; count < 12; count++) {
                  rgb_led(255, 0, 255);
                  delay(100);
                  rgb_led(0, 0, 0);
                  delay(100);
            }
      } else if (get_command == 'c') {
            for (uint8_t count = 0; count < 12; count++) {
                  rgb_led(0, 0, 255);
                  delay(100);
                  rgb_led(0, 0, 0);
                  delay(100);
            }
      }
}

void rgb_led(uint8_t red, uint8_t green, uint8_t blue) {
      analogWrite(RED_PIN, red);
      analogWrite(GREEN_PIN, green);
      analogWrite(BLUE_PIN, blue);
}