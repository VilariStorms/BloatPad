add_executable(BloatNote
    main.c
)

if (BLOATNOTE_INCLUDE_ICONS)
    set(EXE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../resources/icons/bloatedicon.rc")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${EXE_ICON}")
    target_sources(${PROJECT_NAME} PRIVATE ${EXE_ICON})
endif()

find_library(COMCTL32_LIBRARY comctl32)
target_link_libraries(BloatNote PRIVATE comctl32)