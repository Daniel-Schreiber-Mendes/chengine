#include "../../engine.h"


void config_load(void)
{
	char fullscreen;
	uint16_t width, height;
	char title[20];
	char vsync;

	{
		FILE *file = fopen("../resources/config/che.cfg", "r");
		if (!file)
		{
			printf("No configuration file could be found\n");
			return;
		}

		fscanf(file, "fullscreen = %c\n", &fullscreen);
		fscanf(file, "size = %hu, %hu\n", &width, &height);
		fscanf(file, "title = %s", title);
		fscanf(file, "vsync = %c", &vsync);

		fclose(file);
	}

	window_fullscreen_set(fullscreen - '0');
	window_size_set(width, height);
	window_title_set(title);
	window_vsync_set(vsync - '0');
}