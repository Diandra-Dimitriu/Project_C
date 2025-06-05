CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude
LDFLAGS = -mconsole

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# List source files explicitly
SRCS = main1.cpp \
       $(SRC_DIR)/Date.cpp \
       $(SRC_DIR)/Incident.cpp \
       $(SRC_DIR)/Report.cpp \
       $(SRC_DIR)/Util.cpp \
       main2.cpp \
       $(SRC_DIR)/Date.cpp \
       $(SRC_DIR)/Incident.cpp \
       $(SRC_DIR)/Report.cpp \
       $(SRC_DIR)/Util.cpp
# Map each source to its object file in obj/
OBJS = $(SRCS:.cpp=.o)
OBJS := $(patsubst %,$(OBJ_DIR)/%,$(notdir $(OBJS)))

TARGET = $(BIN_DIR)/incident_app

BIN1 = $(BIN_DIR)/incident_app1
BIN2 = $(BIN_DIR)/incident_app2

all: $(BIN1) $(BIN2)

# Linking rule
$(BIN1): obj/main1.o obj/Date.o obj/Incident.o obj/Report.o obj/Util.o
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BIN2): obj/main2.o obj/Date.o obj/Incident.o obj/Report.o obj/Util.o
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

# Compilation rule for all .cpp files
$(OBJ_DIR)/%.o: %.cpp
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

.PHONY: all clean
