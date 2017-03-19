all: herdsim

herdsim: herdsim.cpp func.cpp
	icc -DDEBUG=0 -g -o herdsim herdsim.cpp func.cpp

clean:
	rm herdsim
