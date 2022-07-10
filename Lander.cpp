#include "Engine.h"
#include "Lander.h"


const double g = 1.62;

void Lander::set_borders(int xl, int yl, int xr, int yr)
{
	x_b_left = xl;
	x_b_right = xr;
	y_b_left = yl;
	y_b_right = yr;
}

void Lander::rotate(int direction) 
{
	if (direction > 0 && angle < 90)
		angle+=5;
	else if(direction < 0 && angle > -90)
		angle-=5;
}

void Lander::add_force()
{
	if (m_curr < m_max)
		m_curr += m_max*0.05;
	force = m_curr * v_fuel;
}

void Lander::cut_force() 
{
	if (m_curr > 0)
		m_curr -= m_max * 0.05;
	if (m_curr < 0)
		m_curr = 0;
	force = m_curr * v_fuel;
}

void Lander::init_lander(int seed, int level)
{
	srand(seed);
	x = rand() % SCREEN_WIDTH;
	y = 40;
	x_calc = x;
	y_calc = y;
	x_b_left = x-10;
	x_b_right = y + 3;
	y_b_left = x+10;
	y_b_right = y+3;
	vx = rand() % 10 + 1;
	vy = rand() % 20 + 1;
	switch (level)
	{
	case 1:
		fuel = 200;
		break;
	case 2:
		fuel = 100;
		break;
	case 3:
		fuel = 50;
		break;
	default:
		break;
	}
	m_curr = 0;
	force = 0;
	angle = 0;
}

void Lander::calculate(double dt)
{
	fuel -= m_curr * dt;
	if (fuel <= 0)
	{
		m_curr = 0;
		force = 0;
	}
	double vx_pred = vx + dt * (force * sin(angle * 3.14 / 180) / (mass_empty + fuel));
	double vy_pred = vy + dt * ((-force * cos(angle * 3.14 / 180) + (mass_empty + fuel) * g) / (mass_empty + fuel));
	double x_pred = x_calc + dt * vx;
	double y_pred = y_calc + dt * vy;
	if (x_pred > 1024)
		x_pred -= 1024;
	if (x_pred < 0)
		x_pred += 1024;
	x_calc = x_pred;
	y_calc = y_pred;
	x = (int)x_pred;
	y = (int)y_pred;
	vx = vx_pred;
	vy = vy_pred;
}