# This file was automagically generated by mbed.org. For more information, 
# see http://mbed.org/handbook/Exporting-to-GCC-ARM-Embedded

PROJECT       = DVT
OBJECTS       =  ./lib/system_LPC11Uxx.o ./main.o ./lib/startup_LPC11xx.o
INCLUDE_PATHS = -I. -I./lib/api
LINKER_SCRIPT = ./lib/LPC11U24.ld
MBED_DIR      = $(shell if test -e .mbed_dir ; then cat .mbed_dir ; fi)



############################################################################### 
CC      = arm-none-eabi-gcc
CPP     = arm-none-eabi-g++
LD      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size

ifdef map
	MAP_CODE = -Wl,-Map=$(PROJECT).map,--cref
endif


CPU = -mcpu=cortex-m0 -mthumb
CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP
CC_SYMBOLS = -Os

LD_FLAGS = $(CPU) -Wl,--gc-sections --specs=nano.specs $(MAP_CODE)
LD_SYS_LIBS = -lc -lnosys

CLEAN_MBED = rm -f $(MBED_DIR)/*.bin


.PHONY: all clean lst size deploy

all: $(PROJECT).bin size


clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).lst $(OBJECTS) $(DEPS)
	@if ! test -z $(MBED_DIR) ; then echo "$(CLEAN_MBED)" ; $(CLEAN_MBED) ; fi

deploy: $(MBED_DIR)/$(PROJECT).bin

.S.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99 $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) -o $@ $<


$(PROJECT).elf: $(OBJECTS)
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) -o $@ $^ $(LD_SYS_LIBS)

	@echo ""
	@echo "*****"
	@echo "***** You must modify vector checksum value in *.bin and *.hex files."
	@echo "*****"
	@echo ""

$(MBED_DIR)/$(PROJECT).bin: $(PROJECT).bin
	@if ! [ -z $(MBED_DIR) ] ; then \
		cp $< $@ ; \
	else \
		echo "[ERROR] MBED_DIR NOT SET" ; \
		echo "[INFO] Create a file called .mbed_dir and set your mbed directory there" ; \
		echo "[INFO] This should not contain the trailing slash" ; \
		echo "[INFO] EG: echo /media/mohit/MBED > .mbed_dir" ; \
		exit 1 ; \
	fi

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -SDhx $< > $@

lst: $(PROJECT).lst

size: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)


