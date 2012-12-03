AR = ar rcs
CC = gcc
EDITOR = geany
QUIET = @

BIN = bin
OBJ = obj
SRC = src
INC = inc
DOC = doc
TMP = tmp

CFLAGS = -W -Wextra -Wall -ansi -pedantic -Wdeclaration-after-statement -Wunsafe-loop-optimizations\
		 -std=c99 -Wshadow -O2 -g -I$(INC) 
LDFLAGS = -L$(INC)

EXEC=liblsockets.a

DIRS=$(BIN) $(OBJ) $(SRC) $(INC) $(DOC) $(TMP)

all: $(EXEC)

init:
	mkdir -p $(DIRS)

$(EXEC): $(OBJ)/$(EXEC:%.a=%).o $(OBJ)/wrappers.o $(OBJ)/lpackets.o $(OBJ)/lclists.o
	$(QUIET)echo ""
	$(QUIET)echo "__"$@"__"
	$(QUIET)echo "LINK" $^ "=>" $@
	$(QUIET)$(AR) $(INC)/$@ $^


$(OBJ)/%.o: $(SRC)/%.c $(INC)/%.h $(INC)/utils.h
	$(QUIET)echo "MAKE" $^ "=>" $@
	$(QUIET)$(CC) -c $(CFLAGS) $< -o $@

test: $(SRC)/test.c $(INC)/$(EXEC) $(SRC)/test_inet.c $(SRC)/test_unix.c
	$(QUIET)echo ""
	$(QUIET)echo "__"$@"__"
	$(QUIET)echo "MAKE" $^ "=>" $@
	$(QUIET)$(CC) $(CFLAGS) -o $(OBJ)/test.o -c $<
	$(QUIET)echo "LINK" $^ "=>" $@
	$(QUIET)$(CC) $(LDFLAGS) $(OBJ)/test.o -o $(BIN)/$@ -llsockets

doc: .PHONY
	$(QUIET)echo ""
	$(QUIET)echo "__"$@"__"
	$(QUIET)echo "MAKE DOCS"
	$(QUIET)doxygen Doxyfile
	$(QUIET)echo ""
	
edit:
	$(EDITOR) 2> /dev/null $(SRC)/* $(INC)/*.h makefile Readme.md Doxyfile &
	
clean:
	$(QUIET)echo ""
	$(QUIET)echo "__"$@"__"
	$(QUIET)echo "RMVE" $(OBJ)/*.o $(TMP)/* $(DOC)/* $(INC)/*.a
	$(QUIET)rm -f $(OBJ)/*.o
	$(QUIET)rm -f $(TMP)/*
	$(QUIET)rm -rf $(DOC)/*
	$(QUIET)rm -f $(INC)/*.a
	
cleaner: clean
	$(QUIET)echo "RMVE" $(BIN)/$(EXEC) $(BIN)/test
	$(QUIET)rm -f $(BIN)/*
	$(QUIET)rm -f $(BIN)/test

.PHONY:
