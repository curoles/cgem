TEST_DIR := $(BUILD_DIR)/test/StringBuf
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp StringBuf.h $(INSTALL_INCLUDE_DIR)/StringBuf.h
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) -c StringBuf.c -o $(BUILD_DIR)/obj/c/StringBuf.c.o
	$(AR) -rs $(CGEM_LIB) $(BUILD_DIR)/obj/c/StringBuf.c.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/c/StringBuf.c.o
	$(CXX) $(CFLAGS) -c StringBuf.cpp -o $(BUILD_DIR)/obj/cpp/StringBuf.cpp.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/cpp/StringBuf.cpp.o
	$(CC) $(CFLAGS) -L$(INSTALL_LIB_DIR) test.c -lcgem -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
	$(CXX) $(CFLAGS) -L$(INSTALL_LIB_DIR) test.cpp -lxgem -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
