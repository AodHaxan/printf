#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define BUFF_SIZE 1024

void print_buffer(char buffer[], int *buff_ind);
int get_flags(const char *format, int *i);
int get_width(const char *format, int *i, va_list list);
int get_precision(const char *format, int *i, va_list list);
int get_size(const char *format, int *i);
int handle_print(const char *format, int *i, va_list list, char buffer[],
                 int flags, int width, int precision, int size);

/**
 * _printf - Printf function
 * @format: format.
 * Return: Printed chars.
 */
int _printf(const char *format, ...)
{
    int i, printed = 0, printed_chars = 0;
    int flags, width, precision, size, buff_ind = 0;
    va_list list;
    char buffer[BUFF_SIZE];

    if (format == NULL)
        return (-1);

    va_start(list, format);

    for (i = 0; format && format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            buffer[buff_ind++] = format[i];
            if (buff_ind == BUFF_SIZE)
                print_buffer(buffer, &buff_ind);
            printed_chars++;
        }
        else
        {
            print_buffer(buffer, &buff_ind);
            flags = get_flags(format, &i);
            width = get_width(format, &i, list);
            precision = get_precision(format, &i, list);
            size = get_size(format, &i);
            ++i;
            printed = handle_print(format, &i, list, buffer,
                                   flags, width, precision, size);
            if (printed == -1)
                return (-1);
            printed_chars += printed;
        }
    }

    print_buffer(buffer, &buff_ind);
    va_end(list);

    return (printed_chars);
}

/**
 * print_buffer - Prints the contents of the buffer if it exists
 * @buffer: Array of chars
 * @buff_ind: Pointer to index at which to add next char, represents the length.
 */
void print_buffer(char buffer[], int *buff_ind)
{
    if (*buff_ind > 0)
        write(1, &buffer[0], *buff_ind);

    *buff_ind = 0;
}

/**
 * get_flags - Returns the flags associated with the format specifier
 * @format: Format string
 * @i: Pointer to index of the format specifier
 * Return: Flags (0 for no flags, 1 for left-justified, 2 for zero-padded)
 */
int get_flags(const char *format, int *i)
{
    int flags = 0;

    while (format[*i] == '-' || format[*i] == '0')
    {
        if (format[*i] == '-')
            flags |= 1;  /* left-justified */
        else if (format[*i] == '0')
            flags |= 2;  /* zero-padded */
        (*i)++;
    }

    return (flags);
}
