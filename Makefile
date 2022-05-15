EXEC_FILE = main

all: $(EXEC_FILE)

$(EXEC_FILE): main.cpp Interpretator.cpp Poliz.cpp Executor.cpp Parser.cpp Scanner.cpp Identifier.cpp Lexeme.cpp Table_identifier.cpp Table_label.cpp Table_string.cpp Type_of_lexeme.cpp Label.cpp
	g++ -Wall $^ -o $@

tests: $(EXEC_FILE)
	./$^ tests/test1.txt
	./$^ tests/test2.txt
	./$^ tests/test3.txt
	./$^ tests/test4.txt
	./$^ tests/test5.txt
	./$^ tests/test6.txt
	./$^ tests/test7.txt
	./$^ tests/test8.txt
	./$^ tests/test10.txt
	./$^ tests/test9.txt

clean:
	rm $(EXEC_FILE)

