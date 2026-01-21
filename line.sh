#!/bin/bash

START_DIR=$(pwd)

process_files() {
    for file in *; do
        if [ -f "$file" ]; then
            lines=$(wc -l < "$file")
            echo "$file : $lines lines"
        fi
    done
}

process_directories() {
    for item in *; do
        if [ -d "$item" ]; then
            echo "DIRECTORY ==> $item"
            cd "$item" || continue
            process_files
            cd ..
        fi
    done
}

# Main execution
while true; do
    echo "Current directory: ${PWD#$START_DIR}"

    # Process files in current directory
    process_files

    # Process subdirectories
    process_directories

    # To prevent infinite loop
    echo "Processing complete."
    break
done
