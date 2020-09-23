CFLAGS = -Wall

OBJ = 	main.o\
		LinkedListNode.o\
		JobExecutor.o

SRCS = $(OBJ:%.o=%.c)

app.out: $(SRCS)
	gcc -o $@ $(CFLAGS) $(SRCS) $(LIBS)

run: app.out
	./app.out
