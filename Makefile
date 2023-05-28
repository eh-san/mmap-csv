# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC ?= mmap-csv

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
SCRIPT_DIRS ?= ./scripts
EXAMPLE_DIRS ?= $(BUILD_DIR)/example

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

PREFIX ?= /usr/local
BINDIR ?= $(DESTDIR)$(PREFIX)/bin

PYTHON3 ?= python3

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: with-example
with-example: $(BUILD_DIR)/$(TARGET_EXEC)
	@$(PTYHON3) $(SCRIPT_DIRS)/generator.py --count 10 --path $(EXAMPLE_DIRS)

-include $(DEPS)

MKDIR_P ?= mkdir -p

