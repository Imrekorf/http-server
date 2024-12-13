cmake_minimum_required(VERSION 3.27)

# note this generates a library version of the project contents
# this library can then be used for tests
set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/temp.cpp
)

set(INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/inc/
)

# Define library output
add_library(l${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_link_libraries(l${PROJECT_NAME} PUBLIC
)

target_include_directories(l${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
)

target_link_options(l${PROJECT_NAME} PUBLIC
    ${LINKER_FLAGS}
)

target_compile_options(l${PROJECT_NAME} PUBLIC
    $<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>
    $<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>
)

target_compile_definitions(l${PROJECT_NAME} PUBLIC
    ${DEFINES}

    # Debug definitions
    # $<$<CONFIG:DEBUG>:DEBUG>

    # Release definitions
    $<$<CONFIG:RELEASE>:NDEBUG>

    # C defintions
    # $<$<COMPILE_LANG_AND_ID:C,GNU>:__LANGUAGE=C>
    # C++ definitions
    # $<$<COMPILE_LANG_AND_ID:CXX,GNU>:__LANGUAGE=CXX>
)