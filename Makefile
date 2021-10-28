EXT				= cc

CC				= clang
CXX				= clang++
LD				= clang++

TARGET		= metro

SRCDIR		= src
INCLUDE		= include
BUILD			= build
DEST 			= /usr/local/bin

VERSION 		= $(shell git describe --tags $$(git rev-list --tags --max-count=1) --abbrev=0)
BUILT_DATE	= $(shell date +'%Y-%m-%d')

HEADER		= $(INCLUDE)/$(TARGET).h
CXXFILES	= $(wildcard $(SRCDIR)/*.$(EXT))
OFILES		= $(patsubst %.cc, $(BUILD)/%.o, $(notdir $(CXXFILES)))

OPT				= -O1
CXXFLAGS	= $(OPT) -I $(INCLUDE) -fno-exceptions -std=c++20 -Wall \
	-Wno-pedantic -Wno-switch -Wno-gnu-anonymous-struct \
	-Wno-nested-anon-types -Wno-unused-function -Wno-unused-variable \
	-DVERSION=\"$(VERSION)\" -DBUILT_DATE=\"$(BUILT_DATE)\"

LDFLAGS		= -Wl,--gc-sections

all: $(BUILD) $(TARGET)

clean:
	@rm -rf $(BUILD) $(TARGET)

debug:
	@$(MAKE) --no-print-directory OPT="-O0 -g" LDFLAGS=""

check:
	cppcheck --enable=all --std=c++20 --inconclusive *.cc *.h

re: clean all

install: all
	@echo installing...
	@install -s $(TARGET) $(DEST)

uninstall:
	@echo uninstalling...
	@rm -f $(DEST)/$(TARGET)

$(BUILD):
	@[ -d $@ ] || mkdir $@

$(BUILD)/%.o: $(SRCDIR)/%.$(EXT) $(HEADER)
	@echo $(notdir $<)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET): $(OFILES)
	@echo linking...
	@$(LD) $(LDFLAGS) -o $@ $^