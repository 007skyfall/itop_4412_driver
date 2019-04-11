CC=gcc 
OBJ=test_alloc_chrdev_region
OBJS=test_alloc_chrdev_region.c 
CFLAGS= -Wall -g -static 
$(OBJ):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
$*.o:$%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
.PHONY:clean
clean:
	$(RM) *.o test_alloc_chrdev_region
