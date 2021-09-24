
.DEFAULT_GOAL := default

%.h.ho : %.h
	./compile_header.sh $(CC) $<

HEADERS=$(wildcard *.h)
HEADER_TARGETS=$(patsubst %.h,%.h.ho,$(HEADERS))

default : \
	$(HEADER_TARGETS)

clean :
	rm -f $(HEADER_TARGETS)
