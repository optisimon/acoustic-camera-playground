BIN := acoustic_camera_test
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC := $(call rwildcard,src,*.cpp)
PREFIX ?= /usr/local

PACKAGES := jsoncpp opencv

LDFLAGS = $(shell env PKG_CONFIG_PATH=external pkg-config --libs $(PACKAGES))
# -fopt-info-vec-missed -march=native -ftree-vectorize -ftree-vectorizer-verbose=2
CFLAGS = -O3 -Wall -Wextra -ggdb $(shell env PKG_CONFIG_PATH=external pkg-config --cflags $(PACKAGES)) 
CPPFLAGS = -I src
CXXFLAGS = -std=c++14
BUILDDIR := build/bin
OBJ := $(SRC:%.cpp=$(BUILDDIR)/%.o)
DEP := $(OBJ:.o=.d)
BUILDDIRS := $(sort $(dir $(OBJ)))
BUILDDIRS_T := $(addsuffix .touched,$(BUILDDIRS))


all: $(BIN)

$(BIN): $(BUILDDIR)/$(BIN)
	ln -fs $(BUILDDIR)/$(BIN) $(BIN)

$(BUILDDIR)/$(BIN): $(OBJ) 
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: %.cpp $(BUILDDIR)/%.d Makefile | $(BUILDDIRS_T)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CFLAGS) -c -o $@ $<

.PRECIOUS: $(BUILDDIR)/%.d
$(BUILDDIR)/%.d: %.cpp Makefile | $(BUILDDIRS_T)
	@$(CPP) $(CPPFLAGS) $(CXXFLAGS) $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

$(BUILDDIRS_T):
	@mkdir -p $(dir $@)
	@touch $@

.PHONY: cppcheck
cppcheck:
	cppcheck --enable=all --std=c++11 --library=std.cfg --verbose \
	         --inline-suppr $(SRC)

include Makefile_test

.PHONY: clean
clean::
	rm -rf $(BUILDDIR) $(BIN)

prepare:
	apt-get install libjsoncpp-dev

-include $(wildcard $(DEP))
