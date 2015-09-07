CC = gcc
CFLAGS = -g
CFLAGS += -Wall
CFLAGS += -std=gnu99

OS = $(shell uname -s)

# Load the proper library for Flex
ifeq ($(OS),Darwin)
	CFLAGS += -ll
endif

ifeq ($(OS),Linux)
	CFLAGS += -lfl
endif


myhtml2text: myhtml2text.l
	flex -o myhtml2text.c myhtml2text.l
	${CC} ${CFLAGS} -o myhtml2text myhtml2text.c

clean:
	@rm *.o myhtml2text 2> /dev/null || true
