/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>

#include "../slstatus.h"
#include "../util.h"

#include <limits.h>

#define POWER_SUPPLY_AC_ONLINE "/sys/class/power_supply/%s/online"
#define POWER_SUPPLY_CAPACITY "/sys/class/power_supply/%s/capacity"

static const char *
battery_perc_state(const char *bat)
{
	static struct {
		int perc;
		char *symbol;
	} map[] = {
		{ 100, "" },
		{ 85 , " " },
		{ 50 , "" },
		{ 25 , "" },
		{ 0  , "" },
	};

	size_t i;
	int perc;
	char path[PATH_MAX];

	if (esnprintf(path, sizeof(path), POWER_SUPPLY_CAPACITY, bat) < 0)
		return NULL;
	if (pscanf(path, "%d", &perc) != 1)
		return NULL;

	for (i = 0; i < LEN(map); i++)
		if (perc >= map[i].perc)
			break;

	return (i == LEN(map)) ? "?" : map[i].symbol;
}

const char *
ac_online_state(const char *ac, const char *bat)
{
	int state;
	char path[PATH_MAX];

	if (esnprintf(path, sizeof(path), POWER_SUPPLY_AC_ONLINE, ac) < 0)
		return NULL;
	if (pscanf(path, "%d", &state) != 1)
		return NULL;

	if (state == 1)
		return "";
	else if (state == 0)
		return battery_perc_state(bat);
	else
		return "?";
}
