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
	src/builtin \
	src/builtin/std \
	src/Evaluator \
	src/GC \
	src/Lexer \
	src/Parser \
	src/Semantics \
	src/Types

BASEFLAGS		:= -O2
DEBUGFLAGS		:= -DMETRO_DEBUG=0
CFLAGS			:= $(BASEFLAGS) $(INCLUDES) -Wno-switch
CXXFLAGS		:= $(CFLAGS) -std=c++20
LDFLAGS			:= -Wl,--gc-sections -pthread

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

.PHONY: all debug install clean re  $(BUILD)

all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $@ -f $(CURDIR)/Makefile

debug:
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@$(MAKE) --no-print-directory BASEFLAGS="-O0 -g" DEBUGFLAGS="-DMETRO_DEBUG=1" LDFLAGS="" -C $(BUILD) -f $(CURDIR)/Makefile

install: all
	@install $(notdir $(OUTPUT)) $(BINDIR)/$(TARGET)

clean:
	@rm -rf $(BUILD) $(OUTPUT)

re: clean all

else

DEPENDS	= $(OFILES:.o=.d)

$(OUTPUT): $(OFILES)
	@echo linking...
	@$(LD) $(LDFLAGS) -o $@ $^

-include $(DEPENDS)

endif