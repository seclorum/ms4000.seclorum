#!/bin/bash
make -C firmware/ clean && make new-python-environment && . .venv_firmware/bin/activate && make python-requirements && make -C firmware/ proto
