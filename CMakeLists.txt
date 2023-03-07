cmake_minimum_required(VERSION 3.20)
project(week_02)


add_executable(pomodoro)
target_sources(pomodoro
        PRIVATE
        pomodoro_main.cpp
        pomodoro.cpp
        ascii_fonts.cpp
)

add_executable(text2art text2art.cpp ascii_fonts.cpp)
target_compile_features(text2art PRIVATE cxx_std_20)
set_target_properties(text2art PROPERTIES CXX_EXTENSIONS OFF)



file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})

target_compile_features(pomodoro PRIVATE cxx_std_20)
set_target_properties(pomodoro PROPERTIES CXX_EXTENSIONS OFF)

# Set options for gnu-like
target_compile_options(pomodoro PRIVATE
        # set warnings for all build types
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Wconversion -Wfloat-equal
        -Wmissing-noreturn -Wpointer-arith -Wundef -Wshadow -Wstack-protector -Wswitch-enum -Og -g -fno-omit-frame-pointer
        -Wno-format-nonliteral -Wno-format-security -Wno-unknown-pragmas>

        # static analyser for GNU in Debug
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:Debug>>:-fanalyzer>
        )


target_compile_options(text2art PRIVATE
        # set warnings for all build types
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Wconversion -Wfloat-equal
        -Wmissing-noreturn -Wpointer-arith -Wundef -Wshadow -Wstack-protector -Wswitch-enum -Og -g -fno-omit-frame-pointer
        -Wno-format-nonliteral -Wno-format-security -Wno-unknown-pragmas>
        )

# automatic gitignore for build directories
if (NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
    file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif ()