#include <Arduino.h>
#include <Pixy2I2C.h>
Pixy2I2C pixy;

void setup() {
      Serial.begin(19200);
      while (!Serial)
            ;
      pixy.init();
}

void loop() {
      short yellow_angle = 0, blue_angle = 0, yellow_wide = 0, blue_wide = 0, old_yellow_wide = 0, old_blue_wide = 0;

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
      Serial.write('a');
      Serial.write(yellow_angle);
      Serial.write(blue_angle);
      Serial.flush();
}