clear:
	rm -f test/**/*.in test/**/*.out
make:
	./make_testcases.sh
run:
	./check_testcases.sh