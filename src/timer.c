#include <unistd.h>

void delay_ms(int ms)
{
    usleep(ms * 1000);
}
