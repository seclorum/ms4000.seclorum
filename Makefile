MS_PORT ?= /dev/ttyUSB0
BUILDER_NAME ?= ms4000-builder

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


.PHONY: tools
