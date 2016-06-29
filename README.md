## dde-session-ui
**Description**:This project include those sub-project:

- dde-shutdown
```
User interface of shutdown.
```
- dde-lock
```
User interface of lock screen.
```
- dde-lockservice
```
The back-end service of locking screen.
```
- lightdm-deepin-greeter
```
The user interface when you login in.
```
- dde-switchtogreeter
```
The tools to switch the user to login in.
```
- dde-lowpower
```
The user interface of reminding low power.
```
- dde-osd
```
User interface of on-screen display .
```
- dde-hotzone
```
User interface of setting hot zone.
```

## Dependencies
- Qt
- [deepin-tool-kit](https://github.com/linuxdeepin/deepin-tool-kit)
- gsettings-qt
- glib-2.0 gio-unix-2.0
- gtk+-2.0
- liblightdm-qt5-3
- xrandr
- x11 xext xfixes xcursor x11extras
- pam

## Installation
Those project's installation is all the same, so we just make a example.
A example to install lightdm-deepin-greeter:

- you need go to the directory of lightdm-deepin-greeter.pro, firstly;
- create a directory, go to the directory, and run `qmake .. && make`, secondly;
- run `sudo make install`, finally.
- if you want to make your installation work, you may be need to change some configure file
make sure that the lightdm-deepin-greeter is running, or may be it'will run the old one.
In the deepin operation system, you need to copy the file lightdm-deepin-greeter (which you get by run `make`) to the directory "/usr/local/bin".

## Usage
Those tools you get by the sub-project are system tools, you may use lightdm-deepin-greeter when you login, and you may use dde-shutdown
when you choose to shutdown you pc, and so on.

## Getting help
* [github](https://github.com/linuxdeepin/dde-session-ui)
* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](http://wiki.deepin.org/)

## Getting involved
We encourage you to report issues and contribute changes

* [Contribution guide for users](http://wiki.deepin.org/index.php?title=Contribution_Guidelines_for_Users)
* [Contribution guide for developers](http://wiki.deepin.org/index.php?title=Contribution_Guidelines_for_Developers).

## License
GNU General Public License, Version 3.0
