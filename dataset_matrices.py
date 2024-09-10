import numpy as np

def save_matrix(filename, matrix):
    """Guarda una matriz en un archivo binario."""
    with open(filename, 'wb') as f:
        # Escribir las dimensiones de la matriz
        rows, cols = matrix.shape
        f.write(np.array([rows, cols], dtype=np.int32).tobytes())
        # Escribir los datos de la matriz
        f.write(matrix.astype(np.int32).tobytes())

def generate_datasets():
    # Tamaños de las matrices que se desean generar
    sizes = [128, 256, 512, 1024, 2048]
    
    # Generar matrices cuadradas con valores enteros aleatorios entre 0 y 100
    for size in sizes:
        matrix = np.random.randint(0, 100, size=(size, size))
        save_matrix(f"matriz_{size}x{size}.bin", matrix)

# Ejecutar la función para generar los datasets
generate_datasets()