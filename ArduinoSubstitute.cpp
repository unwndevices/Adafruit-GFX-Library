#ifdef WINDOWS
#include "ArduinoSubstitute.hpp"

#include <stdlib.h>

#include <rack.hpp>

void GPIO_REG_WRITE(uint32_t reg, uint32_t val)
{
}

void pinMode(uint8_t pin, uint8_t mode)
{
}

void digitalWrite(uint8_t pin, uint8_t value)
{
}

int digitalRead(uint8_t pin)
{
    return 0;
}

unsigned long millis()
{
    // convert the double seconds to an unsigned long milliseconds
    return static_cast<unsigned long>(rack::system::getTime() * 1000);
}

void delay(unsigned long ms)
{
}

void delayMicroseconds(unsigned int us)
{
}

SPISubstitute SPI;

size_t Print::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while (size--)
    {
        n += write(*buffer++);
    }
    return n;
}

size_t Print::print(const char str[])
{
    return write(str);
}

size_t Print::print(char c)
{
    return write(c);
}

size_t Print::print(unsigned char b, int base)
{
    return print((unsigned long)b, base);
}

size_t Print::print(int n, int base)
{
    return print((long)n, base);
}

size_t Print::print(unsigned int n, int base)
{
    return print((unsigned long)n, base);
}

size_t Print::print(long n, int base)
{
    int t = 0;
    if (base == 10 && n < 0)
    {
        t = print('-');
        n = -n;
    }
    return printNumber(static_cast<unsigned long>(n), base) + t;
}

size_t Print::print(unsigned long n, int base)
{
    if (base == 0)
    {
        return write(n);
    }
    else
    {
        return printNumber(n, base);
    }
}

size_t Print::print(long long n, int base)
{
    int t = 0;
    if (base == 10 && n < 0)
    {
        t = print('-');
        n = -n;
    }
    return printNumber(static_cast<unsigned long long>(n), base) + t;
}

size_t Print::print(unsigned long long n, int base)
{
    if (base == 0)
    {
        return write(n);
    }
    else
    {
        return printNumber(n, base);
    }
}

size_t Print::print(double n, int digits)
{
    return printFloat(n, digits);
}

size_t Print::println(void)
{
    return print("\r\n");
}

size_t Print::println(const char c[])
{
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(char c)
{
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(unsigned char b, int base)
{
    size_t n = print(b, base);
    n += println();
    return n;
}

size_t Print::println(int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned int num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(long long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned long long num, int base)
{
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(double num, int digits)
{
    size_t n = print(num, digits);
    n += println();
    return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t Print::printNumber(unsigned long n, uint8_t base)
{
    char buf[8 * sizeof(n) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1
    if (base < 2)
    {
        base = 10;
    }

    do
    {
        char c = n % base;
        n /= base;

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

size_t Print::printNumber(unsigned long long n, uint8_t base)
{
    char buf[8 * sizeof(n) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1
    if (base < 2)
    {
        base = 10;
    }

    do
    {
        auto m = n;
        n /= base;
        char c = m - base * n;

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

constexpr bool
isnan(double __x)
{
    return __builtin_isnan(__x);
}

constexpr bool
isinf(double __x)
{
    return __builtin_isinf(__x);
}

size_t Print::printFloat(double number, uint8_t digits)
{
    size_t n = 0;

    if (isnan(number))
    {
        return print("nan");
    }
    if (isinf(number))
    {
        return print("inf");
    }
    if (number > 4294967040.0)
    {
        return print("ovf"); // constant determined empirically
    }
    if (number < -4294967040.0)
    {
        return print("ovf"); // constant determined empirically
    }

    // Handle negative numbers
    if (number < 0.0)
    {
        n += print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
    {
        rounding /= 10.0;
    }

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    n += print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
    {
        n += print(".");
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
        remainder *= 10.0;
        int toPrint = int(remainder);
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}
#endif // !WINDOWS