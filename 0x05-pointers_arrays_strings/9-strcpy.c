#include "main.h"

/**
 * _strcpy - Copy a string
 * @dest: Destination value
 * @src: Source value
 * Return: The pointer to dest
 */

char *_strcpy(char *dest, char *src)
{
int l = 0;
for (i = 0; src[i] != '\0'; i++)
{
dest[i] = src[i];
}
dest[i++] = '\0';
return (dest);
}
