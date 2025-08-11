clear:
	rm -f test/**/*.in test/**/*.out
run:
	./check_testcases.sh | tee -i run.log