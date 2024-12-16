cmake_minimum_required(VERSION 3.27)

set(SOURCE_FILES
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/tcp/server.cpp
	${PROJECT_SOURCE_DIR}/src/tcp/detail.cpp
	${PROJECT_SOURCE_DIR}/src/tcp/stream.cpp
	${PROJECT_SOURCE_DIR}/src/http/request.cpp
)

set(INCLUDE_DIR
	${PROJECT_SOURCE_DIR}/inc/
)

# generate binary
add_executable(${PROJECT_NAME}
	${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	ws2_32
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${INCLUDE_DIR}
)

target_link_options(${PROJECT_NAME} PUBLIC
	${LINKER_FLAGS}
)

target_compile_options(${PROJECT_NAME} PUBLIC
	$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>
	$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
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