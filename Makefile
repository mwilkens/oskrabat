DEPS = sprites.h
OBJ = character.o meat.o bonewand.o sprites.o oskrabat.o

build_os ?= LINUX

ifeq ($(build_os), WIN64)
	RAYLIB_FLAGS = -Lraylib-5.5_win64_mingw-w64/lib -l:libraylibdll.a -Iraylib-5.5_win64_mingw-w64/include
else ifeq ($(build_os), MAC)
	RAYLIB_FLAGS = -Lraylib-5.5_macos/lib -l:libraylib.a -Iraylib-5.5_macos/include
else
	RAYLIB_FLAGS = -Lraylib-5.5_linux_amd64/lib -l:libraylib.a -Iraylib-5.5_linux_amd64/include
endif

CFLAGS = -I.. -Wall -Wextra -std=c11 -static
LIBS = -lm

#-------------------------
# Define Makefile entries
#-------------------------
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(RAYLIB_FLAGS) $(CFLAGS) $(LIBS)

oskrabat: $(OBJ)
	$(CC) -o $@ $^ $(RAYLIB_FLAGS) $(CFLAGS) $(LIBS)

clean:
	rm -rf *.exe *.o *.obj *~ \#* $(FILES) $(EXTRA_FILES)
ifneq ($(EXE_PRE),)
	rm -f $(EXE_PRE)*
endif
