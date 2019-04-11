CC=gcc 
OBJ=test_module_rw
OBJS=test_module_rw.c 
CFLAGS= -Wall -g -static 
$(OBJ):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
$*.o:$%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
.PHONY:clean
clean:
	$(RM) *.o test_module_rw
