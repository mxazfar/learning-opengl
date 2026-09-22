CC = gcc
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit

SRCS = main.c glad/src/glad.c
OBJS = $(SRCS:.c=.o)
TARGET = engine
INCLUDE_DIRS = /opt/homebrew/include glad/include
CFLAGS = -Wall -Wextra -std=c11 $(addprefix -I,$(INCLUDE_DIRS))

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

print-%:
	@echo $* = $($*)

