#include <allegro.h>
#include <cstdlib>
#include <cmath>

BITMAP *backgroundBmp = nullptr;
BITMAP *archerBmp = nullptr;
BITMAP *archerNoArmsBmp = nullptr;
BITMAP *archerArmBmp = nullptr;
BITMAP *arrowBmp = nullptr;
BITMAP *chickenBmp = nullptr;
BITMAP *eagleBmp = nullptr;
BITMAP *seagullBmp = nullptr;
BITMAP *bufferBmp = nullptr;

int ticks;
int bowAngle = 255;
int score;
int timer = 60000;
const int maxBirds = 10;
const int maxArrows = 10;
double acc = 0.3;
bool archerReady = true;
bool gameExit = false;
bool gameOver = false;

int init();
void deinit();
int initBitmaps();
int destroyBitmaps();



struct arrowState
{
	double x, y;
	double vx, vy;
	int px, py;
	bool active;
} arrow[2];

/*struct fireState 
{

} fire[480];*/

struct chickenState
{
	double x, y;
	double vx, vy;
	int curPos;
	bool movingRight;
	bool active;
} chicken[maxBirds];

struct eagleState
{
	double x, y;
	double vx, vy;
	int curPos;
	bool movingRight;
	bool active;
} eagle[maxBirds];

struct seagullState
{
	double x, y;
	double vx, vy;
	int curPos;
	bool movingRight;
	bool active;
} seagull[maxBirds];

int initBitmaps()
{
	backgroundBmp = load_bitmap("sprites\\background.bmp", 0);
	archerBmp = load_bitmap("sprites\\archer.bmp", 0);
	archerNoArmsBmp = load_bitmap("sprites\\archernoarms.bmp", 0);
	archerArmBmp = load_bitmap("sprites\\archerarm.bmp", 0);
	arrowBmp = load_bitmap("sprites\\arrow.bmp", 0);
	chickenBmp = load_bitmap("sprites\\chicken.bmp", 0);
	eagleBmp = load_bitmap("sprites\\eagle.bmp", 0);
	seagullBmp = load_bitmap("sprites\\seagull.bmp", 0);
	bufferBmp = create_bitmap(SCREEN_W, SCREEN_H);
	clear_bitmap(bufferBmp);
	//set_mouse_sprite(load_bitmap("sprites\\crosshair.bmp", 0));
	
	return 0;
}

void drawChicken(int x, int y, int curPos, bool movingRight)
{
	if ((ticks % 100) == 0)
	{
		for (size_t i = 0; i < maxBirds; i++)
		{
			chicken[i].curPos++;
			if (chicken[i].curPos > 2)
				chicken[i].curPos = 0;
		}
		curPos++;
		if (curPos > 2)
			curPos = 0;
	}
	masked_blit(chickenBmp, bufferBmp, curPos * 32, movingRight * 19, x, y, 32, 19);
}

void drawEagle(int x, int y, int curPos, bool movingRight)
{
	if ((ticks % 100) == 0)
	{
		for (size_t i = 0; i < maxBirds; i++)
		{
			eagle[i].curPos++;
			if (eagle[i].curPos > 2)
				eagle[i].curPos = 0;
		}
		curPos++;
		if (curPos > 2)
			curPos = 0;
	}
	masked_blit(eagleBmp, bufferBmp, curPos * 32, movingRight * 31, x, y, 32, 31);
}

void drawSeagull(int x, int y, int curPos, bool movingRight)
{
	if ((ticks % 100) == 0)
	{
		for (size_t i = 0; i < maxBirds; i++)
		{
			seagull[i].curPos++;
			if (seagull[i].curPos > 2)
				seagull[i].curPos = 0;
		}
		curPos++;
		if (curPos > 2)
			curPos = 0;
	}
	masked_blit(seagullBmp, bufferBmp, curPos * 32, movingRight * 29, x, y, 31, 29);
}

