#include <Arduino.h>
#include <Pixy2I2C.h>

Pixy2I2C pixy;

#define TRIG_ECHO_PIN 14
#define DISTABCE_RC 0.6
#define LED_PIN 13

uint16_t pre_distance, distance, duration;

void setup() {
      Serial.begin(19200);
      pixy.init();

      pinMode(LED_PIN, OUTPUT);
      for (uint8_t count = 0; count <= 10; count++) {
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
            delay(100);
      }
}

void (*resetFunc)(void) = 0;

void loop() {
      int16_t yellow_angle = 0, blue_angle = 0, yellow_wide = 0, blue_wide = 0, old_yellow_wide = 0, old_blue_wide = 0;

      pixy.ccc.getBlocks();
      if (pixy.ccc.numBlocks) {
            for (int count = 0; count < pixy.ccc.numBlocks; count++) {
                  if (pixy.ccc.blocks[count].m_signature == 1 && pixy.ccc.blocks[count].m_y > 100) {
                        old_yellow_wide = yellow_wide;
                        yellow_wide = pixy.ccc.blocks[count].m_width;
                        if (yellow_wide > old_yellow_wide) yellow_angle = pixy.ccc.blocks[count].m_x / 1.5;
                  }
                  if (pixy.ccc.blocks[count].m_signature == 2 && pixy.ccc.blocks[count].m_y > 100) {
                        old_blue_wide = blue_wide;
                        blue_wide = pixy.ccc.blocks[count].m_width;
                        if (blue_wide > old_blue_wide) blue_angle = pixy.ccc.blocks[count].m_x / 1.5;
                  }
            }
      }

      pinMode(TRIG_ECHO_PIN, OUTPUT);
      digitalWrite(TRIG_ECHO_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_ECHO_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_ECHO_PIN, LOW);

      pinMode(TRIG_ECHO_PIN, INPUT);
      duration = pulseIn(TRIG_ECHO_PIN, HIGH);   // 往復にかかった時間が返却される[マイクロ秒]
      if (duration > 0) {
            duration /= 2;   // 往路にかかった時間
            distance = duration * 340.000 * 100.000 / 1000000.000;
            distance = distance * (1 - DISTABCE_RC) + pre_distance * DISTABCE_RC;
            pre_distance = distance;
            if (distance > 255) distance = 255;
      }

      Serial.write('a');
      Serial.write(yellow_angle);
      Serial.write(blue_angle);
      Serial.write(distance);
      Serial.flush();

      if (Serial.read() == 'a') resetFunc();   // call reset
}