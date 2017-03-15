/* allegro_test.cpp */

#include <allegro.h>
int main(void)
{
	int retval = 0;
	if (allegro_init() != 0)
	{
		retval = -1;;
	}
	else
	{
		allegro_message("Welcome To Allegro!");
		retval = 0;
	}
	return retval;
}
END_OF_MAIN()
