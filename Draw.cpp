#include "Draw.h"

const uint32_t WHITE = INT32_MAX;
const uint32_t PINK = 15811439;
const uint32_t RED = 16711680;
const uint32_t ORANGE = 16354335;
const uint32_t GREEN = 65280;
const uint32_t GREY = 10987431;
const uint32_t DARK_GREY = 5723991;
const uint32_t DARK_BLUE = 5595;

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) 
{
    const int deltaX = abs(x1 - x0);
    const int deltaY = abs(y1 - y0);
    const int signX = x0 < x1 ? 1 : -1;
    const int signY = y0 < y1 ? 1 : -1;
    int error = deltaX - deltaY;
    if(y1 >= 30)
        buffer[y1][x1] = color;
    while (x0 != x1 || y0 != y1)
    {
        if(y0 >= 30)
            buffer[y0][x0] = color;
        int error2 = error * 2;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x0 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y0 += signY;
        }
    }

}


void affine_rotation(int &x, int &y,int center_x,int center_y, int angle)
{
    angle -= 90;
    double r = sqrt(pow((x - center_x), 2) + pow((y - center_y), 2));
    double phi;
    if (y < center_y)
    {
        phi = acos(double(center_x - x) / r);
    }
    else
    {
        phi = -acos(double(center_x - x) / r);
    }
    double tmpy = -r * cos(phi + angle  * 3.14 / 180);
    double tmpx = r * sin(phi + angle * 3.14 / 180);
    x = center_x + tmpx;
    y = center_y + tmpy;

}



void draw_circle1(int x0, int y0, int r, double start, double end)
{
    for (double i = start; i < end; i += 0.05)
    {
        if (int(y0 + sin(i) * r) >= 30)
            buffer[int(y0 + sin(i) * r)][int(x0 + cos(i) * r)] = INT32_MAX;
    }
}

void draw_lander(int x, int y, int angle, int proportion)
{
    int point1_x = x - 5;
    int point1_y = y;
    int point2_x = x - 7;
    int point2_y = y - 5;
    int point3_x = x - 10;
    int point3_y = y + 2;
    int point4_x = x + 5;
    int point4_y = y;
    int point5_x = x + 7;
    int point5_y = y - 5;
    int point6_x = x + 10;
    int point6_y = y + 2;
    int r = 5;
    if (angle != 0)
    {
        affine_rotation(point1_x, point1_y, x, y, angle);
        affine_rotation(point2_x, point2_y, x, y, angle);
        affine_rotation(point3_x, point3_y, x, y, angle);
        affine_rotation(point4_x, point4_y, x, y, angle);
        affine_rotation(point5_x, point5_y, x, y, angle);
        affine_rotation(point6_x, point6_y, x, y, angle);
    }
    lnd.set_borders(point3_x, point3_y, point6_x, point6_y);
    double phi1;
    if (lnd.y_b_left < y)
    {
        phi1 = acos(double(x - lnd.x_b_left) / sqrt(pow(x - point3_x, 2) + pow(y - point3_y, 2)));
    }
    else
    {
        phi1 = -acos(double(x - lnd.x_b_left) / sqrt(pow(x - point3_x, 2) + pow(y - point3_y, 2)));
    }
    double phi2;
    if (lnd.y_b_right < y)
    {
        phi2 = acos(double(x - lnd.x_b_right) / sqrt(pow(x - point6_x, 2) + pow(y - point6_y, 2)));
    }
    else
    {
        phi2 = -acos(double(x - lnd.x_b_right) / sqrt(pow(x - point6_x, 2) + pow(y - point6_y, 2)));
    }
    if (phi2 > 1.57)
        phi2 -= 6.28;
    if (phi1 > 1.57)
        phi1 -= 6.28;
    if (phi2 < phi1)
        std::swap(phi1, phi2);
    draw_circle1(x, y, r, phi1, phi2);
    draw_line(point1_x, point1_y, point4_x, point4_y, WHITE);
    draw_line(point1_x, point1_y, point2_x, point2_y, WHITE);
    draw_line(point2_x, point2_y, point3_x, point3_y, WHITE);
    draw_line(point4_x, point4_y, point5_x, point5_y, WHITE);
    draw_line(point5_x, point5_y, point6_x, point6_y, WHITE);
    if (proportion > 0)
    {
        int point1_x = x - 3;
        int point1_y = y;
        int point2_x = x + 3;
        int point2_y = y;
        int point3_x = x;
        int point3_y = y + 0.25 * proportion;
        if (angle != 0)
        {
            affine_rotation(point1_x, point1_y, x, y, angle);
            affine_rotation(point2_x, point2_y, x, y, angle);
            affine_rotation(point3_x, point3_y, x, y, angle);
        }
        draw_line(point1_x, point1_y, point3_x, point3_y, ORANGE);
        draw_line(point2_x, point2_y, point3_x, point3_y, ORANGE);
    }
}


