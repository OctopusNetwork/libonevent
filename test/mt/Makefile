rootdir := $(shell pwd)/../../../..
project := ubuntu-base

include $(rootdir)/build/project/$(project).mk
include $(rootdir)/build/common/common.mk

DEP_HEADERS = $(incdir)

####
##  Application targets
####
apps = mt_event mt_evgrp

mt_event_SRCS = $(rootdir)/source/libonevent/test/mt/event_test_main.c
mt_event_ARCHIVES = onevent onplatform
mt_event_LDLIBS = -pthread
mt_event_DISTSOLIBS =

mt_evgrp_SRCS = $(rootdir)/source/libonevent/test/mt/evgrp_test_main.c
mt_evgrp_ARCHIVES = onevent onplatform
mt_evgrp_LDLIBS = -pthread
mt_evgrp_DISTSOLIBS =

include $(rootdir)/build/utils/app.mk
