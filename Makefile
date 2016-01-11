# This file was automagically generated by mbed.org. For more information, 
# see http://mbed.org/handbook/Exporting-to-GCC-ARM-Embedded

GCC_BIN = 
PROJECT = mbed_blinky
OBJECTS =  ./lib/system_LPC11Uxx.o ./main.o ./classify.o ./lib/retarget.o ./lib/startup_LPC11xx.o
INCLUDE_PATHS = -I. -I./lib/api
LINKER_SCRIPT = ./LPC11U24.ld

############################################################################### 
AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
OBJDUMP = $(GCC_BIN)arm-none-eabi-objdump
SIZE    = $(GCC_BIN)arm-none-eabi-size 


CPU = -mcpu=cortex-m0 -mthumb 
CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP
CC_SYMBOLS = -D__CORTEX_M0 -DTARGET_LPC11UXX -DTOOLCHAIN_GCC -DTARGET_LPC11U24_401 -DTARGET_CORTEX_M -DTARGET_NXP -DTOOLCHAIN_GCC_ARM -DTARGET_M0 -DARM_MATH_CM0 -DMBED_BUILD_TIMESTAMP=1447257386.83 -DTARGET_LPC11U24 -D__MBED__=1 

LD_FLAGS = $(CPU) -Wl,--gc-sections --specs=nano.specs -Wl,--wrap,main -Wl,-Map=$(PROJECT).map,--cref
#LD_FLAGS += -u _printf_float -u _scanf_float
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys


ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

.PHONY: all clean lst size

all: $(PROJECT).bin $(PROJECT).hex size


clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).lst $(OBJECTS) $(DEPS)


.S.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) -o $@ $<




$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS)
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) -o $@ $^ $(LD_SYS_LIBS)

	@echo ""
	@echo "*****"
	@echo "***** You must modify vector checksum value in *.bin and *.hex files."
	@echo "*****"
	@echo ""


$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -Sdh $< > $@

lst: $(PROJECT).lst

size: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)


