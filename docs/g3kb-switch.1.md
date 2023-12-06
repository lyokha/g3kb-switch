# NAME
**g3kb-switch** - CLI keyboard layout switcher for Gnome 3 and 4x.

# SYNOPSIS
**g3kb-switch** [`-p` | `-l` | `-n` | `-s` *ARG*]

**g3kb-switch** [`-h` | `--help`]

**g3kb-switch** [`-v` | `--version`]

# DESCRIPTION
**g3kb-switch** implements direct D-Bus messaging with Gnome Shell to list,
get, and set system keyboard layouts. In Gnome 41 and newer, the switcher
requires assistance from Gnome extension *g3kb-switch*. See details at
<https://github.com/lyokha/g3kb-switch#g3kb-switch>.

# OPTIONS
`-p`
: show the current layout group (this is the default action)

`-l`
: show available layout groups

`-n`
: switch to the next layout group

`-s` *ARG*
: switch to layout group *ARG*

`-h`
: show the usage

`-v`
: show the version

# SHARED LIBRARY
Shared library **libg3kbswitch.so** can manage automatic switching of keyboard
layouts in Vim and Neovim when entering and leaving Insert mode via Vim plugin
*vim-xkbswitch*. Details are documented at
<https://github.com/lyokha/vim-xkbswitch>.

# AUTHOR
Written by Alexey Radkov.

# REPORTING BUGS
Report bugs to <https://github.com/lyokha/g3kb-switch/issues>.

