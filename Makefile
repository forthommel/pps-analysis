CC = g++

SRC_DIR = src/
PLG_DIR = plugins/
OBJ_DIR = obj/
BIN_DIR = bin/
INC_DIR = interface/

LDFLAGS = -g
CFLAGS = -g -I$(INC_DIR)

ROOT_CFLAGS = `root-config --cflags`
ROOT_LDFLAGS = `root-config --evelibs`
ROOTINC = `root-config --incdir`

CPP_FILES = $(wildcard $(SRC_DIR)*.cc)
LIB_FILES = $(patsubst $(SRC_DIR)%.cc,$(OBJ_DIR)%.o,$(CPP_FILES))

all: $(BIN_DIR)analyze $(BIN_DIR)CentralSystem

$(BIN_DIR)analyze: $(PLG_DIR)analyze.o $(LIB_FILES) | $(BIN_DIR)
	@echo "Linking $@..."
	@$(CC) $^ -o $@ $(LDFLAGS) $(ROOT_LDFLAGS)

$(BIN_DIR)AlCa: $(PLG_DIR)AlCa.o $(LIB_FILES) | $(BIN_DIR)
	@echo "Linking $@..."
	@$(CC) $^ -o $@ $(LDFLAGS) $(ROOT_LDFLAGS)

$(BIN_DIR)CentralSystem: $(PLG_DIR)CentralSystem.o $(LIB_FILES) | $(BIN_DIR)
	@echo "Linking $@..."
	@$(CC) $^ -o $@ $(LDFLAGS) $(ROOT_LDFLAGS)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(BIN_DIR):
	@mkdir $(BIN_DIR)

$(PLG_DIR)%.o: $(PLG_DIR)%.cc
	@echo "Building (plugin) $<..."
	@$(CC) -c $(CFLAGS) $(ROOT_CFLAGS) $< -o $@ -I$(ROOTINC)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cc $(INC_DIR)%.h | $(OBJ_DIR)
	@echo "Building $<..."
	@$(CC) -c $(CFLAGS) $(ROOT_CFLAGS) $< -o $@ -I$(ROOTINC)

test:
	@echo "Building tests..."
	@cd test && $(MAKE) -s
