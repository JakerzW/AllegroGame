#include <allegro.h>
#include <cstdlib>
#include <cmath>
/* prototypes */
int init();
void deinit();

int initBitmaps();
int destroyBitmaps();
int changeArcherState(int state, int arrowX, int arrowY, int arrowPos);
//int fireArrow();

BITMAP *background = nullptr;
BITMAP *archer = nullptr;
BITMAP *arrow = nullptr;
BITMAP *mainBuffer = nullptr;
BITMAP *spriteBuffer = nullptr;

int main()
{  /* start of main */
	int retval = 0;
	if (init() == 0)
	{
		initBitmaps();
		bool archerReady = false;
		bool arrowOnScreen = false;
		show_mouse(screen);
		while (!key[KEY_ESC])
		{
			/*  main code  */
			
			if (!archerReady)
			{
				if (changeArcherState(0, 'null', 'null', 'null') == 1)
					archerReady = true;
			}
			if (archerReady && (mouse_b & 1))
			{
				arrowOnScreen = true;
				int arrowDist = 100;
				while (arrowOnScreen)
				{
					masked_blit(arrow, screen, 36, 0, arrowDist, 540, 36, 39);
					arrowDist += 100;
					if (arrowDist > 1500)
					{
						arrowOnScreen = false;
					}
				}
			}
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

int initBitmaps()
{
	background = load_bitmap("sprites\\background.bmp", 0);
	archer = load_bitmap("sprites\\archer.bmp", 0);
	arrow = load_bitmap("sprites\\arrow.bmp", 0);
	set_mouse_sprite(load_bitmap("sprites\\crosshair.bmp", 0));
	mainBuffer = create_bitmap(SCREEN_W, SCREEN_H);
	clear_bitmap(mainBuffer);

	return 0;
}
int destroyBitmaps()
{
	destroy_bitmap(background);
	destroy_bitmap(archer);
	destroy_bitmap(arrow);

	return 0;
}

int changeArcherState(int state, int arrowX, int arrowY, int arrowPos)
{
	int currentState = 'null';
	currentState = state;
	switch (currentState)
	{
		case 0:	//archer changing from not being able to shoot upon game opening to being able to shoot
		{
			for (size_t i = 0; i < 6; i++)
			{
				stretch_blit(background, mainBuffer, 0, 0, background->w, background->h, 0, 0, SCREEN_W, SCREEN_H);
				masked_blit(archer, mainBuffer, i * 58, 0, 100, 530, 58, 80);
				blit(mainBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
				clear_bitmap(mainBuffer);
				rest(80);
			}
			return 1;
		}
		case 1:	//archer ready to shoot to shooting and returning to being ready to shoot
		{
			bool archerReady = false;
			int archerPos = 6;
			while (!archerReady)
			{
				++archerPos;
				if (archerPos > 8)
					archerPos = 0;
				stretch_blit(background, mainBuffer, 0, 0, background->w, background->h, 0, 0, SCREEN_W, SCREEN_H);
				masked_blit(archer, mainBuffer, archerPos * 58, 0, 100, 530, 58, 80);
				//insert arrow information (get arrow position and display the location it would be when the archer is moving (every 80 ticks)
				//masked_blit(arrow, mainBuffer, arrowPos * 36, 0, arrowX, arrowY, 36, 39);
				blit(mainBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
				rest(80);
			}
			return 2;
		}
	}
}

int fireArrow()
{
	/*process:
			- x and y pos of mouse at time of click
			- constant velocity value
			- use x and y pos to work out angle using inverse tan
			- using angle and constant velocity, find velocity of x and y direction
			- using 1000 rests as a second and 40 pixels as a meter, find how many pixels per rests it should move
			- use velocity of direction, divide by 100 to find distance travelled in 10 milseconds and then times by 40
			  to find no. of pixels travelled in 10 milseconds
			*/

	int xPos, yPos;
	int v = 65;
	int pixToMoveX, pixToMoveY;
	float vX, vY;
	float rotation;

	xPos = mouse_x; yPos = 720 - mouse_y;	//gets current mouse coords and puts into variables
	rotation = atan(xPos / yPos);	//finds angle needed for rotation of sprite

	vX = v * (cos(rotation));
	vY = v * (sin(rotation));

	pixToMoveX = (vX / 100) * 40;
	pixToMoveY = (vY / 100) * 40;

	return 0;
}