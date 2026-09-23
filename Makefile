CC = gcc
TARGET = engine
ARTIFACT_DIR=artifacts
OBJ_DIR=objs

SRCS = main.c glad/src/glad.c shaders/simple_vertex_shader.c shaders/simple_fragment_shader.c
INCLUDE_DIRS = /opt/homebrew/include glad/include shaders/include
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit

OBJS = $(addprefix $(ARTIFACT_DIR)/$(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
CFLAGS = -Wall -Wextra -std=c11 $(addprefix -I,$(INCLUDE_DIRS))

vpath %.c $(sort $(dir $(SRCS)))

$(ARTIFACT_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(ARTIFACT_DIR)/$(TARGET) $(LDFLAGS)

$(ARTIFACT_DIR)/$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(ARTIFACT_DIR)/$(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ARTIFACT_DIR) $(TARGET)

print-%:
	@echo $* = $($*)

