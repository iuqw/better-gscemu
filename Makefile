CC ?= gcc
CFLAGS ?= -O2 -g
CFLAGS += -std=c11 -Wall -Wextra -Wformat=2 -Werror=format-security -Werror=format-nonliteral -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE
CPPFLAGS ?=
LDFLAGS ?=
LDFLAGS += -pie -Wl,-z,relro,-z,now
LDLIBS ?=

ARCH := $(shell uname -m)
PROJECT_DIR := $(abspath .)
BUILD_DIR := $(abspath $(PROJECT_DIR)/build/$(ARCH))
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BUILD_DIR)/gscemu

FUZZ_CC ?= clang
FUZZ_TIME ?= 60
FUZZ_ARGS ?=
FUZZ_BIN := $(BUILD_DIR)/fuzz_libargs
FUZZ_SRC := tests/fuzz_libargs.c
FUZZ_DICT := tests/fuzz.dict
FUZZ_CORPUS := $(BUILD_DIR)/fuzz-corpus
FUZZ_CFLAGS := -std=c11 -g -O1 -Wall -Wextra -fsanitize=fuzzer,address,undefined -fno-omit-frame-pointer

SRCS := $(shell find src -name '*.c')
OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CPPFLAGS += -Iinclude

.DEFAULT_GOAL := all

.PHONY: all clean run fuzz

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

fuzz: $(FUZZ_BIN)
	@mkdir -p $(FUZZ_CORPUS)
	$(FUZZ_BIN) -max_total_time=$(FUZZ_TIME) -print_final_stats=1 -dict=$(FUZZ_DICT) $(FUZZ_CORPUS) tests/corpus $(FUZZ_ARGS)

$(FUZZ_BIN): $(FUZZ_SRC) $(FUZZ_DICT) src/libargs.c include/libargs.h
	@mkdir -p $(dir $@)
	$(FUZZ_CC) $(FUZZ_CFLAGS) -Iinclude $(FUZZ_SRC) src/libargs.c -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
