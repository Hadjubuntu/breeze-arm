
######################################################################################
# GNU GCC ARM Embeded Toolchain base directories and binaries 
######################################################################################
GCC_BASE = /usr/
GCC_BIN  = $(GCC_BASE)bin/
GCC_LIB  = $(GCC_BASE)arm-none-eabi/lib/
GCC_INC  = $(GCC_BASE)arm-none-eabi/include/
AS       = $(GCC_BIN)arm-none-eabi-as
CC       = $(GCC_BIN)arm-none-eabi-gcc
CPP      = $(GCC_BIN)arm-none-eabi-g++
LD       = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY  = $(GCC_BIN)arm-none-eabi-objcopy
DISAS 	 = $(GCC_BIN)arm-none-eabi-objdump


######################################################################################
# Custom options for cortex-m and cortex-r processors 
######################################################################################
CORTEX_M0PLUS_CC_FLAGS  = -mthumb -mcpu=cortex-m0plus
CORTEX_M0PLUS_LIB_PATH  = $(GCC_LIB)armv6-m
CORTEX_M0_CC_FLAGS      = -mthumb -mcpu=cortex-m0
CORTEX_M0_LIB_PATH      = $(GCC_LIB)armv6-m
CORTEX_M1_CC_FLAGS      = -mthumb -mcpu=cortex-m1
CORTEX_M1_LIB_PATH      = $(GCC_LIB)armv6-m
CORTEX_M3_CC_FLAGS      = -mthumb -mcpu=cortex-m3
CORTEX_M3_LIB_PATH      = $(GCC_LIB)armv7-m
CORTEX_M4_NOFP_CC_FLAGS = -mthumb -mcpu=cortex-m4
CORTEX_M4_NOFP_LIB_PATH = $(GCC_LIB)armv7e-m
CORTEX_M4_SWFP_CC_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CORTEX_M4_SWFP_LIB_PATH = $(GCC_LIB)armv7e-m/softfp
CORTEX_M4_HWFP_CC_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
CORTEX_M4_HWFP_LIB_PATH = $(GCC_LIB)armv7e-m/fpu
CORTEX_R4_NOFP_CC_FLAGS = -mthumb -march=armv7-r
CORTEX_R4_NOFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb
CORTEX_R4_SWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=softfp -mfpu=vfpv3-d16
CORTEX_R4_SWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/softfp
CORTEX_R4_HWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=hard -mfpu=vfpv3-d16
CORTEX_R4_HWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/fpu
CORTEX_R5_NOFP_CC_FLAGS = -mthumb -march=armv7-r
CORTEX_R5_NOFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb
CORTEX_R5_SWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=softfp -mfpu=vfpv3-d16
CORTEX_R5_SWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/softfp
CORTEX_R5_HWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=hard -mfpu=vfpv3-d16
CORTEX_R5_HWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/fpu


######################################################################################
# Main makefile project configuration
#    PROJECT      = <name of the target to be built>
#    MCU_CC_FLAGS = <one of the CC_FLAGS above>
#    MCU_LIB_PATH = <one of the LIB_PATH above>
#    OPTIMIZE_FOR = < SIZE or nothing >
#    DEBUG_LEVEL  = < -g compiler option or nothing >
#    OPTIM_LEVEL  = < -O compiler option or nothing >
######################################################################################
PROJECT           = breeze
MCU_CC_FLAGS      = $(CORTEX_M4_HWFP_CC_FLAGS)
MCU_LIB_PATH      = $(CORTEX_M4_HWFP_LIB_PATH)
OPTIMIZE_FOR      = 
DEBUG_LEVEL       = 
OPTIM_LEVEL       = 
LINKER_SCRIPT     = ./cortex-rm.ld
PROJECT_OBJECTS   = main.o
PROJECT_INC_PATHS = -I.
PROJECT_LIB_PATHS = -L.
PROJECT_LIBRARIES =
PROJECT_SYMBOLS   = -DTOOLCHAIN_GCC_ARM -DNO_RELOC='0'  


######################################################################################
# Main makefile system configuration
######################################################################################
SYS_OBJECTS = 
SYS_INC_PATHS = -I. -I$(GCC_INC) 
SYS_LIB_PATHS = -L$(MCU_LIB_PATH)
ifeq (OPTIMIZE_FOR, SIZE)
SYS_LIBRARIES = -lstdc++_s -lsupc++_s -lm -lc_s -lg_s -lnosys
SYS_LD_FLAGS  = --specs=nano.specs -u _printf_float -u _scanf_float
else 
SYS_LIBRARIES = -lstdc++ -lsupc++ -lm -lc -lg -lnosys
SYS_LD_FLAGS  = 
endif


############################################################################### 
# Command line building
###############################################################################
ifdef DEBUG_LEVEL
CC_DEBUG_FLAGS = -g$(DEBUG_LEVEL)
CC_SYMBOLS = -DDEBUG $(PROJECT_SYMBOLS)
else
CC_DEBUG_FLAGS =
CC_SYMBOLS = -DNODEBUG $(PROJECT_SYMBOLS)
endif 

ifdef OPTIM_LEVEL
CC_OPTIM_FLAGS = -O$(OPTIM_LEVEL)
else 
CC_OPTIM_FLAGS = 
endif

