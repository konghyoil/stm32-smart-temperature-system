#include "control.h"

#define DEBOUNCE_TIME_MS  30U

volatile uint8_t flag = 0;
volatile uint32_t tick = 0;


/* 온도 제어 설정 */
#define TEMP_HYSTERESIS_C    1.0f
#define TEMP_MAX_LIMIT_C     60.0f  // 예시값: 실제 장치의 안전 한계로 설정

SystemState_t state_machine(SystemData_t *data)
{
    SystemState_t next_state = data->state;

    /* 1. 전원 OFF: 모든 상태보다 우선 */
    if (!flag)
    {
        next_state = STATE_OFF;
    }
    /* 2. 센서 오류 또는 과열 */
    else if (data->sensor_error || data->temp_current >= TEMP_MAX_LIMIT_C)
    {
        next_state = STATE_ERROR;
    }
    else
    {
        switch (data->state)
        {
            case STATE_OFF:
                next_state = STATE_IDLE;
                break;

            case STATE_IDLE:
                /* 목표 온도보다 충분히 낮으면 가열 */
                if (data->temp_current <= data->temp_target - TEMP_HYSTERESIS_C)
                {
                    next_state = STATE_HEATING;
                }
                break;

            case STATE_HEATING:
                /* 목표 온도에 도달하면 가열 중지 */
                if (data->temp_current >= data->temp_target)
                {
                    next_state = STATE_IDLE;
                }
                break;

            case STATE_ERROR:
                /* 오류 상태는 전원 OFF 후 재시작할 때까지 유지 */
                next_state = STATE_ERROR;
                break;

            default:
                next_state = STATE_ERROR;
                break;
        }
    }

    return next_state;
}

void gpio_control(SystemState_t state)
{
    /* 히터 릴레이 제어 */
    if (flag && state == STATE_HEATING)
        HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);

    else
        HAL_GPIO_WritePin(Relay_GPIO_Port,Relay_Pin,GPIO_PIN_RESET);

    /* 시스템 전원 표시 LED 제어 */
    if (flag)
    	HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET);

    else
    	HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_3)
    {
    	uint32_t now = HAL_GetTick();

        if(now - tick > 300)
        {
        	flag = !flag;
        	tick = now;
        }
    }
}
