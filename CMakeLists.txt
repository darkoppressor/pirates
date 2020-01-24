cmake_minimum_required(VERSION 3.2.2)
project(pirates)

set(SOURCE_FILES
button_events_game.cpp
console_commands_defs.cpp
data_manager_defs.cpp
dev_info.cpp
directories_defs.cpp
engine_glue.cpp
game.cpp
game.rc
game_constants.cpp
game_data.cpp
game_input_defs.cpp
game_manager_defs.cpp
game_options.cpp
main.cpp
network_game.cpp
special_info.cpp
version.cpp
window_close_function.cpp
window_scrolling_buttons.cpp
)

########################################################################################################################
# Release-Linux-x86_64
########################################################################################################################

add_executable(Release-Linux-x86_64 ${SOURCE_FILES})

set_target_properties(Release-Linux-x86_64 PROPERTIES
OUTPUT_NAME Pirates-Linux-x86_64
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_LINUX"
)

target_include_directories(Release-Linux-x86_64 PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/linux-x86_64/SDL2/include/
/home/tails/build-server/linux-x86_64/SDL2_image/
/home/tails/build-server/linux-x86_64/SDL2_mixer/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/linux-x86_64/raknet/
/home/tails/build-server/linux-x86_64/zlib/
/home/tails/build-server/cheese-engine
)

target_link_libraries(Release-Linux-x86_64
/home/tails/build-server/cheese-engine/libCheese-Engine-Linux-x86_64.a
/home/tails/build-server/linux-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/linux-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/linux-x86_64/libpng/.libs/libpng.a
/home/tails/build-server/linux-x86_64/zlib/libz.a
/home/tails/build-server/linux-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/linux-x86_64/libvorbis/lib/.libs/libvorbisfile.a
/home/tails/build-server/linux-x86_64/libvorbis/lib/.libs/libvorbis.a
/home/tails/build-server/linux-x86_64/libogg/src/.libs/libogg.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2main.a
/usr/lib/x86_64-linux-gnu/libGL.so
/usr/lib/x86_64-linux-gnu/libGLU.so
/usr/lib/x86_64-linux-gnu/libpthread.so
/usr/lib/x86_64-linux-gnu/libdl.so
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/linux-x86_64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/linux64/libsteam_api.so
"-s -m64"
)

########################################################################################################################
# Debug-Linux-x86_64
########################################################################################################################

add_executable(Debug-Linux-x86_64 ${SOURCE_FILES})

set_target_properties(Debug-Linux-x86_64 PROPERTIES
OUTPUT_NAME Pirates-Linux-Debug-x86_64
COMPILE_FLAGS "-std=c++11 -Wall -Wextra -g -m64 -DGAME_OS_LINUX"
)

target_include_directories(Debug-Linux-x86_64 PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/linux-x86_64/SDL2/include/
/home/tails/build-server/linux-x86_64/SDL2_image/
/home/tails/build-server/linux-x86_64/SDL2_mixer/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/linux-x86_64/raknet/
/home/tails/build-server/linux-x86_64/zlib/
/home/tails/build-server/cheese-engine
)

target_link_libraries(Debug-Linux-x86_64
/home/tails/build-server/cheese-engine/libCheese-Engine-Linux-Debug-x86_64.a
/home/tails/build-server/linux-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/linux-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/linux-x86_64/libpng/.libs/libpng.a
/home/tails/build-server/linux-x86_64/zlib/libz.a
/home/tails/build-server/linux-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/linux-x86_64/libvorbis/lib/.libs/libvorbisfile.a
/home/tails/build-server/linux-x86_64/libvorbis/lib/.libs/libvorbis.a
/home/tails/build-server/linux-x86_64/libogg/src/.libs/libogg.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2main.a
/usr/lib/x86_64-linux-gnu/libGL.so
/usr/lib/x86_64-linux-gnu/libGLU.so
/usr/lib/x86_64-linux-gnu/libpthread.so
/usr/lib/x86_64-linux-gnu/libdl.so
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/linux-x86_64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/linux64/libsteam_api.so
"-m64"
)

########################################################################################################################
# Release-Windows-x86_64
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_executable(Release-Windows-x86_64 ${SOURCE_FILES})

set_target_properties(Release-Windows-x86_64 PROPERTIES
OUTPUT_NAME Pirates-Windows-x86_64
SUFFIX .exe
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_WINDOWS"
)

