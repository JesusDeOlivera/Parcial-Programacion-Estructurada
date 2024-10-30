class Libro:
    def __init__(self, titulo, autor, ano, editorial, isbn, paginas):
        self.titulo = titulo
        self.autor = autor
        self.ano_edicion = ano
        self.editorial = editorial
        self.isbn = isbn
        self.paginas = paginas

    def __str__(self):
        return f"Título: {self.titulo}, Autor: {self.autor}, Año: {self.ano_edicion}, Editorial: {self.editorial}, ISBN: {self.isbn}, Páginas: {self.paginas}"


class Lector:
    def __init__(self, nombre, dni, libro_solicitado):
        self.nombre = nombre
        self.dni = dni
        self.libro_solicitado = libro_solicitado


class Biblioteca:
    def __init__(self):
        self.libros = self.cargar_libros()
        self.solicitudes = []
        self.historial = []

    def cargar_libros(self):
        libros = []
        try:
            with open("libros_agregados.txt", "r") as f:
                for line in f:
                    datos = line.strip().split(',')
                    if len(datos) == 6:
                        libro = Libro(datos[0], datos[1], int(datos[2]), datos[3], datos[4], int(datos[5]))
                        libros.append(libro)
        except FileNotFoundError:
            print("El archivo de libros no se encontró.")
        return libros

    def agregar_libro(self, libro):
        self.libros.append(libro)
        movimiento = f"Libro agregado: {libro.titulo}"
        self.historial.append(movimiento)
        print(f"Movimiento: {movimiento}")
        self.guardar_libros()  # Guardar en archivo después de agregar
        self.guardar_historial()

    def guardar_historial(self):
        with open("historial_movimientos.txt", "a") as f:
            for movimiento in self.historial:
                f.write(movimiento + "\n")
        self.historial = []  # Limpiar el historial después de guardarlo

    def solicitar_libro(self, nombre, dni, titulo):
        libro = self.buscar_libro(titulo)
        if libro:
            lector = Lector(nombre, dni, libro)
            self.solicitudes.append(lector)
            self.libros.remove(libro)  # Eliminar libro de la lista
            movimiento = f"Libro solicitado: {titulo} por {nombre}"
            self.historial.append(movimiento)
            print(f"Movimiento: {movimiento}")
            self.guardar_historial()
            self.guardar_libros()  # Guardar cambios en el archivo de libros
        else:
            print("El libro solicitado no está disponible.")

    def devolver_libro(self):
        if self.solicitudes:
            self.mostrar_libros_solicitados()
            seleccion = int(input("Seleccione el número del libro que desea devolver: ")) - 1

            if 0 <= seleccion < len(self.solicitudes):
                lector_atendido = self.solicitudes[seleccion]
                self.solicitudes.remove(lector_atendido)
                self.agregar_libro(lector_atendido.libro_solicitado)  # Reagregar el libro a la biblioteca
                movimiento = f"Libro devuelto: {lector_atendido.libro_solicitado.titulo} por {lector_atendido.nombre}"
                self.historial.append(movimiento)
                print(f"Movimiento: {movimiento}")
                self.guardar_historial()
                self.guardar_libros()  # Guardar cambios en el archivo de libros
            else:
                print("Selección no válida.")
        else:
            print("No hay libros solicitados para devolver.")

    def buscar_libro(self, clave):
        for libro in self.libros:
            if libro.titulo.lower() == clave.lower():
                return libro
        return None

    def mostrar_libros_solicitados(self):
        print("\n--- Libros Solicitados ---")
        for i, lector in enumerate(self.solicitudes, start=1):
            print(f"{i}. {lector.libro_solicitado.titulo} (Autor: {lector.libro_solicitado.autor})")

    def guardar_libros(self):
        with open("libros_agregados.txt", "w") as f:
            for libro in self.libros:
                f.write(f"{libro.titulo},{libro.autor},{libro.ano_edicion},{libro.editorial},{libro.isbn},{libro.paginas}\n")
        print("Libros guardados correctamente.")

    def mostrar_historial(self):
        print("\n--- Historial de Movimientos ---")
        try:
            with open("historial_movimientos.txt", "r") as f:
                for line in f:
                    print(line.strip())
        except FileNotFoundError:
            print("No hay historial disponible.")

    def vaciar_historial(self):
        with open("historial_movimientos.txt", "w") as f:
            f.truncate(0)  # Vaciar el contenido del archivo
        print("Historial vaciado correctamente.")

    def vaciar_biblioteca(self):
        self.libros.clear()
        self.guardar_libros()  # Guardar cambios en el archivo de libros
        print("Biblioteca vaciada.")

    def ordenar_libros(self):
        self.libros.sort(key=lambda x: x.titulo)
        print("Libros ordenados por título.")

    def mostrar_libros(self):
        print("\n--- Libros en la Biblioteca ---")
        for libro in self.libros:
            print(libro)


def main():
    biblioteca = Biblioteca()

    while True:
        print("\n--- Gestión de Biblioteca ---")
        print("1. Agregar libro")
        print("2. Solicitar libro")
        print("3. Devolver libro")
        print("4. Ordenar libros por título")
        print("5. Buscar libro")
        print("6. Guardar libros")
        print("7. Mostrar historial")
        print("8. Vaciar historial")  # Opción para vaciar historial
        print("9. Vaciar biblioteca")
        print("0. Salir")
        opcion = input("Ingrese una opción: ")

        if opcion == "1":
            titulo = input("Título: ")
            autor = input("Autor: ")
            ano = int(input("Año de edición: "))
            editorial = input("Editorial: ")
            isbn = input("ISBN: ")
            paginas = int(input("Número de páginas: "))
            libro = Libro(titulo, autor, ano, editorial, isbn, paginas)
            biblioteca.agregar_libro(libro)
        elif opcion == "2":
            nombre = input("Ingrese su nombre: ")
            dni = int(input("Ingrese su DNI: "))
            titulo = input("Ingrese el título del libro solicitado: ")
            biblioteca.solicitar_libro(nombre, dni, titulo)
        elif opcion == "3":
            biblioteca.devolver_libro()
        elif opcion == "4":
            biblioteca.ordenar_libros()
        elif opcion == "5":
            titulo = input("Ingrese el título del libro a buscar: ")
            libro = biblioteca.buscar_libro(titulo)
            if libro:
                print(f"Libro encontrado: {libro}")
            else:
                print("Libro no encontrado.")
        elif opcion == "6":
            biblioteca.guardar_libros()
        elif opcion == "7":
            biblioteca.mostrar_historial()
        elif opcion == "8":  # Vaciar historial
            biblioteca.vaciar_historial()
        elif opcion == "9":
            biblioteca.vaciar_biblioteca()
        elif opcion == "0":
            break
        else:
            print("Opción no válida.")

if __name__ == "__main__":
    main()
