Summary:   Library of svlock interfaces
Name: svlock
Version: 1.0
Release: 1
License:   Intel Confidential
Group:     System Environment/Libraries
Vendor:    Intel Corp
Packager: Kevin Beadle <kevin.beadle@intel.com>
Source:    %{name}-%{version}.tar.gz
Requires:  libxml2
BuildRequires: libxml2-devel, autoconf, automake, libtool, kernel-devel, kernel-headers, python3-devel
%if 0%{?suse_version}
BuildRequires: kernel-default-devel
%endif

%package   utils
Provides:  svlock-utils
Summary:   Utilities for svlock
Group:     Development/Libraries
Requires:  %{name} = %{version}

%package   devel
Provides:  svlock-devel
Summary:   Header files and static libraries for svlock
Group:     Development/Libraries
Requires:  %{name} = %{version}, libxml2-devel

%package   unit-tests
Provides:  svlock-unit-tests
Summary:   Tests apps for svlock
Group:     Development/Libraries
Requires:  %{name} = %{version}

%package -n python3-%{name}
Provides:  python3-svlock
Summary:   Python bindings for svlock
Group:     Development/Libraries
Requires:  %{name} = %{version}

%package   doc
Provides:  svlock-doc
Summary:   Documentation for svlock
Group:     Development/Libraries

%description 
Library of svlock interfaces

%description utils
Utilities for svlock

%description devel
Header files and static libraries for svlock

%description unit-tests
Test apps for svlock

%description -n python3-%{name}
Python bindings for svlock

%description doc
Documentation for svlock

%prep
%setup -q
libtoolize -c -f
autoreconf -f -i

%build

PARAMS=""
./configure --prefix=/usr \
    --libdir=%{_libdir} \
    --mandir=\$${prefix}/share/man \
    --infodir=\$${prefix}/share/info
make
make check

%install
make DESTDIR=$RPM_BUILD_ROOT install

# Remove example programs
#rm -f $RPM_BUILD_ROOT/usr/bin/simple

%clean
rm -fr $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
#%doc COPYING.LIB COPYING.GPL LICENSE
%{_libdir}/libsvlock*.so.*

%files utils
%defattr(-,root,root)
%{_bindir}/svlock-query
%{_bindir}/svlock-cpuid
%{_bindir}/svlock-trigger
%{_bindir}/svlock-register-target-blocks
%{_bindir}/svlock-supports-atomics
%{_bindir}/svlock-reg


%files devel
%defattr(-,root,root)
%{_bindir}/svlock-config
%{_prefix}/include/svlock/*.h
%{_prefix}/include/svlock/*/*.h
%{_libdir}/libsvlock*.*a
%{_libdir}/libsvlock*.so
%{_libdir}/pkgconfig/*.pc

%files unit-tests
%defattr(-,root,root)
%{_bindir}/osv-ut/*

%files -n python3-%{name}
%defattr(-,root,root)
%if "%{?_vendor}" == "clr"
/usr/lib/python*/site-packages/*
%else
%{_libdir}/python*/site-packages/*
%endif

%files doc
%defattr(-,root,root)
/usr/share/doc
