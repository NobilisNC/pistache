add_library(catch2 INTERFACE)
add_library(deps::catch2 ALIAS catch2)

target_include_directories(catch2
	INTERFACE deps/Catch2/single_include
)
