make clean
make
cd build
make check TESTS="tests/vm/pt-grow-stack tests/vm/pt-grow-stk-sc tests/vm/pt-big-stk-obj tests/vm/pt-grow-pusha tests/vm/page-linear tests/vm/page-parallel tests/vm/page-shuffle tests/vm/page-merge-seq tests/vm/page-merge-par tests/vm/page-merge-mm tests/vm/page-merge-stk tests/vm/pt-bad-addr tests/vm/pt-bad-read tests/vm/pt-write-code tests/vm/pt-write-code2 tests/vm/pt-grow-bad"