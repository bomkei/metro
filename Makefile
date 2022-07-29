CC		= clang
CXX		= clang++
LD		= $(CXX)

TARGET	= metro

BINDIR	= /usr/local/bin

BUILD	= build
INCLUDE	= include
SOURCES	= \
	src \
	src/Application \
	src/Debug \
	src/Error \
	src/GC \
	src/IR \
	src/MetroDriver \
	src/MetroDriver/Compiler \
	src/MetroDriver/Evaluator \
	src/MetroDriver/Lexer \
	src/MetroDriver/Parser \
	src/MetroDriver/Semantics \
	src/Types \
	src/Types/AST \
	src/Utils \
	src/VM

USE_DEBUG			?= 0
DEBUGFLAGS		?=

ifeq ($(USE_DEBUG), 1)
DEBUGFLAGS	:= \
  -DMETRO_DEBUG=1 \
	-DMETRO_DEBUG_USE_ALERT=1 \
	-DMETRO_DEBUG_USE_DEBUGSCOPE=1 \
	-DMETRO_DEBUG_USE_CRASH=1
endif

BASEFLAGS		:= -O2
CFLAGS			:= $(BASEFLAGS) $(DEBUGFLAGS) $(INCLUDES) \
	-Wno-switch -Wimplicit-fallthrough -Wuninitialized
CXXFLAGS		:= $(CFLAGS) -std=c++20
LDFLAGS			:= -Wl,--gc-sections

%.o: %.c
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -MP -MMD -MF $*.d -S -o $*.s $<
	@$(CC) $(CFLAGS) -MP -MMD -MF $*.d -c -o $@ $<

%.o: %.cc
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -MP -MMD -MF $*.d -S -o $*.s $<
	@$(CXX) $(CXXFLAGS) -MP -MMD -MF $*.d -c -o $@ $<

ifneq ($(notdir $(CURDIR)),$(BUILD))

ifeq ($(OS),Windows_NT)
EXT			= exe
else
EXT			= out
endif

export TOPDIR		= $(CURDIR)
export OUTPUT		= $(TOPDIR)/$(TARGET).$(EXT)
export VPATH		= $(foreach dir,$(SOURCES),$(TOPDIR)/$(dir))
export INCLUDES		= $(foreach dir,$(INCLUDE),-I$(TOPDIR)/$(dir))

CFILES			= $(notdir $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c)))
CXXFILES		= $(notdir $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cc)))

export OFILES		= $(CFILES:.c=.o) $(CXXFILES:.cc=.o)

.PHONY: all debug install clean re $(BUILD)

all: $(BUILD)
	clear
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(BUILD):
	@[ -d $@ ] || mkdir -p $@

debug: $(BUILD)
	clear
	@$(MAKE) --no-print-directory BASEFLAGS="-O0 -g" USE_DEBUG="1" LDFLAGS="" -C $(BUILD) -f $(CURDIR)/Makefile

install: all
	@install $(notdir $(OUTPUT)) $(BINDIR)/$(TARGET)

clean:
	@rm -rf $(BUILD) $(OUTPUT)

re: clean all

else

DEPENDS	= $(OFILES:.o=.d)

$(OUTPUT): $(OFILES)
	@echo linking...
	@$(LD) $(LDFLAGS) -pthread -o $@ $^

-include $(DEPENDS)

endif