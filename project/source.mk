COMMON_INCLUDE_DIRS += $(rootdir)/$(MODULE)/include         \
                       $(incdir)/libonplatform

COMMON_SRC_FILES := $(rootdir)/$(MODULE)/src/onevent.c     \
                    $(rootdir)/$(MODULE)/src/onevgrp.c     \
                    $(rootdir)/$(MODULE)/src/onlfds.c

COMMON_INST_HEADER_DIRS += $(rootdir)/$(MODULE)/include
