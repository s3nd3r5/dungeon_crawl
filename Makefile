CC=g++


dungeon_crawl: main.o
	$(CC) main.o -o dungeon_crawl

main.o: main.cc
	$(CC) -c main.cc -o main.o
