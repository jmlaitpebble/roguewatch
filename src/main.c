#include <pebble.h>
#include <time.h>
	
#include "config.h"

#include "mytypes.h"
#include "rand.h"
	
#include "gfxengine.h"
#include "msg.h"
#include "map.h"
#include "mob.h"
#include "rogue.h"

Window *glbWindowP;
TextLayer *glbWatchLayerP;

bool glbLive  = false;

#define WIDTH 144
#define HEIGHT 168
	
#define REFRESH_RATE 50
#define INTEGRATE_TIMER_ID 1
	
char glbMapText[(GFX_TEXTWIDTH+1) * GFX_TEXTHEIGHT];
	
int glbTargetMinute = -1;
bool glbMovedThisMinute = false;
GFont glbMonoFont;

void
click_handler(ClickRecognizerRef clickref, void *context)
{
	ButtonId		button = click_recognizer_get_button_id(clickref);
	
	int				dirkey = 0;
	switch (button)
	{
		case BUTTON_ID_DOWN:
			dirkey = 1;
			break;
		case BUTTON_ID_UP:
			dirkey = -1;
			break;
		case BUTTON_ID_SELECT:
			dirkey = 0;
			break;
		default:
			break;
	}
	
	rogue_tick(dirkey);
	
	glbMovedThisMinute = true;
	text_layer_set_text(glbWatchLayerP, glbMapText);
}

void
config_provider(void *context)
{
	window_single_repeating_click_subscribe(BUTTON_ID_UP, 250, click_handler);
// Do not use key repeat as we want to reserve this for long press
// and also because it reboots the watch.
//	config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 250;
	window_single_click_subscribe(BUTTON_ID_SELECT, click_handler);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 250, click_handler);
}
	
void handle_init() 
{
	glbWindowP = window_create();
	window_set_fullscreen(glbWindowP, true);
	
#if !SCREENSAVER_MODE
	window_set_click_config_provider(glbWindowP, (ClickConfigProvider) config_provider);
#endif
	
	window_stack_push(glbWindowP, true /* Animated */);
#if INVERT_COLOURS
	window_set_background_color(glbWindowP, GColorBlack);
#else
	window_set_background_color(glbWindowP, GColorWhite);
#endif
	
	glbMonoFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DROIDMONO_12));

	glbWatchLayerP = text_layer_create(
		layer_get_frame(window_get_root_layer(glbWindowP)));
		
#if INVERT_COLOURS
	text_layer_set_background_color(glbWatchLayerP, GColorBlack);
	text_layer_set_text_color(glbWatchLayerP, GColorWhite);
#else
	text_layer_set_background_color(glbWatchLayerP, GColorWhite);
	text_layer_set_text_color(glbWatchLayerP, GColorBlack);
#endif
	// I'd wrather have this clip on overflow.
	text_layer_set_overflow_mode(glbWatchLayerP, GTextOverflowModeTrailingEllipsis);
	text_layer_set_text(glbWatchLayerP, glbMapText);
	text_layer_set_font(glbWatchLayerP, glbMonoFont);
	layer_add_child(window_get_root_layer(glbWindowP), (Layer *)glbWatchLayerP);
	
	rand_seed();
	
	for (int y = 0; y < GFX_TEXTHEIGHT; y++)
	{
		for (int x = 0; x < GFX_TEXTWIDTH; x++)
			glbMapText[x + y * (GFX_TEXTWIDTH+1)] = ' '; // a' + x + y;
		glbMapText[GFX_TEXTWIDTH + y * (GFX_TEXTWIDTH+1)] = '\n';
	}
	
	layer_mark_dirty((Layer *)glbWatchLayerP);
	
	gfx_reset();
	rogue_reset();
	map_build(glb_depth);
}

void
handle_deinit()
{
	window_destroy(glbWindowP);
	glbWindowP = 0;
	text_layer_destroy(glbWatchLayerP);
	glbWatchLayerP = 0;
	// Not strictly necessary
	fonts_unload_custom_font(glbMonoFont);
}

void
handle_tick(struct tm *tick_time, TimeUnits units_changed)
{
#if SCREENSAVER_MODE
	int		dirkey;
	bool	moved;
	
	do
	{
		dirkey = rand_range(-1, 1);
		moved = rogue_tick(dirkey);
		// Make sure we at least try to move!
	} while (!moved);
	text_layer_set_text(glbWatchLayerP, glbMapText);
#else
	// Set our targets.
	if (tick_time->tm_min == glbTargetMinute)
		return;
	
	glbTargetMinute = tick_time->tm_min;
	
	if (glbMovedThisMinute)
		glbMovedThisMinute = false;
	else
		msg_clear();
	rogue_draw();
	text_layer_set_text(glbWatchLayerP, glbMapText);
#endif
}


int 
main() 
{
	handle_init();
	tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
	app_event_loop();
	handle_deinit();
}
