# ESPLaboratory STM32F429i-Discovery Makefile

# Name of the main executeable without extention
PROJECT = Demo

# set the Path to the Libraries directory
LIBP ?= /DIST/lehre/lab_espl/tech/Software/Libraries

# set the path to STM32F429I-Discovery firmware package
STDP ?= $(LIBP)/STM32F429I-Discovery_FW_V1.0.1

# set the path to FreeRTOS package
RTOS ?= $(LIBP)/FreeRTOSV8.2.3

# set the path to uGFX package
UGFX ?= $(LIBP)/ugfxr21

# set the path to additional functions/headers
USRP ?= ../Libraries/usr

#
# Any own source files can be added by this line:
# OBJECTS += filename.o
# Any own header files can be added by this line:
# CFLAGS += -I"filename.h

# Anything below here can be left as is
BUILD_DIR = build
LIB_BUILD_DIR = lib_build

EXECUTABLE = $(BUILD_DIR)/$(PROJECT).elf
BIN_IMAGE = $(BUILD_DIR)/$(PROJECT).bin
HEX_IMAGE = $(BUILD_DIR)/$(PROJECT).hex


# Toolchain configurations
CROSS_COMPILE ?= arm-none-eabi
CC = $(CROSS_COMPILE)-gcc
LD = $(CROSS_COMPILE)-ld
OBJCOPY = $(CROSS_COMPILE)-objcopy
OBJDUMP = $(CROSS_COMPILE)-objdump
SIZE = $(CROSS_COMPILE)-size

# Cortex-M4 implements the ARMv7E-M architecture
CPU = cortex-m4
CFLAGS = -mcpu=$(CPU) -march=armv7e-m -mtune=cortex-m4
CFLAGS += -mlittle-endian -mthumb

# FPU
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

# Libraries
LIBS = -lc -lnosys
LDFLAGS =
define get_library_path
    $(shell dirname $(shell $(CC) $(CFLAGS) -print-file-name=$(1)))
endef
LDFLAGS += -L $(call get_library_path,libc.a)
LDFLAGS += -L $(call get_library_path,libgcc.a)

# use VPATH to add source directories
VPATH = ./:$(USRP):$(STDP)/Libraries/STM32F4xx_StdPeriph_Driver/src:$(STDP)/Utilities/STM32F429I-Discovery:$(RTOS)/FreeRTOS/Source:$(RTOS)/FreeRTOS/Source/portable/MemMang:$(RTOS)/FreeRTOS/Demo/Common/Minimal:$(UGFX)/src:$(UGFX)/src/gdisp:$(UGFX)/src/gdisp/mcufont:$(UGFX)/src/gevent:$(UGFX)/src/ginput:$(UGFX)/src/gos:$(UGFX)/src/gtimer:$(UGFX)/src/gwin

# Basic configurations
CFLAGS += -g -std=c99 -Wall

# Optimizations
CFLAGS += -O3 -ffast-math
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections
CFLAGS += -fno-common
CFLAGS += --param max-inline-insns-single=1000

# specify STM32F429
CFLAGS += -DSTM32F429_439xx

# to run from FLASH
LDFLAGS += -T $(USRP)/stm32f429zi_flash.ld

# Project source
CFLAGS += -I.
USR_SRCS = \
    gdisp_lld_ILI9341.c \
    ginput_lld_mouse.c \
    ParTest.c \
    port.c \
    timertest.c \
    system_stm32f4xx.c\
	ESPL_functions.c
CFLAGS += -I$(USRP)
CFLAGS += -I../include

# Startup file
# available as .S in Library/usr
USR_SRCS += startup_stm32f429_439xx.c

# CMSIS
CFLAGS += -I$(STDP)/Libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I$(STDP)/Libraries/CMSIS/Include

# STM32F4xx_StdPeriph_Driver
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -I$(STDP)/Libraries/STM32F4xx_StdPeriph_Driver/inc
#add sources
LIB_SRCS += \
    misc.c \
    stm32f4xx_dma.c \
    stm32f4xx_dma2d.c \
    stm32f4xx_fmc.c \
    stm32f4xx_i2c.c \
    stm32f4xx_ltdc.c \
    stm32f4xx_gpio.c \
    stm32f4xx_rcc.c \
    stm32f4xx_spi.c \
    stm32f4xx_usart.c \
    stm32f4xx_adc.c \
    stm32f4xx_tim.c \
	stm32f4xx_exti.c \
	stm32f4xx_syscfg.c


# STM32F429I-Discovery Utilities
CFLAGS += -I$(STDP)/Utilities/STM32F429I-Discovery
CFLAGS += -I$(STDP)/Utilities/Common
# Add sources
LIB_SRCS += \
    stm32f429i_discovery_ioe.c \
    stm32f429i_discovery_lcd.c \
    stm32f429i_discovery_sdram.c

# FreeRTOS
CFLAGS += -I$(RTOS)/FreeRTOS/Source/include
CFLAGS += -I$(RTOS)/FreeRTOS/Source/portable/GCC/ARM_CM4F
CFLAGS += -I$(RTOS)/FreeRTOS/Demo/Common/include
CFLAGS += -DGFX_USE_OS_FREERTOS=TRUE
# add sources
LIB_SRCS += \
    list.c \
    queue.c \
    tasks.c \
    timers.c \
    heap_1.c \
    flash.c

# uGFX
CFLAGS += -I$(UGFX)
CFLAGS += -I$(UGFX)/src/gdisp/mcufont
# add sources
LIB_SRCS += \
    gfx.c \
    fonts.c \
    gdisp.c \
    mf_encoding.c \
    mf_font.c \
    mf_justify.c \
    mf_scaledfont.c \
    mf_bwfont.c \
    mf_rlefont.c \
    gevent.c \
    ginput.c \
    mouse.c \
    freertos.c \
    gtimer.c \
    console.c \
    gwin.c \
    gwm.c

# define object targets
OBJS += $(patsubst %.c,$(BUILD_DIR)/%.o,$(USR_SRCS))
OBJS += $(BUILD_DIR)/$(PROJECT).o
#OBJS += $(BUILD_DIR)/flipper.o
OBJS += $(BUILD_DIR)/balloon_menu_view.o
OBJS += $(BUILD_DIR)/animation.o
OBJS += $(BUILD_DIR)/mostof_obstacles.o
OBJS += $(BUILD_DIR)/graph_tables.o
OBJS += $(BUILD_DIR)/Engine.o
LIB_OBJS += $(patsubst %.c,$(LIB_BUILD_DIR)/%.o,$(LIB_SRCS))


all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@
	$(OBJCOPY) -O ihex $^ $(HEX_IMAGE)
	$(OBJDUMP) -h -S -D $(EXECUTABLE) > $(BUILD_DIR)/$(PROJECT).lst
	$(SIZE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(LIB_OBJS)
	@echo LD $@
	@$(LD) -o $@ $(OBJS) $(LIB_OBJS) --start-group $(LIBS) --end-group $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@
$(LIB_BUILD_DIR)/%.o: %.c | $(LIB_BUILD_DIR)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	@echo CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@echo creating directory $@
	@mkdir $@

$(LIB_BUILD_DIR):
	@echo creating directory $@
	@mkdir $@

clean:
	@rm -rf $(BUILD_DIR)
	@echo $(BUILD_DIR) deleted.

clean_all:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(LIB_BUILD_DIR)
	@echo $(BUILD_DIR) and $(LIB_BUILD_DIR) deleted.

flash: $(BIN_IMAGE)
	st-flash write $(BIN_IMAGE) 0x8000000

.PHONY: clean clean_all all main flash
