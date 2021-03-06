cmake_minimum_required(VERSION 2.8.7)
project("nucleus")

# General options

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Compiler options

include("cmake/gcc.cmake")
include("cmake/msvc.cmake")

# Platform options

include("cmake/win32.cmake")

# Options

# Turn this on to generate projects for unit testing.
option(NUCLEUS_BUILD_UNITTESTS "Build unittests with library?")

# Third party libraries.

if(NUCLEUS_BUILD_UNITTESTS)
  include("cmake/gtest.cmake")
endif()

# nucleus

include_directories("include")

set("nucleus_INCLUDE_FILES"
  "include/nucleus/config.h"
  "include/nucleus/files/file_path.h"
  "include/nucleus/files/file_utils.h"
  "include/nucleus/logging.h"
  "include/nucleus/macros.h"
  "include/nucleus/message_loop/incoming_task_queue.h"
  "include/nucleus/message_loop/message_loop.h"
  "include/nucleus/message_loop/message_pump.h"
  "include/nucleus/message_loop/message_pump_default.h"
  "include/nucleus/message_loop/pending_task.h"
  "include/nucleus/streams/file_input_stream.h"
  "include/nucleus/streams/input_stream.h"
  "include/nucleus/streams/memory_input_stream.h"
  "include/nucleus/streams/wrapped_memory_input_stream.h"
  "include/nucleus/strings/char_ptr.h"
  "include/nucleus/strings/char_ptr_traits_16.h"
  "include/nucleus/strings/char_ptr_traits_32.h"
  "include/nucleus/strings/char_ptr_traits_8.h"
  "include/nucleus/strings/char_ptr_traits_a.h"
  "include/nucleus/strings/char_ptr_traits_w.h"
  "include/nucleus/strings/string_utf_conversion.h"
  "include/nucleus/strings/string_utils.h"
  "include/nucleus/types.h"
  "include/nucleus/utils/byte_order.h"
  "include/nucleus/utils/detail/observer_list_base.h"
  "include/nucleus/utils/observer_list.h"
  "include/nucleus/utils/signals.h"
  "include/nucleus/utils/stl.h"
  "include/nucleus/win/windows_mixin.h"
)

set("nucleus_SOURCE_FILES"
  "src/files/file_path.cpp"
  "src/logging.cpp"
  "src/message_loop/incoming_task_queue.cpp"
  "src/message_loop/message_loop.cpp"
  "src/message_loop/message_pump.cpp"
  "src/message_loop/message_pump_default.cpp"
  "src/streams/file_input_stream.cpp"
  "src/streams/input_stream.cpp"
  "src/streams/memory_input_stream.cpp"
  "src/streams/wrapped_memory_input_stream.cpp"
  "src/strings/string_utf_conversion.cpp"
  "src/strings/string_utils.cpp"
)

if(WIN32)
  list(APPEND "nucleus_SOURCE_FILES"
    "src/files/file_utils_win.cpp"
    "src/streams/file_input_stream_win.cpp"
  )
endif()

if(NUCLEUS_BUILD_UNITTESTS)
  set("nucleus_TEST_FILES"
    "tests/files/file_path_tests.cpp"
    "tests/logging_test.cpp"
    "tests/message_loop/message_loop_tests.cpp"
    "tests/strings/char_ptr_tests.cpp"
    "tests/strings/string_utf_conversion_tests.cpp"
    "tests/strings/string_utils_tests.cpp"
    "tests/utils/observer_list_tests.cpp"
    "tests/utils/signals_tests.cpp"
  )
endif()

add_library("nucleus" STATIC ${nucleus_INCLUDE_FILES} ${nucleus_SOURCE_FILES})
target_include_directories("nucleus" PUBLIC "${nucleus_SOURCE_DIR}/include")
set_property(TARGET "nucleus" PROPERTY FOLDER "nucleus")

if(NUCLEUS_BUILD_UNITTESTS)
  add_executable("nucleus_tests" ${nucleus_TEST_FILES})
  target_link_libraries("nucleus_tests" "gtest" "gtest_main" "nucleus")
  set_property(TARGET "nucleus_tests" PROPERTY FOLDER "nucleus")
endif()
