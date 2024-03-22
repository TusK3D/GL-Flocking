#configuration
CONFIG +=  def_files_disabled exceptions no_mocdepend release stl qt_no_framework
QT_ARCH = x86_64
QT_EDITION = OpenSource
QT_CONFIG +=  minimal-config small-config medium-config large-config full-config qt3support phonon phonon-backend accessibility opengl reduce_exports ipv6 clock-gettime clock-monotonic mremap getaddrinfo ipv6ifname getifaddrs inotify system-jpeg system-png png system-tiff system-freetype system-zlib nis cups iconv glib gstreamer dbus openssl x11sm xshape xvideo xsync xrender mitshm fontconfig xkb alsa pulseaudio concurrent xmlpatterns multimedia audio-backend svg script scripttools declarative release

#versioning
QT_VERSION = 4.8.1
QT_MAJOR_VERSION = 4
QT_MINOR_VERSION = 8
QT_PATCH_VERSION = 1

#namespaces
QT_LIBINFIX = 
QT_NAMESPACE = 
QT_NAMESPACE_MAC_CRC = 

QMAKE_RPATHDIR += "/opt/QtSDK/Desktop/Qt/4.8.1/gcc/lib"
QT_GCC_MAJOR_VERSION = 4
QT_GCC_MINOR_VERSION = 4
QT_GCC_PATCH_VERSION = 5
