#include <Arduino.h>
#include "config.h"


// #define TEST
#define USING_BEEPER


void choose_channel(uint8_t ch);
void print_channels_values(void);
bool check_channel_value(uint8_t ch);


uint16_t rssi_values[8]; // array for keeping channels values
volatile uint16_t rssi_threshold = 1024;


void setup() {
    init_peripherals();
#ifdef TEST
    choose_channel(0);
    delay(SWITCH_DELAY);
#endif
}


void loop() {
    static uint8_t choosed_channel = 0;
    static volatile uint8_t avg_count = 0;
    static volatile uint32_t threshold_sum = 0;

#ifndef TEST
    choose_channel(choosed_channel);
    if (++choosed_channel >= 8) {
        choosed_channel = 0;
        print_channels_values(); // print all data after updating
    }

    check_channel_value(choosed_channel);

    analogRead(POTENT); // fake read
    threshold_sum += analogRead(POTENT);
    if (++avg_count >= 9) {
        rssi_threshold = map(threshold_sum >> 3, 0, RSSI_MAX, 0, 100);
        rssi_threshold = constrain(rssi_threshold, 0, 100);
        avg_count = 0;
        threshold_sum = 0;
    }

#else
    char symb = Serial.read();
    switch (symb) {
        case 'c':
            if (++choosed_channel >= 8) choosed_channel = 0;
            Serial.print("Channel: "); Serial.println(choosed_channel + 1);
            choose_channel(choosed_channel);
            while (Serial.available()) Serial.read();
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            choosed_channel = (uint8_t)symb - 49;
            Serial.print("Channel: "); Serial.println(choosed_channel + 1);
            choose_channel(choosed_channel);
            while (Serial.available()) Serial.read();
            break;
    }
    Serial.println(analogRead(RSSI));
    delay(100);
#endif
}

/**
 * @brief Choosing channel with delay after switching
 * 
 * @param ch Channel to switch for
 */
void choose_channel(uint8_t ch) {
    switch (ch) {
        case 0:
            digitalWrite(CH_SEL_0, 0);
            digitalWrite(CH_SEL_1, 0);
            digitalWrite(CH_SEL_2, 0);
            break;

        case 1:
            digitalWrite(CH_SEL_0, 1);
            digitalWrite(CH_SEL_1, 0);
            digitalWrite(CH_SEL_2, 0);
            break;

        case 2:
            digitalWrite(CH_SEL_0, 0);
            digitalWrite(CH_SEL_1, 1);
            digitalWrite(CH_SEL_2, 0);
            break;

        case 3:
            digitalWrite(CH_SEL_0, 1);
            digitalWrite(CH_SEL_1, 1);
            digitalWrite(CH_SEL_2, 0);
            break;

        case 4:
            digitalWrite(CH_SEL_0, 0);
            digitalWrite(CH_SEL_1, 0);
            digitalWrite(CH_SEL_2, 1);
            break;

        case 5:
            digitalWrite(CH_SEL_0, 1);
            digitalWrite(CH_SEL_1, 0);
            digitalWrite(CH_SEL_2, 1);
            break;

        case 6:
            digitalWrite(CH_SEL_0, 0);
            digitalWrite(CH_SEL_1, 1);
            digitalWrite(CH_SEL_2, 1);
            break;

        case 7:
            digitalWrite(CH_SEL_0, 1);
            digitalWrite(CH_SEL_1, 1);
            digitalWrite(CH_SEL_2, 1);
            break;
    }
    analogRead(RSSI); // fake read
}

/**
 * @brief Printing via Serial RSSI channels data 
 */
void print_channels_values(void) {
    bool flg = false;
    
    for (uint8_t i = 0; i < 8; i++) {
        Serial.print(rssi_values[i]);
        Serial.print("\t");

        if (!flg && (rssi_values[i] > rssi_threshold)) {
            flg = true;
        }
    }
    Serial.println(rssi_threshold);
    Serial.println();

    if (flg) {
        digitalWrite(LED, 1);
        analogWrite(BEEPER, BEEPER_DUTY);
    } else {
        digitalWrite(LED, 0);
        analogWrite(BEEPER, 0); 
    }
}

/**
 * @brief Checking the channel's analog value
 * 
 * @param ch Channel number for value checking
 * 
 * @return True if value is more than thershold value defined in config.h, false otherwise
 */
bool check_channel_value(uint8_t ch) {
    delay(SWITCH_DELAY);
    rssi_values[ch] = map(analogRead(RSSI), 0, RSSI_MAX, 0, 100);
    rssi_values[ch] = constrain(rssi_values[ch], 0, 100);
    if (rssi_values[ch] > rssi_threshold) return true;
    return false;
}
