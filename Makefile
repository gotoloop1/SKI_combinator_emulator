turing_machine: turing_machine_emulator.cpp
	g++ turing_machine_emulator.cpp -o turing_machine -O2

.PHONY: clean
clean:
	rm turing_machine
