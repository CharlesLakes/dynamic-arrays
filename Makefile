delete:
	rm -f test/**/*.in test/**/*.out
make:
	./make_testcase.sh
run:
	./check_testcases.sh