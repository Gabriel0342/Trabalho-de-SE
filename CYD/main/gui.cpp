#include "gui.h"
#include <cstdio>
#include <string.h>

static lv_obj_t *weight_label = NULL;
lv_obj_t *priceKg_label = NULL;
lv_obj_t *fruit_label = NULL;
lv_obj_t *btn1 = NULL;

static char buf[16];
static bool em_print_screen = false;

float precoKG;
float peso_final;

static void event_handler_btn1(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    float preco_pagar = precoKG * peso_final;
    em_print_screen = true;
    weight_label = NULL;
    gui_updateFruta("", 0);
    lv_obj_clean(lv_screen_active());
    lv_create_print_screen(preco_pagar);
  }
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

  fruit_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(fruit_label, &style_text, 0);
  lv_label_set_text(fruit_label, "Fruit:\n---");
  lv_obj_set_style_text_align(fruit_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(fruit_label, LV_ALIGN_CENTER, -80, 25);

  priceKg_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(priceKg_label, &style_text, 0);
  lv_label_set_text(priceKg_label, "Price:\n-.--$/kg");
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

  btn1 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(btn1, event_handler_btn1, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 75);
  lv_obj_set_size(btn1, 260, 35);

  lv_obj_t *btn_label = lv_label_create(btn1);
  lv_label_set_text(btn_label, "Print Price");
  lv_obj_center(btn_label);
}


void lv_create_print_screen(float preco_pagar) {
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
  lv_label_set_text(text_label, "Printing sticker...");
  lv_obj_set_width(text_label, 320);
  lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);

  lv_obj_t *price_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(price_label, &style_weight, 0);
  lv_label_set_text_fmt(price_label, "%.2f$", preco_pagar);
  lv_obj_align(price_label, LV_ALIGN_CENTER, 0, -40);

  lv_obj_t *warning_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(warning_label, &style_text, 0);
  lv_label_set_text(warning_label, "Please take the fruit of the balance...");
  lv_obj_set_width(warning_label, 320);
  lv_obj_set_style_text_align(warning_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(warning_label, LV_ALIGN_CENTER, 0, 70);
}

void gui_updatePeso(float peso) {
  peso_final = peso;

  if (em_print_screen) {
    if (peso_final > -0.02f && peso_final < 0.02f) {
      em_print_screen = false;
      lv_obj_clean(lv_screen_active());
      gui_updateFruta("", 0);
      gui_init();
    }
    return;
  }

  if (!weight_label) return;

  if (peso > -0.02 && peso < 0.02) {
    strcpy(buf, "-.-- kg");
    gui_updateFruta("", 0);
  } else {
    sprintf(buf, "%.2f kg", peso);
  }

  lv_label_set_text(weight_label, buf);
}

void gui_updateFruta(const char* fruta, float preco) {
  if (!fruit_label || !priceKg_label) return;

  bool tem_fruta = (fruta != NULL && fruta[0] != '\0' && preco != 0.0);

  if (btn1) {
    if (tem_fruta)
      lv_obj_clear_state(btn1, LV_STATE_DISABLED);
    else
      lv_obj_add_state(btn1, LV_STATE_DISABLED);
  }

  char buf_fruta[50];
  if (fruta == NULL || fruta[0] == '\0') {
    strcpy(buf_fruta, "Fruit:\n---");
  } else {
    sprintf(buf_fruta, "Fruit:\n%s", fruta);
  }
  lv_label_set_text(fruit_label, buf_fruta);
  
  char buf_preco[16];
  precoKG = preco; 
  if (preco == 0) {
    strcpy(buf_preco, "Price:\n-.--$/kg");
  } else {
    snprintf(buf_preco, sizeof(buf_preco), "Price:\n%.2f $/kg", preco);  
  }
  lv_label_set_text(priceKg_label, buf_preco);
}