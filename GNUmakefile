# GNUmakefile

DEBUG ?= false
DISTUTILS ?= false

PYTHON ?= python3

ENVPARAM = DEBUG=$(DEBUG)

MODNAME = statfs

.PHONY: all clean install reinstall build-wheel

all:
	@echo "Usage: gmake install"

build-wheel:
	env $(ENVPARAM) $(PYTHON) -m pip wheel .
install: build-wheel
	env $(ENVPARAM) $(PYTHON) -m pip install --user $(MODNAME)-*.whl
reinstall: build-wheel
	env $(ENVPARAM) $(PYTHON) -m pip install --force-reinstall --user $(MODNAME)-*.whl

clean:
	rm -f config.h
	rm -rf build dist $(MODNAME).egg-info
	rm -f *.dll *.so *.whl *.pyc *~

-include GNUmakefile.local
