# g3kb-switch

[![pre-commit.ci
status](https://results.pre-commit.ci/badge/github/lyokha/g3kb-switch/master.svg)](https://results.pre-commit.ci/latest/github/lyokha/g3kb-switch/master)
[![github/workflow](https://github.com/lyokha/g3kb-switch/actions/workflows/main.yml/badge.svg)](https://github.com/lyokha/g3kb-switch/actions)

This is a CLI keyboard layout switcher for *Gnome 3* and *4x*. It is not
based on the X interface but rather implements direct D-Bus messaging
with the Gnome Shell.

## Installation

### Install

You can directly install this program by:

*In Arch Linux*, this program can be got from
[AUR](https://aur.archlinux.org/packages/g3kb-switch).

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="paru -S g3kb-switch"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">paru -S g3kb-switch</span>
</pre>
</div>

*In Fedora*.

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="sudo dnf install g3kb-switch"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">sudo dnf install g3kb-switch</span>
</pre>
</div>

*In Nix / NixOS*, official repository have included it.

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="nix-env -iA nixos.g3kb-switch"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">nix-env -iA nixos.g3kb-switch</span>
</pre>
</div>

You can just run this program without installing it to your OS by

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="nix run &apos;github#lyokha/g3kb-switch&apos; -- --help"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">nix run &apos;github#lyokha/g3kb-switch&apos; -- --help</span>
</pre>
</div>

### Prerequisites

Build requires *glib-2.0*, so you need to install the corresponding
development package.

*In Ubuntu.*

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="sudo apt-get install libglib2.0-dev"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">sudo apt-get install libglib2.0-dev</span>
</pre>
</div>

*In Fedora / RHEL / CentOS.*

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="sudo dnf install glib2-devel"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">sudo dnf install glib2-devel</span>
</pre>
</div>

*In Gentoo.*

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="sudo emerge -av dev-libs/glib"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">sudo emerge -av dev-libs/glib</span>
</pre>
</div>

Commands may differ in other Linux distributions.

### Gnome 45 and newer

Build and install the switcher.

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="cmake -Bbuild
cmake --build build
sudo cmake --install build"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">cmake -Bbuild</span>
<span style="color: #005FAF;">cmake --build build</span>
<span style="color: #005FAF;">sudo cmake --install build</span>
</pre>
</div>

In *Gnome 41* and newer, the switcher will only work with *G3kbSwitch
Gnome Shell extension*, because method *org.gnome.Shell.Eval* which was
used in the original implementation of the switcher is now disabled for
security reasons. Note that in *Gnome 41* through *44* the extension
differs from that which is required in *Gnome 45*, see build
instructions for older Gnome versions in the next section.

You can use
[unsafe-mode-menu](https://github.com/linushdot/unsafe-mode-menu) to
toggle unsafe mode.

After installing the extension, restart Gnome Shell for changes to take
effect.

It is wise to make sure that the extension is enabled and works
correctly. To enable the extension, use program *gnome-extensions-app*
or command-line program *gnome-extensions*.

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="gnome-extensions enable g3kb-switch@g3kb-switch.org
gnome-extensions info g3kb-switch@g3kb-switch.org
g3kb-switch@g3kb-switch.org
  Name: G3kbSwitch
  Description: G3kbSwitch helper
  Path: /usr/local/share/gnome-shell/extensions/g3kb-switch@g3kb-switch.org
  URL: https://github.com/lyokha/g3kb-switch
  Version: 1
  State: ENABLED"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">gnome-extensions enable g3kb-switch@g3kb-switch.org</span>
<span style="color: #005FAF;">gnome-extensions info g3kb-switch@g3kb-switch.org</span>
<span style="color: #000000;">g3kb-switch@g3kb-switch.org</span>
<span style="color: #000000;">  Name: G3kbSwitch</span>
<span style="color: #000000;">  Description: G3kbSwitch helper</span>
<span style="color: #000000;">  Path: /usr/local/share/gnome-shell/extensions/g3kb-switch@g3kb-switch.org</span>
<span style="color: #000000;">  URL: https://github.com/lyokha/g3kb-switch</span>
<span style="color: #000000;">  Version: 1</span>
<span style="color: #000000;">  State: ENABLED</span>
</pre>
</div>

A small smoke-test of the extension.

<!-- markdownlint-disable MD013 -->
<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.List
(true, &apos;[{&quot;key&quot;:&quot;0&quot;,&quot;value&quot;:&quot;ru&quot;},{&quot;key&quot;:&quot;1&quot;,&quot;value&quot;:&quot;us&quot;}]&apos;)
gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Get
(true, &apos;1&apos;)
gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Set 1
(true, &apos;&apos;)"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.List</span>
<span style="color: #000000;">(true, &apos;[{&quot;key&quot;:&quot;0&quot;,&quot;value&quot;:&quot;ru&quot;},{&quot;key&quot;:&quot;1&quot;,&quot;value&quot;:&quot;us&quot;}]&apos;)</span>
<span style="color: #005FAF;">gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Get</span>
<span style="color: #000000;">(true, &apos;1&apos;)</span>
<span style="color: #005FAF;">gdbus call --session --dest org.gnome.Shell --object-path /org/g3kbswitch/G3kbSwitch --method org.g3kbswitch.G3kbSwitch.Set 1</span>
<span style="color: #000000;">(true, &apos;&apos;)</span>
</pre>
</div>
<!-- markdownlint-enable MD013 -->

### Older versions of Gnome (41 through 44)

Older versions of Gnome Shell require a different implementation of the
extension. In this case, the cmake configuration step requires one extra
option.

Build and install the switcher.

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="cmake -Bbuild -DG3KBSWITCH_WITH_LEGACY_GNOME_SHELL_EXTENSION=ON
cmake --build build
sudo cmake --install build"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">cmake -Bbuild -DG3KBSWITCH_WITH_LEGACY_GNOME_SHELL_EXTENSION=ON</span>
<span style="color: #005FAF;">cmake --build build</span>
<span style="color: #005FAF;">sudo cmake --install build</span>
</pre>
</div>

### Even older versions of Gnome (3.x and 40)

Even older versions of Gnome Shell expose method *org.gnome.Shell.Eval*
which means that the switcher can be built to work with this method
rather than using the extension. In this case, the cmake configuration
step requires one extra option.

Build and install the switcher.

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="cmake -Bbuild -DG3KBSWITCH_WITH_GNOME_SHELL_EXTENSION=OFF
cmake --build build
sudo cmake --install build"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">cmake -Bbuild -DG3KBSWITCH_WITH_GNOME_SHELL_EXTENSION=OFF</span>
<span style="color: #005FAF;">cmake --build build</span>
<span style="color: #005FAF;">sudo cmake --install build</span>
</pre>
</div>

### Package

After `cmake --build build`, run:

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="cd build
# generate *.deb for debian/ubuntu
cpack -G DEB
# generate *.rpm for redhat
cpack -G RPM"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">cd build</span>
<span style="color: #005FAF;"># generate *.deb for debian/ubuntu</span>
<span style="color: #005FAF;">cpack -G DEB</span>
<span style="color: #005FAF;"># generate *.rpm for redhat</span>
<span style="color: #005FAF;">cpack -G RPM</span>
</pre>
</div>

## Usage

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="Usage: g3kb-switch [-p]      Show the current layout group
       g3kb-switch -l        Show available layout groups
       g3kb-switch -n        Switch to the next layout group
       g3kb-switch -s ARG    Switch to layout group ARG
       g3kb-switch -h        Show this message and exit
       g3kb-switch -v        Show the program version and exit"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #000000;">Usage: g3kb-switch [-p]      Show the current layout group</span>
<span style="color: #000000;">       g3kb-switch -l        Show available layout groups</span>
<span style="color: #000000;">       g3kb-switch -n        Switch to the next layout group</span>
<span style="color: #000000;">       g3kb-switch -s ARG    Switch to layout group ARG</span>
<span style="color: #000000;">       g3kb-switch -h        Show this message and exit</span>
<span style="color: #000000;">       g3kb-switch -v        Show the program version and exit</span>
</pre>
</div>

## Integration with vim-xkbswitch

Basically, put in *.vimrc* lines

``` vim
let g:XkbSwitchEnabled = 1
let g:XkbSwitchLib = '/usr/local/lib64/libg3kbswitch.so'
```

Variable *g:XkbSwitchLib* must contain the actual path to the installed
library. The path can be found with *pkg-config*.

<div class="highlight notranslate position-relative overflow-auto" dir="auto" data-snippet-clipboard-copy-content="pkg-config g3kb-switch --variable libpath
/usr/local/lib64/libg3kbswitch.so"><pre style="overflow-x: auto; border-style: none none none solid; border-color: #6DF; border-width: 3px; padding: 5px 10px">
<span style="color: #005FAF;">pkg-config g3kb-switch --variable libpath</span>
<span style="color: #000000;">/usr/local/lib64/libg3kbswitch.so</span>
</pre>
</div>

Notice that setting this variable is optional because vim-xkbswitch
tries its best to autodetect the path.

See details about configuration rules for vim-xkbswitch
[here](https://github.com/lyokha/vim-xkbswitch#basic-configuration).
