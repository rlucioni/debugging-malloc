CC = $(shell if test -f /opt/local/bin/gcc-mp-4.7; then \
	    echo gcc-mp-4.7; else echo gcc; fi)
CFLAGS = -std=gnu99 -g -W -Wall

TESTS = $(patsubst %.c,%,$(sort $(wildcard test[0-9][0-9][0-9].c)))

%.o: %.c m61.h
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(TESTS) hhtest
	@echo "*** Run 'make check' or 'make check-all' to check your work."

test%: test%.o m61.o
	$(CC) $(CFLAGS) -o $@ $^

test017: test017-help.o

hhtest: hhtest.o m61.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

check: $(TESTS) $(patsubst %,check-%,$(TESTS))
	@echo "*** All tests succeeded!"

check-all: $(TESTS)
	@x=true; for i in $(TESTS); do $(MAKE) check-$$i || x=false; done; \
	if $$x; then echo "*** All tests succeeded!"; fi; $$x

check-test%: test%
	@test -d out || mkdir out
	@rm -f out/test$*.fail
	@-sh -c "./$^ > out/test$*.output 2>&1" >/dev/null 2>&1; true
	@perl compare.pl out/test$*.output test$*.c test$*

clean:
	rm -f $(TESTS) hhtest *.o
	rm -rf out

MALLOC_CHECK_=0
export MALLOC_CHECK_

.PRECIOUS: %.o
.PHONY: all clean check check-% prepare-check
