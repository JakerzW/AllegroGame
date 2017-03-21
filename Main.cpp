#include <allegro.h>
#include <cstdlib>
/* prototypes */
int init();
void deinit();

int main()
{  /* start of main */
	int retval = 0;
	if (init() == 0)
	{
		while (!key[KEY_ESC])
		{
			/*  put your code here  */
			putpixel(screen, SCREEN_W / 2, SCREEN_H / 2, makecol(0, 255, 0));
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
	int res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1200, 640, 0, 0);
	if (res != 0)
	{
		if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0)
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
}
