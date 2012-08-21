Name:       libcamsrcjpegenc
Summary:    camerasrc JPEG encoder Development library
Version:    0.1.4
Release:    1
Group:      libdevel
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(glib-2.0)

%description
Camerasrc JPEG encoder Development library.

%package devel
Summary:   camerasrc JPEG encoder Development library
Group:     devel
Requires:   %{name} = %{version}-%{release}

%description devel
camerasrc JPEG encoder Development library

%prep
%setup -q -n %{name}-%{version}

%build
./autogen.sh
%configure --disable-static --enable-dlog
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%defattr(-,root,root,-)
%{_libdir}/libcamsrcjpegenc.so.*

%files devel
%defattr(-,root,root,-)
%{_libdir}/libcamsrcjpegenc.so
%{_libdir}/pkgconfig/camsrcjpegenc.pc
%{_includedir}/camsrcjpegenc.h
%{_includedir}/camsrcjpegenc_sub.h
%{_includedir}/camsrcjpegenc_type.h

