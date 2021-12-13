################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/msp430

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./MSP-EXP430F5438 HAL/hal_usb.obj" \
"./MSP-EXP430F5438 HAL/hal_rf.obj" \
"./MSP-EXP430F5438 HAL/hal_lcd.obj" \
"./MSP-EXP430F5438 HAL/hal_buttons.obj" \
"./MSP-EXP430F5438 HAL/hal_board.obj" \
"./MSP-EXP430F5438 HAL/hal_adc.obj" \
"./MSP-EXP430F5438 HAL/RTCASMFunctions_CCE.obj" \
"./UserExperienceDemo/fsm_transition.obj" \
"./UserExperienceDemo/fsm.obj" \
"./UserExperienceDemo/flashUtils.obj" \
"./UserExperienceDemo/UserExperience.obj" \
"./UserExperienceDemo/Puzzle.obj" \
"./main.obj" \
-l"libc.a" \
"../lnk_msp430f5438a.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include UserExperienceDemo/subdir_vars.mk
-include MSP-EXP430F5438\ HAL/subdir_vars.mk
-include subdir_rules.mk
-include UserExperienceDemo/subdir_rules.mk
-include MSP-EXP430F5438\ HAL/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: FSM\ Demo.out

# Tool invocations
FSM\ Demo.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmspx --abi=coffabi -O0 -g --define=__MSP430F5438A__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal -z -m"FSM Demo.map" --stack_size=160 --heap_size=160 --use_hw_mpy=F5 -i"C:/ti/ccsv5/ccs_base/msp430/include" -i"C:/ti/ccsv5/tools/compiler/msp430/lib" -i"C:/ti/ccsv5/tools/compiler/msp430/include" --reread_libs --warn_sections --rom_model -o "FSM Demo.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(MSP430_EXECUTABLE_OUTPUTS__QUOTED) "FSM Demo.out"
	-$(RM) "main.pp" "UserExperienceDemo\Puzzle.pp" "UserExperienceDemo\UserExperience.pp" "UserExperienceDemo\flashUtils.pp" "UserExperienceDemo\fsm.pp" "UserExperienceDemo\fsm_transition.pp" "MSP-EXP430F5438 HAL\hal_adc.pp" "MSP-EXP430F5438 HAL\hal_board.pp" "MSP-EXP430F5438 HAL\hal_buttons.pp" "MSP-EXP430F5438 HAL\hal_lcd.pp" "MSP-EXP430F5438 HAL\hal_rf.pp" "MSP-EXP430F5438 HAL\hal_usb.pp" 
	-$(RM) "main.obj" "UserExperienceDemo\Puzzle.obj" "UserExperienceDemo\UserExperience.obj" "UserExperienceDemo\flashUtils.obj" "UserExperienceDemo\fsm.obj" "UserExperienceDemo\fsm_transition.obj" "MSP-EXP430F5438 HAL\RTCASMFunctions_CCE.obj" "MSP-EXP430F5438 HAL\hal_adc.obj" "MSP-EXP430F5438 HAL\hal_board.obj" "MSP-EXP430F5438 HAL\hal_buttons.obj" "MSP-EXP430F5438 HAL\hal_lcd.obj" "MSP-EXP430F5438 HAL\hal_rf.obj" "MSP-EXP430F5438 HAL\hal_usb.obj" 
	-$(RM) "MSP-EXP430F5438 HAL\RTCASMFunctions_CCE.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
