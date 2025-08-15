import subprocess
import os

# Carpeta de entrada
input_dir = "inputs"

# Archivos fuente
source_files = [
    "main.cpp", "parser.cpp", "scanner.cpp", "token.cpp",
    "visitor.cpp", "exp.cpp"
]

# Compilar
print("Compilando...")
compile_cmd = ["g++"] + source_files
result = subprocess.run(compile_cmd)

if result.returncode != 0:
    print("Error de compilación.")
    exit(1)

print("Compilación exitosa.\n")

for i in range(1, 15):
    input_file = os.path.join(input_dir, f"input{i}.txt")

    if not os.path.exists(input_file):
        print(f"{input_file} no existe. Se omite.")
        continue

    print(f"\nEjecutando con {input_file}")
    subprocess.run(["./a.exe", input_file])
    print()