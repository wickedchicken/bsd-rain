rain -- a console screensaver

-----

this is a port of the NetBSD `rain`, which draws raindrops on your terminal. it
has been modified to compile straight on Linux and have some better defaults.

`rain` is great for keeping idle SSH terminals open, or filling up your screen
with something less distracting than email. use it wisely.

installation: `make`, then copy `./rain` to wherever you like.

usage: `rain [-d delay] [-f foreground] [-b background]`

delay is ms between stages and is nominally set to 90. setting it to zero will
run the program as fast as possible, which is probably not what you want.

foreground and background colors are the standard curses colors: black, red,
green, yellow, blue, magenta, cyan, white, and default. with no options `rain`
will use 'blue' for foreground and 'default' for background. if your terminal
has transparency enabled, 'default' will honor that transparency while 'black'
will not.

-----

`rain` was originally authored by Eric P. Scott.

<!--- vim: set tw=80: -->
