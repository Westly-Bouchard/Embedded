# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(lab10_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(lab10_default_default_XC8_FILE_TYPE_assemble)
add_library(lab10_default_default_XC8_assemble OBJECT ${lab10_default_default_XC8_FILE_TYPE_assemble})
    lab10_default_default_XC8_assemble_rule(lab10_default_default_XC8_assemble)
    list(APPEND lab10_default_library_list "$<TARGET_OBJECTS:lab10_default_default_XC8_assemble>")
endif()

# Handle files with suffix S, for group default-XC8
if(lab10_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(lab10_default_default_XC8_assemblePreprocess OBJECT ${lab10_default_default_XC8_FILE_TYPE_assemblePreprocess})
    lab10_default_default_XC8_assemblePreprocess_rule(lab10_default_default_XC8_assemblePreprocess)
    list(APPEND lab10_default_library_list "$<TARGET_OBJECTS:lab10_default_default_XC8_assemblePreprocess>")
endif()

# Handle files with suffix [cC], for group default-XC8
if(lab10_default_default_XC8_FILE_TYPE_compile)
add_library(lab10_default_default_XC8_compile OBJECT ${lab10_default_default_XC8_FILE_TYPE_compile})
    lab10_default_default_XC8_compile_rule(lab10_default_default_XC8_compile)
    list(APPEND lab10_default_library_list "$<TARGET_OBJECTS:lab10_default_default_XC8_compile>")
endif()

add_executable(lab10_default_image_vG7fRCyr ${lab10_default_library_list})

set_target_properties(lab10_default_image_vG7fRCyr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${lab10_default_output_dir})
set_target_properties(lab10_default_image_vG7fRCyr PROPERTIES OUTPUT_NAME "default")
set_target_properties(lab10_default_image_vG7fRCyr PROPERTIES SUFFIX ".elf")

target_link_libraries(lab10_default_image_vG7fRCyr PRIVATE ${lab10_default_default_XC8_FILE_TYPE_link})


# Add the link options from the rule file.
lab10_default_link_rule(lab10_default_image_vG7fRCyr)



