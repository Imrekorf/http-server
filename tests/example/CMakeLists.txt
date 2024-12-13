cmake_minimum_required(VERSION 3.27)

set(TEST_NAME "TEST_EXAMPLE")

# Define executable output to test library
add_executable(${TEST_NAME}
	test.cpp
)

add_dependencies(${TEST_NAME}
	l${PROJECT_NAME}
)

target_link_libraries(${TEST_NAME} PUBLIC
	l${PROJECT_NAME}
)

target_include_directories(${TEST_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/inc/
)

target_link_options(${TEST_NAME} PUBLIC
	${LINKER_FLAGS}
)

target_compile_options(${TEST_NAME} PUBLIC
	$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>
	$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>
)

target_compile_definitions(${TEST_NAME} PUBLIC
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