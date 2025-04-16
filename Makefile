clear:
	rm -f test/**/*.in test/**/*.out
make:
	./make_testcases.sh | tee -i make.log
run:
	./check_testcases.sh | tee -i run.log