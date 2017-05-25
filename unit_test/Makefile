#
# Makefile文件
# author: liangshiping
#

#c程序编译器.
CC = gcc

#cpp程序编译器.
CXX = g++

# Un-comment the following line to compile C programs as C++ ones.
CC = $(CXX)

#自己想要添加的选项  如：添加支持c++11标准(-std=c++11)
MY_CFLAGS =

# The pre-processor options used by the cpp (man cpp for more).
CPPFLAGS = -Wall

# The pre-processor and compiler options.
# Users can override those variables from the command line.
CCFLAGS  = -g -O2
CXXFLAGS = -g -O2

# 链接选项(ld).
LDFLAGS  =


#头文件路径定义段  
DOT = .
INCLUDE = -I$(DOT) 

#库文件定义段
LDLIBS = -lpthread


## Define some useful variables.
COMPILE.c   = $(CC)  $(INCLUDE) $(MY_CFLAGS) $(CCFLAGS)  $(CPPFLAGS) -c
COMPILE.cxx = $(CXX) $(INCLUDE) $(MY_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c
LINK.c      = $(CC)  $(MY_CFLAGS) $(CCFLAGS)  $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)
LINK.cxx    = $(CXX) $(MY_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

#定义源文件和头文件后缀类型
SRCEXTS = .c .C .cc .cpp .CPP .c++ .cxx .cp
HDREXTS = .h .H .hh .hpp .HPP .h++ .hxx .hp

#源文件所在目录
VPATH := ..  
SRCDIRS = ..

#如果未定义源文件所在目录，默认当前目录
ifeq ($(SRCDIRS),)
	SRCDIRS = .
endif

#源文件定义
#SOURCES = $(wildcard $(addprefix *,$(SRCEXTS)))
TEMP = $(addprefix /*,$(SRCEXTS))
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
SRCCXX  = $(filter-out *.c,$(SOURCES))

#OBJ定义段
#OBJS = $(addsuffix .o, $(basename $(SOURCES)))
#OBJS = $(patsubst %.cpp, %.o, $(notdir $(SOURCES)))
OBJS = $(addsuffix .o, $(notdir $(basename $(SOURCES))))

#输出物定义段   
OUTPUT = demo

##==========================================================================
# 如果没有定义OUTPUT, 则取当前路径最后一个文件名，还是为空则定义为a.out
SHELL   = /bin/sh
EMPTY   =
SPACE   = $(EMPTY) $(EMPTY)

ifeq ($(OUTPUT),)
  CUR_PATH_NAMES = $(subst /,$(SPACE),$(subst $(SPACE),_,$(CURDIR)))
  OUTPUT = $(word $(words $(CUR_PATH_NAMES)),$(CUR_PATH_NAMES))
  ifeq ($(OUTPUT),)
    OUTPUT = a.out
  endif
endif
##==========================================================================

.PHONY: all objs install clean show help

#删除所有已定义的可识别后缀
#.SUFFIXES:


#ALL定义段  
all:$(OUTPUT) 
$(OUTPUT): $(OBJS)  
	$(LINK.cxx) $(OBJS) -o $@


#编译规则
.SUFFIXES: .cpp .cxx .c .h .hpp
objs:$(OBJS) 
%.o:%.cpp  
	@echo 'CC' $< '-->' $@
	$(COMPILE.cxx) $< -o $@  
	
%.o:%.cxx  
	@echo '#compiling file:' $< '-->' $@
	$(COMPILE.cxx) $< -o $@ 
	
%.o:%.c  
	@echo '#compiling file:' $< '-->' $@
	$(COMPILE.c) $< -o $@ 
	
#INSTALL定义段	
install:  
	mkdir -p ~/lsp/bin
	cp -f $(OUTPUT) ~/lsp/bin

#CLEAN定义段  
clean:  
	rm -f $(OBJS) 
	rm -f $(OUTPUT)
	
	
###############################################################
# 显示变量
show:
	@echo 'OUTPUT              ' $(OUTPUT)
	@echo 'CURDIR              ' $(CURDIR)
	@echo 'COMPILE.c           ' $(COMPILE.c)
	@echo 'COMPILE.cxx         ' $(COMPILE.cxx)
	@echo 'LINK.c              ' $(LINK.c)
	@echo 'LINK.cxx            ' $(LINK.cxx)
	@echo 'SRCCXX              ' $(SRCCXX)
	@echo 'SOURCES             ' $(SOURCES)
	@echo 'OBJS                ' $(OBJS)
	@echo 'TEMP                ' $(TEMP)
	@echo 
	
###############################################################
# 显示帮助.
help:
	@echo 'Usage: make [TARGETS]'
	@echo 'TARGETS:'
	@echo '  all       (=make) compile and link.'
	@echo '  objs      compile only (no linking).'
	@echo '  clean     clean objects and the executable file.'
	@echo '  show      show variables (for debug use only).'
	@echo '  help      print this message.'
	@echo
###############################################################
