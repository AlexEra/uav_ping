#include <Arduino.h>


/**** ANALOG pins ****/
constexpr uint16_t RSSI     = A0;
constexpr uint16_t POTENT   = A1;
constexpr uint16_t BEEPER   = 10;

/**** DIGITAL pins ****/
constexpr uint16_t LED      = 11;
constexpr uint16_t CH_SEL_0 = 4;
constexpr uint16_t CH_SEL_1 = 5;
constexpr uint16_t CH_SEL_2 = 6;

/**** Other constants ****/
constexpr uint16_t RSSI_THRESHOLD   = 250;
constexpr uint16_t RSSI_MAX         = 900;
constexpr uint16_t SWITCH_DELAY     = 25;
constexpr uint16_t BEEPER_DUTY      = 200;

/**
 * @brief Peripheral stuff initialization
 */
void init_peripherals(void) {
    Serial.begin(9600);
    delay(200);

    pinMode(LED, OUTPUT);
    pinMode(CH_SEL_0, OUTPUT);
    pinMode(CH_SEL_1, OUTPUT);
    pinMode(CH_SEL_2, OUTPUT);

    // ~1 kHz freq on D9 & D10
    TCCR1A = 0b00000001;  // 8bit
    TCCR1B = 0b00000010;  // x8 phase correct
}
