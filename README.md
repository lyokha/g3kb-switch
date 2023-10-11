g3kb-switch
===========

This is a CLI keyboard layout switcher for *Gnome 3* and *4x*. It is not based
on the X interface but rather implements direct D-Bus messaging with the Gnome
Shell.

Installation
------------

### Install

You can directly install this program by:

*In Arch Linux*, this program can be got from
[AUR](https://aur.archlinux.org/packages/g3kb-switch).

```ShellSession
$ yay -S g3kb-switch
```

*In Fedora*.

```ShellSession
$ sudo dnf install g3kb-switch
```

*In Nix / NixOS*, official repository have included it.

```ShellSession
$ nix-env -iA nixos.g3kb-switch
```

You can just run this program without installing it to your OS by

```ShellSession
$ nix run 'github#lyokha/g3kb-switch' -- --help
```

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

### Gnome 45 and newer

Build and install the switcher.

```ShellSession
$ cmake -Bbuild
$ cmake --build build
$ sudo cmake --install build
```

In *Gnome 41* and newer, the switcher will only work with *G3kbSwitch Gnome
Shell extension*, because method *org.gnome.Shell.Eval* which was used in the
original implementation of the switcher is now disabled for security reasons.

After installing the extension, restart Gnome Shell for changes to take effect.

It is wise to make sure that the extension is enabled and works correctly. To
enable the extension, use program *gnome-extensions-app* or command-line program
*gnome-extensions*.

```ShellSession
$ gnome-extensions enable g3kb-switch@g3kb-switch.org
$ gnome-extensions info g3kb-switch@g3kb-switch.org
g3kb-switch@g3kb-switch.org
  Name: G3kbSwitch
  Description: G3kbSwitch helper
  Path: /usr/local/share/gnome-shell/extensions/g3kb-switch@g3kb-switch.org
  URL: https://github.com/lyokha/g3kb-switch
  Version: 1
  State: ENABLED
```

A small smoke-test of the extension.

```ShellSession
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.List
(true, '[{"key":"0","value":"ru"},{"key":"1","value":"us"}]')
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Get
(true, '1')
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Set 1
(true, '')
```

### Older versions of Gnome (41 through 44)

Older versions of Gnome Shell require a different implementation of the
extension. In this case, the cmake configuration step requires one extra option.

Build and install the switcher.

```ShellSession
$ cmake -Bbuild -DG3KBSWITCH_WITH_LEGACY_GNOME_SHELL_EXTENSION=ON
$ cmake --build build
$ sudo cmake --install build
```

### Even older versions of Gnome (3.x and 40)

Even older versions of Gnome Shell expose method *org.gnome.Shell.Eval* which
means that the switcher can be built to work with this method rather than using
the extension. In this case, the cmake configuration step requires one extra
option.

Build and install the switcher.

```ShellSession
$ cmake -Bbuild -DG3KBSWITCH_WITH_GNOME_SHELL_EXTENSION=OFF
$ cmake --build build
$ sudo cmake --install build
```

### Package

After `cmake --build build`, run:

```ShellSession
$ cd build
# generate *.deb for debian/ubuntu
$ cpack -G DEB
# generate *.rpm for redhat
$ cpack -G RPM
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
let g:XkbSwitchLib = '/usr/local/lib64/libg3kbswitch.so'
```

Variable *g:XkbSwitchLib* must contain the actual path to the installed library.
The path can be found with *pkg-config*.

```ShellSession
$ pkg-config g3kb-switch --variable libpath
/usr/local/lib64/libg3kbswitch.so
```

Notice that setting this variable is optional because vim-xkbswitch tries its
best to autodetect the path. 

See details about configuration rules for vim-xkbswitch
[here](https://github.com/lyokha/vim-xkbswitch#basic-configuration).

