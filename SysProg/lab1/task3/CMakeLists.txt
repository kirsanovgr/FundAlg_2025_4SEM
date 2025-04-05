add_compile_options(-fsanitize=thread)
add_link_options(-fsanitize=thread)

add_executable(lab1task3 ${LIB_SOURCES} main.h main.c)