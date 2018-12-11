FILES = oskrabat

EXTRA_FILE = use_jpeg_buffer

VERSION = $(shell grep 'cimg_version\ '  CImg.h | tail -c4 | head -c3)
VERSION1 = $(shell grep 'cimg_version\ ' CImg.h | tail -c4 | head -c1)
VERSION2 = $(shell grep 'cimg_version\ ' CImg.h | tail -c3 | head -c1)
VERSION3 = $(shell grep 'cimg_version\ ' CImg.h | tail -c2 | head -c1)
SVERSION=$(VERSION1).$(VERSION2).$(VERSION3)

X11PATH = /usr/X11R6

EXE_PRE =
EXE_EXT =
ifeq ($(MSYSTEM),MINGW32)
EXE_EXT = .exe
endif
ifeq ($(MSYSTEM),MINGW64)
EXE_EXT = .exe
endif

ifeq ($(shell echo $(notdir $(CXX)) | head -c3),g++)
IS_GCC = 1
endif
ifeq ($(shell echo $(notdir $(CXX)) | head -c7),clang++)
IS_CLANG = 1
endif
ifeq ($(shell echo $(notdir $(CXX)) | head -c4),icpc)
IS_ICPC = 1
endif

CXXVER = $(CXX)
CFLAGS = -I.. -Wall -Wextra -Wfatal-errors
LIBS = -lm
ifdef IS_GCC
CXXVER = $(shell $(CXX) -v 2>&1 | tail -n 1)
endif
ifdef IS_CLANG
CXXVER = $(shell $(CXX) -v 2>&1 | head -n 1)
endif
ifdef IS_ICPC
CXXVER = $(shell $(CXX) -v 2>&1)
CFLAGS = -I..
LIBS =
endif

#--------------------------------------------------
# Set compilation flags allowing to customize CImg
#--------------------------------------------------

# Flags to enable strict code standards
ifeq ($(notdir $(CXX)),icpc)
ANSI_CFLAGS = -std=c++11
else
ANSI_CFLAGS = -std=c++11 -pedantic
endif

# Flags to enable code debugging.
DEBUG_CFLAGS = -Dcimg_verbosity=3 -Dcimg_strict_warnings -g -fsanitize=address

# Flags to enable color output messages.
# (requires a VT100 compatible terminal)
VT100_CFLAGS = -Dcimg_use_vt100

# Flags to enable code optimization by the compiler.
OPT_CFLAGS = -Ofast
ifdef IS_GCC
OPT_CFLAGS = -Ofast -mtune=generic
endif
ifdef IS_ICPC
OPT_CFLAGS = -fast
endif

# Flags to enable OpenMP support.
OPENMP_DEFINE = -Dcimg_use_openmp -fopenmp
OPENMP_INCDIR =
OPENMP_CFLAGS = $(OPENMP_DEFINE) $(OPENMP_INCDIR)
ifdef IS_ICPC
OPENMP_CFLAGS = #-Dcimg_use_openmp -openmp -i-static    # -> Seems to bug the compiler!
endif
ifdef IS_CLANG
OPENMP_CFLAGS =
endif

# Flags to enable OpenCV support.
OPENCV_DEFINE = -Dcimg_use_opencv
OPENCV_INCDIR = $(shell pkg-config opencv --cflags || echo -I/usr/include/opencv) -I/usr/include/opencv
OPENCV_CFLAGS = $(OPENCV_DEFINE) $(OPENCV_INCDIR)
OPENCV_LIBS = $(shell pkg-config opencv --libs || echo -lopencv_core -lopencv_highgui)

# Flags used to disable display capablities of CImg
NODISPLAY_CFLAGS = -Dcimg_display=0

# Flags to enable the use of the X11 library.
# (X11 is used by CImg to handle display windows)
X11_DEFINE = -Dcimg_display=1
X11_INCDIR = $(shell pkg-config --cflags x11 || echo -I/usr/X11R6/include)
X11_CFLAGS = $(X11_DEFINE) $(X11_INCDIR)
X11_LIBS = $(shell pkg-config --libs x11 || echo -L/usr/X11R6/lib -lX11) -lpthread

