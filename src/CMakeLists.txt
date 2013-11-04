CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)
PROJECT(MetaLibrary)

# is TRUE on Apple OS X
# is TRUE when using the MinGW compiler in Windows

IF(APPLE)
    # Apple specific
    MESSAGE( STATUS "APPLE: " ${APPLE} )
    SET (CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
ENDIF()

IF(WIN32)
    # Windows specific
    MESSAGE( STATUS "MINGW: " ${MINGW} )
    IF(MINGW)
        SET (CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic")
    ENDIF()
ENDIF()

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Binaries
ADD_EXECUTABLE(MetaLibrary test.cpp MetaLibrary.hpp
	Meta/Condition.hpp
	Meta/FlowControl.hpp
	Meta/TupleUtilities.hpp
        Meta/Sequence.hpp)

