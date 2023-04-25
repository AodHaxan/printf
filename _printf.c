#include "main.h"

/**
 * _printf - Custom printf function
 * @format: format string
 * Return: number of characters printed
 */
int _printf(const char *format, ...)
{
	va_list args;
	char buffer[BUFF_SIZE];
	int buffer_index = 0, i, printed_chars = 0;

	if (format == NULL)
		return (-1);

	va_start(args, format);

	for (i = 0; format[i] != '\0'; i++)
	{
		if (format[i] != '%')
		{
			buffer[buffer_index++] = format[i];
			if (buffer_index == BUFF_SIZE)
			{
				print_buffer(buffer, &buffer_index);
				printed_chars += BUFF_SIZE;
			}
			else
			{
				printed_chars++;
			}
		}
		else
		{
			print_buffer(buffer, &buffer_index);
			i++;
			printed_chars += handle_print(format[i], args, buffer, &buffer_index);
		}
	}

	print_buffer(buffer, &buffer_index);
	va_end(args);

	return (printed_chars);
}

/**
 * handle_print - handles conversion and printing of format specifier
 * @specifier: format specifier character
 * @args: variable arguments list
 * @buffer: buffer to print output to
 * @buffer_index: current index in buffer
 * Return: number of characters printed
 */
int handle_print(char specifier, va_list args, char buffer[], int *buffer_index)
{
	int width = 0, precision = 0;
	int flags[5] = {0, 0, 0, 0, 0};
	char *str;
	int num;
	int num_len;
	char c;

	parse_flags(specifier, flags);
	width = parse_width(args);
	precision = parse_precision(args);

	switch (specifier)
	{
		case 'c':
			c = va_arg(args, int);
			return (print_char(buffer, buffer_index, c, flags, width));
		case 's':
			str = va_arg(args, char *);
			return (print_string(buffer, buffer_index, str, flags, width, precision));
		case 'd':
		case 'i':
			num = va_arg(args, int);
			num_len = num_len_int(num);
			return (print_number(buffer, buffer_index, num, num_len, flags, width, precision));
		case 'u':
			num = va_arg(args, unsigned int);
			num_len = num_len_uint(num);
			return (print_number(buffer, buffer_index, num, num_len, flags, width, precision));
		case 'o':
			num = va_arg(args, unsigned int);
			num_len = num_len_oct(num);
			return (print_number(buffer, buffer_index, num, num_len, flags, width, precision));
		case 'x':
		case 'X':
			num = va_arg(args, unsigned int);
			num_len = num_len_hex(num);
			return (print_number(buffer, buffer_index, num, num_len, flags, width, precision));
		case 'p':
			num = (unsigned long)va_arg(args, void *);
			num_len = num_len_hex(num);
			return (print_address(buffer, buffer_index, num, num_len, flags, width));
		case '%':
			return (print_percent(buffer, buffer_index, flags, width));
		default:
			return (print_error(buffer, buffer_index, specifier));
	}
}

/**
 * print_buffer - prints buffer to stdout
