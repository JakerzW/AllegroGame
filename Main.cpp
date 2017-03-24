#include <allegro.h>
#include <cstdlib>
/* prototypes */
int init();
void deinit();
//int drawBackground();
int destroyBitmaps();

BITMAP *background = nullptr;
BITMAP *archer = nullptr;
BITMAP *arrow = nullptr;
BITMAP *buffer = nullptr;



int main()
{  /* start of main */
	int retval = 0;
	if (init() == 0)
	{
		background = load_bitmap("sprites\\background.bmp", 0);
		archer = load_bitmap("sprites\\archer.bmp", 0);
		arrow = load_bitmap("sprites\\arrow.bmp", 0);
		set_mouse_sprite(load_bitmap("sprites\\crosshair.bmp", 0));
		buffer = create_bitmap(SCREEN_W, SCREEN_H);
		clear_bitmap(buffer);
		bool archerReady = false;
		bool arrowOnScreen = false;
		while (!key[KEY_ESC])
		{
			/*  main code  */
			
			if (!archerReady)
			{
				for (size_t i = 0; i < 6; i++)
				{
					stretch_blit(background, buffer, 0, 0, background->w, background->h, 0, 0, SCREEN_W, SCREEN_H);
					masked_blit(archer, buffer, i * 58, 0, 100, 530, 58, 80);
					//masked_blit(arrow, buffer, 4 * 36, 0, (i + 1) * 100, 540, 36, 39);
					blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
					rest(100);
				}
				archerReady = true;
			}
			if (key[KEY_SPACE])
			{
				arrowOnScreen = true;
				int arrowDist = 100;
				while (arrowOnScreen)
				{
					masked_blit(arrow, buffer, 4 * 36, 0, arrowDist, 540, 36, 39);
					arrowDist += 100;
				}
			}
			show_mouse(screen);
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
	destroyBitmaps();
	return retvalue;
}

void deinit()
{
	clear_keybuf();
	/* add other deinitializations here */
}

/*int drawBackground()
{
	background = load_bitmap("sprites\\background.bmp", 0);
	if (!background)
	{
		allegro_message("Unable to find background image\n%s\n", allegro_error);
		return -1;
		destroy_bitmap(background);
	}
}*/

int destroyBitmaps()
{
	destroy_bitmap(background);
	destroy_bitmap(archer);
	destroy_bitmap(arrow);

	return 0;
}