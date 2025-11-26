.PHONY: all tests doc clean cleanall

all:
	$(MAKE) -C src all

tests: all
	$(MAKE) -C tests all

doc:
	$(MAKE) -C doc all

clean:
	$(MAKE) -C src clean
	$(MAKE) -C tests clean
	$(MAKE) -C doc clean

cleanall:
	$(MAKE) -C src cleanall
	$(MAKE) -C tests cleanall
	$(MAKE) -C doc clean
