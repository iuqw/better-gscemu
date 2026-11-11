CC ?= gcc
CFLAGS ?= -std=c11 -O2 -g -Wall -Wextra
CPPFLAGS ?=
LDFLAGS ?=
LDLIBS ?=

ARCH := $(shell uname -m)
PROJECT_DIR := $(abspath .)
BUILD_DIR := $(abspath $(PROJECT_DIR)/build/$(ARCH))
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BUILD_DIR)/gscemu

SRCS := $(shell find src -name '*.c')
OBJS := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CPPFLAGS += -Iinclude

.DEFAULT_GOAL := all

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
