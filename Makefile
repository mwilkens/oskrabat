DEPS = sprites.h
OBJ = character.o meat.o bonewand.o sprites.o oskrabat.o

RAYLIB_FLAGS = -Lraylib-5.5_linux_amd64/lib -l:libraylib.a -Iraylib-5.5_linux_amd64/include

CFLAGS = -I.. -Wall -Wextra -std=c11
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
