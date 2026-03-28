docker-build: 
	docker build . -t ms4000-builder

docker-shell:
	docker run -it ms4000-builder /bin/bash

factory:
	make -C tools/factoryFlashing
