#!/bin/bash

set -e  # Detiene el script si ocurre un error

SCRIPT_PATH="$(realpath "$0")"
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

# Compile the generator
g++ -O2 "${SCRIPT_DIR}/spectral-gen.cpp" -o "${SCRIPT_DIR}/spectral-gen.out"

v=10
pot=1
while [ "$v" -lt 100000000 ]; do
    "${SCRIPT_DIR}/spectral-gen.out" --generate --vertices "$v" --output "${SCRIPT_DIR}/T${pot}_spectral_temp.in"

    python3 "${SCRIPT_DIR}/dimacs-converter.py" < "${SCRIPT_DIR}/T${pot}_spectral_temp.in" > "${SCRIPT_DIR}/T${pot}_spectral.in"

    rm -f "${SCRIPT_DIR}/T${pot}_spectral_temp.in"

    v=$((v * 10))
    pot=$((pot + 1))
done
