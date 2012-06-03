SRC 	:= ./src
INC 	:= ./inc

CROSS_COMPILE 	:= arm-linux-

CC 		= $(CROSS_COMPILE)gcc
LD 		= $(CROSS_COMPILE)ld

CFLAG	= -Wall
LDFLAG  =

SOURCE		= $(SRC)/%.c
INCLUDE 	= $(wildcard $(INC)/%.h)
OBJ			= comm.o terminal_user.o 485enable_user.o

OBJELF		= terminal


$(OBJELF): $(OBJ)
	$(CC) $(LDFLAG) -I$(INC) $(OBJ) -o $(OBJELF)

terminal_user.o: $(SRC)/terminal_user.c $(INC)/terminal_user.h
	$(CC) $(CFLAG) -I$(INC) -c $< -o $@ 

485enable_user.o: $(SRC)/485enable_user.c $(INC)/485enable_user.h
	$(CC) $(CFLAG) -I$(INC) -c $< -o $@ 

comm.o: $(SRC)/comm.c $(INC)/comm.h
	$(CC) $(CFLAG) -I$(INC) -c $< -o $@

.PHONY: clean

clean:
	rm -rf *.o $(OBJELF)
