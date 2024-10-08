# g3kb-switch

[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/lyokha/g3kb-switch/master.svg)](https://results.pre-commit.ci/latest/github/lyokha/g3kb-switch/master)
[![github/workflow](https://github.com/lyokha/g3kb-switch/actions/workflows/main.yml/badge.svg)](https://github.com/lyokha/g3kb-switch/actions)

This is a CLI keyboard layout switcher for _Gnome 3_ and _4x_. It is not based
on the X interface but rather implements direct D-Bus messaging with the Gnome
Shell.

## Installation

<a href="https://repology.org/project/g3kb-switch/versions"><img align="right" src="https://repology.org/badge/vertical-allrepos/g3kb-switch.svg" alt="Packaging status"></a>

### Install

You can directly install this program by:

_In Arch Linux_, this program can be got from
[AUR](https://aur.archlinux.org/packages/g3kb-switch).

```ShellSession
paru -S g3kb-switch
```

_In Fedora_.

```ShellSession
sudo dnf install g3kb-switch
```

_In Nix / NixOS_, official repository have included it.

```ShellSession
nix-env -iA nixos.g3kb-switch
```

You can just run this program without installing it to your OS by

```ShellSession
nix run 'github#lyokha/g3kb-switch' -- --help
```

### Prerequisites

Build requires _glib-2.0_, so you need to install the corresponding development
package.

_In Ubuntu._

```ShellSession
sudo apt-get install libglib2.0-dev
```

_In Fedora / RHEL / CentOS._

```ShellSession
sudo dnf install glib2-devel
```

_In Gentoo._

```ShellSession
sudo emerge -av dev-libs/glib
```

Commands may differ in other Linux distributions.

### Gnome 45 and newer

Build and install the switcher.

```ShellSession
cmake -Bbuild
cmake --build build
sudo cmake --install build
```

In _Gnome 41_ and newer, the switcher will only work with _G3kbSwitch Gnome
Shell extension_, because method _org.gnome.Shell.Eval_ which was used in the
original implementation of the switcher is now disabled for security reasons.
Note that in _Gnome 41_ through _44_ the extension differs from that which is
required in _Gnome 45_, see build instructions for older Gnome versions in the
next section.

You can use [unsafe-mode-menu](https://github.com/linushdot/unsafe-mode-menu) to
toggle unsafe mode.

After installing the extension, restart Gnome Shell for changes to take effect.

It is wise to make sure that the extension is enabled and works correctly. To
enable the extension, use program _gnome-extensions-app_ or command-line program
_gnome-extensions_.

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

<!-- markdownlint-disable MD013 -->

```ShellSession
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.List
(true, '[{"key":"0","value":"ru"},{"key":"1","value":"us"}]')
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Get
(true, '1')
$ gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Set 1
(true, '')
```

<!-- markdownlint-enable MD013 -->

### Older versions of Gnome (41 through 44)

Older versions of Gnome Shell require a different implementation of the
extension. In this case, the cmake configuration step requires one extra option.

Build and install the switcher.

```ShellSession
cmake -Bbuild -DG3KBSWITCH_WITH_LEGACY_GNOME_SHELL_EXTENSION=ON
cmake --build build
sudo cmake --install build
```

### Even older versions of Gnome (3.x and 40)

Even older versions of Gnome Shell expose method _org.gnome.Shell.Eval_ which
means that the switcher can be built to work with this method rather than using
the extension. In this case, the cmake configuration step requires one extra
option.

Build and install the switcher.

```ShellSession
cmake -Bbuild -DG3KBSWITCH_WITH_GNOME_SHELL_EXTENSION=OFF
cmake --build build
sudo cmake --install build
```

### Package

After `cmake --build build`, run:

```ShellSession
cd build
# generate *.deb for debian/ubuntu
cpack -G DEB
# generate *.rpm for redhat
cpack -G RPM
```

## Usage

```ShellSession
Usage: g3kb-switch [-p]      Show the current layout group
       g3kb-switch -l        Show available layout groups
       g3kb-switch -n        Switch to the next layout group
       g3kb-switch -s ARG    Switch to layout group ARG
       g3kb-switch -h        Show this message and exit
       g3kb-switch -v        Show the program version and exit
```

## Integration with vim-xkbswitch

Basically, put in _.vimrc_ lines

```vim
let g:XkbSwitchEnabled = 1
let g:XkbSwitchLib = '/usr/local/lib64/libg3kbswitch.so'
```

Variable _g:XkbSwitchLib_ must contain the actual path to the installed library.
The path can be found with _pkg-config_.

```ShellSession
$ pkg-config g3kb-switch --variable libpath
/usr/local/lib64/libg3kbswitch.so
```

Notice that setting this variable is optional because vim-xkbswitch tries its
best to autodetect the path.

See details about configuration rules for vim-xkbswitch
[here](https://github.com/lyokha/vim-xkbswitch#basic-configuration).
