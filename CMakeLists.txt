cmake_minimum_required(VERSION 3.20)
 
if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "No In-source builds!
    Please create a subfolder, cd into it and use `cmake ..` inside it.
    If it doesn't exist `mkdir build` will create a directory called build.
    You can use the build folder or create a folder with any name, have fun!
    NOTE: The command you just ran creates CMakeCache.txt and the folder CMakeFiles.
    Unfortunately because you ran the command inside the wrong folder, they are in the wrong place!
    You gotta remove them, or cmake will refuse to work. 
    Don't worry, this isn't a big deal, cmake is a real pain to work with sometimes.")
endif()

project(BloatNote VERSION 0.0.1 LANGUAGES C)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

# huh? Not sure what I was thinking here but it works I guess so I'm not gonna bother modifying it
if(WIN32)
  set(GUI_TYPE WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
  add_definitions(-DWIN32 -D_WINDOWS)
else()
message(FATAL_ERROR "Nuh Uh! You should know better!")
endif()

# Include the icons by default. If you don't want to include them, set BLOATNOTE_INCLUDE_ICONS to OFF
# You can do this by running cmake with -DBLOATNOTE_INCLUDE_ICONS=OFF
option(BLOATNOTE_INCLUDE_ICONS "Include icons in the build" ON)

if(BLOATNOTE_INCLUDE_ICONS)
  set(RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/bloatedicon.ico")
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
else()
  # Tell the user that they are buikding with icons 
  message(WARNING "You are building with icons! This is wiill increase the file size by 5kb! If you want to build without icons, set BLOATNOTE_INCLUDE_ICONS to OFF")
endif()

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(src)

install(
  TARGETS BloatNote
  DESTINATION bin
  )


# CPack stuff. We're gonna use NSIS to make an installer for windows
# If you don't have NSIS installed, don't worry, it's not required to build the project. It's only required to make an installer


set(CPACK_PACKAGE_NAME "BloatNote")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "XtraDepressedGayRainCorps")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C version of BloatNote - An EXTREMELY small text editor with no dependencies, bloat or features! Just kidding, it's got all the features needed..all three of them!")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_START_MENU_SHORTCUTS "bin/BloatNote.exe")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "BloatNote")

set(CPACK_NSIS)
set(CPACK_NSIS_HELP_LINK "https://vilari.be.sexy")
#set (CPACK_NSIS_MODIFY_PATH "ON") # Note, we don't actually want to modify the path as we don't really need it. This is just a hack to aet around some problemns allow the user to set a start menu  and desktop shortcut
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/VilariStorms/BloatPad")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
set(CPACK_NSIS_WELCOME_TITLE "Welcome to BloatNote!")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "BloatNote.exe")

if(BLOATNOTE_INCLUDE_ICONS)
  set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/bloatedicon.ico")
  set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/bloatedicon.ico")
  set(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe'")
  set(CPACK_NSIS_DELETE_ICONS "Delete '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk'")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\BloatNote.exe")
endif()



include(CPack)