CC = gcc
TARGET = engine
ARTIFACT_DIR=artifacts
OBJ_DIR=objs
SHADER_DIR=shaders
TEXTURE_DIR=textures

SRCS = shader_manager.c main.c glad/src/glad.c
INCLUDE_DIRS = /opt/homebrew/include glad/include
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit

TEXTURE_SOURCE = $(addprefix $(ARTIFACT_DIR)/,$(wildcard $(TEXTURE_DIR)/*))
SHADER_SOURCE = $(addprefix $(ARTIFACT_DIR)/,$(wildcard $(SHADER_DIR)/*))
OBJS = $(addprefix $(ARTIFACT_DIR)/$(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
CFLAGS = -Wall -Wextra -std=c11 $(addprefix -I,$(INCLUDE_DIRS))

vpath %.c $(sort $(dir $(SRCS)))

$(ARTIFACT_DIR)/$(TARGET): $(OBJS) $(SHADER_SOURCE) $(TEXTURE_SOURCE)
	$(CC) $(OBJS) -o $(ARTIFACT_DIR)/$(TARGET) $(LDFLAGS)

$(ARTIFACT_DIR)/$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(ARTIFACT_DIR)/$(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ARTIFACT_DIR)/$(SHADER_DIR)/%: $(SHADER_DIR)/%
	@mkdir -p $(ARTIFACT_DIR)/$(SHADER_DIR)
	cp $< $@

$(ARTIFACT_DIR)/$(TEXTURE_DIR)/%: $(TEXTURE_DIR)/%
	@mkdir -p $(ARTIFACT_DIR)/$(TEXTURE_DIR)
	cp $< $@

clean:
	rm -rf $(ARTIFACT_DIR) $(TARGET)

print-%:
	@echo $* = $($*)

