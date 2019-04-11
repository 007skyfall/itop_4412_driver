CC=gcc 
OBJ=test_unlock_ioctl_update
OBJS=test_unlock_ioctl_update.c 
CFLAGS= -Wall -g -static 
$(OBJ):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
$*.o:$%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
.PHONY:clean
clean:
	$(RM) *.o test_unlock_ioctl_update
