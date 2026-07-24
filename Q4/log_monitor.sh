#!/bin/bash

LOG_FILE="$1"
REPORT_FILE="error_report.txt"

grep "ERROR" "$LOG_FILE" > "$REPORT_FILE"
echo "Errors saved to $REPORT_FILE"
cat "$REPORT_FILE"

echo ""
echo "Total errors: $(grep -c "ERROR" "$LOG_FILE")"

grep -v "ERROR" "$LOG_FILE" > /dev/null

echo ""
echo "Monitoring new entries (Ctrl+C to stop):"
tail -f "$LOG_FILE" | grep "ERROR" >> "$REPORT_FILE"
