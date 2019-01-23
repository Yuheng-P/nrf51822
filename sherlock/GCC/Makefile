
PREFIX := arm-none-eabi-

CC			= $(PREFIX)gcc
LD			= $(PREFIX)ld
AS        	= $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY		= $(PREFIX)objcopy
OBJDUMP		= $(PREFIX)objdump
AR			= $(PREFIX)ar
SIZE		= $(PREFIX)size
HEX       	= $(OBJCOPY) -O ihex
BIN       	= $(OBJCOPY) -O binary -S

TOP_DIR = ..

TARGET = sherlock

BUILD_DIR = build

DEVICE = nrf51

NETWORK = sim900a

CPU = cortex-m0

# mcu
MCU = -mcpu=$(CPU) -mthumb -mabi=aapcs

#file 
LD_SCRIPT = $(TOP_DIR)/components/toolchain/gcc/$(DEVICE)_xxac.ld

#defines
C_DEFS = -DUSE_SIM900A -DWITH_AT_FRAMEWORK -DUSE_$(shell echo $(NETWORK)| tr a-z A-Z) -DLWM2M_LITTLE_ENDIAN -DLWM2M_CLIENT_MODE -DAGENT_TINY_NOS -D$(shell echo $(DEVICE) |tr a-z A-Z)  -L$(TOP_DIR)/components/toolchain/gcc -T$(LD_SCRIPT)

ASM_SOURCE_s = $(TOP_DIR)/components/toolchain/gcc/gcc_startup_$(DEVICE).s

#sources
NRFDRIVER_SRC = $(TOP_DIR)/components/toolchain/system_$(DEVICE).c \
				$(TOP_DIR)/components/drivers_nrf/delay/nrf_delay.c \
				$(TOP_DIR)/components/drivers_nrf/clock/nrf_drv_clock.c \
				$(TOP_DIR)/components/drivers_nrf/uart/nrf_drv_uart.c \
				$(TOP_DIR)/components/drivers_nrf/common/nrf_drv_common.c
C_SOURCES += $(NRFDRIVER_SRC)

NRFLIB_SRC = $(TOP_DIR)/components/libraries/uart/app_uart.c \
			$(TOP_DIR)/components/libraries/mailbox/app_mailbox.c \
			$(TOP_DIR)/components/libraries/timer/app_timer.c \
			$(TOP_DIR)/components/libraries/mem_manager/mem_manager.c \
			$(TOP_DIR)/components/libraries/util/app_error.c \
			$(TOP_DIR)/components/libraries/util/app_util_platform.c
C_SOURCES += $(NRFLIB_SRC)

HW_SRC = $(TOP_DIR)/hw_init/hw_mailbox.c \
			$(TOP_DIR)/hw_init/hw_timer.c \
			$(TOP_DIR)/hw_init/hw_uart.c \
			$(TOP_DIR)/hw_init/sys_init.c \
			$(TOP_DIR)/hw_init/network/at_api_interface.c \
			$(TOP_DIR)/hw_init/network/$(NETWORK).c
C_SOURCES += $(HW_SRC)

AGENTTINY_SRC =  ${wildcard $(TOP_DIR)/agent_tiny/examples/*.c} \
				 ${wildcard $(TOP_DIR)/agent_tiny/osadapter/*.c} \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/agenttiny.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/atiny_log.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/atiny_rpt.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/commandline.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/connection.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_access_control.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_binary_app_data_container.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_device.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_security.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/object_server.c \
				 $(TOP_DIR)/agent_tiny/lwm2m_client/platform_adapter.c 
C_SOURCES += $(AGENTTINY_SRC)

LWM2M_SRC = ${wildcard $(TOP_DIR)/lwm2m/core/*.c} \
			${wildcard $(TOP_DIR)/lwm2m/core/er-coap-13/*.c} 
C_SOURCES += $(LWM2M_SRC)

USER_SRC = ${wildcard $(TOP_DIR)/Src/*.c}
C_SOURCES += $(USER_SRC)

#includes
NRFDRIVERS_INCLUDE = -I$(TOP_DIR)/components/drivers_nrf/delay \
						-I$(TOP_DIR)/components/drivers_nrf/clock \
						-I$(TOP_DIR)/components/drivers_nrf/uart \
						-I$(TOP_DIR)/components/drivers_nrf/common \
						-I$(TOP_DIR)/components/drivers_nrf/config \
						-I$(TOP_DIR)/components/drivers_nrf/hal \
						-I$(TOP_DIR)/components/device \
						-I$(TOP_DIR)/components/toolchain/gcc \
						-I$(TOP_DIR)/components/toolchain \
						-I$(TOP_DIR)/components/softdevice/s110/headers
C_INCLUDES += $(NRFDRIVERS_INCLUDE)						

NRFLIB_INCLUDES = -I$(TOP_DIR)/components/libraries/uart/ \
					-I$(TOP_DIR)/components/libraries/mailbox \
					-I$(TOP_DIR)/components/libraries/timer \
					-I$(TOP_DIR)/components/libraries/mem_manager \
					-I$(TOP_DIR)/components/libraries/util \
					-I$(TOP_DIR)/components/libraries/trace
C_INCLUDES += $(NRFLIB_INCLUDES)

HW_INCLUDES = -I$(TOP_DIR)/hw_init/ \
				-I$(TOP_DIR)/hw_init/network
C_INCLUDES += $(HW_INCLUDES)

AGENTTINY_INCLUDES = -I$(TOP_DIR)/agent_tiny/examples \
						-I$(TOP_DIR)/agent_tiny/lwm2m_client \
						-I$(TOP_DIR)/agent_tiny/osadapter/ \
						-I$(TOP_DIR)/agent_tiny/comm/include
C_INCLUDES += $(AGENTTINY_INCLUDES)

LWM2M_INCLUDES = -I$(TOP_DIR)/lwm2m/core/ \
				-I$(TOP_DIR)/lwm2m/core/er-coap-13
C_INCLUDES += $(LWM2M_INCLUDES)

USER_INCLUDES = -I$(TOP_DIR)/Src 
C_INCLUDES += $(USER_INCLUDES)

#build
# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCE_s:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCE_s)))

$(BUILD_DIR)/%.o:%.c $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	
$(BUILD_DIR)/%.o:%.s $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/$(TARGET).elf :$(OBJECTS) $(LD_SCRIPT)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@
	$(SIZE) $@
	
$(BUILD_DIR)/%.hex:$(BUILD_DIR)/%.elf $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin:$(BUILD_DIR)/%.elf $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@
	
clean:
	rm -rf .dep $(BUILD_DIR)
	
	
	
