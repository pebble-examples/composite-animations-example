#include "main_window.h"

static Window *s_window;
static Layer *s_layer_a, *s_layer_b;

static void animate(int width) {
  const int box_size = 40;
  const int y_offset = PBL_IF_ROUND_ELSE(45, 0);

  // Move boxes
  GRect start = GRect(0, y_offset, box_size, box_size);
  GRect finish = GRect(0, y_offset + box_size, box_size, box_size);
  PropertyAnimation *prop_anim_move_1_a = 
              property_animation_create_layer_frame(s_layer_a, &start, &finish);
  Animation *anim_move_1_a = property_animation_get_animation(prop_anim_move_1_a);
  animation_set_duration(anim_move_1_a, 1000);

  start = GRect(width - box_size, y_offset + box_size, box_size, box_size);
  finish = GRect(width - box_size, y_offset, box_size, box_size);
  PropertyAnimation *prop_anim_move_1_b = 
              property_animation_create_layer_frame(s_layer_b, &start, &finish);
  Animation *anim_move_1_b = property_animation_get_animation(prop_anim_move_1_b);
  animation_set_duration(anim_move_1_b, 1000);

  // Swap sides
  start = GRect(0, y_offset + box_size, box_size, box_size);
  finish = GRect(width - box_size, y_offset + box_size, box_size, box_size);
  PropertyAnimation *prop_anim_swap_1_a = 
              property_animation_create_layer_frame(s_layer_a, &start, &finish);
  Animation *anim_swap_1_a = property_animation_get_animation(prop_anim_swap_1_a);
  animation_set_duration(anim_swap_1_a, 1000);

  start = GRect(width - box_size, y_offset, box_size, box_size);
  finish = GRect(0, y_offset, box_size, box_size);
  PropertyAnimation *prop_anim_swap_1_b = 
              property_animation_create_layer_frame(s_layer_b, &start, &finish);
  Animation *anim_swap_1_b = property_animation_get_animation(prop_anim_swap_1_b);
  animation_set_duration(anim_swap_1_b, 1000);

  // Move again
  start = GRect(width - box_size, y_offset + box_size, box_size, box_size);
  finish =  GRect(width - box_size, y_offset, box_size, box_size);
  PropertyAnimation *prop_anim_move_2_a = 
              property_animation_create_layer_frame(s_layer_a, &start, &finish);
  Animation *anim_move_2_a = property_animation_get_animation(prop_anim_move_2_a);
  animation_set_duration(anim_move_2_a, 1000);

  start = GRect(0, y_offset, box_size, box_size);
  finish = GRect(0, y_offset + box_size, box_size, box_size);
  PropertyAnimation *prop_anim_move_2_b = 
              property_animation_create_layer_frame(s_layer_b, &start, &finish);
  Animation *anim_move_2_b = property_animation_get_animation(prop_anim_move_2_b);
  animation_set_duration(anim_move_2_b, 1000);

  // Return to start
  start = GRect(width - box_size, y_offset, box_size, box_size);
  finish = GRect(0, y_offset, box_size, box_size);
  PropertyAnimation *prop_anim_swap_2_a = 
              property_animation_create_layer_frame(s_layer_a, &start, &finish);
  Animation *anim_swap_2_a = property_animation_get_animation(prop_anim_swap_2_a);
  animation_set_duration(anim_swap_2_a, 1000);

  start = GRect(0, y_offset + box_size, box_size, box_size);
  finish = GRect(width - box_size, y_offset + box_size, box_size, box_size);
  PropertyAnimation *prop_anim_swap_2_b = 
              property_animation_create_layer_frame(s_layer_b, &start, &finish);
  Animation *anim_swap_2_b = property_animation_get_animation(prop_anim_swap_2_b);
  animation_set_duration(anim_swap_2_b, 1000);

  // Create two sequence animations
  Animation *seq_a = animation_sequence_create(anim_move_1_a, anim_swap_1_a, 
                                            anim_move_2_a, anim_swap_2_a, NULL);
  Animation *seq_b = animation_sequence_create(anim_move_1_b, anim_swap_1_b, 
                                            anim_move_2_b, anim_swap_2_b, NULL);

  // Create a spawn animation using the two sequences, set play count and begin
  Animation *spawn = animation_spawn_create(seq_a, seq_b, NULL);
  animation_set_play_count(spawn, PLAY_COUNT_INFINITE);
  animation_schedule(spawn);
}

static void update_proc(Layer *layer, GContext *ctx) {
  const int corner_radius = 4;

  graphics_context_set_fill_color(ctx, GColorRed);
  graphics_fill_rect(ctx, layer_get_bounds(layer), corner_radius, GCornersAll);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create shape Layers
  s_layer_a = layer_create(GRectZero);
  layer_set_update_proc(s_layer_a, update_proc);
  layer_add_child(window_layer, s_layer_a);

  s_layer_b = layer_create(GRectZero);
  layer_set_update_proc(s_layer_b, update_proc);
  layer_add_child(window_layer, s_layer_b);

  animate(bounds.size.w);
}

static void window_unload(Window *window) {
  // Destroy shape Layers
  layer_destroy(s_layer_a);
  layer_destroy(s_layer_b);

  window_destroy(s_window);
}

void main_window_push() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}
