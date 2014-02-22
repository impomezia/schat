Name:		schat2
Version:	##SCHAT_VERSION##
Release:	0
Summary:	Simple Chat
Group:		Productivity/Networking/Talk/Clients
License:	GPL-3.0
URL:		https://schat.me/
Source0:	%{name}-%{version}.tar.bz2
Source1: 	schatd2.service
Source2: 	schat-authd.service
BuildRequires:	gcc-c++ hicolor-icon-theme pkgconfig(QtGui) pkgconfig(QtWebKit) pkgconfig(xscrnsaver) pkgconfig(hunspell)
%if 0%{?fedora}
BuildRequires:	GeoIP-devel
%endif
%if 0%{?suse_version}
BuildRequires:	update-desktop-files libGeoIP-devel
%endif
%if 0%{?suse_version} < 1230
BuildRequires: systemd 
%endif
Requires:	hicolor-icon-theme
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-build

%package	-n schatd2
Summary:	Simple Chat server
Group:		Productivity/Networking/Talk/Servers

%description
Simple and powerful cross-platform chat for local networks and the Internet.

%description	-n schatd2
Simple and powerful cross-platform server for local networks and the Internet.

%prep
%setup -q

%build
`pkg-config --variable=exec_prefix QtCore`/bin/qmake PREFIX=/usr LIBDIR=%{_libdir} GEOIP=1
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%{makeinstall} INSTALL_ROOT=%{buildroot}
rm -f %{buildroot}/%{_libdir}/lib*.so
%if 0%{?suse_version}
%suse_update_desktop_file %{name}
%endif

install -D -p -m 0644 %{SOURCE1} %{buildroot}%{_unitdir}/schatd2.service
install -D -p -m 0644 %{SOURCE2} %{buildroot}%{_unitdir}/schat-authd.service

mkdir -p %{buildroot}%{_sysconfdir}/schatd2
mkdir -p %{buildroot}%{_var}/lib/schatd2
mkdir -p %{buildroot}%{_var}/log/schatd2

%clean
rm -rf %{buildroot}

%pre -n schatd2
/usr/sbin/useradd -s /sbin/nologin -M -r -d %{_datadir}/%{name} \
    -c 'Simple Chat Daemon' schatd2 &>/dev/null || :

%post
ldconfig
%if 0%{?suse_version}
%desktop_database_post
%icon_theme_cache_post
%endif

%post	-n schatd2
ldconfig
if [ $1 -eq 1 ] ; then 
    /bin/systemctl daemon-reload &>/dev/null || :
fi

%preun	-n schatd2
if [ $1 -eq 0 ]; then
    /bin/systemctl --no-reload disable schatd2.service &>/dev/null || :
    /bin/systemctl stop schatd2.service &>/dev/null || :
fi

%postun
ldconfig
%if 0%{?suse_version}
%desktop_database_postun
%icon_theme_cache_postun
%endif

%postun	-n schatd2
ldconfig
/bin/systemctl daemon-reload &>/dev/null || :
if [ $1 -ge 1 ]; then
    /bin/systemctl try-restart schatd2.service &>/dev/null || :
fi

%files
%defattr(-,root,root,-)
%doc README COPYING
%{_bindir}/*
%{_libdir}/libschat-client.so.*
%{_libdir}/libschat.so.*
%{_libdir}/%{name}/
%{_datadir}/%{name}/
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png

%files	-n schatd2
%defattr(-,root,root,-)
%{_sbindir}/*
%{_libdir}/libschat-rest.so.*
%{_libdir}/libschat-tufao.so.*
%{_libdir}/libschatd.so.*
%{_libdir}/schatd2/
%{_datadir}/schatd2/
%{_unitdir}/schatd2.service
%{_unitdir}/schat-authd.service
%attr(0750, schatd2, schatd2) %{_sysconfdir}/schatd2
%attr(0750, schatd2, schatd2) %{_var}/lib/schatd2
%attr(0750, schatd2, schatd2) %{_var}/log/schatd2

%changelog
* Wed Feb 20 2013 ti.eugene@gmail.com 2.0.0
- Initial build
