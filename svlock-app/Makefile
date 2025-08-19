ifneq ($(CROSSCOMPILE),)
	CC := $(CROSSCOMPILE)gcc
endif

OBJPRE ?=

# ifneq ($(SIMICS_BASE),)
# VPATH = $(SIMICS_BASE)/src/misc/simicsfs-client
# include $(HOST_DIR)/host-config
# INCLUDES = -I$(SIMICS_BASE)/src/misc/libmagicpipe
# LIBS := $(TARGET_DIR)/libmagicpipe.a
# CFLAGS += $(FUSE_INCLUDE_PATH) -D_FILE_OFFSET_BITS=64
# LIBS := -lmagicpipe -lfuse
# LIBS += /usr/lib/x86_64-linux-gnu/libfuse.a
# LIBS := /usr/lib/simics/libmagicpipe.a
# LIBS += -lpthread
# LIBS += $(LIBFUSE)
# LIBS += -ldl
# LDLIBS=-lcpu -lmrframe -lceg -lrocket -laltair -lares -lale -lsv -lpopt -ldl
#LIBS= -lsv
# else
TARGET_DIR ?= .
CFLAGS += -std=gnu99 -D_FILE_OFFSET_BITS=64
CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += $(EXTRA_LDFLAGS)
# endif

CFLAGS += $(INCLUDES)

OBJS := svlock_app.o
OBJECTS=$(patsubst %,$(OBJPRE)%,$(OBJS))

TARGET := $(TARGET_DIR)/svlock-app

.PHONY: all clean
.DEFAULT: all

all: $(TARGET)

clean:
	${RM} ${TARGET} ${OBJECTS}

$(TARGET): $(OBJECTS)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJPRE)%.o: %.c
	@echo "Compiling $^"
	$(CC) $(CFLAGS) -c -o $@ $^
