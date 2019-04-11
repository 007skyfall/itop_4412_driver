CC= /home/skyfall/qt/arm-2014.05/bin/arm-none-linux-gnueabi-gcc
OBJ=test_leds
OBJS=test_leds.c 
CFLAGS= -Wall -g -static
$(OBJ):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
$*.o:$%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
.PHONY:clean
clean:
	$(RM) *.o test_leds
