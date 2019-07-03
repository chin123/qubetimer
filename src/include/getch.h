#if defined (__linux__) || defined (__APPLE__) || defined (__unix)

#include <unistd.h>
#include <termios.h>

#define CLEAR "clear"

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

#elif _WIN32

#include <conio.h>

#define CLEAR "cls"

#else

#error "Unsupported OS!"

#endif
