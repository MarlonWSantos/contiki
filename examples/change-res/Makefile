CONTIKI_PROJECT = change-res

all: $(CONTIKI_PROJECT)

CONTIKI = ../..

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
LDFLAGS+=-lm
CFLAGS+=-lm
TARGET_LIBFILES+=-lm


REST_RESOURCES_DIR = ./resources
REST_RESOURCES_FILES = $(notdir $(shell find $(REST_RESOURCES_DIR) -name '*.c'))

PROJECTDIRS += $(REST_RESOURCES_DIR)
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

# linker optimizations
SMALL=1

# REST Engine shall use Erbium CoAP implementation
APPS += er-coap
APPS += rest-engine
APPS+=powertrace

CONTIKI_WITH_IPV6 = 1

include $(CONTIKI)/Makefile.include

# minimal-net target is currently broken in Contiki
ifeq ($(TARGET), minimal-net)
CFLAGS += -DHARD_CODED_ADDRESS=\"fdfd::10\"
${info INFO: er-example compiling with large buffers}
CFLAGS += -DUIP_CONF_BUFFER_SIZE=1300
CFLAGS += -DREST_MAX_CHUNK_SIZE=1024
CFLAGS += -DCOAP_MAX_HEADER_SIZE=176
CONTIKI_WITH_RPL=0
endif


