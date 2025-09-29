CC = gcc

CFLAGS = -Wall -I/mingw64/include

LDFLAGS = -L/mingw64/lib -lglfw3 -lopengl32 -lgdi32 -lglad -lm

TARGET = main.exe

SRC = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)


