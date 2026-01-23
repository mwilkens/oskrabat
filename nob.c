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

    for( int i = 0; i < ARRAY_LEN(include); i++)    
    {    
        char * buff = NULL;    
        const char * fmt = "-I%s";
        size_t bufbuff = snprintf(buff, 0, fmt, include[i]) + 1;
        assert(bufbuff > 0);    
    
        buff = (char*)malloc( bufbuff );    
        if( snprintf( buff, bufbuff, fmt, include[i] ) <= 0) return 1;
    
        nob_cmd_append(&cmd, buff);    
    }    
    
    for( int i = 0; i < ARRAY_LEN(lib); i++)    
    {    
        char * buff = NULL;
        const char * fmt = "-L%s";
        size_t buff_s = snprintf(buff, 0, fmt, lib[i]) + 1;
        assert(buff_s > 0);    
    
        buff = (char*)malloc( buff_s );    
        if( snprintf( buff, buff_s, fmt, lib[i] ) <= 0) return 1;
    
        nob_cmd_append(&cmd, buff);    
    }    
    
    for( int i = 0; i < ARRAY_LEN(libraries); i++)    
    {    
        char * buff = NULL;    
        const char * fmt = "-l%s";
        size_t buff_s = snprintf(buff, 0, fmt, libraries[i]) + 1;
        assert(buff_s > 0);   
   
        buff = (char*)malloc( buff_s );   
        if( snprintf( buff, buff_s, fmt, libraries[i] ) <= 0) return 1;
  
        nob_cmd_append(&cmd, buff);  
    }
 
    nob_cc_flags(&cmd);
    //nob_cmd_append(&cmd, "-static");

    if(!nob_cmd_run(&cmd)) return 1;
    return 0;
}
