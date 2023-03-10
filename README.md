g3kb-switch
===========

This is a CLI keyboard layout switcher for *Gnome 3* and *4x*. It is not based
on the X interface but rather implements direct D-Bus messaging with the Gnome
Shell.

Installation
------------

### Prerequisites

Build requires *glib-2.0*, so you need to install the corresponding development
package.

*In Ubuntu.*

```ShellSession
$ sudo apt-get install libglib2.0-dev
```

*In Fedora / RHEL / CentOS.*

```ShellSession
$ sudo dnf install glib2-devel
```

*In Gentoo.*

```ShellSession
$ sudo emerge -av dev-libs/glib
```

Commands may differ in other Linux distributions.

### Gnome 41 and newer

Build and install the switcher.

```ShellSession
$ mkdir build && cd build
$ cmake ..
$ make
$ sudo make install
```

In *Gnome 41* and newer, the switcher will only work with *G3kbSwitch Gnome
Shell extension*, because method *org.gnome.Shell.Eval* which was used in the
original implementation of the switcher is now disabled for security reasons.

Build and install the extension.

```ShellSession
$ cd ../extension
$ make install  # no sudo required!
```

It is wise to make sure that the extension is enabled and works correctly. To
enable the extension, use program *gnome-extensions-app* or command-line program
*gnome-extensions*.

A small smoke-test of the extension.

```ShellSession
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.List
(true, '[{"key":"0","value":"ru"},{"key":"1","value":"us"}]')
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Get
(true, '1')
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Set 1
(true, '')
```

### Older versions of Gnome (3.x and 40)

Older versions of Gnome Shell expose method *org.gnome.Shell.Eval* which means
that the switcher can be built to work with this method rather than using the
extension. In this case, the cmake configuration step requires one extra option.

Build and install the switcher.

```ShellSession
$ mkdir build && cd build
$ cmake -DG3KBSWITCH_WITH_GNOME_SHELL_EXTENSION=OFF ..
$ make
$ sudo make install
```

Usage
-----

```ShellSession
Usage: g3kb-switch [-p]      Show the current layout group
       g3kb-switch -l        Show available layout groups
       g3kb-switch -n        Switch to the next layout group
       g3kb-switch -s ARG    Switch to layout group ARG
       g3kb-switch -h        Show this message and exit
       g3kb-switch -v        Show the program version and exit
```

Integration with vim-xkbswitch
------------------------------

Basically, put in *.vimrc* lines

```vim
let g:XkbSwitchEnabled = 1
let g:XkbSwitchLib = '/usr/local/lib/libg3kbswitch.so'
```

See details about configuration rules for vim-xkbswitch
[here](https://github.com/lyokha/vim-xkbswitch#basic-configuration).

