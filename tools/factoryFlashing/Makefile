 
# Configuration
 
MS4_PYTHON = $(shell which python3)

 
MS4_PORT ?= $(MS4_PORT)

# Auto-detect port for CP210x (Silabs) devices
MS4_PORT=`pio device list --json-output | $(MS4_PYTHON) -c 'exec("""\nimport json,sys\n\nobj=json.load(sys.stdin)\nfor y in obj:\n if "10C4:EA60" in y["hwid"].upper():\n  print (y["port"])\n""") ' | head -1`

# Fallback to default if detection fails
MS4_PORT := $(if $(MS4_PORT),$(MS4_PORT),$(MS4_PORT))
 
bitmaps:
	./MS3000Tool.py bitmaps $(MS4_PORT)

factory:
	./MS3000Tool.py init $(MS4_PORT)
	./MS3000Tool.py web $(MS4_PORT)
	./MS3000Tool.py bitmaps $(MS4_PORT)

build:
	make -C ../../firmware
