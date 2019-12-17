all: cost

cost: cost.c
	cc -D_GNU_SOURCE -Wall -g -o cost cost.c

clean:
	rm cost
run:
	./cost

