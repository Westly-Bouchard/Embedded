# The following variables contains the files used by the different stages of the build process.
set(lab10_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${lab10_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${lab10_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(lab10_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${lab10_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${lab10_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(lab10_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/adc/src/adc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/capture/src/ccp5.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/pwm/src/eccp1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/spi/src/mssp2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/system/src/clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/system/src/config_bits.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/system/src/interrupt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/system/src/pins.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/system/src/system.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/timer/src/tmr0.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/timer/src/tmr1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/timer/src/tmr2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/uart/src/eusart1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../sdCard.c")
set_source_files_properties(${lab10_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(lab10_default_default_XC8_FILE_TYPE_link)
set(lab10_default_image_name "default.elf")
set(lab10_default_image_base_name "default")

# The output directory of the final image.
set(lab10_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/lab10")

# The full path to the final image.
set(lab10_default_full_path_to_image ${lab10_default_output_dir}/${lab10_default_image_name})
