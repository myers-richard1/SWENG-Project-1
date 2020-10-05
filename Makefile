CFLAGS = -Wall

LIBS = -pthread

CC = gcc

OBJ = 	main.o\
		Job.o\
		UI.o\
		Scheduler.o\
		Dispatcher.o\
		StringUtils.o\
		Performance.o

SRCS = $(OBJ:%.o=%.c)

app.out: $(SRCS)
	$(CC) -o $@ $(CFLAGS) $(SRCS) $(LIBS)
	$(CC) samplejob.c -o samplejob
	$(CC) batch_job.c -o batch_job

run: app.out
	./app.out
