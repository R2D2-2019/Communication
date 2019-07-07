#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

SEARCH  += $(external_comm)/code/headers
SEARCH  += $(external_comm)/code/src

# source files in this library
SOURCES += $(wildcard $(external_comm)/code/src/*.cpp)

# header files for the library
HEADERS += $(wildcard $(external_comm)/code/headers/*.hpp)

# search for queue
SEARCH += $(external_comm)/../datastructures/code/headers
HEADERS += $(wildcard $(extrnal_comm)/../datastructures/code/headers/queue.hpp)

# search for internal communication 
SEARCH += $(external_comm)/../internal_communication/code/headers
HEADERS += $(wildcard $(external_comm)/../internal_communication/code/headers/*.hpp)