void draw_terrain()
{
    for (int i = 0; i < SCREEN_WIDTH - 1; i++)
    {
        Platform pl;
        if (check_if_over_platform(i, pl))
        {
            draw_line(i, SCREEN_HEIGHT - terrain[i], i + 1, SCREEN_HEIGHT - terrain[i + 1], GREEN);
            draw_line(i, SCREEN_HEIGHT - terrain[i] - 1, i + 1, SCREEN_HEIGHT - terrain[i + 1] - 1, GREEN);
            draw_line(i, SCREEN_HEIGHT - terrain[i] + 1, i + 1, SCREEN_HEIGHT - terrain[i + 1] + 1, GREEN);
        }
        else
            draw_line(i, SCREEN_HEIGHT - terrain[i], i + 1, SCREEN_HEIGHT - terrain[i + 1], WHITE);

    }
        
}



void draw_bitmap(uint32_t matrix[5][5], int x, int y, int color)
{
    int k = 2;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matrix[i][j] == 1)
            {
                buffer[y + k*i][x + k*j] = color;
                buffer[y + k*i+1][x + k*j+1] = color;
                buffer[y + k*i+1][x + k*j] = color;
                buffer[y + k*i][x + k*j+1] = color;
            }
}



void draw_char(char ch, int x, int y, int color)
{
    switch (ch)
    {
    case 'a':
        draw_bitmap(a, x, y, color);
        break;
    case 'b':
        draw_bitmap(b, x, y, color);
        break;
    case 'c':
        draw_bitmap(c, x, y, color);
        break;
    case 'd':
        draw_bitmap(d, x, y, color);
        break;
    case 'e':
        draw_bitmap(e, x, y, color);
        break;
    case 'f':
        draw_bitmap(f, x, y, color);
        break;
    case 'g':
        draw_bitmap(gl, x, y, color);
        break;
    case 'h':
        draw_bitmap(h, x, y, color);
        break;
    case 'i':
        draw_bitmap(i, x, y, color);
        break;
    case 'j':
        draw_bitmap(j, x, y, color);
        break;
    case 'k':
        draw_bitmap(k, x, y, color);
        break;
    case 'l':
        draw_bitmap(l, x, y, color);
        break;
    case 'm':
        draw_bitmap(m, x, y, color);
        break;
    case 'n':
        draw_bitmap(n, x, y, color);
        break;
    case 'o':
        draw_bitmap(o, x, y, color);
        break;
    case 'p':
        draw_bitmap(p, x, y, color);
        break;
    case 'q':
        draw_bitmap(q, x, y, color);
        break;
    case 'r':
        draw_bitmap(r, x, y, color);
        break;
    case 's':
        draw_bitmap(s, x, y, color);
        break;
    case 't':
        draw_bitmap(t, x, y, color);
        break;
    case 'u':
        draw_bitmap(u, x, y, color);
        break;
    case 'v':
        draw_bitmap(v, x, y, color);
        break;
    case 'w':
        draw_bitmap(w, x, y, color);
        break;
    case 'x':
        draw_bitmap(xl, x, y, color);
        break;
    case 'y':
        draw_bitmap(yl, x, y, color);
        break;
    case 'z':
        draw_bitmap(z, x, y, color);
        break;
    case '0':
        draw_bitmap(num0, x, y, color);
        break;
    case '1':
        draw_bitmap(num1, x, y, color);
        break;
    case '2':
        draw_bitmap(num2, x, y, color);
        break;
    case '3':
        draw_bitmap(num3, x, y, color);
        break;
    case '4':
        draw_bitmap(num4, x, y, color);
        break;
    case '5':
        draw_bitmap(num5, x, y, color);
        break;
    case '6':
        draw_bitmap(num6, x, y, color);
        break;
    case '7':
        draw_bitmap(num7, x, y, color);
        break;
    case '8':
        draw_bitmap(num8, x, y, color);
        break;
    case '9':
        draw_bitmap(num9, x, y, color);
        break;
    case ':':
        draw_bitmap(double_dot, x, y, color);
        break;
    case '.':
        draw_bitmap(dot, x, y, color);
        break;
    case '-':
        draw_bitmap(dash, x, y, color);
        break;
    case '>':
        draw_bitmap(arrow_right, x, y, color);
        break;
    case '<':
        draw_bitmap(arrow_left, x, y, color);
        break;
    default:
        break;
    }
}

void draw_text(std::string text, int x, int y, int color)
{
    for (auto ch : text)
    {
        draw_char(ch, x, y, color);
        x += 11;
    }
}

