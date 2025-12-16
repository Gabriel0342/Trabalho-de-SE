#ifndef GUI_H
#define GUI_H

#include <lvgl.h>

void gui_init();
void lv_create_print_screen(float preco_pagar);
void gui_updatePeso(float peso);
void gui_updateFruta(const char* fruta, float preco);

#endif