#pragma once
#include "Engine.h"
#include "Draw.h"
#include <stdlib.h>
#include <memory.h>
#include <ctime>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

int seed;
Lander lnd;
Platform pl;
double n_check = 0.1;
double wait = 1;
double N = 0;
int score;
int lifes = 5;
bool crashed = false;
bool landed = false;
bool init = true;
bool ask_restart = false;
bool crash_landed = false;
bool paused = false;
int Game_Hardness;

// initialize game data in this function
void initialize()
{
	score = 0;
	seed = time(0);
	Game_Hardness = 2;
    generate_total_terrain(seed);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
	N += dt;
	if (is_key_pressed('P'))
		paused = true;
	if (is_key_pressed('C'))
		paused = false;
	if (!is_window_active())
		paused = true;
	if (init)
	{
		if (is_key_pressed('1'))
			Game_Hardness = 1;
		if (is_key_pressed('2'))
			Game_Hardness = 2;
		if (is_key_pressed('3'))
			Game_Hardness = 3;
		if (is_key_pressed(VK_RETURN))
		{
			init = false;
			lnd.init_lander(seed, Game_Hardness);
		}
	}
	else if(crashed)
		if (lifes == 0)
		{
			ask_restart = true;
			if (is_key_pressed('1'))
				Game_Hardness = 1;
			if (is_key_pressed('2'))
				Game_Hardness = 2;
			if (is_key_pressed('3'))
				Game_Hardness = 3;
			if (is_key_pressed(VK_RETURN))
			{
				N = 0;
				seed = time(0);
				lnd.init_lander(seed, Game_Hardness);
				generate_total_terrain(seed);
				lifes = 5;
				score = 100;
				crashed = false;
				crash_landed = false;
				ask_restart = false;
			}
		}
		else
		{
			if (N > wait && lifes != 0)
			{
				N = 0;
				seed = time(0);
				lnd.init_lander(seed, Game_Hardness);
				generate_total_terrain(seed);
				crashed = false;
				crash_landed = false;
			}
		}		
	else if (landed)
	{
		if(N > wait)
		{
			N = 0;
			seed = time(0);
			lnd.init_lander(seed, Game_Hardness);
			generate_total_terrain(seed);
			landed = false;
		}
	}
	else if (paused)
	{

	}
	else
	{
		if (N > n_check)
		{
			if (is_key_pressed(VK_UP))
			{
				lnd.add_force();
			}
			if (is_key_pressed(VK_DOWN))
			{
				lnd.cut_force();
			}
			if (is_key_pressed(VK_RIGHT))
			{
				lnd.rotate(1);
			}
			if (is_key_pressed(VK_LEFT))
			{
				lnd.rotate(-1);
			}
			N = 0;
		}
		lnd.calculate(dt);
		int left, right, y;
		if (check_if_over_platform(lnd.x, pl))
		{
			pl.get_borders(left, right);
			y = SCREEN_HEIGHT - pl.get_y();
			if (lnd.y_b_left == y || lnd.y_b_right == y)
				if (lnd.x_b_left >= left && lnd.x_b_right <= right && lnd.vy <= 15 && lnd.vx <= 5 && lnd.vx >= -5 && lnd.angle >= -15 && lnd.angle <= 15)
				{
					landed = true;
					score += pl.get_score();
				}
				else
				{
					crashed = true;
					crash_landed = true;
					lifes--;
				}
		}
		if (!landed && !crash_landed)
		{
			if (check_crash(lnd.x_b_left - 1, lnd.x_b_right + 1))
			{
				crashed = true;
				lifes --;
			}
		}
	}
    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
	clear_buffer();
	if (landed)
	{
		draw_text("success", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, WHITE);
		
	}
	else if (ask_restart)
	{
		draw_text("game over", SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 4 + 100, WHITE);
		draw_text("final score: " + std::to_string(score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 4 + 120, WHITE);
		
		draw_text("choose difficulty level:", SCREEN_WIDTH / 2 - 240, SCREEN_HEIGHT / 4 + 150, WHITE);
		draw_text("press 1 - easy", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 165, WHITE);
		draw_text("press 2 - medium", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 180, WHITE);
		draw_text("press 3 - hard", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 195, WHITE);
		switch (Game_Hardness)
		{
		case 1:
			draw_text("current level: easy", SCREEN_WIDTH / 2 - 240, SCREEN_HEIGHT / 4 + 220, WHITE);
			break;
		case 2:
			draw_text("current level: medium", SCREEN_WIDTH / 2 - 240, SCREEN_HEIGHT / 4 + 220, WHITE);
			break;
		case 3:
			draw_text("current level: hard", SCREEN_WIDTH / 2 - 240, SCREEN_HEIGHT / 4 + 220, WHITE);
			break;
		default:
			break;
		}
		draw_text("press enter to restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 4 + 240, WHITE);
		draw_text("press esc to exit", 10, 10, WHITE);
		
	}
	else if (init)
	{
		draw_text("welcome to lunar lander", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 4, WHITE);
		draw_text("use up and down to control thust", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 30, WHITE);
		draw_text("use left and right to control orientation", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 45, WHITE);
		draw_text("to land successfully make sure to keep at final stage:", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 70, WHITE);
		draw_text("vx between -5 and 5", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 85, WHITE);
		draw_text("vy < 15", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 100, WHITE);
		draw_text("angle between -15 and 15", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 115, WHITE);
		draw_text("you have 5 lives. game is over when all lives are lost", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 140, WHITE);
		draw_text("choose difficulty level:", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 160, WHITE);
		draw_text("press 1 - easy", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 175, WHITE);
		draw_text("press 2 - medium", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 190, WHITE);
		draw_text("press 3 - hard", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 4 + 205, WHITE);
		switch (Game_Hardness)
		{
		case 1:
			draw_text("current level: easy", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 230, WHITE);
			break;
		case 2:
			draw_text("current level: medium", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 230, WHITE);
			break;
		case 3:
			draw_text("current level: hard", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 4 + 230, WHITE);
			break;
		default:
			break;
		}
		draw_text("press enter to start", SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT - 30, WHITE);
		draw_text("press esc to exit", 10, 10, INT32_MAX);

	}
	else if (crashed)
	{
		draw_text("you crashed", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, WHITE);
	}
	else if (paused)
	{
		draw_text("game paused", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 -20, WHITE);
		draw_text("press c to continue", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, WHITE);
	}
	else
	{
		draw_terrain();
		draw_lander(lnd.x, lnd.y, lnd.angle, lnd.force / (lnd.m_max * lnd.v_fuel / 100));
		bool over_platform = check_if_over_platform(lnd.x, pl);
		if (over_platform)
		{
			int y = SCREEN_HEIGHT - pl.get_y();
			bool fl_vx_drawn = false, fl_vy_drawn = false, fl_angle_drawn = false;
			if (y - lnd.y_b_left < 70 || y - lnd.y_b_right < 70)
			{
				if (lnd.vx > 5 || lnd.vx < -5)
				{
					draw_text("vx: " + std::to_string(lnd.vx).erase(std::to_string(lnd.vx).size() - 4), 550, 5, RED);
					fl_vx_drawn = true;
				}
				if (lnd.vy > 15)
				{
					draw_text("vy: " + std::to_string(lnd.vy).erase(std::to_string(lnd.vy).size() - 4), 550, 18, RED);
					fl_vy_drawn = true;
				}
				if (lnd.angle > 15 || lnd.angle < -15)
				{
					draw_text("angle: " + std::to_string(lnd.angle), 864, 5, RED);
					fl_angle_drawn = true;
				}
			}
			else
			{
				draw_text("vx: " + std::to_string(lnd.vx).erase(std::to_string(lnd.vx).size() - 4), 550, 5, WHITE);
				fl_vx_drawn = true;
				draw_text("vy: " + std::to_string(lnd.vy).erase(std::to_string(lnd.vy).size() - 4), 550, 18, WHITE);
				fl_vy_drawn = true;
				draw_text("angle: " + std::to_string(lnd.angle), 864, 5, WHITE);
				fl_angle_drawn = true;
			}
			if (!fl_angle_drawn)
				draw_text("angle: " + std::to_string(lnd.angle), 864, 5, WHITE);
			if (!fl_vx_drawn)
				draw_text("vx: " + std::to_string(lnd.vx).erase(std::to_string(lnd.vx).size() - 4), 550, 5, WHITE);
			if (!fl_vy_drawn)
				draw_text("vy: " + std::to_string(lnd.vy).erase(std::to_string(lnd.vy).size() - 4), 550, 18, WHITE);
			over_platform = true;
		}
		if (!over_platform)
		{
			draw_text("angle: " + std::to_string(lnd.angle), SCREEN_WIDTH - 152, 5, WHITE);
			draw_text("vx: " + std::to_string(lnd.vx).erase(std::to_string(lnd.vx).size() - 4), 550, 5, WHITE);
			draw_text("vy: " + std::to_string(lnd.vy).erase(std::to_string(lnd.vy).size() - 4), 550, 18, WHITE);
		}

		
		draw_text("x:" + std::to_string(lnd.x), 726, 5, WHITE);
		if(!over_platform && SCREEN_HEIGHT - lnd.y - terrain[lnd.x] < 70)
			draw_text("y:" + std::to_string(SCREEN_HEIGHT - lnd.y - terrain[lnd.x]), 726, 18, RED);
		else
			draw_text("y:" + std::to_string(SCREEN_HEIGHT - lnd.y - terrain[lnd.x]), 726, 18, WHITE);
		
		draw_text("thrust: " + std::to_string(int(lnd.force / (lnd.m_max * lnd.v_fuel / 100))), 373, 5, WHITE);
		if (lnd.fuel < 20.0)
			draw_text("fuel: " + std::to_string(int(lnd.fuel)), 373, 18, RED);
		else
			draw_text("fuel: " + std::to_string(int(lnd.fuel)), 373, 18, WHITE);


		draw_text("score: " + std::to_string(score), 10, 18, WHITE);
		int x = 10;
		for (int i = 0; i < lifes; i++)
		{
			draw_bitmap(heart, x, 5, PINK);
			x += 11;
		}
		draw_line(0, 30, 1023, 30, WHITE);

		for (auto &platform : platforms)
		{
			int x, y, d;
			platform.get_location(x, y);
			d = platform.get_difficulty();
			draw_text(std::to_string(d) + "x", x - 10, SCREEN_HEIGHT - y + 3, GREY);
		}
		draw_text("press esc to exit", 10, SCREEN_HEIGHT - 15, DARK_GREY);
		draw_text("press p to pause", 847, SCREEN_HEIGHT - 15, DARK_GREY);
	}
}

// free game data in this function
void finalize()
{
}

