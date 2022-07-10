#include "Terrain.h"

void Platform::get_location(int& x, int& y)
{
    x = loc_x;
    y = loc_y;
}

int Platform::get_width()
{
    return width;
}


int Platform::get_score()
{
    return points;
}

int Platform::get_difficulty()
{
    return difficulty;
}

int Platform::get_y()
{
    return loc_y;
}

int Platform::get_x()
{
    return loc_x;
}


void Platform::get_borders(int& left, int& right)
{
    left = loc_x - width / 2;
    right = loc_x + width / 2;
}

std::vector<Platform> platforms;

bool check_platform_intersection(int x, int w)
{
    for (auto platform : platforms)
    {
        int left, right;
        platform.get_borders(left, right);
        if ((x - w / 2 <= right && x - w / 2 >= left) || ((x + w / 2 <= right && x + w / 2 >= left)) || (x <= right && x >= left))
        {
            return true;
        }
    }
    return false;
}

void generate_platfroms()
{
    platforms.clear(); 
    int num_platforms = rand() % 3 + 2;
    for (int i = 0; i < num_platforms; i++)
    {
        int diff = rand() % 3 + 1;
        int w = 90 / diff;
        int x = rand() % SCREEN_WIDTH + 1;
        if (x + w / 2 > SCREEN_WIDTH)
            x = x - w / 2;
        if (x - w / 2 < 0)
            x = x + w / 2;
        while (check_platform_intersection(x, w))
        {
            x = rand() % SCREEN_WIDTH + 1;
            if (x + w / 2 > SCREEN_WIDTH)
                x = x - w / 2;
            if (x - w / 2 < 0)
                x = x + w / 2;
        }
        int y = rand() % (SCREEN_HEIGHT / 2) + 10;
        platforms.push_back(Platform(x, y, w, diff));
    }

}

void set_platforms()
{
    for (auto& platform : platforms)
    {
        int x, w;
        x = platform.get_x();
        w = platform.get_width();
        int pl_s = x - w / 2;
        int pl_end = x + w / 2;
        for (int i = pl_s - 3; i <= pl_end + 3; i++)
            terrain[i] = platform.get_y();
    }
}


int terrain[SCREEN_WIDTH] = {};

void generate_total_terrain(int seed)
{
    srand(seed);
    memset(terrain, 0, SCREEN_WIDTH * sizeof(int));
    generate_platfroms();
    set_platforms();
    int s = 0;
    int end = 1;
    while (end < SCREEN_WIDTH)
    {
        if (terrain[end] != 0 && terrain[end - 1] == 0)
        {
            generate_terrain(s, end);
            s = end;
        }
        else
        {
            end++;
            if (terrain[s] != 0)
                s++;
        }
    }
    generate_terrain(s, end);
}

void generate_terrain(int s, int end)
{
    int max_height = SCREEN_HEIGHT / 2;
    int step_height = 10;
    terrain[s] =  rand() % 200 + 1;
    for (int i = s; i < end; i++)
    {
        if (terrain[i] == 0)
        {
            terrain[i] =  terrain[i - 1] + rand() % (2 * step_height) + 1 - step_height;
            if (terrain[i] > max_height)
                terrain[i] = max_height;
            else if (terrain[i] < 0)
                terrain[i] =  2;
        }
    }
    int smooth = 10;
    if (end == SCREEN_WIDTH)
        end = end - 2;
    else
        end = end + 3;
    if (s != 0)
        s = s - 3;
    for (int start = 1; start <= smooth; start++)
    {

        for (int i = s + start; i < end; i += 2)
        {

            terrain[i] = (terrain[i - 1] + terrain[i + 1]) / 2;
        }
    }
}

const double EPS = 1E-9;

inline int det(int a, int b, int c, int d) {
    return a * d - b * c;
}

inline bool between(int a, int b, double c) {
    return std::min(a, b) <= c + EPS && c <= std::max(a, b) + EPS;
}

inline bool intersect_1(int a, int b, int c, int d) {
    if (a > b)  std::swap(a, b);
    if (c > d)  std::swap(c, d);
    return std::max(a, c) <= std::min(b, d);
}

bool intersect(int x1_b, int y1_b, int x1_e, int y1_e, int x2_b, int y2_b, int x2_e, int y2_e)
{
    int A1 = y1_b - y1_e, B1 = x1_e - x1_b, C1 = -A1 * x1_b - B1 * y1_b;
    int A2 = y2_b - y2_e, B2 = x2_e - x2_b, C2 = -A2 * x2_b - B2 * y2_b;
    int zn = det(A1, B1, A2, B2);
    if (zn != 0) {
        double x = -det(C1, B1, C2, B2) * 1. / zn;
        double y = -det(A1, C1, A2, C2) * 1. / zn;
        return between(x1_b, x1_e, x) && between(y1_b, y1_e, y) && between(x2_b, x2_e, x) && between(y2_b, y2_e, y);
    }
    else
        return det(A1, C1, A2, C2) == 0 && det(B1, C1, B2, C2) == 0 && intersect_1(x1_b, x1_e, x2_b, x2_e) && intersect_1(y1_b, y1_e, y2_b, y2_e);
}

bool check_crash(int begin, int end)
{
    int fl = 0;
    if (end >= 1024)
    {
        begin -= 1024;
        end -= 1024;
    }
    int xb, xe, x1, x2;
    for (int i = begin; i < end; i++)
    {
        if (i < 0)
            xb = 1024 + i;
        else
            xb = i;
        if (xb + 1 == 1024)
            xe = 0;
        else
            xe = xb + 1;
        if (lnd.x_b_right >= 1023)
        {
            x1 = lnd.x_b_left - 1024;
            x2 = lnd.x_b_right - 1024;
        }
        else
        {
            x1 = lnd.x_b_left;
            x2 = lnd.x_b_right;
        }
        if (intersect(i, SCREEN_HEIGHT - terrain[xb], i + 1, SCREEN_HEIGHT - terrain[xe], x1, lnd.y_b_left, x2, lnd.y_b_right))
        {
            fl = 1;
            break;
        }
    }
    if (fl == 1)
        return true;
    else 
        return false;

}

bool check_if_over_platform(int x, Platform &pl)
{
    for (auto platform : platforms)
    {
        int left, right;
        platform.get_borders(left, right);
        if (x <= right && x >= left)
        {
            pl = platform;
            return true;
        }
    }
    return false;
}
