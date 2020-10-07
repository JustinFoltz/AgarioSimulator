CC=gcc
DEBUGFLAGS= -g -fsanitize=address -fsanitize=leak -fsanitize=undefined
CFLAGS= -Wall -Wextra -std=gnu11 $(DEBUGFLAGS)
LIBS= -lSDL2 -lpthread -lm -lrt 

all: agario

agario: main.o agario.o cell.o food.o random.o domain.o cell_queue.o food_queue.o safe_thread.o gfx.o collision.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

agario.o : agario.c agario.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c 
	$(CC) $(CFLAGS) -c $<

food.o: food.c food.h
	$(CC) $(CFLAGS) -c $<

cell.o: cell.c cell.h 
	$(CC) $(CFLAGS) -c $<

random.o: random.c random.h
	$(CC) $(CFLAGS) -c $<

domain.o: domain.c domain.h
	$(CC) $(CFLAGS) -c $<

cell_queue.o : cell_queue.c cell_queue.h
	$(CC) $(CFLAGS) -c $<

food_queue.o : food_queue.c food_queue.h
	$(CC) $(CFLAGS) -c $<

safe_thread.o : safe_thread.c safe_thread.h
	$(CC) $(CFLAGS) -c $<

gfx.o : ./lib/gfx.c ./lib/gfx.h
	$(CC) $(CFLAGS) -c $<
	
collision.o : collision.c collision.h
	$(CC) $(CFLAGS) -c $<


tests: test_food test_cell test_cell_queue test_food_queue test_collision 
	./test_food
	./test_cell
	./test_cell_queue
	./test_food_queue
	./test_collision

test_food: test_food.o food.o domain.o random.o domain.o safe_thread.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_cell: test_cell.o cell.o domain.o random.o  safe_thread.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_cell_queue : test_cell_queue.o cell_queue.o cell.o random.o safe_thread.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_food_queue: test_food_queue.o food_queue.o food.o cell.o random.o safe_thread.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_collision: test_collision.o collision.o food_queue.o cell_queue.o cell.o food.o random.o safe_thread.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_cell.o: test_cell.c
	$(CC) $(CFLAGS) -c $<

test_food.o: test_food.c
	$(CC) $(CFLAGS) -c $<

test_food_queue.o : test_food_queue.c
	$(CC) $(CFLAGS) -c $<

test_cell_queue.o : test_cell_queue.c
	$(CC) $(CFLAGS) -c $<

test_collision.o : test_collision.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o agario test_cell test_food test_agario test_cell_queue test_food_queue test_collision