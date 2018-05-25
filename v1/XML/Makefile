TEST_DIR := $(BUILD_DIR)/test/XML
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp XMLParser.h $(INSTALL_INCLUDE_DIR)/XMLParser.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c XMLParser.cpp -o $(BUILD_DIR)/obj/cpp/XMLParser.cpp.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/cpp/XMLParser.cpp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) -L$(INSTALL_LIB_DIR) test.cpp -lxgem -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
