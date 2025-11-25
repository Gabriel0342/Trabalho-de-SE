//Parte interface de balança
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// Pinos do Ecra Touch
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

//Medidas do Ecra
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

int x, y, z;  // Cordenadas do Ecra: (x, y) e a Pressão (z)

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// Lê os dados do touchscreen
void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data) {
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    data->state = LV_INDEV_STATE_PRESSED;

    data->point.x = x;
    data->point.y = y;

  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
int peso = 0;

int btn1_count = 0;
static void event_handler_btn1(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    lv_create_print_screen();
  }
}



void lv_create_main_gui(void) {

  static lv_style_t style_weight;
  static lv_style_t style_text;

  lv_style_init(&style_weight);
  lv_style_set_text_font(&style_weight, &lv_font_montserrat_40);
  lv_style_set_text_color(&style_weight, lv_color_black());

  lv_style_init(&style_text);
  lv_style_set_text_font(&style_text, &lv_font_montserrat_20);
  lv_style_set_text_color(&style_text, lv_palette_darken(LV_PALETTE_GREY, 2));


  if (!peso) {
    lv_obj_t* text_label = lv_label_create(lv_screen_active());
    lv_obj_add_style(text_label, &style_text, 0);
    lv_label_set_text(text_label, "Put the fruit on the scale!");
    lv_obj_set_width(text_label, 320);
    lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(text_label, LV_ALIGN_CENTER, 0, -90);
  }

  lv_obj_t* weight_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(weight_label, &style_weight, 0);
  lv_label_set_text(weight_label, "-.-- kg");
  lv_obj_align(weight_label, LV_ALIGN_CENTER, 0, -40);

  lv_obj_t* fruit_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(fruit_label, &style_text, 0);
  lv_label_set_text(fruit_label, "Fruit:\nbanana");
  lv_obj_set_style_text_align(fruit_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(fruit_label, LV_ALIGN_CENTER, -80, 25);

  lv_obj_t* priceKg_label = lv_label_create(lv_screen_active());
  lv_obj_add_style(priceKg_label, &style_text, 0);
  lv_label_set_text(priceKg_label, "Price:\n2$/kg");
  lv_obj_set_style_text_align(priceKg_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(priceKg_label, LV_ALIGN_CENTER, 80, 25);

  lv_obj_t* linha_hor = lv_obj_create(lv_screen_active());
  lv_obj_set_size(linha_hor, 260, 2);
  lv_obj_set_style_bg_color(linha_hor, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(linha_hor, LV_OPA_COVER, 0);
  lv_obj_align(linha_hor, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t* linha_ver = lv_obj_create(lv_screen_active());
  lv_obj_set_size(linha_ver, 2, 35);
  lv_obj_set_style_bg_color(linha_ver, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(linha_ver, LV_OPA_COVER, 0);
  lv_obj_align(linha_ver, LV_ALIGN_CENTER, 0, 20);

  lv_obj_t* btn1 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(btn1, event_handler_btn1, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 75);
  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);
  lv_obj_set_size(btn1, 260, 35);

  lv_obj_t* btn_label = lv_label_create(btn1);
  lv_label_set_text(btn_label, "Print Price");
  lv_obj_center(btn_label);
}

void lv_create_print_screen(void) {

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
}

void setup() {
  String LVGL_Arduino = String("LVGL Library Version: ") + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);

  lv_init();  // LVGL

  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(2);

  //Inicia variaveis para o display TFT
  lv_display_t* disp;
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

  //Inicia variaveis para TouchScreen
  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchscreen_read);

  //Desenha a interface
  lv_create_main_gui();
}

void loop() {
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}