.PHONY: clean

dims: png-tools.h png-tools.c png-dims.c
	clang -g -o png-dims png-tools.c png-dims.c

map: png-tools.h png-tools.c png-map.c
	clang -g -o png-map png-tools.c png-map.c

tEXt: png-tools.h png-tools.c png-tEXt.c
	clang -g -o png-tEXt png-tools.c png-tEXt.c

test: png-tools.h png-tools.c test_project1_phase1.c
	clang -o run_tests png-tools.c test_project1_phase1.c -lcriterion

clean:
	rm -rf png-dims png-map png-tEXt run_tests *.dSYM a.out

