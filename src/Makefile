
.DEFAULT_GOAL := default

PROJECT_CFLAGS=-Wall -Werror

%.h.ho : %.h
	../scripts/compile_header.sh $(CC) $<

%.o : %.c
	$(CC) $(PROJECT_CFLAGS) $(CFLAGS) -c -o $@ $<

HEADERS=$(wildcard *.h)
HEADER_TARGETS=$(patsubst %.h,%.h.ho,$(HEADERS))

C_SOURCES=$(wildcard *.c)
C_TARGETS=$(patsubst %.c,%.o,$(C_SOURCES))

tests/test_resolve : test_resolve.o resolve.o
	$(CC) $(LDFLAGS) -o $@ $^

TESTS = tests/test_resolve

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

.PHONY: clean_headers
clean_headers :
	rm -f $(HEADER_TARGETS)
