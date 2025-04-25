#!/usr/bin/env bash
$(poetry env info -p)/bin/python3 "$(dirname "$0")"/main.py "$@"