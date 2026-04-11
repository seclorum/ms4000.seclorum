.PHONY:	build

all:	build

build:
	cp -fvp ../../MS4.proto public/MS4.proto
	npm run build && ls -alF dist/

clean:
	rm -rf dist/*

package:	
	rsync -avpz dist/ ../../firmware/data/
	find ../../firmware/data -name "*.map" -exec rm -rf {} \;

deps:
	npm i
 
