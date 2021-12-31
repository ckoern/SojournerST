cmake_minimum_required(VERSION 3.14)
option(SOJOURNER_TESTS "Native Compile for unit tests" OFF)

if(NOT SOJOURNER_TESTS)
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake )
endif()

project(SojournerST LANGUAGES C CXX ASM)
set(CMAKE_CXX_STANDARD 20)

if (SOJOURNER_TESTS)

    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(
    sojournerst_test
    Test/test_packet.cpp
    Test/test_board.cpp

    App/Src/Packet.cpp
    App/Src/MotorController.cpp
    App/Src/TB6612.cpp
    App/Src/SojournerST.cpp

    Test/Stubs/Src/stm32g4xx_hal.c
    )

    target_compile_options(sojournerst_test PRIVATE 
                   -Wall -Wextra "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>" )

    target_include_directories(sojournerst_test PUBLIC 
                    App/Inc
                    Test/Stubs/Inc
    )


    target_link_libraries(
    sojournerst_test
    gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(sojournerst_test)


else()
    set(STM32CUBEMX_GENERATED_FILES
    
    App/Src/MotorController.cpp
    App/Src/Packet.cpp
    App/Src/SojournerST.cpp
    App/Src/TB6612.cpp
    App/Src/app.cpp
    Core/Src/app_freertos.c
    Core/Src/gpio.c
    Core/Src/main.c
    Core/Src/spi.c
    Core/Src/stm32g4xx_hal_msp.c
    Core/Src/stm32g4xx_hal_timebase_tim.c
    Core/Src/stm32g4xx_it.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/system_stm32g4xx.c
    Core/Src/tim.c
    Core/Src/usart.c
    Core/Startup/startup_stm32g441kbtx.s
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c
    Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.c
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    USB_Device/App/usb_device.c
    USB_Device/App/usbd_cdc_if.c
    USB_Device/App/usbd_desc.c
    USB_Device/Target/usbd_conf.c

    )
    set(EXECUTABLE ${PROJECT_NAME}.out)

    add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES})

    target_compile_definitions(${EXECUTABLE} PRIVATE
            -DUSE_HAL_DRIVER
            -DSTM32G441xx
            )

    target_include_directories(${EXECUTABLE} PRIVATE
            Core/Inc 
            App/Inc
            USB_Device/App 
            USB_Device/Target 
            Drivers/STM32G4xx_HAL_Driver/Inc 
            Drivers/STM32G4xx_HAL_Driver/Inc/Legacy 
            Middlewares/Third_Party/FreeRTOS/Source/include 
            Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 
            Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F 
            Middlewares/ST/STM32_USB_Device_Library/Core/Inc 
            Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc 
            Drivers/CMSIS/Device/ST/STM32G4xx/Include 
            Drivers/CMSIS/Include 

            )
    # arm-none-eabi-g++ -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"App/Src/Packet.d" -MT"App/Src/Packet.o" --specs=nano.specs 
    #-mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "App/Src/Packet.o"


    target_compile_options(${EXECUTABLE} PRIVATE
            -mcpu=cortex-m4
            -mthumb
            -mfpu=fpv4-sp-d16
            -mfloat-abi=hard

            -fdata-sections
            -ffunction-sections
            -fno-exceptions 
            -fstack-usage

            -Wall

            $<$<CONFIG:Debug>:-Og>
            )

    target_link_options(${EXECUTABLE} PRIVATE
            -T${CMAKE_SOURCE_DIR}/STM32G441KBTX_FLASH.ld
            -mcpu=cortex-m4
            -mthumb
            -mfpu=fpv4-sp-d16
            -mfloat-abi=hard
            -specs=nano.specs
            -lc
            -lm
            -lnosys
            -Wl,-Map=${PROJECT_NAME}.map,--cref
            -Wl,--gc-sections
            )

    # Print executable size
    add_custom_command(TARGET ${EXECUTABLE}
            POST_BUILD
            COMMAND arm-none-eabi-size ${EXECUTABLE})

    # Create hex file
    add_custom_command(TARGET ${EXECUTABLE}
            POST_BUILD
            COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
            COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
endif()