
.DEFAULT_GOAL := default

%.h.ho : %.h
	../scripts/compile_header.sh $(CC) $<

HEADERS=$(wildcard *.h)
HEADER_TARGETS=$(patsubst %.h,%.h.ho,$(HEADERS))

.PHONY: doc
doc :
	doxygen Doxyfile

default : \
	$(HEADER_TARGETS)

.PHONY: clean
clean :
	rm -f $(HEADER_TARGETS)
