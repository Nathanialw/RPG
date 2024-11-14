import shutil
import os
import time
import filecmp

# Record the start time
start_time = time.time()

# List of files and directories to be copied
files_to_copy = [
    'assets',
    'Data',
    'libfreetype-6.dll',
    'SDL2.dll',
    'SDL2_image.dll',
    'SDL2_mixer.dll',
    'SDL2_ttf.dll',
    'sqlite3.dll',
    'zlib1.dll',
    'cmake-build-minsizerel/RPG.exe',
]

# Destination folder
destination_folder = '../RPG_Release_Win'

# Ensure the destination folder exists
os.makedirs(destination_folder, exist_ok=True)

def are_files_equal(src, dest):
    return filecmp.cmp(src, dest, shallow=False)

def copy_updated_files(src_dir, dest_dir):
    for root, _, files in os.walk(src_dir):
        for file in files:
            src_file = os.path.join(root, file)
            dest_file = os.path.join(dest_dir, os.path.relpath(src_file, src_dir))
            dest_file_dir = os.path.dirname(dest_file)
            os.makedirs(dest_file_dir, exist_ok=True)
            if not os.path.exists(dest_file) or not are_files_equal(src_file, dest_file):
                shutil.copy2(src_file, dest_file)
                print(f"Copying {src_file} to {dest_file}")

# Strip everything before the last / prefix from the item
def strip_path(p):
    return p.split('/')[-1]

# Copy each file or directory to the destination folder if it has been updated
for item in files_to_copy:
    src_path = os.path.join(os.getcwd(), item)
    dest_path = os.path.join(destination_folder, strip_path(item))

    if os.path.exists(src_path):
        if os.path.isfile(src_path):
            if not os.path.exists(dest_path) or not are_files_equal(src_path, dest_path):
                shutil.copy2(src_path, dest_path)
                print(f"Copying {src_path} to {dest_path}")
        elif os.path.isdir(src_path):
            copy_updated_files(src_path, dest_path)
    else:
        print(f"Skipping {item} (not found)")

# Calculate and print the elapsed time
end_time = time.time()
elapsed_time = end_time - start_time
print(f"Completed in: {elapsed_time:.2f} seconds")