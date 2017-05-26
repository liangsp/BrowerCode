#
# Makefile�ļ�
# author: liangshiping
#

#c���������.
CC = gcc

#cpp���������.
CXX = g++

# Un-comment the following line to compile C programs as C++ ones.
CC = $(CXX)

#�Լ���Ҫ���ӵ�ѡ��  �磺����֧��c++11��׼(-std=c++11)
MY_CFLAGS =

# The pre-processor options used by the cpp (man cpp for more).
CPPFLAGS = -Wall

# The pre-processor and compiler options.
# Users can override those variables from the command line.
CCFLAGS  = -g -O2
CXXFLAGS = -g -O2

# ����ѡ��(ld).
LDFLAGS  =


#ͷ�ļ�·�������  
DOT = .
INCLUDE = -I$(DOT) 

#���ļ������
LDLIBS = -lpthread


## Define some useful variables.
COMPILE.c   = $(CC)  $(INCLUDE) $(MY_CFLAGS) $(CCFLAGS)  $(CPPFLAGS) -c
COMPILE.cxx = $(CXX) $(INCLUDE) $(MY_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c
LINK.c      = $(CC)  $(MY_CFLAGS) $(CCFLAGS)  $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)
LINK.cxx    = $(CXX) $(MY_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

#����Դ�ļ���ͷ�ļ���׺����
SRCEXTS = .c .C .cc .cpp .CPP .c++ .cxx .cp
HDREXTS = .h .H .hh .hpp .HPP .h++ .hxx .hp

#Դ�ļ�����Ŀ¼
VPATH := ..  
SRCDIRS = ..

#���δ����Դ�ļ�����Ŀ¼��Ĭ�ϵ�ǰĿ¼
ifeq ($(SRCDIRS),)
	SRCDIRS = .
endif

#Դ�ļ�����
#SOURCES = $(wildcard $(addprefix *,$(SRCEXTS)))
TEMP = $(addprefix /*,$(SRCEXTS))
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
SRCCXX  = $(filter-out *.c,$(SOURCES))

#OBJ�����
#OBJS = $(addsuffix .o, $(basename $(SOURCES)))
#OBJS = $(patsubst %.cpp, %.o, $(notdir $(SOURCES)))
OBJS = $(addsuffix .o, $(notdir $(basename $(SOURCES))))

#����ﶨ���   
OUTPUT = demo

##==========================================================================
# ���û�ж���OUTPUT, ��ȡ��ǰ·�����һ���ļ���������Ϊ������Ϊa.out
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

#ɾ�������Ѷ���Ŀ�ʶ���׺
#.SUFFIXES:


#ALL�����  
all:$(OUTPUT) 
$(OUTPUT): $(OBJS)  
	$(LINK.cxx) $(OBJS) -o $@


#�������
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
	
#INSTALL�����	
install:  
	mkdir -p ~/lsp/bin
	cp -f $(OUTPUT) ~/lsp/bin

#CLEAN�����  
clean:  
	rm -f $(OBJS) 
	rm -f $(OUTPUT)
	
	
###############################################################
# ��ʾ����
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
# ��ʾ����.
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