# Flags to enable fast image display, using the XSHM library (when using X11).
# !!! Seems to randomly crash when used on MacOSX and 64bits systems, so use it only when necessary !!!
XSHM_CFLAGS = # -Dcimg_use_xshm $(shell pkg-config --cflags xcb-shm)
XSHM_LIBS = # $(shell pkg-config --libs xcb-shm || echo -L$(USR)/X11R6/lib -lXext)

# Flags to enable GDI32 display (Windows native).
GDI32_DEFINE = -mwindows
GDI32_INCDIR =
GDI32_CFLAGS = $(GDI32_DEFINE) $(GDI32_INCDIR)
GDI32_LIBS = -lgdi32

# Flags to enable screen mode switching, using the XRandr library (when using X11).
# ( http://www.x.org/wiki/Projects/XRandR )
# !!! Not supported by the X11 server on MacOSX, so do not use it on MacOSX !!!
XRANDR_DEFINE = -Dcimg_use_xrandr
XRANDR_INCDIR =
XRANDR_CFLAGS = $(XRANDR_DEFINE) $(XRANDR_INCDIR)
XRANDR_LIBS = -lXrandr

# Flags to enable native support for PNG image files, using the PNG library.
# ( http://www.libpng.org/ )
PNG_DEFINE = -Dcimg_use_png
PNG_INCDIR =
PNG_CFLAGS = $(PNG_DEFINE) $(PNG_INCDIR)
PNG_LIBS = -lpng -lz

# Flags to enable native support for JPEG image files, using the JPEG library.
# ( http://www.ijg.org/ )
JPEG_DEFINE = -Dcimg_use_jpeg
JPEG_INCDIR =
JPEG_CFLAGS = $(JPEG_DEFINE) $(JPEG_INCDIR)
JPEG_LIBS = -ljpeg

# Flags to enable native support for TIFF image files, using the TIFF library.
# ( http://www.libtiff.org/ )
TIFF_DEFINE = -Dcimg_use_tiff
TIFF_INCDIR =
TIFF_CFLAGS = $(TIFF_DEFINE) $(TIFF_INCDIR)
TIFF_LIBS = -ltiff

# Flags to enable native support for MINC2 image files, using the MINC2 library.
# ( http://en.wikibooks.org/wiki/MINC/Reference/MINC2.0_Users_Guide )
MINC2_DEFINE = -Dcimg_use_minc2
MINC2_INCDIR = -I${HOME}/local/include
MINC2_CFLAGS = $(MINC2_DEFINE) $(MINC2_INCDIR)
MINC2_LIBS = -lminc_io -lvolume_io2 -lminc2 -lnetcdf -lhdf5 -lz -L${HOME}/local/lib

# Flags to enable native support for EXR image files, using the OpenEXR library.
# ( http://www.openexr.com/ )
OPENEXR_DEFINE = -Dcimg_use_openexr
OPENEXR_INCDIR = -I/usr/include/OpenEXR
OPENEXR_CFLAGS = $(OPENEXR_DEFINE) $(OPENEXR_INCDIR)
OPENEXR_LIBS = -lIlmImf -lHalf

# Flags to enable native support for various video files, using the FFMPEG library.
# ( http://www.ffmpeg.org/ )
FFMPEG_DEFINE = -Dcimg_use_ffmpeg -D__STDC_CONSTANT_MACROS
FFMPEG_INCDIR = -I/usr/include/libavcodec -I/usr/include/libavformat -I/usr/include/libswscale -I/usr/include/ffmpeg
FFMPEG_CFLAGS = $(FFMPEG_DEFINE) $(FFMPEG_INCDIR)
FFMPEG_LIBS = -lavcodec -lavformat -lswscale

# Flags to enable native support for compressed .cimgz files, using the Zlib library.
# ( http://www.zlib.net/ )
ZLIB_DEFINE = -Dcimg_use_zlib
ZLIB_INCDIR = $(shell pkg-config --cflags zlib || echo -I$(USR)/$(INCLUDE))
ZLIB_CFLAGS = $(ZLIB_DEFINE) $(ZLIB_INCDIR)
ZLIB_LIBS = $(shell pkg-config --libs zlib || echo -lz)

