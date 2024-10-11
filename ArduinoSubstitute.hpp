#ifndef ARDUINO_SUBSTITUTE_H
#define ARDUINO_SUBSTITUTE_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define PI 3.1415926535897932384626433832795

#define GPIO_OUT_W1TC_ADDRESS 0x60000308
#define GPIO_OUT_W1TS_ADDRESS 0x60000308
#define _BV(bit) (bit)
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

void GPIO_REG_WRITE(uint32_t reg, uint32_t val);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);
unsigned long millis();
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Print
{
public:
    // New overloads
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(long long, int = DEC);
    size_t print(unsigned long long, int = DEC);
    size_t print(double, int = 2);

    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(long long, int = DEC);
    size_t println(unsigned long long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);

    virtual size_t write(uint8_t) = 0;
    size_t write(const char *str)
    {
        if (str == NULL)
        {
            return 0;
        }
        return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size)
    {
        return write((const uint8_t *)buffer, size);
    }

private:
    int write_error;
    size_t printNumber(unsigned long, uint8_t);
    size_t printNumber(unsigned long long, uint8_t);
    size_t printFloat(double, uint8_t);

protected:
    void setWriteError(int err = 1)
    {
        write_error = err;
    }
};

/// SPISubstitute is a substitute for the Arduino SPI class.

#define SPI_CLOCK_DIV2 0
#define SPI_CLOCK_DIV4 0
#define SPI_CLOCK_DIV8 0
#define SPI_CLOCK_DIV16 0
#define SPI_MODE0 0

class SPISubstitute
{
public:
    void begin() {}
    void end() {}
    void write(uint8_t data) {}
    void write16(uint16_t data) {}
    void setBitOrder(uint8_t bitOrder) {}
    void setDataMode(uint8_t dataMode) {}
    void setClockDivider(uint8_t clockDiv) {}
    uint8_t transfer(uint8_t data) { return 0; }
    uint16_t transfer16(uint16_t data) { return 0; }
    void transfer(void *buf, size_t count) {}
    void usingInterrupt(uint8_t interruptNumber) {}
    void notUsingInterrupt(uint8_t interruptNumber) {}
};

#endif // !ARDUINO_SUBSTITUTE_H