target_include_directories(Release-Windows-x86_64 PRIVATE
/usr/x86_64-w64-mingw32/include
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/windows-x86_64/SDL2/include/
/home/tails/build-server/windows-x86_64/SDL2_image/
/home/tails/build-server/windows-x86_64/SDL2_mixer/
/home/tails/build-server/windows-x86_64/boost/
/home/tails/build-server/windows-x86_64/raknet/
/home/tails/build-server/windows-x86_64/zlib/
/home/tails/build-server/cheese-engine
)

target_link_libraries(Release-Windows-x86_64
/usr/x86_64-w64-mingw32/lib/libmingw32.a
/home/tails/build-server/cheese-engine/libCheese-Engine-Windows-x86_64.a
/home/tails/build-server/windows-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/windows-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/windows-x86_64/libpng/.libs/libpng.a
/home/tails/build-server/windows-x86_64/zlib/libz.a
/home/tails/build-server/windows-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/windows-x86_64/libvorbis/lib/.libs/libvorbisfile.a
/home/tails/build-server/windows-x86_64/libvorbis/lib/.libs/libvorbis.a
/home/tails/build-server/windows-x86_64/libogg/src/.libs/libogg.a
/home/tails/build-server/windows-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/windows-x86_64/SDL2/build/.libs/libSDL2main.a
/home/tails/build-server/windows-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/windows-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/windows-x86_64/raknet/raknet/Lib/DLL/Lib/libRakNet.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/win64/steam_api64.lib
/usr/x86_64-w64-mingw32/lib/libhid.a
/usr/x86_64-w64-mingw32/lib/libsetupapi.a
/usr/x86_64-w64-mingw32/lib/libws2_32.a
/usr/x86_64-w64-mingw32/lib/libwinmm.a
/usr/x86_64-w64-mingw32/lib/libole32.a
/usr/x86_64-w64-mingw32/lib/libimm32.a
/usr/x86_64-w64-mingw32/lib/libgdi32.a
/usr/x86_64-w64-mingw32/lib/liboleaut32.a
/usr/x86_64-w64-mingw32/lib/libversion.a
"-s -m64 -Wl,--stack=67108864 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic"
)

endif()

########################################################################################################################
# Release-macOS-x86_64
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_executable(Release-macOS-x86_64 ${SOURCE_FILES})

set_target_properties(Release-macOS-x86_64 PROPERTIES
OUTPUT_NAME Pirates-macOS-x86_64
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -stdlib=libc++ -DGAME_OS_MACOS"
)

target_include_directories(Release-macOS-x86_64 PRIVATE
/home/tails/build-server/osx-x86_64/osxcross/target/SDK/MacOSX.sdk/usr/include/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/osx-x86_64/SDL2/include/
/home/tails/build-server/osx-x86_64/SDL2_image/
/home/tails/build-server/osx-x86_64/SDL2_mixer/
/home/tails/build-server/osx-x86_64/boost/
/home/tails/build-server/osx-x86_64/raknet/
/home/tails/build-server/osx-x86_64/zlib/
/home/tails/build-server/cheese-engine
)

target_link_libraries(Release-macOS-x86_64
/home/tails/build-server/cheese-engine/libCheese-Engine-macOS-x86_64.a
/home/tails/build-server/osx-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/osx-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/osx-x86_64/libpng/.libs/libpng.a
/home/tails/build-server/osx-x86_64/zlib/libz.a
/home/tails/build-server/osx-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/osx-x86_64/libvorbis/lib/.libs/libvorbis.a
/home/tails/build-server/osx-x86_64/libvorbis/lib/.libs/libvorbisfile.a
/home/tails/build-server/osx-x86_64/libogg/src/.libs/libogg.a
/home/tails/build-server/osx-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/osx-x86_64/SDL2/build/.libs/libSDL2main.a
/home/tails/build-server/osx-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/osx-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/osx-x86_64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/osx/libsteam_api.dylib
"-m64 -arch x86_64 -mmacosx-version-min=10.9 -stdlib=libc++ -liconv -framework CoreFoundation -framework ApplicationServices -framework IOKit -framework AppKit -framework Carbon -framework ForceFeedback -framework CoreAudio -framework AudioUnit -framework AudioToolbox -framework OpenGL -framework CoreVideo"
)

endif()

########################################################################################################################
# Android-NoBuild
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_executable(Android-NoBuild ${SOURCE_FILES})

set_target_properties(Android-NoBuild PROPERTIES
OUTPUT_NAME Pirates-Android
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_ANDROID"
)

target_include_directories(Android-NoBuild PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/android/SDL2/include/
/home/tails/build-server/android/SDL2_image/
/home/tails/build-server/android/SDL2_mixer/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/android/raknet/
)

target_link_libraries(Android-NoBuild
"-s -m64"
)

endif()
