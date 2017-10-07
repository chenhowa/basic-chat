CC = gcc
CFLAGS = -Wall -pedantic # -std=gnu99 

SRCS = client.c
HEADERS = 
OBJS = client.o

client: ${OBJS} ${HEADERS}
	${CC} ${SRCS} -o prog

${OBJS}: ${SRCS}
	${CC} ${CFLAGS} -c $(@:.o=.c)

debug: ${OBJS}
	${CC} ${CFLAGS} -g ${SRCS} -o debug

clean: 
	rm *.o prog debug *~
