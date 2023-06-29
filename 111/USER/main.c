#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "menu.h"
#include "paj7620u2.h"
#include "key.h"
#include "sign.h"

#define KEY11 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define KEY22 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define KEY33 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define KEY41 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define KEY42 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define KEY51 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)
#define KEY52 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)



typedef enum {//??????
    WAVE_NONE,
    WAVE_SINE,
    WAVE_TRIANGULAR,
    WAVE_SQUARE
} WaveType;

static WaveType currentWaveType = WAVE_NONE;//???????
static uint16_t frequency = 100;//???????

bool isKeyPressed(uint8_t key) {//???
    if (key == 0) {
        delay_ms(10);
        return key == 0;
    }
    return false;
}

void changeWaveform(WaveType waveType, uint16_t freq) {//??????
    switch (waveType) {
        case WAVE_SINE:
            Sine_Wave_Init(freq, 0.4);
            break;
        case WAVE_TRIANGULAR:
            Triangular_Wave_Init(freq, 0.8);
            break;
        case WAVE_SQUARE:
            Square_Wave_Init(freq, 0.6);
            break;
        default:
            break;
    }
}

int main() {
    while (1) {
        if (isKeyPressed(KEY11)) {
            currentWaveType = WAVE_SINE;
        } else if (isKeyPressed(KEY22)) {
            currentWaveType = WAVE_TRIANGULAR;
        } else if (isKeyPressed(KEY33)) {
            currentWaveType = WAVE_SQUARE;
        }

        if (isKeyPressed(KEY42)) {
            frequency += 100;
            changeWaveform(currentWaveType, frequency);
            while(KEY42 == 0); // ??????
        }
    }
    return 0;
}


				