# Flags to enable native support for downloading files from the network.
# ( http://curl.haxx.se/libcurl/ )
CURL_DEFINE = -Dcimg_use_curl
CURL_INCDIR =
CURL_CFLAGS = $(CURL_DEFINE)
CURL_LIBS = -lcurl

# Flags to enable native support of most classical image file formats, using the Magick++ library.
# ( http://www.imagemagick.org/Magick++/ )
MAGICK_DEFINE = -Dcimg_use_magick
MAGICK_INCDIR = $(shell pkg-config --cflags GraphicsMagick++ || echo -I$(USR)/$(INCLUDE)/GraphicsMagick)
MAGICK_CFLAGS = $(MAGICK_DEFINE) $(MAGICK_INCDIR)
MAGICK_LIBS = $(shell pkg-config --libs GraphicsMagick++ || echo -lGraphicsMagick++)

# Flags to enable faster Discrete Fourier Transform computation, using the FFTW3 library
# ( http://www.fftw.org/ )
FFTW3_DEFINE = -Dcimg_use_fftw3
FFTW3_INCDIR =
FFTW3_CFLAGS = $(FFTW3_DEFINE) $(FFTW3_INCDIR)
ifeq ($(OSTYPE),msys)
FFTW3_LIBS = -lfftw3-3
else
FFTW3_LIBS = -lfftw3 -lfftw3_threads
endif

# Flags to enable the use of LAPACK routines for matrix computation
# ( http://www.netlib.org/lapack/ )
LAPACK_DEFINE = -Dcimg_use_lapack
LAPACK_INCDIR =
LAPACK_CFLAGS = $(LAPACK_DEFINE) $(LAPACK_INCDIR)
LAPACK_LIBS = -lblas -llapack

# Flags to enable the use of the Board library
# ( https://github.com/c-koi/libboard )
BOARD_DEFINE = -Dcimg_use_board
BOARD_INCDIR = -I/usr/include/board
BOARD_CFLAGS = $(BOARD_DEFINE) $(BOARD_INCDIR)
BOARD_LIBS = -lboard

# Flags to compile on Sun Solaris
SOLARIS_LIBS = -R$(X11PATH)/lib -lrt -lnsl -lsocket

# Flags to compile GIMP plug-ins.
ifeq ($(MSYSTEM),MINGW32)
GIMP_CFLAGS = -mwindows
endif

#-------------------------
# Define Makefile entries
#-------------------------
.cpp:
	@echo
	@echo "** Compiling '$* ($(SVERSION))' with '$(CXXVER)'"
	@echo
	$(CXX) -o $(EXE_PRE)$*$(EXE_EXT) $< $(CFLAGS) $(CONF_CFLAGS) $(LIBS) $(CONF_LIBS)
ifeq ($(STRIP_EXE),true)
	strip $(EXE_PRE)$*$(EXE_EXT)
endif
menu:
	@echo
	@echo "CImg Library $(SVERSION) : Examples"
	@echo "-----------------------------"
	@echo "  > linux    : Linux/BSD target, X11 display, optimizations disabled."
	@echo "  > dlinux   : Linux/BSD target, X11 display, debug mode."
	@echo "  > olinux   : Linux/BSD target, X11 display, optimizations enabled."
	@echo "  > mlinux   : Linus/BSD target, no display, minimal features, optimizations enabled."
	@echo "  > Mlinux   : Linux/BSD target, X11 display, maximal features, optimizations enabled."
	@echo
	@echo "  > solaris  : Sun Solaris target, X11 display, optimizations disabled."
	@echo "  > dsolaris : Sun Solaris target, X11 display, debug mode."
	@echo "  > osolaris : Sun Solaris target, X11 display, optimizations enabled."
	@echo "  > msolaris : Sun Solaris target, no display, minimal features, optimizations enabled."
	@echo "  > Msolaris : Sun Solaris target, X11 display, maximal features, optimizations enabled."
	@echo
	@echo "  > macosx   : MacOSX target, X11 display, optimizations disabled."
	@echo "  > dmacosx  : MacOSX target, X11 display, debug mode."
	@echo "  > omacosx  : MacOSX target, X11 display, optimizations enabled."
	@echo "  > mmacosx  : MacOSX target, no display, minimal features, optimizations enabled."
	@echo "  > Mmacosx  : MacOSX target, X11 display, maximal features, optimizations enabled."
	@echo
	@echo "  > windows  : Windows target, GDI32 display, optimizations disabled."
	@echo "  > dwindows : Windows target, GDI32 display, debug mode."
	@echo "  > owindows : Windows target, GDI32 display, optimizations enabled."
	@echo "  > mwindows : Windows target, no display, minimal features, optimizations enabled."
	@echo "  > Mwindows : Windows target, GDI32 display, maximal features, optimizations enabled."
	@echo
	@echo "  > clean    : Clean generated files."
	@echo
	@echo "Choose your option :"
	@read CHOICE; echo; $(MAKE) $$CHOICE; echo; echo "> Next time, you can bypass the menu by typing directly 'make $$CHOICE'"; echo;

