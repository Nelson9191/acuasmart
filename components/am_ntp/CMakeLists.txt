# include paths of this components.
set(COMPONENT_ADD_INCLUDEDIRS include)

# source files of this components.
set(COMPONENT_SRCDIRS src)

# component dependencies on other components
set(COMPONENT_REQUIRES am_udp am_rtc)


# add this components, this will define a CMake library target.
register_component()