/* MAX7219 8x32 LED Matrix Driver Demo Code
 *
 * This is a demo program for Arduino/AVR Atmega 328p microcontroller. It can display hearts on a 4x8x8 LED matrix.
 * This program is great for learning how to use the MAX7219 LED matrix driver with an Arduino/AVR microcontroller.
 * If you have a different number of 8x8 LED matrices, you can change the for loop part.
 *
 * Created by William Mao <https://github.com/WaylonMao>on 2023-05-21.
 */
#include <avr/io.h>
#include <util/delay.h>

#define DECODE_MODE_REG 0x09
#define INTENSITY_REG 0x0A
#define SCAN_LIMIT_REG 0x0B
#define SHUTDOWN_REG 0x0C
#define DISPLAY_TEST_REG 0x0F

#define DIN PB3 // MOSI
#define CS PB2  // SS
#define CLK PB5 // SCK

const unsigned char heart_1[8] = {
        0b00000000,
        0b01100110,
        0b11111111,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100,
        0b00011000
};

const unsigned char heart_2[8] = {
        0b01100110,
        0b11111111,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100,
        0b00011000,
        0b00000000
};

void sendByte(uint8_t data) {
    for (uint8_t i = 8; i > 0; i--) {
        PORTB &= ~(1 << CLK);
        PORTB &= ~(1 << DIN);
        if (data & (1 << (i - 1))) {
            PORTB |= (1 << DIN);
        }
        PORTB |= (1 << CLK);
    }
}

void setReg(uint8_t reg, uint8_t value) {
    PORTB &= ~(1 << CS);
    sendByte(reg);
    sendByte(value);
    PORTB |= (1 << CS);
}

void setup() {
    // Set pins as output
    DDRB |= (1 << DIN) | (1 << CLK) | (1 << CS);
    // Set MAX7219 registers
    // Enable TEST mode
    setReg(DISPLAY_TEST_REG, 0x01);
    _delay_ms(1000);
    // Disable TEST mode
    setReg(DISPLAY_TEST_REG, 0x00);
    // Disable BCD mode
    setReg(DECODE_MODE_REG, 0x00);
    // Set Scan Limit
    setReg(SCAN_LIMIT_REG, 0x07);
    // Set Intensity
    setReg(INTENSITY_REG, 0x01);
    // Disable Shutdown mode
    setReg(SHUTDOWN_REG, 0x01);
}

int main() {
    setup();
    while (1) {
        setReg(INTENSITY_REG, 0x0f & 3);
        for (int j = 1; j <= 4; j++) {
            for (int i = 1; i <= 8; i++) {
                setReg(i, heart_2[i - 1]);
            }
        }
        _delay_ms(100);

        setReg(INTENSITY_REG, 0x0f & 0);
        for (int j = 1; j <= 4; j++) {
            for (int i = 1; i <= 8; i++) {
                setReg(i, heart_1[i - 1]);
            }
        }
        _delay_ms(50);

        setReg(INTENSITY_REG, 0x0f & 3);
        for (int j = 1; j <= 4; j++) {
            for (int i = 1; i <= 8; i++) {
                setReg(i, heart_2[i - 1]);
            }
        }
        _delay_ms(100);

        setReg(INTENSITY_REG, 0x0f & 0);
        for (int j = 1; j <= 4; j++) {
            for (int i = 1; i <= 8; i++) {
                setReg(i, heart_1[i - 1]);
            }
        }
        _delay_ms(1000);
    }
}
