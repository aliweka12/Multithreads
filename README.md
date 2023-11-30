# LED Controller

The LED Controller is a software component designed to manage two LED threads that control LEDs on a microcontroller. It uses the CMSIS-RTOS (Real-Time Operating System) library for thread management and HAL (Hardware Abstraction Layer) for hardware initialization and control.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Prerequisites

Before using the LED Controller, make sure you have the following prerequisites in place:

- [CMSIS-RTOS](https://github.com/ARM-software/CMSIS_5) library installed.
- [HAL (Hardware Abstraction Layer)](https://www.st.com/en/ecosystems/stm32cube.html) configured for your microcontroller platform.

## Usage

1. Clone or download this repository to your development environment.
2. Include the necessary headers and source files in your project.
3. Configure the system clock according to your microcontroller specifications using the `LEDController::ConfigureSystemClock()` method.
4. Initialize the LEDs you want to control using `BSP_LED_Init()` (Board Support Package LED Initialization).
5. Define and create threads for LED control using the `osThreadDef` and `osThreadCreate` functions.
6. Start the CMSIS-RTOS kernel using `osKernelStart()`.

The code contains two threads, `Thread1Function` and `Thread2Function`, which toggle LEDs based on a specified time interval. You can customize the behavior of these threads as needed.

## Configuration

### System Clock Configuration

You can configure the system clock settings in the `LEDController::ConfigureSystemClock()` method. Adjust the PLL, oscillator type, and other clock parameters to match your microcontroller's specifications.

### LED Thread Configuration

The code provides two LED threads, `Thread1Function` and `Thread2Function`, which control LED1 and LED3, respectively. You can customize the LED pins and toggle intervals within these thread functions.

## Troubleshooting

If you encounter any issues or errors, you can use the `HandleError()` method to handle errors. This method turns on LED3 and enters an infinite loop to indicate an error condition.

If you're using assertion checks, the `assert_failed` function can also help you handle assertion failures.

