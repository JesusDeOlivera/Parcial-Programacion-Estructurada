#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

struct Libro {
    string titulo;
    string autor;
    int anoEdicion;
    string editorial;
    string isbn;
    int paginas;
    Libro* siguiente;
};

struct Lector {
    string nombre;
    int dni;
    string libroSolicitado; 
    string autor;
    int anoEdicion; 
    string editorial; 
    string isbn; 
    int paginas; 
    Lector* siguiente;
};

// Lista de libros (Lista enlazada simple)
Libro* listaLibros = nullptr;

// Cola de lectores que solicitaron libros
queue<Lector*> colaSolicitudes;

// Pila de historial de operaciones
stack<string> historial;

// Función para agregar un libro a la lista enlazada
void agregarLibro(Libro* &lista, const string& titulo, const string& autor, int ano, const string& editorial, const string& isbn, int paginas) {
    Libro* nuevoLibro = new Libro{titulo, autor, ano, editorial, isbn, paginas, nullptr};
    if (!lista) {
        lista = nuevoLibro;
    } else {
        Libro* actual = lista;
        while (actual->siguiente) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoLibro;
    }
    string movimiento = "Libro agregado: " + titulo;
    historial.push(movimiento);
    cout << "Movimiento: " << movimiento << endl; // Mostrar movimiento
    // Guardar movimiento en el archivo de historial
    ofstream archivoHistorial("historial.txt", ios::app);
    archivoHistorial << movimiento << endl;
    archivoHistorial.close();
}

// Función para ordenar libros por título (Método de burbuja)
void ordenarLibrosPorTitulo(Libro* &lista) {
    if (!lista || !lista->siguiente) return;
    bool ordenado = false;
    while (!ordenado) {
        ordenado = true;
        Libro* actual = lista;
        while (actual->siguiente) {
            if (actual->titulo > actual->siguiente->titulo) {
                swap(actual->titulo, actual->siguiente->titulo);
                swap(actual->autor, actual->siguiente->autor);
                swap(actual->anoEdicion, actual->siguiente->anoEdicion);
                swap(actual->editorial, actual->siguiente->editorial);
                swap(actual->isbn, actual->siguiente->isbn);
                swap(actual->paginas, actual->siguiente->paginas);
                ordenado = false;
            }
            actual = actual->siguiente;
        }
    }
    string movimiento = "Libros ordenados";
    historial.push(movimiento);
    cout << "Movimiento: " << movimiento << endl; // Mostrar movimiento
    // Guardar movimiento en el archivo de historial
    ofstream archivoHistorial("historial.txt", ios::app);
    archivoHistorial << movimiento << endl;
    archivoHistorial.close();
}

