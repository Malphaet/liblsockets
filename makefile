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
LDFLAGS = -L../../

EXEC=liblsockets.a
MAIN=liblsockets

DEST = ../..
PLACE= src/sck
DIRS=$(BIN) $(OBJ) $(SRC) $(INC) $(DOC) $(TMP)

all: $(EXEC)

init:
	mkdir -p $(DIRS)

$(EXEC): $(OBJ)/$(MAIN).o $(OBJ)/wrappers.o $(OBJ)/lpackets.o $(OBJ)/lclists.o
	$(QUIET)echo ""
	$(QUIET)echo "__"$@"__"
	$(QUIET)echo "LINK" $^ "=>" $@
	$(QUIET)$(AR) $@ $^
	$(QUIET)echo "MOVE" $@ "=>" $(DEST) $(DEST)/$(MAIN).h
	$(QUIET)mv $@ $(DEST)/
	$(QUIET)ln -sf $(PLACE)/$(INC)/$(MAIN).h $(DEST)/$(MAIN).h
	$(QUIET)ln -sf $(PLACE)/$(INC)/structures.h $(DEST)/structures.h
	$(QUIET)ln -sf $(PLACE)/$(INC)/wrappers.h $(DEST)/wrappers.h
	$(QUIET)ln -sf $(PLACE)/$(INC)/lpackets.h $(DEST)/lpsckets.h


$(OBJ)/%.o: $(SRC)/%.c $(INC)/%.h $(INC)/utils.h
	$(QUIET)echo "MAKE" $^ "=>" $@
	$(QUIET)$(CC) -c $(CFLAGS) $< -o $@

test: $(SRC)/test.c $(DEST)/$(EXEC) $(SRC)/test_inet.c $(SRC)/test_unix.c
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
	$(EDITOR) 2> /dev/null $(SRC)/* $(INC)/* makefile Readme.md Doxyfile &
	
clean:
	$(QUIET)echo ""
	$(QUIET)echo "__"$@"__"
	$(QUIET)echo "RMVE" $(OBJ)/*.o $(TMP)/* $(DOC)/*
	$(QUIET)rm -f $(OBJ)/*.o
	$(QUIET)rm -f $(TMP)/*
	$(QUIET)rm -rf $(DOC)/*
	
cleaner: clean
	$(QUIET)echo "RMVE" $(BIN)/$(EXEC) $(DEST)/$(EXEC) $(DEST)/$(MAIN).h $(BIN)/test
	$(QUIET)rm -f $(BIN)/*
	$(QUIET)rm -f $(DEST)/$(EXEC)
	$(QUIET)rm -f $(DEST)/$(MAIN).h
	$(QUIET)rm -f $(BIN)/test

.PHONY:
