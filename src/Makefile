
.DEFAULT_GOAL := default

%.h.ho : %.h
	../scripts/compile_header.sh $(CC) $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

HEADERS=$(wildcard *.h)
HEADER_TARGETS=$(patsubst %.h,%.h.ho,$(HEADERS))

C_SOURCES=$(wildcard *.c)
C_TARGETS=$(patsubst %.c,%.o,$(C_SOURCES))

test_resolve : test_resolve.o resolve.o
	$(CC) $(LDFLAGS) -o tests/$@ $^

TESTS = test_resolve

.PHONY: doc
doc :
	doxygen Doxyfile

.PHONE: headers
headers : \
	$(HEADER_TARGETS) \

default : \
	$(HEADER_TARGETS) \
	$(C_TARGETS) \
	$(TESTS)

.PHONY: clean
clean :
	rm -f $(HEADER_TARGETS) $(C_TARGETS) $(TESTS)
