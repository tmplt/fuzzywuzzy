file(GLOB SOURCES
        "*.hpp" "*.cpp"
        "*.h" "*.c"
)

add_library(fuzzywuzzy ${SOURCES})
