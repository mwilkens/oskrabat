#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define NOB_IMPLEMENTATION

#ifdef __WIN32
#define RAYLIB_DIR "raylib-5.5_win64_mingw-win64"  
#define RAYLIB_LIB ":libraylibdll.a"
#else
#ifdef __MACOS
#define RAYLIB_DIR "raylib-5.5_macos"  
#define RAYLIB_LIB ":libraylib.a"
#else // LINUX
#define RAYLIB_DIR "raylib-5.5_linux_amd64"  
#define RAYLIB_LIB ":libraylib.a"
#endif
#endif

#include "nob.h"

bool nob_push_cmds(Nob_Cmd * cmd, const char ** arr, size_t arr_size, char * fmt)
{
    for( int i = 0; i < arr_size; i++)    
    {    
        char * buff = NULL;    
        size_t buff_s = snprintf(buff, 0, fmt, arr[i]) + 1;
        assert(buff_s > 0);    
    
        buff = (char*)malloc( buff_s );    
        if( snprintf( buff, buff_s, fmt, arr[i] ) <= 0) return false;
    
        nob_cmd_append(cmd, buff);
    }
    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char * include[] = {
        RAYLIB_DIR"/include",
        ".."
    };

    const char * lib[] = {
        RAYLIB_DIR"/lib",
    };

    const char * libraries[] = {
        RAYLIB_LIB,
        "m"
    };

    const char * inputs[] = {
        "oskrabat.c",
        "sprites.c",
        "bonewand.c",
        "character.c",
        "meat.c" 
    };  
  
    Nob_Cmd cmd = {0};  
   
    nob_cc(&cmd);    

    nob_cc_output(&cmd, "oskrabat"); 

    for( int i = 0; i < ARRAY_LEN(inputs); i++)    
    {    
        nob_cmd_append(&cmd, inputs[i]);  
    }

    nob_push_cmds(&cmd, include, ARRAY_LEN(include), "-I%s");
    nob_push_cmds(&cmd, lib, ARRAY_LEN(lib), "-L%s");
    nob_push_cmds(&cmd, libraries, ARRAY_LEN(libraries), "-l%s");
 
    nob_cc_flags(&cmd);
    //nob_cmd_append(&cmd, "-static");
    //nob_cmd_append(&cmd, "-O3");
    //nob_cmd_append(&cmd, "-g");

    if(!nob_cmd_run(&cmd)) return 1;
    return 0;
}
