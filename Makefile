MS_PORT ?= /dev/ttyUSB0
BUILDER_NAME ?= ms4000-builder
PWD := $(shell pwd)
USER := $(shell id -u)
GROUP := $(shell id -g)

MS4_PYTHON := $(shell which python3)

builder:
	docker build -t $(BUILDER_NAME) .

builder-burn:
	docker run 	--device /dev/ttyUSB0 \
			-v $(PWD):/home/builder/workspace \
			--user $(USER):$(GROUP) \
                        --group-add dialout \
                        -e PLATFORMIO_CORE_DIR=/home/builder/.platformio \
			$(BUILDER_NAME) \
			sh -c "\
			make tools \
			&& cd firmware \
			&& make clean \
			&& make proto \
			&& make \
			"

builder-shell:
	docker run 	-it \
			--device /dev/ttyUSB0 \
			-v $(PWD):/home/builder/workspace \
			--user $(USER):$(GROUP) \
			--group-add dialout \
			-e PLATFORMIO_CORE_DIR=/home/builder/.platformio \
			$(BUILDER_NAME) \
			/bin/bash

builder-test:
	docker run 	-it \
			--device /dev/ttyUSB0 \
			-v $(PWD):/home/builder/workspace \
			--user $(USER):$(GROUP) \
			--group-add dialout \
			$(BUILDER_NAME) \
			/bin/bash


tools:
	make -C tools/esptool-ck

factory:
	make -C tools/factoryFlashing
#
# Note! The Dockerfile will use its own internal python - however, for development
# please create and use your own local python .venv with these targets:
#
install-python-venv:
	sudo apt install python3-venv

new-python-environment:
	rm -rf .venv_firmware
	$(MS4_PYTHON) -m venv .venv_firmware
	echo "IMPORTANT: please run . .venv_firmware/bin/activate to use the MS4000-local python environment!"

python-requirements:
	@echo "Python in-use is: $(MS4_PYTHON)"
	$(MS4_PYTHON) -m pip install -r firmware/requirements.txt

.PHONY: tools
