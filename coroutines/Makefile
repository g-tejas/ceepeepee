CC = g++
CFLAGS = -std=c++23
DEPS = main.cpp scheduler.cpp
OUTFILE = scheduler

all: $(OUTFILE)

$(OUTFILE): $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OUTFILE)
