#include <pebble.h>

#define KEY_DATA 5
static Layer *s_path_layer;
static Window *main_window;
static TextLayer *distance;
static TextLayer *timeTo;
static GBitmap *s_background_bitmap;
static BitmapLayer *arrow;
static TextLayer *output;
static TextLayer *s_output_layer;
static double bering = 0;
static BitmapLayer *s_bitmap_layer;

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

// Vector paths for the compass needles
static const GPathInfo NEEDLE_NORTH_POINTS = { 
  3,
  (GPoint[]) { { -8, 0 }, { 8, 0 }, { 0, -36 } }
};
static GPath *s_needle_north;
#define KEY_BERING     1


static void compass_handler(CompassHeadingData data) 
{
}

static void path_layer_update_callback(Layer *path, GContext *ctx) 
{
  gpath_draw_filled(ctx, s_needle_north);gpath_draw_filled(ctx, s_needle_north);       
              
  // creating centerpoint                 
  GRect bounds = layer_get_frame(path);          
  GPoint path_center = GPoint(bounds.size.w / 2, bounds.size.h / 2);  
  graphics_fill_circle(ctx, path_center, 3);       

  // then put a white circle on top               
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, path_center, 2);
	gpath_rotate_to(s_needle_north, bering);
	layer_mark_dirty(s_path_layer);
}

static void main_window_load(Window *window)
{	
	Layer *window_layer = window_get_root_layer(window);
	//arrow
	/*directionArrow = gbitmap_create_with_resource(RESOURCE_ID_COMPASS_BACKGROUND;
	arrow = bitmap_layer_create(GRect(0,0,144,138));
	bitmap_layer_set_bitmap(arrow,directionArrow);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(arrow));
	*/
	GRect bounds = layer_get_frame(window_layer);
	s_bitmap_layer = bitmap_layer_create(bounds);
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_COMPASS_BACKGROUND);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_background_bitmap);
	bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpAnd);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bitmap_layer));
	// Create the layer in which we will draw the compass needles
  s_path_layer = layer_create(bounds);
  
  //  Define the draw callback to use for this layer
  layer_set_update_proc(s_path_layer, path_layer_update_callback);
  layer_add_child(window_layer, s_path_layer);
	s_needle_north = gpath_create(&NEEDLE_NORTH_POINTS);
	GPoint center = GPoint(bounds.size.w / 2, bounds.size.h / 2);
  gpath_move_to(s_needle_north, center);
	
  
	
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



/*
static void send(int key, int msg) 
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &msg, sizeof(int), true);
  app_message_outbox_send();
}
*/

static void inbox_received_callback(DictionaryIterator *iter, void *context) 
{
	Tuple *t = dict_read_first(iter);
	while(t != NULL) 
	{
		if(t->key == KEY_BERING)
		{
			int x = (t->value->int32);
			bering = x/1000;
		}
		t = dict_read_next(iter);
	}	
}

static void main_window_unload(Window *window)
{
	bitmap_layer_destroy(arrow);
	text_layer_destroy(distance);
	text_layer_destroy(timeTo);
	text_layer_destroy(output);
	text_layer_destroy(s_output_layer);
	layer_destroy(s_path_layer);
	gpath_destroy(s_needle_north);
}

static void outbox_sent_handler(DictionaryIterator *iter, void *context) {
  // Ready for next command
  text_layer_set_text (output, "Press up or down.");
}

static void outbox_failed_handler(DictionaryIterator *iter, AppMessageResult reason, void *context) {
  text_layer_set_text(output, "Send failed!");
  APP_LOG(APP_LOG_LEVEL_ERROR, "Fail reason: %d", (int)reason);
}

static void init ()
{
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	main_window = window_create();
	window_set_window_handlers(main_window, (WindowHandlers)
	{
		.load = main_window_load,
		.unload = main_window_unload
	});
		
	app_message_register_outbox_sent(outbox_sent_handler); 
	app_message_register_outbox_failed(outbox_failed_handler); 
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	compass_service_set_heading_filter(10);
	compass_service_subscribe(&compass_handler); 
	window_stack_push(main_window, true);

}

static void deinit()
{
	compass_service_unsubscribe();
	window_destroy(main_window);
}

int main()
{
	init();
	app_event_loop();
	deinit();
}