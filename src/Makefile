CC=g++ -std=c++17
CFLAGS=-Wall -Wextra -Werror
TFLAGS=-lgtest -std=c++17
TEST=test/test.cc
TEST_OUTPUT=unit_test
GCOV_PATH=report
SRC_INSTALL=main.cc ui/*.cc graph/*.a algorithms/*.a
SRC_TEST=graph/*.cc algorithms/*.cc
SRC_FROM_REPORT=../graph/*.cc ../algorithms/*.cc ../test/*.cc
ALL_SRC=*.cc graph/*.cc algorithms/*.cc ui/*.cc test/*.cc
GCOV_PATH=report
APPNAME=app

all: gcov_report

test: clean
	$(CC) $(TEST) $(SRC_TEST) $(CFLAGS) $(TFLAGS) -o $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

gcov_report: clean
	mkdir $(GCOV_PATH) && cd $(GCOV_PATH) && $(CC) --coverage $(SRC_FROM_REPORT) $(CFLAGS) $(TFLAGS) -o gcov_report
	./$(GCOV_PATH)/gcov_report
	lcov -t "gcov_report" -o $(GCOV_PATH)/gcov_report.info --no-external -c -d ./
	genhtml -o report $(GCOV_PATH)/gcov_report.info
	open $(GCOV_PATH)/index.html

s21_graph.o:
	$(CC) $(CFLAGS) -c graph/s21_graph.cc -o graph/s21_graph.o

s21_graph.a: s21_graph.o
	ar rcs graph/s21_graph.a graph/s21_graph.o
	ranlib graph/s21_graph.a

s21_graph_algorithms.o:
	$(CC) $(CFLAGS) -c algorithms/s21_graph_algorithms.cc -o algorithms/s21_graph_algorithms.o

s21_graph_algorithms.a: s21_graph_algorithms.o
	ar rcs algorithms/s21_graph_algorithms.a algorithms/s21_graph_algorithms.o
	ranlib algorithms/s21_graph_algorithms.a

clean:
	rm -rf ./a.out data/*.gch test/*.dot *.dot *.txt *.a *.o graph/*.a graph/*.o algorithms/*.o algorithms/*.a $(TEST_OUTPUT) $(GCOV_PATH) $(APPNAME)

install: clean s21_graph.a s21_graph_algorithms.a
	$(CC) $(CFLAGS) $(SRC_INSTALL) -o $(APPNAME)
	./$(APPNAME)

valgrind: test
	valgrind --leak-check=full ./$(TEST_OUTPUT)

clang:
	clang-format -style=google -n $(ALL_SRC)
	clang-format -style=google -i $(ALL_SRC)