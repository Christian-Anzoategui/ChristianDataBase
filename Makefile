CPP = g++
INCLUDE_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj

EXE = cdb 

HEADERS = $(INCLUDE_DIR)/Map.h $(INCLUDE_DIR)/Mmap.h \
	$(INCLUDE_DIR)/Btree.h \
	$(INCLUDE_DIR)/Cdb.h \
	$(INCLUDE_DIR)/Constants.h \
	$(INCLUDE_DIR)/DParser.h \
	$(INCLUDE_DIR)/FTokenizer.h \
	$(INCLUDE_DIR)/STokenizer.h \
	$(INCLUDE_DIR)/Record.h \
	$(INCLUDE_DIR)/Table.h \
	$(INCLUDE_DIR)/Token.h \
	$(INCLUDE_DIR)/state_machine.h \
	$(INCLUDE_DIR)/BPlusTree.h \
	$(INCLUDE_DIR)/util_functions.h

OBJECTS = $(OBJ_DIR)/main.o

#LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

$(EXE): $(OBJECTS)
	$(CPP) -o $@ $(OBJECTS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJECTS): $(HEADERS)

clean:
	@echo "**** Cleaining objects ***"
	rm -f $(OBJ_DIR)/*.o $(EXE)


