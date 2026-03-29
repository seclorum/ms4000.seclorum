#!/bin/bash
@echo "Generating required sources (.c.h/.js/.pb, etc.) from MS4.proto protobuf definition"
make -C firmware/ clean && make new-python-environment && . .venv_firmware/bin/activate && make python-requirements && make -C firmware/ proto
