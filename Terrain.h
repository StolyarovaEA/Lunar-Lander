#pragma once
#include <vector>
#include "Engine.h"
#include <algorithm>
#include "Lander.h"

class Platform
{
public:
	Platform() {};
	Platform(int x, int y, int w, int diff)
	{
		loc_x = x;
		loc_y = y;
		width = w;
		difficulty = diff;
		points = 100 * diff;
	}

	~Platform() {}

	void get_location(int& x, int& y);
	int get_width();
	int get_score();
	int get_difficulty();
	int get_y();
	int get_x();
	void get_borders(int& left, int& right);

private:
	int loc_x;
	int loc_y;
	int width;
	int difficulty;
	int points;
};

extern std::vector<Platform> platforms;
extern int terrain[SCREEN_WIDTH];

void set_platforms();

void generate_platfroms();

void generate_terrain(int s, int end);

void generate_total_terrain(int seed);

bool intersect(int x1_b, int y1_b, int x1_e, int y1_e, int x2_b, int y2_b, int x2_e, int y2_e);

bool check_crash(int begin, int end);

bool check_platform_intersection(int x, int w);

bool check_if_over_platform(int x, Platform& pl);



