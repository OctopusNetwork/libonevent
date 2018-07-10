rootdir := $(shell pwd)/../..
project ?= ubuntu-base

include $(rootdir)/build/project/$(project).mk
include $(rootdir)/build/common/common.mk

TARGET_ARCHIVE = libonevent.a
MODULENAME = libonevent
MODULEVERSION = 0.1.0

COMMON_INCLUDE_DIRS += $(rootdir)/source/$(MODULENAME)/include
COMMON_SRC_FILES := $(rootdir)/source/$(MODULENAME)/src/onlfds.c		\
					$(rootdir)/source/$(MODULENAME)/src/onevent.c		\
					$(rootdir)/source/$(MODULENAME)/src/onevgrp.c
COMMON_INST_HEADER_DIRS += $(rootdir)/source/$(MODULENAME)/include

include $(rootdir)/build/utils/archive.mk

.PHONY : sync
