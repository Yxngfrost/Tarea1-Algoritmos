import random
import struct

def generar_datos_desordenados(tamano):
    return [random.randint(0, 10000) for _ in range(tamano)]

def generar_datos_semi_ordenados(tamano):
    mitad = tamano // 2
    primera_mitad = generar_datos_desordenados(mitad)
    segunda_mitad = list(range(mitad, tamano))  # Segunda mitad ordenada
    return primera_mitad + segunda_mitad

def generar_datos_ordenados_descendente(tamano):
    return list(range(tamano, 0, -1))

def generar_datos_ordenados_ascendente(tamano):
    return list(range(tamano))

def guardar_dataset_a_archivo(nombre_archivo, datos):
    with open(nombre_archivo, 'wb') as archivo:
        for numero in datos:
            archivo.write(struct.pack('i', numero))

def main():
    tamano = 100000  # Ajusta el tamaño del dataset según sea necesario

    datasets = {
        'desorden': generar_datos_desordenados(tamano),
        'semi': generar_datos_semi_ordenados(tamano),
        'ordenado_descen': generar_datos_ordenados_descendente(tamano),
        'ordenado_ascen': generar_datos_ordenados_ascendente(tamano)
    }

    for nombre, datos in datasets.items():
        nombre_archivo = f'{nombre}_dataset.bin'
        guardar_dataset_a_archivo(nombre_archivo, datos)
        print(f"Dataset '{nombre}' creado en '{nombre_archivo}'")

if __name__ == "__main__":
    main()