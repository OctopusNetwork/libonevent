rootdir := $(shell pwd)/../../..
project := ubuntu-base

include $(rootdir)/build/common/inc.mk
ifeq ($(hostmaketype), windows)
rootdir := ../../..
endif

####
##  Application targets
####
DEP_HEADERS +=  $(incdir)/libonevent $(incdir)/libonplatform

apps = mt_event mt_evgrp

mt_event_SRCS = $(rootdir)/linonevent/test/mt/event_test_main.c
mt_event_ARCHIVES = libonevent libonplatform
mt_event_LDLIBS = -pthread
mt_event_DISTSOLIBS =

mt_evgrp_SRCS = $(rootdir)/libonevent/test/mt/evgrp_test_main.c
mt_evgrp_ARCHIVES = libonevent libonplatform
mt_evgrp_LDLIBS = -pthread
mt_evgrp_DISTSOLIBS =

include $(rootdir)/build/utils/app.mk
