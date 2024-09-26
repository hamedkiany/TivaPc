################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/CCS/ARM_CM4F/%.obj: ../FreeRTOS/Source/portable/CCS/ARM_CM4F/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10/FreeRTOS/Source/include" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10/remotelink" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

FreeRTOS/Source/portable/CCS/ARM_CM4F/%.obj: ../FreeRTOS/Source/portable/CCS/ARM_CM4F/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10/FreeRTOS/Source/include" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/Users/hamed/OneDrive/micro/examen2016-TIVA-adaptado2020-sol1a8y10/remotelink" --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --define=DEBUG -g --c89 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="FreeRTOS/Source/portable/CCS/ARM_CM4F/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/Source/portable/CCS/ARM_CM4F" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


