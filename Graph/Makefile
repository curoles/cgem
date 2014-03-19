TEST_DIR := $(BUILD_DIR)/test/Graph
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp Graph.h $(INSTALL_INCLUDE_DIR)/Graph.h
	cp dijkstra.h $(INSTALL_INCLUDE_DIR)/dijkstra.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) test.cpp -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
