#!/bin/bash


current_dir=$(pwd)
echo "Starting directory: $current_dir"


expected_base_directory="IU_Compilulki_Compiler"


navigate_to_base_dir() {
    while [[ $(basename "$current_dir") != "$expected_base_directory" && "$current_dir" != "/" ]]; do
        current_dir=$(dirname "$current_dir")
    done
}

navigate_to_base_dir


GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

if [[ $(basename "$current_dir") == "$expected_base_directory" ]]; then

    find "$current_dir/examples" -type f -name "*.olang" | while read -r file; do

        "$current_dir/cmake-build-debug/compilulki" "$file" > /dev/null

        exit_code=$?
        if [[ $exit_code -eq 0 ]]; then
            echo -e "${GREEN}Successfully processed: $file${NC}"
        elif [[ $exit_code -eq 1 ]]; then
            echo -e "${RED}Error processing: $file (Exit Code: $exit_code)${NC}"
        else
            echo -e "${RED}Unexpected exit code for file: $file (Exit Code: $exit_code)${NC}"
        fi
        echo "============="
    done
else
    echo "The base directory '$expected_base_directory' was not found in the directory structure."
fi