all: $(FILES)

clean:
	rm -rf *.exe *.o *.obj *~ \#* $(FILES) $(EXTRA_FILES)
ifneq ($(EXE_PRE),)
	rm -f $(EXE_PRE)*
endif

# Custom user-defined target
custom:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(VT100_CFLAGS) \
$(TIFF_CFLAGS) \
$(X11_CFLAGS) \
$(LAPACK_CFLAGS) \
$(XSHM_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS) \
$(TIFF_LIBS) \
$(LAPACK_LIBS) \
$(XSHM_LIBS)" \
all $(EXTRA_FILES)

# Linux/BSD/Mac OSX targets, with X11 display.

#A target for Travis-CI
travis:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(FFTW3_CFLAGS) \
$(PNG_CFLAGS) \
$(JPEG_CFLAGS) \
$(ZLIB_CFLAGS) \
$(CURL_CFLAGS) \
$(XSHM_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS) \
$(FFTW3_LIBS) \
$(PNG_LIBS) \
$(JPEG_LIBS) \
$(ZLIB_LIBS) \
$(CURL_LIBS) \
$(XSHM_LIBS)" \
all

linux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS) \
$(PNG_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS) \
$(PNG_LIBS) \
$(XSHM_LIBS)" \
all

dlinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(DEBUG_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS) \
$(XSHM_LIBS)" \
all

olinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(OPT_CFLAGS) \
$(OPENMP_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS) \
$(PNG_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS) \
$(PNG_LIBS) \
$(XSHM_LIBS)" \
"STRIP_EXE=true" \
all

mlinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(NODISPLAY_CFLAGS) \
$(OPT_CFLAGS)" \
"STRIP_EXE=true" \
all

Mlinux:
	@$(MAKE) \
"CONF_CFLAGS = \
$(OPT_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS) \
$(XRANDR_CFLAGS) \
$(TIFF_CFLAGS) \
$(OPENEXR_CFLAGS) \
$(PNG_CFLAGS) \
$(JPEG_CFLAGS) \
$(ZLIB_CFLAGS) \
$(CURL_CFLAGS) \
$(OPENCV_CFLAGS) \
$(MAGICK_CFLAGS) \
$(FFTW3_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS) \
$(XSHM_LIBS) \
$(XRANDR_LIBS) \
$(TIFF_LIBS) -ltiffxx \
$(OPENEXR_LIBS) \
$(PNG_LIBS) \
$(JPEG_LIBS) \
$(ZLIB_LIBS) \
$(CURL_LIBS) \
$(OPENCV_LIBS) \
$(MAGICK_LIBS) \
$(FFTW3_LIBS)" \
"STRIP_EXE=true" \
all $(EXTRA_FILES)

# Sun Solaris targets, with X11 display.
solaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS)" \
"CONF_LIBS = \
$(SOLARIS_LIBS) \
$(X11_LIBS) \
$(XSHM_LIBS)" \
all

dsolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(DEBUG_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS)" \
"CONF_LIBS = \
$(SOLARIS_LIBS) \
$(X11_LIBS) \
$(XSHM_LIBS)" \
all

osolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(OPT_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS)" \
"CONF_LIBS = \
$(SOLARIS_LIBS) \
$(X11_LIBS) \
$(XSHM_LIBS)" \
"STRIP_EXE=true" \
all

msolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(NODISPLAY_CFLAGS) \
$(OPT_CFLAGS)" \
"STRIP_EXE=true" \
all

Msolaris:
	@$(MAKE) \
"CONF_CFLAGS = \
$(OPT_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(XSHM_CFLAGS) \
$(XRANDR_CFLAGS) \
$(TIFF_CFLAGS) \
$(MINC2_CFLAGS) \
$(OPENEXR_CFLAGS) \
$(PNG_CFLAGS) \
$(JPEG_CFLAGS) \
$(ZLIB_CFLAGS) \
$(OPENCV_CFLAGS) \
$(MAGICK_CFLAGS) \
$(FFTW3_CFLAGS)" \
"CONF_LIBS = \
$(SOLARIS_LIBS) \
$(X11_LIBS) \
$(XSHM_LIBS) \
$(XRANDR_LIBS) \
$(TIFF_LIBS) \
$(MINC2_LIBS) \
$(OPENEXR_LIBS) \
$(PNG_LIBS) \
$(JPEG_LIBS) \
$(ZLIB_LIBS) \
$(OPENCV_LIBS) \
$(MAGICK_LIBS) \
$(FFTW3_LIBS)" \
"STRIP_EXE=true" \
all $(EXTRA_FILES)

# MacOsX targets, with X11 display.
macosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS)" \
all

dmacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(DEBUG_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS)" \
all

omacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(OPT_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS)" \
all

mmacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(ANSI_CFLAGS) \
$(NODISPLAY_CFLAGS) \
$(OPT_CFLAGS)" \
all

Mmacosx:
	@$(MAKE) \
"CONF_CFLAGS = \
$(OPT_CFLAGS) \
$(VT100_CFLAGS) \
$(X11_CFLAGS) \
$(TIFF_CFLAGS) \
$(MINC2_CFLAGS) \
$(OPENEXR_CFLAGS) \
$(PNG_CFLAGS) \
$(JPEG_CFLAGS) \
$(ZLIB_CFLAGS) \
$(OPENCV_CFLAGS) \
$(MAGICK_CFLAGS) \
$(FFTW3_CFLAGS)" \
"CONF_LIBS = \
$(X11_LIBS) \
$(TIFF_LIBS) \
$(MINC2_LIBS) \
$(OPENEXR_LIBS) \
$(PNG_LIBS) \
$(JPEG_LIBS) \
$(ZLIB_LIBS) \
$(OPENCV_LIBS) \
$(MAGICK_LIBS) \
$(FFTW3_LIBS)" \
all $(EXTRA_FILES)

# Windows targets, with GDI32 display.
windows:
	@$(MAKE) \
"CONF_CFLAGS = " \
"CONF_LIBS = \
$(GDI32_LIBS)" \
all

dwindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(DEBUG_CFLAGS)" \
"CONF_LIBS = \
$(GDI32_LIBS)" \
all

owindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(OPT_CFLAGS)" \
"CONF_LIBS = \
$(GDI32_LIBS)" \
"STRIP_EXE=true" \
all

mwindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(NODISPLAY_CFLAGS) \
$(OPT_CFLAGS)" \
"STRIP_EXE=true" \
all

Mwindows:
	@$(MAKE) \
"CONF_CFLAGS = \
$(OPT_CFLAGS) \
$(TIFF_CFLAGS) \
$(PNG_CFLAGS) \
$(JPEG_CFLAGS) \
$(ZLIB_CFLAGS) \
$(OPENCV_CFLAGS) \
$(FFTW3_CFLAGS)" \
"CONF_LIBS = \
$(GDI32_LIBS) \
$(TIFF_LIBS) \
$(PNG_LIBS) \
$(JPEG_LIBS) \
$(ZLIB_LIBS) \
$(OPENCV_LIBS) \
$(FFTW3_LIBS)" \
"STRIP_EXE=true" \
all $(EXTRA_FILES)

# End of makefile
