%global repo dde-session-ui
%global __provides_exclude_from ^%{_libdir}/dde-dock/.*\\.so$

%if 0%{?fedora}
Name:           deepin-session-ui
%else
Name:           %{repo}
%endif
Version:        5.4.8
Release:        1%{?fedora:%dist}
Summary:        Deepin desktop-environment - Session UI module
License:        GPLv3
URL:            https://github.com/linuxdeepin/%{repo}
Source0:        %{url}/archive/%{version}/%{repo}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  deepin-gettext-tools
BuildRequires:  pkgconfig(dtkwidget) >= 5.1
BuildRequires:  pkgconfig(dframeworkdbus)
BuildRequires:  pkgconfig(dde-dock)
BuildRequires:  pkgconfig(gsettings-qt)
BuildRequires:  pkgconfig(gtk+-2.0)
BuildRequires:  pkgconfig(libsystemd)
BuildRequires:  pkgconfig(xcb-ewmh)
BuildRequires:  pkgconfig(xcursor)
BuildRequires:  pkgconfig(xtst)
BuildRequires:  pkgconfig(xext)
BuildRequires:  golang-github-msteinert-pam-devel
BuildRequires:  dtkcore-devel >= 5.1
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5X11Extras)
BuildRequires:  pkgconfig(Qt5Multimedia)
%if 0%{?fedora}
Requires:       deepin-daemon
%else
Requires:       dde-daemon
%endif
Requires:       startdde

Provides:       deepin-notifications = %{version}-%{release}
Obsoletes:      deepin-notifications <= 3.3.4

%description
This project include those sub-project:

- dde-shutdown: User interface of shutdown.
- dde-lock: User interface of lock screen.
- dde-lockservice: The back-end service of locking screen.
- lightdm-deepin-greeter: The user interface when you login in.
- dde-switchtogreeter: The tools to switch the user to login in.
- dde-lowpower: The user interface of reminding low power.
- dde-osd: User interface of on-screen display.
- dde-hotzone: User interface of setting hot zone.

%prep
%autosetup -p1 -n %{repo}-%{version}
sed -i 's|lib|libexec|' \
    misc/applications/deepin-toggle-desktop.desktop* \
    dde-osd/dde-osd_autostart.desktop \
    dde-osd/com.deepin.dde.osd.service \
    dde-osd/notification/files/com.deepin.dde.*.service* \
    dde-osd/dde-osd.pro \
    dde-welcome/com.deepin.dde.welcome.service \
    dde-welcome/dde-welcome.pro \
    dde-bluetooth-dialog/dde-bluetooth-dialog.pro \
    dde-touchscreen-dialog/dde-touchscreen-dialog.pro \
    dde-warning-dialog/com.deepin.dde.WarningDialog.service \
    dde-warning-dialog/dde-warning-dialog.pro \
    dde-offline-upgrader/dde-offline-upgrader.pro \
    dde-suspend-dialog/dde-suspend-dialog.pro \
    dnetwork-secret-dialog/dnetwork-secret-dialog.pro \
    dde-lowpower/dde-lowpower.pro
sed -i 's|/usr/lib/dde-dock|%{_libdir}/dde-dock|' dde-notification-plugin/notifications/notifications.pro

%build
export PATH=%{_qt5_bindir}:$PATH
%qmake_qt5 PREFIX=%{_prefix} PKGTYPE=rpm
%make_build

%install
%make_install INSTALL_ROOT=%{buildroot}

%files
%doc README.md
%license LICENSE
%{_bindir}/dde-*
%{_bindir}/dmemory-warning-dialog
%{_libexecdir}/deepin-daemon/*
%{_datadir}/%{repo}/
%{_datadir}/icons/hicolor/*/apps/*
%{_datadir}/dbus-1/services/*.service
%{_libdir}/dde-dock/plugins/libnotifications.so
%{_prefix}/share/glib-2.0/schemas/com.deepin.dde.dock.module.notifications.gschema.xml

%changelog
* Wed Mar 31 2021 uoser <uoser@uniontech.com> - 5.4.8-1
- Update to 5.4.8
