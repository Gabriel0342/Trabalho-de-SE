#include "gui.h"
#include <cstdio>
#include <string.h>

static lv_obj_t* weight_label = NULL;
static char buf[16];

static void event_handler_btn1(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    weight_label = NULL;
    lv_obj_clean(lv_screen_active());
    lv_create_print_screen();
  }
}

static void voltar_para_menu(lv_timer_t* timer) {
  lv_obj_clean(lv_screen_active());
  gui_init();
  lv_timer_del(timer);
}

void gui_init() {
    static lv_style_t style_weight;
    static lv_style_t style_text;

    lv_style_init(&style_weight);
    lv_style_set_text_font(&style_weight, &lv_font_montserrat_40);
    lv_style_set_text_color(&style_weight, lv_color_black());

    lv_style_init(&style_text);
    lv_style_set_text_font(&style_text, &lv_font_montserrat_20);
    lv_style_set_text_color(&style_text, lv_palette_darken(LV_PALETTE_GREY, 2));

    lv_obj_t *text_label = lv_label_create(lv_screen_active());
    lv_obj_add_style(text_label, &style_text, 0);
    lv_label_set_text(text_label, "Put the fruit on the scale!");
    lv_obj_set_width(text_label, 320);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);

    weight_label = lv_label_create(lv_screen_active());
    lv_obj_add_style(weight_label, &style_weight, 0);
    lv_label_set_text(weight_label, "-.-- kg");
    lv_obj_align(weight_label, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t *fruit_label = lv_label_create(lv_screen_active());
    lv_obj_add_style(fruit_label, &style_text, 0);
    lv_label_set_text(fruit_label, "Fruit:\nbanana");
    lv_obj_set_style_text_align(fruit_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(fruit_label, LV_ALIGN_CENTER, -80, 25);

    lv_obj_t *priceKg_label = lv_label_create(lv_screen_active());
    lv_obj_add_style(priceKg_label, &style_text, 0);
    lv_label_set_text(priceKg_label, "Price:\n2$/kg");
    lv_obj_set_style_text_align(priceKg_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(priceKg_label, LV_ALIGN_CENTER, 80, 25);

    lv_obj_t *linha_hor = lv_obj_create(lv_screen_active());
    lv_obj_set_size(linha_hor, 260, 2);
    lv_obj_set_style_bg_color(linha_hor, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(linha_hor, LV_OPA_COVER, 0);
    lv_obj_align(linha_hor, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *linha_ver = lv_obj_create(lv_screen_active());
    lv_obj_set_size(linha_ver, 2, 35);
    lv_obj_set_style_bg_color(linha_ver, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(linha_ver, LV_OPA_COVER, 0);
    lv_obj_align(linha_ver, LV_ALIGN_CENTER, 0, 20);

    lv_obj_t *btn1 = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(btn1, event_handler_btn1, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 75);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_set_size(btn1, 260, 35);

    lv_obj_t *btn_label = lv_label_create(btn1);
    lv_label_set_text(btn_label, "Print Price");
    lv_obj_center(btn_label);
}


void lv_create_print_screen() {
  static lv_style_t style_weight;
  static lv_style_t style_text;

  lv_style_init(&style_weight);
  lv_style_set_text_font(&style_weight, &lv_font_montserrat_40);
  lv_style_set_text_color(&style_weight, lv_color_black());

  lv_style_init(&style_text);
  lv_style_set_text_font(&style_text, &lv_font_montserrat_20);
  lv_style_set_text_color(&style_text, lv_palette_darken(LV_PALETTE_GREY, 2));

  lv_obj_t* text_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(text_label, &style_text, 0);
  lv_label_set_text(text_label, "Printing sticker...");
  lv_obj_set_width(text_label, 320);
  lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);

  lv_timer_create(voltar_para_menu, 3000, NULL);
}

void gui_updatePeso(float peso) {
    if (!weight_label) return;

    if (peso > -0.02 && peso < 0.02) {
        strcpy(buf, "-.-- kg");
    } else {
        sprintf(buf, "%.2f kg", peso);
    }

    lv_label_set_text(weight_label, buf);
}