void game()
{
	//exit game
	if (key[KEY_ESC])
		gameOver = true;

	//aim left
	if (key[KEY_LEFT] || key[KEY_A])
	{
		bowAngle -= 1;
		if (bowAngle < 195)
			bowAngle = 195;
		/*if(!turreton)
		{
		play_sample(turretsound,255,tankpan,1000,TRUE);
		turreton = true;
		}
		adjust_sample(turretsound,255,tankpan,1000,TRUE);

		add for bow sounds*/
	}

	//aim right
	if (key[KEY_RIGHT] || key[KEY_D])
	{
		bowAngle += 1;
		if (bowAngle > 270)
			bowAngle = 270;
	}

	//fire arrow with keyboard
	if (key[KEY_SPACE] && archerReady == true)
	{
		archerReady = false;
		for (size_t i = 0; i < maxArrows; i++)
		{
			if (arrow[i].active == 0)
			{
				//play arrow sound
				arrow[i].active = 1;
				arrow[i].x = 100 - ((270 - bowAngle) / 1.5);
				arrow[i].y = 530 + ((bowAngle - 195) / 1.8);
				arrow[i].vx = (bowAngle - 195) / 3;
				arrow[i].vy = -(255 - bowAngle) / 1.5;
				break;
			}
		}
	}

	//fire release
	if ((!key[KEY_SPACE] || mouse_b & 1) && archerReady == false)
		archerReady = true;

	//delete birds that aren't on screen 
	for (size_t i = 0; i < maxBirds; i++)
	{
		if (((chicken[i].movingRight == 0) && (chicken[i].x < 0)) || ((chicken[i].movingRight == 1) && (chicken[i].x > 1280)))
			chicken[i].active = 0;

		if (((eagle[i].movingRight == 0) && (eagle[i].x < 0)) || ((eagle[i].movingRight == 1) && (eagle[i].x > 1280)))
			eagle[i].active = 0;

		if (((seagull[i].movingRight == 0) && (seagull[i].x < 0)) || ((seagull[i].movingRight == 1) && (seagull[i].x > 1280)))
			seagull[i].active = 0;
	}

	//add chicken
	if (rand() % 1000 > 992)
	{
		for (size_t i = 0; i < maxBirds; i++)
		{
			if (chicken[i].active == 0)
			{
				chicken[i].active = 1;
				if (((rand() % 100) % 2) == 0)
				{
					chicken[i].movingRight = true;
					chicken[i].x = 0;
					chicken[i].vx = (rand() % 5 + 5) / 5;
				}
				else
				{
					chicken[i].movingRight = false;
					chicken[i].x = 1280;
					chicken[i].vx = -((rand() % 5 + 5) / 5);
				}
				chicken[i].y = (rand() % 100 + 600);
				chicken[i].vy = 0;
				break;
			}
		}
	}

	//add seagull
	if (rand() % 1000 > 990)
	{
		for (size_t i = 0; i < maxBirds; i++)
		{
			if (seagull[i].active == 0)
			{
				seagull[i].active = 1;
				if (((rand() % 100) % 2) == 0)
				{
					seagull[i].movingRight = true;
					seagull[i].x = 0;
					seagull[i].vx = (rand() % 5 + 8) / 5;
				}
				else
				{
					seagull[i].movingRight = false;
					seagull[i].x = 1280;
					seagull[i].vx = -((rand() % 5 + 8) / 5);
				}
				seagull[i].y = rand() % SCREEN_H / 2 + 25;
				seagull[i].vy = 0;
				break;
			}
		}
	}

	//add eagle
	if (rand() % 1000 > 998)
	{
		for (size_t i = 0; i < maxBirds; i++)
		{
			if (eagle[i].active == 0)
			{
				eagle[i].active = 1;

				if (((rand() % 100) % 2) == 0)
				{
					eagle[i].movingRight = true;
					eagle[i].x = 0;
					eagle[i].vx = (rand() % 5 + 8) / 5;
				}
				else
				{
					eagle[i].movingRight = false;
					eagle[i].x = 1280;
					eagle[i].vx = -((rand() % 5 + 8) / 5);
				}
				eagle[i].y = rand() % SCREEN_H / 2 + 25;
				eagle[i].vy = 0;
				break;
			}
		}
	}

	stretch_blit(backgroundBmp, bufferBmp, 0, 0, backgroundBmp->w, backgroundBmp->h, 0, 0, SCREEN_W, SCREEN_H);
	masked_blit(archerNoArmsBmp, bufferBmp, 0, 0, 100, 530, 58, 80);
	rotate_sprite(bufferBmp, archerArmBmp, 80, 540, itofix(bowAngle));

	//draw arrow position
	for (size_t i = 0; i < maxArrows; i++)
	{
		if (arrow[i].active == 1)
		{
			arrow[i].px = arrow[i].x;
			arrow[i].py = arrow[i].y;
			arrow[i].x = arrow[i].x + arrow[i].vx;
			arrow[i].y = arrow[i].y + arrow[i].vy;
			arrow[i].vy = arrow[i].vy + acc;
			rotate_sprite(bufferBmp, arrowBmp, arrow[i].x, arrow[i].y, itofix(bowAngle - 192));
			if ((arrow[i].px > SCREEN_W) || (arrow[i].py > SCREEN_H))
				arrow[i].active = 0;
			if ((arrow[i].active == 1) && (arrow[i].py > SCREEN_H))
			{
				arrow[i].active = 0;
				//insert dead arrow graphic
			}
		}
	}

	//check chicken collisions
	for (size_t i = 0; i < maxBirds; i++)
	{
		for (size_t o = 0; o < maxArrows; o++)
		{
			if ((chicken[i].active == 1) && (arrow[o].active == 1))
				if ((abs(chicken[i].x - arrow[o].x) < 10) && (abs(chicken[i].y - arrow[o].y) < 10))
				{
					chicken[i].active = 0;
					arrow[o].active = 0;
					//insert dead bird img
					score += 1;
				}
		}
	}

	//check seagull collisions
	for (size_t i = 0; i < maxBirds; i++)
	{
		for (size_t o = 0; o < maxArrows; o++)
		{
			if ((seagull[i].active == 1) && (arrow[o].active == 1))
				if ((abs(seagull[i].x - arrow[o].x) < 10) && (abs(seagull[i].y - arrow[o].y) < 10))
				{
					seagull[i].active = 0;
					arrow[o].active = 0;
					//insert dead bird img
					score += 3;
				}
		}
	}

	//check for eagle collisions
	for (size_t i = 0; i < maxBirds; i++)
	{
		for (size_t o = 0; o < maxArrows; o++)
		{
			if ((eagle[i].active == 1) && (arrow[o].active == 1))
				if ((abs(eagle[i].x - arrow[o].x) < 10) && (abs(eagle[i].y - arrow[o].y) < 10))
				{
					eagle[i].active = 0;
					arrow[o].active = 0;
					//insert dead bird img
					score += 10;
				}
		}
	}

	//move and draw birds
	for (size_t i = 0; i < maxBirds; i++)
	{
		if (chicken[i].active == 1)
		{
			drawChicken(chicken[i].x, chicken[i].y, chicken[i].curPos, chicken[i].movingRight);
			chicken[i].x += chicken[i].vx;
			chicken[i].y += chicken[i].vy;
		}
		if (eagle[i].active == 1)
		{
			drawEagle(eagle[i].x, eagle[i].y, eagle[i].curPos, eagle[i].movingRight);
			eagle[i].x += eagle[i].vx;
			eagle[i].y += eagle[i].vy;
		}
		if (seagull[i].active == 1)
		{
			drawSeagull(seagull[i].x, seagull[i].y, seagull[i].curPos, seagull[i].movingRight);
			seagull[i].x += seagull[i].vx;
			seagull[i].y += seagull[i].vy;
		}
	}

	textprintf_ex(bufferBmp, font, 10, 10, makecol(0, 0, 0), -1, "Score: %d", score);

	blit(bufferBmp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	rest(20);
	ticks += 20;
	timer -= 20;
	if (timer <= 0)
		gameOver = true;
}

int main()
{  /* start of main */
	int retval = 0;
	if (init() == 0)
	{
		initBitmaps();
		show_mouse(screen);
		bool menuDrawn = false;
		bool instrDrawn = false;

		while (!gameExit)
		{
			if (!menuDrawn)
			{
				clear_bitmap(bufferBmp);
				stretch_blit(backgroundBmp, bufferBmp, 0, 0, backgroundBmp->w, backgroundBmp->h, 0, 0, SCREEN_W, SCREEN_H);
				textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 100, makecol(0, 0, 0), -1, "Welcome to ARCHER");
				textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 60, makecol(0, 0, 0), -1, "1. Play");
				textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 40, makecol(0, 0, 0), -1, "2. Instructions");
				textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 20, makecol(0, 0, 0), -1, "3. Exit");
				blit(bufferBmp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
				menuDrawn = true;
			}
			
			if (key[KEY_1])
			{
				score = 0;
				timer = 60000;
				while (!gameOver)
					game();
				menuDrawn = false;
				while (!key[KEY_ENTER])
				{
					clear_bitmap(bufferBmp);
					stretch_blit(backgroundBmp, bufferBmp, 0, 0, backgroundBmp->w, backgroundBmp->h, 0, 0, SCREEN_W, SCREEN_H);
					textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 20, makecol(0, 0, 0), -1, "Your score: %d", score);
					textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 + 20, makecol(0, 0, 0), -1, "Press ENTER to return to the menu.");
					blit(bufferBmp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
				}
			}
			if (key[KEY_2])
			{
				while (!key[KEY_ENTER])
				{
					if (!instrDrawn)
					{
						clear_bitmap(bufferBmp);
						stretch_blit(backgroundBmp, bufferBmp, 0, 0, backgroundBmp->w, backgroundBmp->h, 0, 0, SCREEN_W, SCREEN_H);
						textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 100, makecol(0, 0, 0), -1, "Use the left and right arrow keys or A and D to aim.");
						textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 80, makecol(0, 0, 0), -1, "When ready to shoot your arrow, press SPACE.");
						textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 40, makecol(0, 0, 0), -1, "You only have 1 minute to score as high as you can,");
						textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 20, makecol(0, 0, 0), -1, "so don't waste your time!");
						textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 + 20, makecol(0, 0, 0), -1, "Press ENTER to return to the menu.");
						blit(bufferBmp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
						instrDrawn = true;
					}
				}
				menuDrawn = false;
			}
			if (key[KEY_3])
				gameExit = true;

			if (mouse_b & 1)
			{
				if (((mouse_x > ((SCREEN_W/2) - 16)) && (mouse_x < ((SCREEN_W/2) + 16)) && (mouse_y > ((SCREEN_H/2) - 64)) && (mouse_y < ((SCREEN_H / 2) - 56))) || key[KEY_1])
				{
					while (!gameOver)
						game();
					menuDrawn = false;
				}
				if (((mouse_x >((SCREEN_W / 2) - 48)) && (mouse_x < ((SCREEN_W / 2) + 48)) && (mouse_y > ((SCREEN_H / 2) - 44)) && (mouse_y < ((SCREEN_H / 2) - 36))) || key[KEY_2])
				{
					while (!key[KEY_ENTER])
					{
						if (!instrDrawn)
						{
							clear_bitmap(bufferBmp);
							stretch_blit(backgroundBmp, bufferBmp, 0, 0, backgroundBmp->w, backgroundBmp->h, 0, 0, SCREEN_W, SCREEN_H);
							textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 100, makecol(0, 0, 0), -1, "Use the left and right arrow keys or A and D to aim.");
							textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 80, makecol(0, 0, 0), -1, "When ready to shoot your arrow, press SPACE.");
							textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 40, makecol(0, 0, 0), -1, "You only have 1 minute to score as high as you can,");
							textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 - 20, makecol(0, 0, 0), -1, "so don't waste your time!");
							textprintf_centre_ex(bufferBmp, font, SCREEN_W / 2, SCREEN_H / 2 + 20, makecol(0, 0, 0), -1, "Press ENTER to return to the menu.");
							instrDrawn = true;
						}
					}
					menuDrawn = false;
				}
				if (((mouse_x >((SCREEN_W / 2) - 16)) && (mouse_x < ((SCREEN_W / 2) + 16)) && (mouse_y >((SCREEN_H / 2) - 24)) && (mouse_y < ((SCREEN_H / 2) - 16))) || key[KEY_3])
				{
					gameExit = true;
				}
			}
			//game();
			/*if (key[KEY_ESC])
				gameExit = true;*/
		}
	}
	else
	{
		deinit();
		retval = -1;
	}
	return retval;;
} /* end of Normal main */

END_OF_MAIN()     /* line must be included at the end of the nor

/* function definitions */
int init()
{
	int depth = 0;
	int retvalue = 0;
	if (allegro_init() != 0)
	{
		retvalue = -1;
	}
	depth = desktop_color_depth();
	if (depth == 0)
	{
		depth = 32;
	}
	set_color_depth(depth);
	bool graphtestOK = true;
	int res = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1280, 720, 0, 0);
	if (res != 0)
	{
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0)
		{
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			allegro_message(allegro_error);
			graphtestOK = false;
			retvalue = -1;
		}
	}
	if (graphtestOK == true)
	{
		install_timer();
		install_keyboard();
		install_mouse();
		retvalue = 0;
	}
	/* add other initializations here */
	return retvalue;
}

void deinit()
{
	clear_keybuf();
	/* add other deinitializations here */
	destroyBitmaps();
}



int destroyBitmaps()
{
	destroy_bitmap(backgroundBmp);
	destroy_bitmap(archerBmp);
	destroy_bitmap(arrowBmp);

	return 0;
}