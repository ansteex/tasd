CC = gcc -g
CFLAGS = -std=c99 -Wall -Werror -Wpedantic
CHECK_LIBS = -lcheck -lrt -lpthread -lm -lsubunit
INC_DIR = inc
SRC_DIR = src
OUT_DIR = out
UNIT_TESTS_DIR = unit_tests
FUNC_TESTS_DIR = func_tests/scripts

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(SRCS))

UNIT_SRCS = $(wildcard $(UNIT_TESTS_DIR)/*.c)
UNIT_OBJS = $(patsubst $(UNIT_TESTS_DIR)/%.c,$(OUT_DIR)/%.o,$(UNIT_SRCS))

app.exe: $(OBJS)
	$(CC) -o $@ $^

unit_tests.exe: $(UNIT_OBJS) $(filter-out $(OUT_DIR)/main.o,$(OBJS))
	$(CC) -o $@ $^ $(CHECK_LIBS)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OUT_DIR)/%.o: $(UNIT_TESTS_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OUT_DIR):
	mkdir -p $@

unit: unit_tests.exe
	./unit_tests.exe

func: app.exe
	@if [ -d "$(FUNC_TESTS_DIR)" ] && [ -f "$(FUNC_TESTS_DIR)/func_tests.sh" ]; then \
		cd $(FUNC_TESTS_DIR) && ./func_tests.sh; \
	fi

clean:
	rm -rf $(OUT_DIR) app.exe unit_tests.exe

.PHONY: unit func clean