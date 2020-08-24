all: compile move run

compile:
	@ gcc -c ./src/PriorityQueue.c
	@ gcc -c ./src/DiscreteEventSimulation.c
	@ gcc -c ./src/Event.c

move:
	@ mv PriorityQueue.o src
	@ mv DiscreteEventSimulation.o src
	@ mv Event.o src

run:
	@ gcc ./src/PriorityQueue.o ./src/Event.o ./src/DiscreteEventSimulation.o -o qSim -lm
	@ mv qSim bin

testcase1:
	./bin/qSim 100 4 60 4.60

testcase2:
	./bin/qSim 100 8 60 2.30

testcase3:
	./bin/qSim 50 4 60 4.60

testcase4:
	./bin/qSim 50 8 60 2.30

clean:
	@ rm -rf ./src/*.o *.exe ./bin/qSim
	@ rm -rf ./output/data.txt
	@ rm -rf ./output/plot