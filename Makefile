CFLAGS = -Wall

LIBS = -pthread

OBJ = 	main.o\
		Job.o\
		UI.o\
		Scheduler.o\
		Dispatcher.o

SRCS = $(OBJ:%.o=%.c)

app.out: $(SRCS)
	gcc -o $@ $(CFLAGS) $(SRCS) $(LIBS)

run: app.out
	./app.out
