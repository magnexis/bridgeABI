# Generate BridgeABI artifacts as a build dependency.
function(bridgeabi_generate)
  cmake_parse_arguments(BRG "" "TARGET;OUTPUT_DIRECTORY" "SOURCES" ${ARGN})
  if(NOT BRG_TARGET OR NOT BRG_SOURCES)
    message(FATAL_ERROR "bridgeabi_generate requires TARGET and SOURCES")
  endif()
  if(NOT BRG_OUTPUT_DIRECTORY)
    set(BRG_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bridgeabi-generated")
  endif()
  foreach(source IN LISTS BRG_SOURCES)
    get_filename_component(name "${source}" NAME_WE)
    add_custom_command(OUTPUT "${BRG_OUTPUT_DIRECTORY}/.bridgeabi-${name}.stamp"
      COMMAND bridgeabi generate "${source}" --output "${BRG_OUTPUT_DIRECTORY}"
      COMMAND ${CMAKE_COMMAND} -E touch "${BRG_OUTPUT_DIRECTORY}/.bridgeabi-${name}.stamp"
      DEPENDS "${source}" bridgeabi_cli
      VERBATIM)
    list(APPEND generated_stamps "${BRG_OUTPUT_DIRECTORY}/.bridgeabi-${name}.stamp")
  endforeach()
  add_custom_target(${BRG_TARGET} DEPENDS ${generated_stamps})
endfunction()
