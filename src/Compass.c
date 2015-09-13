#include <pebble.h>

static Layer *layer;
static Window *main_window;
static TextLayer *distance;
static TextLayer *timeTo;
static GBitmap *directionArrow;
static BitmapLayer *arrow;

static void main_window_load(Window *window)
{	
	//arrow
	directionArrow = gbitmap_create_with_resource(RESOURCE_ID_direction_arrow);
	arrow = bitmap_layer_create(GRect(0,0,144,138));
	bitmap_layer_set_bitmap(arrow,directionArrow);

	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(arrow));
		
	//distance
	distance = text_layer_create(GRect(0, 0, 144, 30));
	text_layer_set_text(distance, "Distance is x m");
	text_layer_set_text_alignment(distance, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(distance));
	
	//est time
	timeTo = text_layer_create(GRect(0, 144, 144, 30));
	text_layer_set_text(timeTo, "It will take x min");
	text_layer_set_text_alignment(timeTo, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(timeTo));
}

static void main_window_unload(Window *window)
{
	bitmap_layer_destroy(arrow);
	text_layer_destroy(distance);
	text_layer_destroy(timeTo);
}

static void init ()
{
	main_window = window_create();
	
	window_set_window_handlers(main_window, (WindowHandlers)
	{
		.load = main_window_load,
		.unload = main_window_unload
	});
	window_stack_push(main_window, true);
}

static void deinit()
{
	window_destroy(main_window);
}

int main()
{
	init();
	app_event_loop();
	deinit();
}