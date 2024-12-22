ASM_FILENAME = "program1.asm"
OUTPUT_BINARY_FILENAME = "program1.bin"

asm_lines = []
instructions = []


def removeComment(line: str):
    start = line.index("//")
    return line[:start]

def binToDec(binString: str):
    return format(int(binString), '05b')


with open(ASM_FILENAME) as f:
    lines = f.readlines()
    for lineNum, line in enumerate(lines):
        for i in instructions:
            if i not in line:
                print(f"Error parsing line {lineNum}")
                exit()
