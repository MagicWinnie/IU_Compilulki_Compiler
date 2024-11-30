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

# Define colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

if [[ $(basename "$current_dir") == "$expected_base_directory" ]]; then

    find "$current_dir/examples" -type f -name "*.olang" | while read -r file; do

        "$current_dir/cmake-build-debug/compilulki" -g "$file" > /dev/null

        exit_code=$?

        # Check if the file is in the "errors" folder
        if [[ "$file" == *"/errors/"* ]]; then
            # For errors folder, successful execution is red and error is green
            if [[ $exit_code -eq 0 ]]; then
                echo -e "${RED}Successfully processed: $file${NC}"
            else
                echo -e "${GREEN}Error processing: $file (Exit Code: $exit_code)${NC}"
            fi
        else
            # For non-errors folder, default color scheme
            if [[ $exit_code -eq 0 ]]; then
                echo -e "${GREEN}Successfully processed: $file${NC}"
            else
                echo -e "${RED}Error processing: $file (Exit Code: $exit_code)${NC}"
            fi
        fi
        echo "============="
    done
else
    echo "The base directory '$expected_base_directory' was not found in the directory structure."
fi
