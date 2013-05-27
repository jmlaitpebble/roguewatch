//
// Message functions
//
//
// RAM used: 37 bytes
//

#pragma once

#define MSG_MAXLEN	(4 * GFX_TEXTWIDTH)
#define MSG_MINDRAWLEN (2 * GFX_TEXTWIDTH)
#define MSG_WIDTH	GFX_TEXTWIDTH
	
void msg_clear();

bool msg_empty();

void msg_report(const char *s);
void msg_reportint(int number);

void msg_draw();

void msg_endline();

