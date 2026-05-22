add_library(bulk_average
    bulk_average.cpp
)

target_include_directories(bulk_average PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
