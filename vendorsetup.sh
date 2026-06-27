#!/bin/bash

# Path to the vibrator directory
VIBRATOR_DIR="vendor/qcom/opensource/vibrator"
PATCH_URL="https://raw.githubusercontent.com/Rve27/local_manifests/master/lineage-23.x/vendor/qcom/opensource/vibrator/0001-vibrator-Add-support-for-amplitude-control-and-hapti.patch"

# Check if the directory exists
if [ -d "$VIBRATOR_DIR" ]; then
    echo "Checking for vibrator patch in $VIBRATOR_DIR..."
    
    # Store current directory
    CURRENT_DIR=$(pwd)
    
    # Navigate to the vibrator directory
    cd "$VIBRATOR_DIR" || exit
    
    # Check if the commit is already applied
    if ! git log -n 50 --oneline | grep -q "Add support for amplitude control and haptic"; then
        echo "Applying vibrator patch..."
        curl -sL "$PATCH_URL" | git am
        if [ $? -eq 0 ]; then
            echo "Vibrator patch applied successfully."
        else
            echo "Failed to apply vibrator patch. You may need to resolve conflicts manually or abort git am."
            git am --abort > /dev/null 2>&1
        fi
    else
        echo "Vibrator patch already applied."
    fi
    
    # Return to the original directory
    cd "$CURRENT_DIR" || exit
fi
