import sys
import os
from typing import List
import shutil
from pathlib import Path

extensions : List[str] = [
    ".h",
    ".hpp",
    ".inl"
]

class InputData:
    def __init__(self):
        self.path: Path()
        self.dir: Path()
        self.source: Path()
        self.target: Path()
        self.includes: List[str]
        self.excludes: List[str]

input = InputData()

num_files_copied = 0

def main():
    global input
    global num_files_copied

    print("Building headers")

    # Get Input file path and check existance
    if len(sys.argv) != 3:
        sys.exit(f"Error: Wrong number of arguments (got {sys.argv[1:]}, expected [input, target])")

    # Check input file
    input.path = Path(sys.argv[1]).resolve()
    if not input.path.exists():
        sys.exit(f"Error: Cannot find input file '{input.path}'")

    input.dir = input.path.parents[0]
    
    # Set target folder
    input.target = Path(sys.argv[2]).resolve()

    # Read Input file
    lines = clean_lines(open(str(input.path)).readlines())

    read_input_data(lines)

    build()

    print(f"Done (files copied: {num_files_copied})")


# Cleans the input
# Removes tabs, newlines, comments leading and trailing spaces)
def clean_lines(lines):
    lines_cleaned = []
    for line in lines:
        line = line.replace('\n', '')
        line = line.replace('\t', '')
        line = line.strip() # Remove spaces in front and back

        if len(line) == 0:
            continue

        if line[0] == '#':
            continue

        lines_cleaned.append(line)
    return lines_cleaned


def read_input_data(lines: List[str]):
    global input

    # Source
    line = lines.pop(0)
    if line.casefold() != "source:":
        sys.exit(f"Error: Expected 'SOURCE:' but found '{line}'")

    if len(lines) == 0:
        sys.exit(f"Error: Expected source path, but found end of file")

    source_path = Path(lines.pop(0))
    if source_path.is_absolute():
        sys.exit(f"Error: Source must be a relative path")

    input.source = (input.dir / source_path).resolve()

    if not input.source.exists():
        sys.exit(f"Error: Cannot locate source path '{input.source}'")
    if not input.source.is_dir():
        sys.exit(f"Error: Source path '{input.source}' is not a directory")

    # Includes
    
    if len(lines) == 0:
        sys.exit(f"Error: Expected 'INCLUDE:', but found end of file")

    line = lines.pop(0)
    if line.casefold() != "include:":
        sys.exit(f"Error: Expected 'INCLUDE:' but found '{line}'")

    input.includes = []
    while len(lines) > 0:
        if lines[0].casefold() == "exclude:":
            break

        include = Path(lines.pop(0))
        if include.is_absolute():
            sys.exit(f"Error: Includes have relative path")
        include = (input.source / include).resolve()
        input.includes.append(str(include))

    # Exclude
    if len(lines) == 0:
        sys.exit(f"Error: Expected 'EXCLUDE:', but found end of file")

    line = lines.pop(0)
    if line.casefold() != "exclude:":
        sys.exit(f"Error: Expected 'EXCLUDE:' but found '{line}'")

    input.excludes = []
    for line in lines:
        exclude = Path(line)
        if exclude.is_absolute():
            sys.exit(f"Error: Excludes have relative path")
        exclude = (input.source / exclude)
        exclude.resolve()
        input.excludes.append(str(exclude))


def build() -> int: 
    global input
    global num_files_copied

    # Clean target directory
    target_dir_action = "Created"
    if input.target.exists():
        shutil.rmtree(str(input.target))
        target_dir_action = "Cleaned"

    print("Using:")
    print(f"  Input file: '{input.path}'")
    print(f"  Source directory: '{input.source}'")
    print(f"  Target directory: '{input.target}' ({target_dir_action})")

    # Copy defaults 
    for root, directories, files  in os.walk(str(input.source)):
        root_path = Path(root)

        if str(root_path) in input.excludes:
            directories.clear()
            files.clear()
            continue

        if len(files) == 0:
            continue

        # Create directory
        target_dir = input.target / root_path.relative_to(input.source)
        target_dir.mkdir(parents=True, exist_ok=True)

        for file in files:
            file_path = root_path / Path(file)
            if not file_path.suffix in extensions:
                continue
            if str(file_path) in input.excludes:
                continue

            # Copy file
            copy_file(file_path)


    # Copy includes
    for include_path_str in input.includes:
        
        # Copy file immediately
        include_path = Path(include_path_str)

        if not include_path.exists():
            sys.exit(f"Error: Cannot locate include file/directory '{include_path}'")

        if include_path.is_file():
            copy_file(Path(include_path))
            continue
        
        # Traverse hierarchy
        for root, directories, files  in os.walk(include_path_str):
            root_path = Path(root)
        
            for file in files:
                file_path = root_path / Path(file) 
                
                copy_file(file_path)


# Expects absolute path
def copy_file(file: Path):
    global num_files_copied
    global input

    file_source = file.relative_to(input.source)
    file_target = input.target / file_source
    
    shutil.copy(str(file), str(file_target))
    num_files_copied = num_files_copied + 1
    


if __name__ == "__main__":
    main() 