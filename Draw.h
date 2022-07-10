#pragma once
#include "Engine.h"
#include "Bitmapalphabet.h"
#include "Lander.h"
#include "Terrain.h"
#include <math.h>
#include <map>
#include <string>

void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

void affine_rotation(int &x, int &y, int center_x, int center_y, int angle);

void draw_circle1(int x0, int y0, int r, double start, double end);

void draw_lander(int x, int y, int angle, int proportion);

void draw_terrain();

void draw_char(char c, int x, int y, int color);

void draw_bitmap(uint32_t matrix[5][5], int x, int y, int color);

void draw_text(std::string text, int x, int y, int color);


extern const uint32_t WHITE;
extern const uint32_t PINK;
extern const uint32_t RED;
extern const uint32_t ORANGE;
extern const uint32_t GREEN;
extern const uint32_t GREY;
extern const uint32_t DARK_GREY;
extern const uint32_t DARK_BLUE;


