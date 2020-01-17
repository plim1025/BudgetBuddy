CC = g++
exe_file = BudgetBuddy
$(exe_file): BudgetBuddy.o prog.o
	$(CC) BudgetBuddy.o prog.o -o $(exe_file)
BudgetBuddy.o: BudgetBuddy.cpp
	$(CC) -c BudgetBuddy.cpp
prog.o: prog.cpp
	$(CC) -c prog.cpp
clean:
	rm -f *.out *.o BudgetBuddy