INCLUDE_PATHS = $(PROJECT_INC_LIB) $(SYS_INC_PATHS)
LIBRARY_PATHS = $(PROJECT_LIB_LIB) $(SYS_LIB_PATHS)
CC_FLAGS = $(MCU_CC_FLAGS) -c $(CC_OPTIM_FLAGS) $(CC_DEBUG_FLAGS) -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections 
LD_FLAGS = $(MCU_CC_FLAGS) -Wl,--gc-sections $(SYS_LD_FLAGS) -Wl,-Map=$(PROJECT).map 
LD_SYS_LIBS = $(SYS_LIBRARIES)

BUILD_PATH = build
BUILD_TARGET = $(BUILD_PATH)/$(PROJECT)

   MCU := STM32F103RE
   PRODUCT_ID := 0003
   ERROR_LED_PORT := GPIOA
   ERROR_LED_PIN := 5
   DENSITY := STM32_HIGH_DENSITY
   FLASH_SIZE := 524288
   SRAM_SIZE := 65536

UPLOADER := dfu-util
USBID := 1EAF:0003
PRODUCT_ID := 0003
LD_MEM_DIR := sram_64k_flash_512k

###############################################################################
# Library
###############################################################################
LIB_PATH = lib
COREOBJS = #$(LIB_PATH)/libmaple/*.o $(LIB_PATH)/libmaple/usb/stm32f1/*.o $(LIB_PATH)/libmaple/stm32f1/*.o $(LIB_PATH)/libmaple/usb/usb_lib/*.o $(LIB_PATH)/libmaple/usb/usb_lib/*.o $(LIB_PATH)/libmaple/stm32f1/performance/*.o $(LIB_PATH)/wirish/*.o $(LIB_PATH)/wirish/boards/maple/*.o $(LIB_PATH)/wirish/stm32f1/*.o $(LIB_PATH)/libraries/Wire/*.o
COREINCLUDES = -I$(LIB_PATH)/libmaple -I$(LIB_PATH)/wirish -I$(LIB_PATH)/libraries  -I$(LIB_PATH)/libmaple/include/libmaple -I$(LIB_PATH)/wirish/include/wirish   -I$(LIB_PATH)/libmaple/include -I$(LIB_PATH)/libmaple/stm32f1/include -I$(LIB_PATH)/wirish/include -I$(LIB_PATH)/wirish/boards/maple/include  -I$(LIB_PATH)/Wire

# Library object files
LIBOBJS			:=	$(COREOBJS)


#
# Build sketch objects
#
#SKETCH_INCLUDES	= $(COREINCLUDES)
#SRCROOT = lib/libmaple
##
#$(LIB_PATH)/libmaple/*.o: $(LIB_PATH)/libmaple/*.cpp
#	$(CPP) $(CC_FLAGS) -c -o $@ $< $(SKETCH_INCLUDES)
##
#$(LIB_PATH)/*.o: $(SRCROOT)/*.c
#	$(v)$(CC) $(CC_FLAGS) -c -o $@ $< $(SKETCH_INCLUDES)
##
#$(LIB_PATH)/*.o: $(SRCROOT)/*.S
#	$(v)$(AS) $(CC_FLAGS) -c -o $@ $< $(SKETCH_INCLUDES)
##	
##	
##
#$(LIB_PATH)/libmaple.a: $(BUILDDIRS) $(TGT_BIN)
#	- rm -f $@
#	$(AR) crv $(LIB_PATH)/libmaple.a $(TGT_BIN)
##
#library: $(BUILD_PATH)/libmaple.a


############################################################################### 
# Makefile execution
###############################################################################

all: $(BUILD_TARGET).bin

flymaple-upload: upload

.PHONY: upload


clean:
	rm -f $(BUILD_TARGET).bin $(BUILD_TARGET).elf $(PROJECT_OBJECTS) $(BUILD_PATH)/*
	
	
.s.o:
	$(AS) $(MCU_CC_FLAGS) -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=c++11 $(INCLUDE_PATHS) -o $@ $<
	
$(PROJECT_OBJECTS): src/main.cpp
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=c++11 $(INCLUDE_PATHS) -Lstaticlib/libmaple.a -o $@ $<
	
#$(BUILD_PATH)/main.o: src/main.cpp
#	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=c++11 $(INCLUDE_PATHS) $(LIBMAPLE_INCLUDES) $(WIRISH_INCLUDES) -o $@ -c $< 

$(BUILD_TARGET).elf: $(PROJECT_OBJECTS) $(SYS_OBJECTS) $(LIBOBJS)
	$(LD) $(LD_FLAGS)  $(LIBRARY_PATHS) -o $@ $^ $(PROJECT_LIBRARIES) $(SYS_LIBRARIES) $(PROJECT_LIBRARIES) $(SYS_LIBRARIES) -Lstaticlib/libmaple.a

#removed : -T$(LINKER_SCRIPT)
# OLD bin elf $(BUILD_TARGET).bin: $(BUILD_TARGET).elf
	# $(OBJCOPY) -O binary $< $@

$(BUILD_TARGET).bin: $(BUILD_TARGET).elf
	$(OBJCOPY) -v -Obinary $(BUILD_TARGET).elf $@ 1>/dev/null
	$(DISAS) -d $(BUILD_TARGET).elf > $(BUILD_TARGET).disas
	
upload: $(SKETCHBIN)
	scripts/reset.py && sleep 1 &&  $(UPLOADER) -a1 -d $(USBID) -D $(BUILD_TARGET).bin -R
