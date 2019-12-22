g3kb-switch
===========

This is a keyboard layout switcher for Gnome 3. It is not based on the X
interface but rather implements direct D-Bus messaging with the Gnome Shell.

Installation
------------

Build requires *glib-2.0*, so you need to install the corresponding development
package.

*In Ubuntu.*

```ShellSession
$ sudo apt-get install libglib2.0-dev
```

*In Fedora / RHEL / CentOS.*

```ShellSession
# dnf install glib2-devel
```

Commands may differ in other Linux distributions.

Now build the program as a regular user.

```ShellSession
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

Install being a superuser.

```ShellSession
# make install
```

Usage
-----

```ShellSession
Usage: g3kb-switch [-p]      Show the current layout group
       g3kb-switch -l        Show available layout groups
       g3kb-switch -n        Switch to the next layout group
       g3kb-switch -s ARG    Switch to layout group ARG
       g3kb-switch -h        Show this message and exit
       g3kb-switch -v        Show program version and exit
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

