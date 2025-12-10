#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#include "balanca.h"
#include "gui.h"

//------------------------------------

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

//------------------------------------

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

//------------------------------------

#define HX711_DOUT 22
#define HX711_SCK 27

float escala = -234162;
Balanca balanca(HX711_DOUT, HX711_SCK, escala);

//------------------------------------

TFT_eSPI tft = TFT_eSPI();

int x, y, z;

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

//------------------------------------

void setup() {
    Serial.begin(115200);
    delay(500); 

    //----------- BALANCA ------------
    
    balanca.init();

    //------------- LVGL -------------

    lv_init();

    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(2);

    lv_display_t* disp;
    disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touchscreen_read);

    gui_init();
}

//------------------------------------

void loop() {
    static uint32_t last_tick = 0;
    uint32_t now = millis();

    lv_tick_inc(now - last_tick);
    last_tick = now;
    lv_task_handler();    

    static unsigned long last_read = 0;
    if (millis() - last_read > 80) {
        last_read = millis();

        float peso = balanca.getPeso(1);
        Serial.print("Peso: ");
        Serial.print(peso, 2);
        Serial.println(" kg");
        gui_updatePeso(peso);
    }

    delay(5);
}