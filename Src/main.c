#include "main.h"
#include "cmsis_os.h"

/**
 * @brief LEDController class to manage LED threads.
 */
class LEDControl {
public:
    static osThreadId Thread1Handle, Thread2Handle;

    /**
     * @brief Thread function for LED 1.
     * @param argument not used.
     */
    static void Thread1Function(void const *argument);

    /**
     * @brief Thread function for LED 2.
     * @param argument not used.
     */
    static void Thread2Function(void const *argument);

    /**
     * @brief Configure the system clock.
     */
    static void ConfigureSystemClock();

    /**
     * @brief Handle errors by turning on LED 2 and entering an infinite loop.
     */
    static void HandleError();
};

osThreadId LEDControl::Thread1Handle, LEDControl::Thread2Handle;

int main(void)
{
    HAL_Init();
    LEDControl::ConfigureSystemClock();
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED3);

    // Define thread structures for LED1 and LED3
    osThreadDef(LED1, LEDControl::Thread1Function, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadDef(LED3, LEDControl::Thread2Function, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);

    // Create threads for LED1 and LED3
    LEDControl::Thread1Handle = osThreadCreate(osThread(LED1), nullptr);
    LEDControl::Thread2Handle = osThreadCreate(osThread(LED3), nullptr);

    osKernelStart();

    while (true);
}

void LEDControl::Thread1Function(void const *argument)
{
    uint32_t count = 0;

    for (;;)
    {
        count = osKernelSysTick() + 5000;

        while (count >= osKernelSysTick())
        {
            BSP_LED_Toggle(LED1);
            osDelay(200);
        }

        BSP_LED_Off(LED1);
        osThreadSuspend(nullptr);
        count = osKernelSysTick() + 5000;

        while (count >= osKernelSysTick())
        {
            BSP_LED_Toggle(LED1);
            osDelay(400);
        }

        osThreadResume(Thread2Handle);
    }
}

void LEDControl::Thread2Function(void const *argument)
{
    uint32_t count;

    for (;;)
    {
        count = osKernelSysTick() + 10000;

        while (count >= osKernelSysTick())
        {
            BSP_LED_Toggle(LED3);
            osDelay(500);
        }

        BSP_LED_Off(LED3);
        osThreadResume(Thread1Handle);
        osThreadSuspend(nullptr);
    }
}

void LEDControl::ConfigureSystemClock()
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    // Enable Power Control clock and configure voltage scaling
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Configure and initialize the system oscillator and PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    RCC_OscInitStruct.PLL.PLLR = 6;

    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK)
        HandleError();

    ret = HAL_PWREx_EnableOverDrive();
    if (ret != HAL_OK)
        HandleError();

    // Configure and initialize the system clocks
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    if (ret != HAL_OK)
        HandleError();
}

void LEDControl::HandleError()
{
    // Turn on LED3 and enter an infinite loop to handle errors
    BSP_LED_On(LED3);
    while (true) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    // Enter an infinite loop to handle assertion failures
    while (true) {}
}
#endif
