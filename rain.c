/*
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>

/*
 * rain 11/3/1980 EPS/CITHEP
 * cc rain.c -o rain -O -ltermlib
 */

#include <sys/types.h>
#include <curses.h>
#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

static volatile sig_atomic_t sig_caught = 0;

int main(int, char **);
static void onsig(int);

int matchcolor(char* color){
  if(0 == strcasecmp(color,"black")) { return COLOR_BLACK; }
  if(0 == strcasecmp(color,"red")) { return COLOR_RED; }
  if(0 == strcasecmp(color,"green")) { return COLOR_GREEN; }
  if(0 == strcasecmp(color,"yellow")) { return COLOR_YELLOW; }
  if(0 == strcasecmp(color,"blue")) { return COLOR_BLUE; }
  if(0 == strcasecmp(color,"magenta")) { return COLOR_MAGENTA; }
  if(0 == strcasecmp(color,"cyan")) { return COLOR_CYAN; }
  if(0 == strcasecmp(color,"white")) { return COLOR_WHITE; }
  return -1;
}

int setcolors(char* fg, char* bg){
  if(start_color() == OK){
    use_default_colors();
    init_pair(1, matchcolor(fg), matchcolor(bg));
    bkgdset(COLOR_PAIR(1) | ' ');
    attrset(COLOR_PAIR(1));
    return 1;
  } else {
    return 0;
  }
}

int
main(int argc, char **argv)
{
	int x, y, j;
	long cols, lines;
	unsigned int delay = (90 * 1000);
	unsigned long val = 0;
	int ch;
	char *ep;
	int xpos[5], ypos[5];
  char *bg = "default";
  char *fg = "blue";

	while ((ch = getopt(argc, argv, "d:b:f:")) != -1)
		switch (ch) {
		case 'd':
			val = strtoul(optarg, &ep, 0);
			if (ep == optarg || *ep)
				errx(1, "Invalid delay `%s'", optarg);
			if (errno == ERANGE && val == ULONG_MAX)
				err(1, "Invalid delay `%s'", optarg);
			if (val >= 1000)
				errx(1, "Invalid delay `%s' (1-999)", optarg);
			delay = (unsigned int)val * 1000;  /* ms -> us */
			break;
    case 'f':
      fg = optarg;
      break;
    case 'b':
      bg = optarg;
      break;
		default:
			(void)fprintf(stderr, "Usage: %s [-d delay] [-f fground] [-b bground]\n",
			    argv[0]);
			return 1;
		}

	if (!initscr())
		errx(0, "couldn't initialize screen");

  setcolors(fg,bg);
  erase();

	cols = COLS - 4;
	lines = LINES - 4;

	(void)signal(SIGHUP, onsig);
	(void)signal(SIGINT, onsig);
	(void)signal(SIGTERM, onsig);

	(void)curs_set(0);
	for (j = 4; j >= 0; --j) {
		xpos[j] = random() % cols + 2;
		ypos[j] = random() % lines + 2;
	}
	for (j = 0;;) {
		if (sig_caught) {
			(void)endwin();
			exit(0);
		}
		x = random() % cols + 2;
		y = random() % lines + 2;
		(void)mvaddch(y, x, '.');
		(void)mvaddch(ypos[j], xpos[j], 'o');
		if (!j--)
			j = 4;
		(void)mvaddch(ypos[j], xpos[j], 'O');
		if (!j--)
			j = 4;
		(void)mvaddch(ypos[j] - 1, xpos[j], '-');
		(void)mvaddstr(ypos[j], xpos[j] - 1, "|.|");
		(void)mvaddch(ypos[j] + 1, xpos[j], '-');
		if (!j--)
			j = 4;
		(void)mvaddch(ypos[j] - 2, xpos[j], '-');
		(void)mvaddstr(ypos[j] - 1, xpos[j] - 1, "/ \\");
		(void)mvaddstr(ypos[j], xpos[j] - 2, "| O |");
		(void)mvaddstr(ypos[j] + 1, xpos[j] - 1, "\\ /");
		(void)mvaddch(ypos[j] + 2, xpos[j], '-');
		if (!j--)
			j = 4;
		(void)mvaddch(ypos[j] - 2, xpos[j], ' ');
		(void)mvaddstr(ypos[j] - 1, xpos[j] - 1, "   ");
		(void)mvaddstr(ypos[j], xpos[j] - 2, "     ");
		(void)mvaddstr(ypos[j] + 1, xpos[j] - 1, "   ");
		(void)mvaddch(ypos[j] + 2, xpos[j], ' ');
		xpos[j] = x;
		ypos[j] = y;
		(void)refresh();
		if (delay) (void)usleep(delay);
	}
}

static void
onsig(int dummy)
{
	sig_caught = 1;
}
