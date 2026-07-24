#!/bin/bash

SUBMISSIONS_DIR="./submissions"
BACKUP_DIR="./backup_unique"
REPORT_FILE="./report.txt"
ERROR_LOG="./errors.log"

mkdir -p "$BACKUP_DIR" 2>>"$ERROR_LOG"

total=0
duplicates=0
backed_up=0

md5sum "$SUBMISSIONS_DIR"/* 2>>"$ERROR_LOG" | sort > /tmp/checksums.txt
dup_hashes=$(awk '{print $1}' /tmp/checksums.txt | sort | uniq -d)

for file in "$SUBMISSIONS_DIR"/*; do
    total=$((total + 1))
    hash=$(md5sum "$file" 2>>"$ERROR_LOG" | awk '{print $1}')

    if echo "$dup_hashes" | grep -q "$hash"; then
        first_file=$(grep "$hash" /tmp/checksums.txt | head -1 | awk '{print $2}')
        if [ "$file" != "$first_file" ]; then
            echo "DUPLICATE: $file (same as $first_file)"
            duplicates=$((duplicates + 1))
        else
            cp "$file" "$BACKUP_DIR/" 2>>"$ERROR_LOG"
            backed_up=$((backed_up + 1))
        fi
    else
        cp "$file" "$BACKUP_DIR/" 2>>"$ERROR_LOG"
        backed_up=$((backed_up + 1))
    fi
done

echo "Files processed: $total" > "$REPORT_FILE"
echo "Duplicates found: $duplicates" >> "$REPORT_FILE"
echo "Files backed up: $backed_up" >> "$REPORT_FILE"

echo "Report saved to $REPORT_FILE"
cat "$REPORT_FILE"
