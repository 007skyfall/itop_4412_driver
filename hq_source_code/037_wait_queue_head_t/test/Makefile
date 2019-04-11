CC=gcc 
OBJ=test_wait_queue_head_t
OBJS=test_wait_queue_head_t.c 
CFLAGS= -Wall -g -static 
$(OBJ):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
$*.o:$%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
.PHONY:clean
clean:
	$(RM) *.o test_wait_queue_head_t
