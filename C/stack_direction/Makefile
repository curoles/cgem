TEST_DIR := $(BUILD_DIR)/test/C/stack_direction
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp stack_direction.h $(INSTALL_INCLUDE_DIR)/C/stack_direction.h
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) -c stack_direction.c -o $(BUILD_DIR)/obj/c/stack_direction.c.o
	$(AR) -rs $(CGEM_LIB) $(BUILD_DIR)/obj/c/stack_direction.c.o
	$(CC) $(CFLAGS)  -L$(INSTALL_LIB_DIR) test.c -lcgem -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)

