---
title: g3kb-switch
section: 1
header: User Manual
---

# NAME
**g3kb-switch** - a CLI keyboard layout switcher for Gnome 3 and 4x.

# SYNOPSIS
**g3kb-switch** [`-p` | `-l` | `-n` | `-s` *ARG*]

**g3kb-switch** [`-h` | `--help`]

**g3kb-switch** [`-v` | `--version`]

# DESCRIPTION
**g3kb-switch** implements direct D-Bus messaging with Gnome Shell to show,
get, and set system keyboard layouts. In Gnome 41 and newer, the switcher
requires assistance of Gnome extension *g3kb-switch* that ships with the
switcher. See details in https://github.com/lyokha/g3kb-switch#g3kb-switch.

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

