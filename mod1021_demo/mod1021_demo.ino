/*
Copyright (c) 2015, Embedded Adventures
All rights reserved.
Contact us at source [at] embeddedadventures.com
www.embeddedadventures.com
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
*/

// SoundOut MP3 MOD-1021 audio playback Arduino test sketch
// Written originally by Embedded Adventures

#include <MOD-1021.h>

void setup() {
  Serial.begin(115200);
  mod1021.init(&Serial1);
  while(!Serial1){  }
  while(!Serial){  };
  Serial.println("Embedded Adventures MOD-1021 audio playback module test sketch");
  Serial.println("Embedded Adventures (embeddedadventures.com)");
  mod1021.normal_mode();
  printMenu();
}

void printMenu() {
  Serial.println("\nMOD-1021 demonstration sketch OPTIONS");
  Serial.println("------------------------------------\n");
  Serial.println("Play\t\ta\nPause\t\tp\nNext Track\tn\nPrevious Track\tm\nVolume Down\td\nVolume Up\tu\nVolume?\t\tv");
}

void loop() {
  char input;
  if (Serial.available()) {
    input = Serial.read();
    translate_cmd(input);
  }
}

void translate_cmd(char input) {
  Serial.println();
  switch (input) {
    case 'a':
      Serial.println("Play entered");
      mod1021.play();
      break;
    case 'p':
      Serial.println("Pause");
      mod1021.pause();
      break;
    case 'n':
      Serial.println("Next track");
      mod1021.next_track();
      break;
    case 'm':
      Serial.println("Previous track");
      mod1021.next_track();
      break;
    case 'd':
      Serial.println("Volume down");
      mod1021.volume_down();
      break;
    case 'u':
      Serial.println("Volume up");
      mod1021.volume_up();
      break;
    case 'v':
      Serial.println("Query volume");
      mod1021.get_volume();
      break;
  }

}

