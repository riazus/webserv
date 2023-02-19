#include <stdio.h>

int main(int argc, char *argv, char **env)
{
    printf("<!DOCTYPE html>");
    for(int i = 0; env[i]; i++)
        printf(":<br><b>%s</b><p>", env[i]);
    printf("<html>");
    return 0;
}