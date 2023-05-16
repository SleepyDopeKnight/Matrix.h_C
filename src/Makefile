.PHONY: all clean check test rebuild
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
TST_LIBS =  -lcheck -lm -lpthread -lsubunit
OS = $(shell uname -s)

all: s21_matrix.a test check gcov_report

s21_matrix.a:
	$(CC) $(CFLAGS) -c s21_matrix.c -o s21_matrix.o
	ar rcs s21_matrix.a s21_matrix.o
	ranlib s21_matrix.a

test: s21_matrix.a
	$(CC) $(CFLAGS) s21_matrix_test.c s21_matrix.a $(TST_LIBS) -o s21_tests.out
	./s21_tests.out

gcov_report: s21_matrix.a
ifeq ($(OS), Darwin)
	$(CC) $(FLAGS) --coverage -lcheck s21_matrix_test.c s21_matrix.c -o gcov_report
else
	$(CC) $(FLAGS) --coverage -lcheck s21_matrix_test.c s21_matrix.c $(TST_LIBS) -o gcov_report
endif
	./gcov_report
	lcov -t "./gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
ifeq ($(OS), Darwin)
	open ./report/index.html
else
	xdg-open ./report/index.html
endif

clean:
	rm -rf test gcov_test *.gcno *.gcda *.info report *.o *.a gcov_report *.dSYM *.out

check:
	cp ../materials/linters/CPPLINT.cfg .
	rm -rf *.cfg
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./s21_tests.out
else
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./s21_tests.out
endif

rebuild: clean all
