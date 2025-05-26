#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "keyboard.h"

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void keyboardInit()
{
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void keyboardDestroy()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

int keyhit()
{
    unsigned char ch;
    int nread;

    if (peek_character != -1)
        return 1;

    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    nread = read(0, &ch, 1);

    fcntl(0, F_SETFL, flags);

    if (nread == 1)
    {
        peek_character = ch;
        return 1;
    }

    return 0;
}

int readch()
{
    char ch;

    if (peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }

    read(0, &ch, 1);
    return ch;
}
