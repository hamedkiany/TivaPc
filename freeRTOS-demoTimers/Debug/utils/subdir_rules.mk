################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
utils/%.obj: ../utils/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/Users/hamed/OneDrive/micro/freeRTOS-demoTimers" --include_path="C:/Users/hamed/OneDrive/micro/freeRTOS-demoTimers/FreeRTOS/Source/include" --include_path="C:/Users/hamed/OneDrive/micro/freeRTOS-demoTimers/FreeRTOS/Source/portable/CCS/ARM_CM4F" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="utils/$(basename $(<F)).d_raw" --obj_directory="utils" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

