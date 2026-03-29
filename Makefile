MS_PORT ?= /dev/ttyUSB0
BUILDER_NAME ?= ms4000-builder

MS4_PYTHON := $(shell which python3)

builder: 
	docker build -t $(BUILDER_NAME) .

builder-burn:
	docker run --device /dev/ttyUSB0 -v $(shell pwd):/usr/local/ms4000-builder $(BUILDER_NAME) sh -c "make tools && cd firmware && make clean && make proto && make" 

builder-shell:
	docker run -it --device /dev/ttyUSB0 -v $(shell pwd):/usr/local/ms4000-builder $(BUILDER_NAME) /bin/bash

tools:
	make -C tools/esptool-ck

factory:
	make -C tools/factoryFlashing

install-python-venv:
	sudo apt install python3-venv

new-python-environment:
	rm -rf .venv_firmware
	$(MS4_PYTHON) -m venv .venv_firmware
	echo "please run: . .venv_firmware/bin/activate"

python-requirements:
	@echo "Python in-use is: $(MS4_PYTHON)"
	$(MS4_PYTHON) -m pip install -r firmware/requirements.txt

.PHONY: tools
