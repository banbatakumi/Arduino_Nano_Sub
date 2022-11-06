//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This sketch is a good place to start if you're just getting started with 
// Pixy and Arduino.  This program simply prints the detected object blocks 
// (including color codes) through the serial console.  It uses the Arduino's 
// ICSP SPI port.  For more information go here:
//
// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
//

#include <Arduino.h>
#include <Pixy2I2C.h>
Pixy2I2C pixy;

void setup() {
	Serial.begin(19200);
	while (!Serial);
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




/*
#include <Arduino.h>

#define I2C
#ifdef I2C
#include <Pixy2I2C.h>
Pixy2I2C pixy;
#else

#include <Pixy2.h>
Pixy2 pixy;
#endif

#define LED 13

short left_distance = 0, right_distance = 0;

void setup() {
	Serial.begin(19200); // シリアルモニタの開始
	pinMode(LED, OUTPUT);

	for (int count = 0; count < 20; count++) {
		digitalWrite(LED, HIGH);
		delay(50);
		digitalWrite(LED, LOW);
		delay(50);
	}
}

void loop() {
	Serial.write('a');
	Serial.write();
	Serial.write();

	/*シリアルモニタ出力
	Serial.print(left_distance);
	Serial.print(", ");
	Serial.println(right_distance);
	delay(100);

}



void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");


}

void loop()
{
  int i;
  // grab blocks!
  pixy.ccc.getBlocks();



  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks){
	Serial.print("Detected ");
	Serial.println(pixy.ccc.numBlocks);
	for (i=0; i<pixy.ccc.numBlocks; i++){
	  Serial.print(" block ");
	  Serial.print(i);
	  Serial.print(": ");
	  pixy.ccc.blocks[i].print();
	}
  }
}
*/