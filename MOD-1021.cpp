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

// SoundOut MOD-1021 MP3 audio playback module Arduino library
// Written originally by Embedded Adventures

#include "MOD-1021.h"
#include "Arduino.h"
#include "HardwareSerial.h"

uns8 cmd_buffer[10];
uns8 response[10];

void MOD1021Class::init(HardwareSerial *cereal) {
	mySerial = cereal;
	mySerial->begin(9600);
	Serial.println("MOD1021 Initialized");
	send_command(MP3_RESET);
	delay(5);
}

void MOD1021Class::send_command(uns8 cmd, uns16 param) {
	cmd_buffer[0] = MP3_START;
	int checksum = 0;
	cmd_buffer[1] = MP3_VERSION;
	cmd_buffer[2] = MP3_LENGTH;
	cmd_buffer[3] = cmd;
	cmd_buffer[4] = MP3_NOFEEDBK;
	cmd_buffer[5] = param >> 8;
	cmd_buffer[6] = param & 0xFF;
	cmd_buffer[9] = MP3_END;
	
	for (int i = 1; i < 7; i++) {
		checksum -= cmd_buffer[i];
	}
	
	cmd_buffer[7] = checksum >> 8;
	cmd_buffer[8] = checksum & 0xFF;
	
	for (int i = 0; i < 10; i++) {
		mySerial->write(cmd_buffer[i]);
	}
}

void MOD1021Class::print_response() {
	Serial.println("Reponse from module");
	for (int i = 0; i < 10; i++) {
		Serial.print(response[i], HEX);
		Serial.print(" ");
	}
	Serial.println();
}

void MOD1021Class::get_response() {
	int i = 0;
	delay(1000);
	while(mySerial->available()){
		response[i] = (char)mySerial->read();
		i++;
	}
}

void MOD1021Class::print_stats() {
	current_volume = get_volume();
	current_equalizer = get_EQ();
	current_playback_mode = get_playback_mode();
	Serial.println("Current: Volume, EQ, Playback Mode");
	String s = (String)current_volume;
	s += " ";
	s += (String)current_equalizer;
	s += " ";
	s += (String)current_playback_mode;
	Serial.println(s);
}

void MOD1021Class::normal_mode() {
	send_command(MP3_NORMAL);
}

void MOD1021Class::standby_mode() {
	send_command(MP3_STANDBY);
}

void MOD1021Class::next_track() {
	send_command(0x01, 0);
}

void MOD1021Class::previous_track() {
	send_command(0x02, 0);
}

void MOD1021Class::set_track(uns16 track) {
	if (track >= 3000)
		track = 0;
	send_command(0x03, track);
}

void MOD1021Class::volume_up() {
	send_command(0x04, 0);
	if (get_volume() == 30)
		Serial.println("Volume is at maximum");
}

void MOD1021Class::volume_down() {
	send_command(0x05, 0);
	if (get_volume() == 0)
		Serial.println("Volume is at minimum");
}
void MOD1021Class::set_volume(uns16 volume){
	if ((volume <= 30) || (volume >= 0))
		send_command(0x06, volume);
	else
		send_command(0x06, 30);
}

void MOD1021Class::set_EQ(uns16 eq) {
	if ((eq > 5)||(eq < 0))
		Serial.println("Invalid EQ input");
	else
		send_command(0x07, eq);
}

void MOD1021Class::set_playback_mode(uns16 mode) {
	if (mode < 4)
		send_command(0x08, mode);
	else
		Serial.println("Invalid playback mode input");
}

void MOD1021Class::set_playback_source(uns16 src) {
	if (src < 5)
		send_command(0x09, src);
	else
		Serial.println("Invalid playback source input");
	delay(200);
}

void MOD1021Class::play() {
	send_command(0x0D, 0);
}

void MOD1021Class::pause() {
	send_command(0x0E, 0);
}

void MOD1021Class::set_folder(uns8 folder, uns8 mp3) {
	if ((folder < 1) || (folder > 10))
		Serial.println("Invalid folder input");
	else {
		uns16 param = (folder << 8) + (mp3);
		send_command(0x0F, param);
	}
}

void MOD1021Class::volume_adjust(uns8 gain) {
	if (gain > 31)
		gain = 30;
	send_command(0x10, (0x0100 + gain));
}

void MOD1021Class::repeat_play(uns16 repeat) {
	send_command(0x11, repeat);
}


uns8 MOD1021Class::get_volume() {
	send_command(0x43, 0);
	get_response();
	Serial.print("Query volume response: ");
	Serial.println((uns16)(response[5] << 8 | response[6]), HEX);
	return response[5];
}

uns8  MOD1021Class::get_EQ() {
	send_command(0x44, 0);
	get_response();
	Serial.print("Query EQ response: ");
	Serial.println(response[5], HEX);
	return response[5];
}

uns8  MOD1021Class::get_playback_mode() {
	send_command(0x45, 0);
	get_response();
	Serial.print("Query playback mode response: ");
	Serial.println(response[5], HEX);
	return response[5];
}

uns8 MOD1021Class::get_version() {
	send_command(0x46, 0);
	get_response();
	Serial.print("Query version response: ");
	Serial.println(response[5], HEX);
	return response[5];
}

uns16 MOD1021Class::get_num_files() {
	send_command(0x47, 0);
	get_response();
	Serial.print("Number of files in card: ");
	Serial.println(response[5], HEX);
	return response[5];
}

MOD1021Class mod1021;