// Función para buscar un libro por título, autor o ISBN
Libro* buscarLibro(Libro* lista, const string& clave, const string& tipo) {
    Libro* actual = lista;
    while (actual) {
        if ((tipo == "titulo" && actual->titulo == clave) || 
            (tipo == "autor" && actual->autor == clave) ||
            (tipo == "isbn" && actual->isbn == clave)) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

// Función para mostrar la información de un libro
void mostrarLibro(Libro* libro) {
    if (libro) {
        cout << "\n--- Información del Libro ---" << endl;
        cout << "Título: " << libro->titulo << endl;
        cout << "Autor: " << libro->autor << endl;
        cout << "Año de Edición: " << libro->anoEdicion << endl;
        cout << "Editorial: " << libro->editorial << endl;
        cout << "ISBN: " << libro->isbn << endl;
        cout << "Páginas: " << libro->paginas << endl;
    } else {
        cout << "Libro no encontrado." << endl;
    }
}

// Función para realizar la búsqueda por título, autor o ISBN
void buscarLibroPorClave() {
    string clave, tipo;
    int opcionBusqueda;
    
    cout << "\n--- Buscar Libro ---" << endl;
    cout << "1. Buscar por Título" << endl;
    cout << "2. Buscar por Autor" << endl;
    cout << "3. Buscar por ISBN" << endl;
    cout << "Ingrese una opción: ";
    cin >> opcionBusqueda;
    cin.ignore();

    if (opcionBusqueda == 1) {
        tipo = "titulo";
        cout << "Ingrese el título del libro: ";
    } else if (opcionBusqueda == 2) {
        tipo = "autor";
        cout << "Ingrese el autor del libro: ";
    } else if (opcionBusqueda == 3) {
        tipo = "isbn";
        cout << "Ingrese el ISBN del libro: ";
    } else {
        cout << "Opción no válida." << endl;
        return;
    }
    getline(cin, clave);

    Libro* libro = buscarLibro(listaLibros, clave, tipo);
    mostrarLibro(libro);
}

// Solicita un libro (Encola el lector en espera y elimina el libro de la lista)
void solicitarLibro(queue<Lector*> &cola, const string& nombre, int dni, const string& libroSolicitado) {
    Lector* nuevoLector = new Lector;
    Libro* libro = buscarLibro(listaLibros, libroSolicitado, "titulo");
    
    if (libro) {
        nuevoLector->nombre = nombre;
        nuevoLector->dni = dni;
        nuevoLector->libroSolicitado = libro->titulo;
        nuevoLector->autor = libro->autor;
        nuevoLector->anoEdicion = libro->anoEdicion;
        nuevoLector->editorial = libro->editorial;
        nuevoLector->isbn = libro->isbn;
        nuevoLector->paginas = libro->paginas;
        nuevoLector->siguiente = nullptr;

        cola.push(nuevoLector);

        // Eliminar el libro de la lista
        Libro* anterior = nullptr;
        Libro* actual = listaLibros;

        while (actual) {
            if (actual->titulo == libroSolicitado) {
                if (anterior) {
                    anterior->siguiente = actual->siguiente;
                } else {
                    listaLibros = actual->siguiente; 
                }
                delete actual; // Eliminar el libro de la lista
                break;
            }
            anterior = actual;
            actual = actual->siguiente;
        }

        string movimiento = "Libro solicitado: " + libroSolicitado;
        historial.push(movimiento);
        cout << "Movimiento: " << movimiento << endl; // Mostrar movimiento
        // Guardar movimiento en el archivo de historial
        ofstream archivoHistorial("historial.txt", ios::app);
        archivoHistorial << movimiento << endl;
        archivoHistorial.close();
    } else {
        cout << "El libro solicitado no está disponible." << endl;
        delete nuevoLector; // Liberar memoria si no se solicitó
    }
}

// Muestra los libros que han sido solicitados
void mostrarLibrosSolicitados(queue<Lector*> cola) {
    cout << "\n--- Libros Solicitados ---" << endl;
    int i = 1;
    while (!cola.empty()) {
        Lector* lector = cola.front();
        cout << i++ << ". " << lector->libroSolicitado << " (Autor: " << lector->autor << ")" << endl;
        cola.pop();
    }
}

// Devuelve un libro específico de la cola
void devolverLibro(queue<Lector*> &cola) {
    if (!cola.empty()) {
        mostrarLibrosSolicitados(cola);
        
        int seleccion;
        cout << "Seleccione el número del libro que desea devolver: ";
        cin >> seleccion;

        // Validar selección
        if (seleccion < 1 || seleccion > cola.size()) {
            cout << "Selección no válida." << endl;
            return;
        }

        // Eliminar el lector de la cola
        Lector* lectorAtendido = nullptr;
        for (int i = 1; i <= seleccion; ++i) {
            lectorAtendido = cola.front();
            if (i == seleccion) break;
            cola.pop();
        }

        // Eliminar el lector de la cola (devolver el libro)
        cola.pop();

        // Agregar el libro devuelto a la lista de libros con los datos originales
        agregarLibro(listaLibros, lectorAtendido->libroSolicitado, lectorAtendido->autor, 
                     lectorAtendido->anoEdicion, lectorAtendido->editorial, 
                     lectorAtendido->isbn, lectorAtendido->paginas);
        
        string movimiento = "Libro devuelto: " + lectorAtendido->libroSolicitado;"por " + lectorAtendido->nombre;
        historial.push(movimiento);
        cout << "Movimiento: " << movimiento << endl;
        
        ofstream archivoHistorial("historial.txt", ios::app);
        archivoHistorial << movimiento << endl;
        archivoHistorial.close();

        delete lectorAtendido; // Liberar memoria del lector atendido
    } else {
        cout << "No hay libros solicitados para devolver." << endl;
    }
}

// Guarda los libros actuales en el archivo biblioteca.txt
void guardarLibros(Libro* lista) {
    ofstream archivo("biblioteca.txt");
    if (!archivo) {
        cerr << "No se pudo abrir el archivo para guardar." << endl;
        return;
    }

    Libro* actual = lista;
    while (actual) {
        archivo << actual->titulo << "," << actual->autor << "," << actual->anoEdicion << ","
                << actual->editorial << "," << actual->isbn << "," << actual->paginas << endl;
        actual = actual->siguiente;
    }
    archivo.close();
    cout << "Libros guardados correctamente." << endl;
}

// Muestra el historial de movimientos
void mostrarHistorial() {
    cout << "\n--- Historial de Movimientos ---" << endl;
    stack<string> temp = historial;
    while (!temp.empty()) {
        cout << temp.top() << endl;
        temp.pop();
    }
}

// Vacía la biblioteca liberando la memoria
void vaciarBiblioteca(Libro* &lista) {
    while (lista) {
        Libro* temp = lista;
        lista = lista->siguiente;
        delete temp;
    }
    cout << "Biblioteca vaciada." << endl;
}

int main() {
    int opcion;

    do {
        cout << "\n--- Gestión de Biblioteca ---" << endl;
        cout << "1. Agregar libro" << endl;
        cout << "2. Solicitar libro" << endl;
        cout << "3. Devolver libro" << endl;
        cout << "4. Ordenar libros por título" << endl;
        cout << "5. Buscar libro" << endl;
        cout << "6. Guardar libros" << endl;
        cout << "7. Mostrar historial" << endl;
        cout << "8. Vaciar biblioteca" << endl;
        cout << "0. Salir" << endl;
        cout << "Ingrese una opción: "; cin >> opcion; cin.ignore();

        if (opcion == 1) {
            string titulo, autor, editorial, isbn;
            int ano, paginas;
            cout << "Título: "; getline(cin, titulo);
            cout << "Autor: "; getline(cin, autor);
            cout << "Año de edición: "; cin >> ano;
            cin.ignore();
            cout << "Editorial: "; getline(cin, editorial);
            cout << "ISBN: "; getline(cin, isbn);
            cout << "Número de páginas: "; cin >> paginas; cin.ignore();
            agregarLibro(listaLibros, titulo, autor, ano, editorial, isbn, paginas);
        } else if (opcion == 2) {
            string nombreLibro, nombreLector;
            int dniLector;
            cout << "Ingrese su nombre: "; getline(cin, nombreLector);
            cout << "Ingrese su DNI: "; cin >> dniLector; cin.ignore();
            cout << "Ingrese el título del libro solicitado: "; getline(cin, nombreLibro);
            solicitarLibro(colaSolicitudes, nombreLector, dniLector, nombreLibro);
        } else if (opcion == 3) {
            devolverLibro(colaSolicitudes);
        } else if (opcion == 4) {
            ordenarLibrosPorTitulo(listaLibros);
        } else if (opcion == 5) {
            buscarLibroPorClave();
        } else if (opcion == 6) {
            guardarLibros(listaLibros);
        } else if (opcion == 7) {
            mostrarHistorial();
        } else if (opcion == 8) {
            vaciarBiblioteca(listaLibros);
        }
    } while (opcion != 0);

    // Liberar memoria antes de salir
    while (listaLibros) {
        Libro* temp = listaLibros;
        listaLibros = listaLibros->siguiente;
        delete temp;
    }

    while (!colaSolicitudes.empty()) {
        delete colaSolicitudes.front(); // Liberar memoria del lector que solicito el libro
        colaSolicitudes.pop();
    }

    return 0;
}
