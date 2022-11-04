/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>

#include "../slstatus.h"
#include "../util.h"

#include <limits.h>

#define BRIGHTNESS_CURRENT "/sys/class/backlight/%s/brightness"
#define BRIGHTNESS_MAX     "/sys/class/backlight/%s/max_brightness"

const char *
backlight_perc(const char *bl)
{
	int max, c;
	char path[PATH_MAX];

	if (esnprintf(path, sizeof(path), BRIGHTNESS_MAX, bl) < 0)
		return NULL;
	if (pscanf(path, "%d", &max) != 1)
		return NULL;

	if (esnprintf(path, sizeof(path), BRIGHTNESS_CURRENT, bl) < 0)
		return NULL;
	if (pscanf(path, "%d", &c) != 1)
		return NULL;

	float perc = (float)c / max * 100.0;
	return bprintf("%d", (int)perc);
}
