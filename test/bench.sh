#!/bin/bash
TESTS="bench_sched"

GREEN='\033[1;32m'
NO_COLOR='\033[0m'

set -e

make clean
make TESTS="$TESTS" TEST_DEPS="" BENCHES=""
#export OCCLUM_LOG_LEVEL=debug
export RUST_BACKTRACE=1
cd ../build/test/
for t in $TESTS
do
    /bin/echo -e "[TEST] ${GREEN}$t${NO_COLOR}"
    RUST_BACKTRACE=1 occlum run /bin/$t 20000000
    /bin/echo -e ""
done
