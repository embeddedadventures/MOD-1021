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

#ifndef __MOD-1021_h
#define	__MOD-1021_h

#include "HardwareSerial.h"

#define uns8 unsigned char
#define uns16 unsigned int
//Byte definitions
#define MP3_START		0x7E
#define MP3_VERSION		0xFF
#define MP3_LENGTH		0x06
#define	MP3_FEEDBK		0x01
#define MP3_NOFEEDBK	0x00
#define MP3_END			0xEF
//Command Types
#define MP3_RESET		0x0C,	0x00
#define MP3_NORMAL		0x0B,	0x00
#define MP3_STANDBY		0x0A,	0x00



class MOD1021Class 
{
private:
	HardwareSerial *mySerial;
	int fastPress1;
	int fastPress2;
	uns16 current_volume;
	uns8 current_equalizer;
	uns8 current_playback_mode;
	uns8 current_playback_source;
	void send_command(uns8 cmd, uns16 param);
	void print_response();
	void get_response();
	
public:
	void init(HardwareSerial *cereal);
	void normal_mode();
	void standby_mode();
	void print_stats();
	void next_track();
	void previous_track();
	void set_track(uns16 track);				//Track 0 - 2999, defaults to 0 if above 2999
	void volume_up();
	void volume_down();
	void set_volume(uns16 volume);				//Volume 0 - 30, defaults to 30
	void set_EQ(uns16 eq);						//EQ 0 - 5, prints error if invalid input
	void set_playback_mode(uns16 mode);
	void set_playback_source(uns16 src);
	void play();
	void pause();
	void set_folder(uns8 folder, uns8 mp3);
	void volume_adjust(uns8 gain);
	void repeat_play(uns16 repeat);
	
	uns8  get_volume();
	uns8  get_EQ();
	uns8  get_playback_mode();
	uns8  get_version();
	uns16 get_num_files();
	
};

extern MOD1021Class mod1021;

#endif