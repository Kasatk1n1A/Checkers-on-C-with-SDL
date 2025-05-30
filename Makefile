# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

# Директории
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Исходные файлы
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/core/*.c) \
       $(wildcard $(SRC_DIR)/ui/*.c) \
       $(wildcard $(SRC_DIR)/utils/*.c)

# Объектные файлы
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Имя исполняемого файла
TARGET = $(BIN_DIR)/checkers

# Правило по умолчанию
all: create_dirs $(TARGET)

# Создание директорий
create_dirs:
	@mkdir -p $(BUILD_DIR)/core
	@mkdir -p $(BUILD_DIR)/ui
	@mkdir -p $(BUILD_DIR)/utils
	@mkdir -p $(BIN_DIR)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Правило для объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Установка зависимостей (Ubuntu/Debian)
install_deps:
	sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

.PHONY: all create_dirs clean install_deps