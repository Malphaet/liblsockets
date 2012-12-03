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
	$(QUIET)$(PRINT) ""
	$(QUIET)$(PRINT) "__"$@"__"
	$(QUIET)$(PRINT) "LINK" $^ "=>" $@
	$(QUIET)$(AR) $(INC)/$@ $^


$(OBJ)/%.o: $(SRC)/%.c $(INC)/%.h $(INC)/utils.h
	$(QUIET)$(PRINT) "MAKE" $^ "=>" $@
	$(QUIET)$(CC) -c $(CFLAGS) $< -o $@

test: $(SRC)/test.c $(INC)/$(EXEC) $(SRC)/test_inet.c $(SRC)/test_unix.c
	$(QUIET)$(PRINT) ""
	$(QUIET)$(PRINT) "__"$@"__"
	$(QUIET)$(PRINT) "MAKE" $^ "=>" $@
	$(QUIET)$(CC) $(CFLAGS) -o $(OBJ)/test.o -c $<
	$(QUIET)$(PRINT) "LINK" $^ "=>" $@
	$(QUIET)$(CC) $(LDFLAGS) $(OBJ)/test.o -o $(BIN)/$@ -llsockets

doc: .PHONY
	$(QUIET)$(PRINT) ""
	$(QUIET)$(PRINT) "__"$@"__"
	$(QUIET)$(PRINT) "MAKE DOCS"
	$(QUIET)doxygen Doxyfile
	$(QUIET)$(PRINT) ""
	
edit:
	$(EDITOR) 2> /dev/null $(SRC)/* $(INC)/*.h makefile Readme.md Doxyfile &
	
clean:
	$(QUIET)$(PRINT) ""
	$(QUIET)$(PRINT) "__"$@"__"
	$(QUIET)$(PRINT) "RMVE" $(OBJ)/*.o $(TMP)/* $(DOC)/* $(INC)/*.a
	$(QUIET)rm -f $(OBJ)/*.o
	$(QUIET)rm -f $(TMP)/*
	$(QUIET)rm -rf $(DOC)/*
	$(QUIET)rm -f $(INC)/*.a
	
cleaner: clean
	$(QUIET)$(PRINT) "RMVE" $(BIN)/$(EXEC) $(BIN)/test
	$(QUIET)rm -f $(BIN)/*
	$(QUIET)rm -f $(BIN)/test

.PHONY:
