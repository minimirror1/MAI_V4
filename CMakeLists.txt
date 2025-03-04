cmake_minimum_required(VERSION 3.15)

# 프로젝트 이름과 언어 설정
project(MAI_V2 C CXX ASM)

# ARM 툴체인 설정
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# 툴체인 경로 설정 (사용자 환경에 맞게 수정 필요)
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)

# MCU 관련 설정
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F446xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4 
    -mthumb 
    -mfpu=fpv4-sp-d16 
    -mfloat-abi=hard
)

# 컴파일러 설정
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# 컴파일러 플래그
add_compile_options(
    ${CPU_PARAMETERS}
    -Wall
    -fdata-sections
    -ffunction-sections
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:Release>:-O2>
)

# 링커 플래그
add_link_options(
    ${CPU_PARAMETERS}
    -Wl,--gc-sections
    -specs=nano.specs
    -T${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld
)

# 소스 파일 설정 (기존 내용 유지)
set(SOURCES_DIR
    ${CMAKE_SOURCE_DIR}/Core/Src
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_APP/Src
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_DL/Src
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_NET/PDA/Src
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_NET/PHA/Src
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_NET/PHD/Src
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_PRTC/Src
    ${CMAKE_SOURCE_DIR}/Component/cpp_Motor_Class
    ${CMAKE_SOURCE_DIR}/Component/cpp_tick
    ${CMAKE_SOURCE_DIR}/MAL_Driver/Src
    ${CMAKE_SOURCE_DIR}/Imonitor/Src
)

# 헤더 디렉토리 설정 (기존 내용 유지)
include_directories(
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_APP/Inc
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_DL/Inc  
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_NET/PDA/Inc
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_NET/PHA/Inc
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_NET/PHD/Inc
    ${CMAKE_SOURCE_DIR}/Component/GM_MOTION/common/GM_PRTC/Inc
    ${CMAKE_SOURCE_DIR}/Component/cpp_Motor_Class
    ${CMAKE_SOURCE_DIR}/Component/cpp_tick
    ${CMAKE_SOURCE_DIR}/MAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Imonitor/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
)

# 실제 소스 파일 목록 (주요 파일만 예시로 표시)
set(SOURCES
    Core/Src/main.c
    Core/Src/stm32f4xx_hal_msp.c
    Core/Src/stm32f4xx_it.c
    Core/Src/system_stm32f4xx.c
    Core/Startup/startup_stm32f446retx.s
    # 필요한 다른 소스 파일들 추가
)

# 컴파일 정의 추가
add_definitions(
    -DUSE_HAL_DRIVER
    -DSTM32F446xx
    -DDEBUG
)

# 실행 파일 타겟 생성
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# 바이너리 생성을 위한 커스텀 커맨드
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
)

# IDE 지원을 위한 모든 파일 포함
file(GLOB_RECURSE ALL_SOURCES 
    "Core/*.*"
    "Component/*.*"
    "MAL_Driver/*.*"
    "Imonitor/*.*"
    "Drivers/*.*"
)

add_custom_target(ide_support SOURCES ${ALL_SOURCES}) 