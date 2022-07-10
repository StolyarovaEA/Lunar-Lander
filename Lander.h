#pragma once
#include <random>

extern const double g;


class Lander 
{
public:
	Lander() {}
	~Lander() {}
	double vx;
	double vy;
	int angle;
	double fuel;
	double force;
	double x_calc;
	double y_calc;
	int x;
	int y;
	int x_b_left;
	int x_b_right;
	int y_b_left;
	int y_b_right;
	double m_curr;

	void rotate(int direction);
	void add_force();
	void cut_force();
	void set_borders(int xl, int yl, int xr, int yr);
	void init_lander(int seed, int level);
	void calculate(double dt);

	const int v_fuel = 3120;
	const double m_max = 2.4;
	const int mass_empty = 2000;
};

extern Lander lnd;
