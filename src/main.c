#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "config.h"


#if SCREENSAVER_MODE
#define MY_UUID { 0x8E, 0xFD, 0x77, 0xD9, 0x8C, 0xCB, 0x4B, 0x16, 0xA4, 0xAB, 0x6D, 0x84, 0xA2, 0xAD, 0xEA,  0xD0 }
#else
#define MY_UUID { 0x8E, 0xFD, 0x77, 0xD9, 0x8C, 0xCB, 0x4B, 0x16, 0xA4, 0xAB, 0x6D, 0x84, 0xA2, 0xAD, 0xEA,  0xCF }
#endif
PBL_APP_INFO(MY_UUID,
             "RogueWatch", "Jeff Lait",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
#if SCREENSAVER_MODE
             APP_INFO_WATCH_FACE);
#else
             APP_INFO_STANDARD_APP);
#endif

#include "mytypes.h"
#include "rand.h"
	
#include "gfxengine.h"
#include "msg.h"
#include "map.h"
#include "mob.h"
#include "rogue.h"

Window glbWindow;
TextLayer glbWatchLayer;

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
click_handler(ClickRecognizerRef clickref, Window *window)
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
	text_layer_set_text(&glbWatchLayer, glbMapText);
}

void
config_provider(ClickConfig **config, Window *window)
{
	config[BUTTON_ID_UP]->click.handler = (ClickHandler) click_handler;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = 250;
	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) click_handler;
	config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 250;
	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) click_handler;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 250;
}
	
void handle_init(AppContextRef ctx) 
{
	(void)ctx;
	
	window_init(&glbWindow, "Main");
	window_set_fullscreen(&glbWindow, true);
	
#if !SCREENSAVER_MODE
	window_set_click_config_provider(&glbWindow, (ClickConfigProvider) config_provider);
#endif
	
	window_stack_push(&glbWindow, true /* Animated */);
#if INVERT_COLOURS
	window_set_background_color(&glbWindow, GColorBlack);
#else
	window_set_background_color(&glbWindow, GColorWhite);
#endif
	resource_init_current_app(&APP_RESOURCES);
	
	glbMonoFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DROIDMONO_12));

	text_layer_init(&glbWatchLayer, glbWindow.layer.frame);
#if INVERT_COLOURS
	text_layer_set_background_color(&glbWatchLayer, GColorBlack);
	text_layer_set_text_color(&glbWatchLayer, GColorWhite);
#else
	text_layer_set_background_color(&glbWatchLayer, GColorWhite);
	text_layer_set_text_color(&glbWatchLayer, GColorBlack);
#endif
	// I'd wrather have this clip on overflow.
	text_layer_set_overflow_mode(&glbWatchLayer, GTextOverflowModeTrailingEllipsis);
	text_layer_set_text(&glbWatchLayer, glbMapText);
	text_layer_set_font(&glbWatchLayer, glbMonoFont);
	layer_add_child(&glbWindow.layer, (Layer *)&glbWatchLayer);
	
	rand_seed();
	
	for (int y = 0; y < GFX_TEXTHEIGHT; y++)
	{
		for (int x = 0; x < GFX_TEXTWIDTH; x++)
			glbMapText[x + y * (GFX_TEXTWIDTH+1)] = ' '; // a' + x + y;
		glbMapText[GFX_TEXTWIDTH + y * (GFX_TEXTWIDTH+1)] = '\n';
	}
	
	app_timer_send_event(ctx, REFRESH_RATE, INTEGRATE_TIMER_ID);
	
	layer_mark_dirty((Layer *)&glbWatchLayer);
	
	gfx_reset();
	rogue_reset();
	map_build(glb_depth);
}

void
handle_deinit(AppContextRef ctx)
{
	// Not strictly necessary
	fonts_unload_custom_font(glbMonoFont);
}

void
handle_tick(AppContextRef ctx, PebbleTickEvent *event)
{
#if SCREENSAVER_MODE
	int		dirkey;
	
	do
	{
		dirkey = rand_range(-1, 1);
		rogue_tick(dirkey);
		// Make sure we at least try to move!
	} while (dirkey != 0);
	text_layer_set_text(&glbWatchLayer, glbMapText);
#else
	// Set our targets.
	PblTm	time;
	
	get_time(&time);
	
	if (time.tm_min == glbTargetMinute)
		return;
	
	glbTargetMinute = time.tm_min;
	
	if (glbMovedThisMinute)
		glbMovedThisMinute = false;
	else
		msg_clear();
	rogue_draw();
	text_layer_set_text(&glbWatchLayer, glbMapText);
#endif
}

void
handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie)
{
	if (cookie == INTEGRATE_TIMER_ID)
	{
	}
}


void pbl_main(void *params) 
{
	PebbleAppHandlers handlers = 
	{
		.init_handler = &handle_init,
		.deinit_handler = &handle_deinit,
		.tick_info =
		{
			.tick_handler = &handle_tick,
			.tick_units = SECOND_UNIT
		},
		.timer_handler = &handle_timer
	};
	
	app_event_loop(params, &handlers);
}
