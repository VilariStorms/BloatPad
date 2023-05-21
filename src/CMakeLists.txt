add_executable(BloatNote
    main.c
)


find_library(COMCTL32_LIBRARY comctl32)
target_link_libraries(BloatNote PRIVATE comctl32)