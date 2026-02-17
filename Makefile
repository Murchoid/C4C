
CC=g++
SRC=src
TARGET=target
TEST=tests

memcheck: clean $(TARGET)/driver
	valgrind --leak-check=full -s $(TARGET)/driver $(TEST)/test.rs

run:clean $(TARGET)/driver
	$(TARGET)/driver --input  $(TEST)/test.rs --output huh


$(TARGET)/driver: $(SRC)/driver.cpp
	$(CC) $< -o $@

clean:
	rm -rf $(TARGET)/driver
