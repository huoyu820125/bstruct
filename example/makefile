#makefile�ļ�����ָ��
#���make�ļ�����makefile��ֱ��ʹ��make�Ϳ��Ա���
#���make�ļ�������makefile������test.txt����ôʹ��make -f test.txt

#------------------------------------------������ϵͳ32λ64λ--------------------------------------------------------
#SYS_BIT=$(shell getconf LONG_BIT)
#SYS_BIT=$(shell getconf WORD_BIT)
SYS_BIT=$(shell getconf LONG_BIT)
ifeq ($(SYS_BIT),32)
	CPU =  -march=i686 
else 
	CPU = 
endif

#------------------------------------------�༭��--------------------------------------------------------

#c++���빤��
CC = g++ 
#apacheģ����밲װ����
APXS = apxs 

#------------------------------------------�༭��End--------------------------------------------------------

#------------------------------------------Ŀ¼--------------------------------------------------------

#����Ŀ��/�ļ�����Ŀ¼
VPATH = $(OBJ_OUTPUT_DIR) 
#���Ŀ¼
OBJ_OUTPUT_DIR=./output
OUTPUT_DIR=./output
$(shell mkdir $(OBJ_OUTPUT_DIR))
$(shell mkdir $(OUTPUT_DIR))

BStruct = /mnt/HYLib/bstruct
#.cppĿ¼
CPP_DIR= 

#.hĿ¼
H_DIR= 

#------------------------------------------Ŀ¼End--------------------------------------------------------

#------------------------------------------����ѡ��--------------------------------------------------------

#SO�ļ�����ѡ��
CFLAGS= -O -g -fPIC -Wall -D_REENTRANT -DUSE_APACHE -DNO_STRING_CIPHER $(CPU) 

#���漶��
WARNING_LEVEL += -O3 

#ͷ�ļ�Ŀ¼��-I Ŀ¼
INCLUDE = -I. -I../include -I$(BStruct)/include  

#��Ŀ¼�����ļ�:-L Ŀ¼ -����
#SYSLIB = -lnsl -lc -lm -lpthread -lstdc++ 
LIB =  

#��̬�⣺.a�ļ�·����
LIB += $(BStruct)/lib/bstruct.a 

#------------------------------------------����ѡ��End--------------------------------------------------------

#------------------------------------------���--------------------------------------------------------
#��������ĳ����ļ���
OUTPUT = example

#Ŀ���ļ�
OBJ_PRO = $(notdir $(patsubst %.cpp,%.o,$(wildcard *.cpp))) 

#������Ŀ���ļ�
DEPENDENCE = $(OBJ_PRO) 

#������Ŀ����Ҫ����������Դ�ļ���Ŀ���ļ�(��Ŀ¼)
OBJ = $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ_PRO)) 

#------------------------------------------���End--------------------------------------------------------

#-------------------------------------------����ָ��-----------------------------------------------------
#����EXE
$(OUTPUT_DIR)/$(OUTPUT):$(DEPENDENCE)
	@echo "Complie (OUTPUT_DIR)/$(OUTPUT)"
	@echo ""
	$(CC) -o $@ $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(OBJ)$(LIB)
	@echo ""
	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

#------------------------------------------����Object----------------------------------------------------
$(OBJ_PRO):%.o:%.cpp %.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

#------------------------------------------�������±���----------------------------------------------------
clean:
	-rm -f $(OUTPUT_DIR)/$(OUTPUT) $(OBJ_OUTPUT_DIR)/*.o
	
.PHONY: clean
