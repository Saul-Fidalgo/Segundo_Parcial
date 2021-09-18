#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#ifdef __linux__
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
    int _getch ();
#else
    #include <conio.h>
    #define CLEAR_SCREEN "cls"
#endif
using namespace std;

class Empleado {
public:
    string codigo_empleado;
    string apellidos;
    string nombres;
    string departamento;
    string puesto;
    float sueldo;

    Empleado() {}
    ~Empleado() {}

    bool operator==(const Empleado &empleado) const
    {
        return this==&empleado || this->codigo_empleado==codigo_empleado;
    }

    bool operator!=(const Empleado &empleado) const
    {
        return this!=&empleado && this->codigo_empleado!=codigo_empleado;
    }

    bool operator<(const Empleado &empleado) const
    {
        return this->codigo_empleado<empleado.codigo_empleado;
    }

    Empleado& operator=(const Empleado &empleado)
    {
        if (this!=&empleado)
        {
            this->codigo_empleado = empleado.codigo_empleado;
            this->apellidos = empleado.apellidos;
            this->nombres = empleado.nombres;
            this->departamento = empleado.departamento;
            this->puesto = empleado.puesto;
            this->sueldo = empleado.sueldo;
        }
        return *this;
    }

    static void print (Empleado &empleado, int *counter);
    static void print_on_file (Empleado &empleado, ostream *file_stream);
};

template <class T>
class LinkedList
{
private:
    LinkedList<T> *link;
    T datum;
    LinkedList() {};
    ~LinkedList()
    {
        if (link!=NULL)
            delete link;
    }
public:
    static LinkedList<T>* insert  (LinkedList<T> *header, const T &datum);
    static T*             search  (LinkedList<T> *header, const T &datum);
    static LinkedList<T>* remove  (LinkedList<T> *header, const T &datum);
    static void           foreach (LinkedList<T> *header, void *params, void (*func)(T&, void*));
    static bool           sort    (LinkedList<T> *header);
};

void   pause_screen (const char *message);
string read_str (const char *message);
int    read_integer (const char *message);
float  read_decimal (const char *message);
bool   read_field (istream &file_stream, char *field, char delimiter);

const char *path = "empleados.tsv";

int main ()
{
    LinkedList<Empleado> *header=NULL;
    Empleado *datum, empleado;
    int counter=0, main_option, suboption;
    char field[255];
    ifstream file_stream (path);
    if (file_stream!=NULL)
    {
        while (read_field (file_stream, field, '\t'))
        {
            empleado.codigo_empleado = field;
            read_field (file_stream, field, '\t');
            empleado.apellidos = field;
            read_field (file_stream, field, '\t');
            empleado.nombres = field;
            read_field (file_stream, field, '\t');
            empleado.departamento = field;
            read_field (file_stream, field, '\t');
            empleado.puesto = field;
            read_field (file_stream, field, '\n');
            empleado.sueldo = atof (field);
            header = LinkedList<Empleado>::insert (header, empleado);
        }
        file_stream.close();
    }
    do {
        cout << endl;
        system (CLEAR_SCREEN);
        cout << "MEN\351" << endl;
        cout << "1.- Registros" << endl;
        cout << "2.- Consultas" << endl;
        cout << "3.- Actualizaciones" << endl;
        cout << "4.- Bajas" << endl;
        cout << "5.- Ordenar registros" << endl;
        cout << "6.- Listar registros" << endl;
        cout << "7.- Salir" << endl;
        cout << "Selecciona una opci\242n: ";
        do
            main_option = _getch ();
        while (main_option<'1' || main_option>'7');
        cout << (char)main_option << endl << endl;
        if (header==NULL && main_option!='1' && main_option!='7')
        {
            pause_screen ("No hay registros.\n");
            continue;
        }
        if (main_option<'5')
        {
            empleado.codigo_empleado = read_str ("Ingresa el codigo del empleado");
            datum = LinkedList<Empleado>::search (header, empleado);
            if (datum!=NULL)
            {
                cout << endl;
                Empleado::print (*datum, &counter);
            }
        }
        if (main_option=='1' && datum!=NULL)
            cout << "El registro ya existe." << endl;
        else if (main_option>='2' && main_option<='4' && datum==NULL)
            cout << endl << "Registro no encontrado." << endl;
        else switch (main_option)
        {
            case '1':
                empleado.apellidos = read_str ("Ingresa el apellidos");
                empleado.nombres = read_str ("Ingresa el nombres");
                empleado.departamento = read_str ("Ingresa el departamento");
                empleado.puesto = read_str ("Ingresa el puesto");
                empleado.sueldo = read_decimal ("Ingresa el sueldo");
                header = LinkedList<Empleado>::insert (header, empleado);
                cout << endl << "Registro agregado correctamente." << endl;
                break;
            case '3':
                cout << "Men\243 de actualizaci\242n de campos" << endl;
                cout << "1.- apellidos" << endl;
                cout << "2.- nombres" << endl;
                cout << "3.- departamento" << endl;
                cout << "4.- puesto" << endl;
                cout << "5.- sueldo" << endl;
                do {
                    suboption = read_integer ("Selecciona el n\243mero de campo a modificar");
                    if (suboption<1 || suboption>5)
                        printf ("Opci\242n incorrecta\n");
                } while (suboption<1 || suboption>5);
                switch (suboption)
                {
                    case 1:
                        datum->apellidos = read_str ("Ingresa el nuevo apellidos");
                        break;
                    case 2:
                        datum->nombres = read_str ("Ingresa el nuevo nombres");
                        break;
                    case 3:
                        datum->departamento = read_str ("Ingresa el nuevo departamento");
                        break;
                    case 4:
                        datum->puesto = read_str ("Ingresa el nuevo puesto");
                        break;
                    case 5:
                        datum->sueldo = read_decimal ("Ingresa el nuevo sueldo");
                        break;
                }
                cout << endl << "Registro actualizado correctamente." << endl;
                break;
            case '4':
                header = LinkedList<Empleado>::remove (header, *datum);
                cout << "Registro eliminado correctamente." << endl;
                break;
            case '5':
                LinkedList<Empleado>::sort (header);
                cout << "Registros ordenados correctamente." << endl;
                break;
            case '6':
                counter = 0;
                LinkedList<Empleado>::foreach (header, &counter, (void(*)(Empleado&,void*))Empleado::print);
                cout << "Total de registros: " << counter << "." << endl;
                break;
        }
        if (main_option!='7')
            pause_screen ("");
    } while (main_option!='7');
    ofstream output_stream (path);
    if (output_stream!=NULL)
    {
        LinkedList<Empleado>::foreach (header, &output_stream, (void(*)(Empleado&,void*))Empleado::print_on_file);
        output_stream.close();
    }
    return EXIT_SUCCESS;
}

template <class T>
LinkedList<T>* LinkedList<T>::insert (LinkedList<T> *header, const T &datum)
{
    LinkedList<T>* node = new LinkedList<T>();
    node->link = header;
    node->datum = datum;
    return node;
}

template <class T>
T* LinkedList<T>::search (LinkedList<T> *node, const T &datum)
{
    return node==NULL ? NULL : (node->datum==datum ? &node->datum : search (node->link, datum));
}

template <class T>
LinkedList<T>* LinkedList<T>::remove (LinkedList<T> *node, const T &datum)
{
    if (node!=NULL)
    {
        if (node->datum==datum)
        {
            LinkedList<T>* link = node->link;
            node->link = NULL;
            delete node;
            node = link;
        }
        else
            node->link = remove (node->link, datum);
    }
    return node;
}

template <class T>
void LinkedList<T>::foreach (LinkedList<T> *node, void *params, void (*func)(T&, void*))
{
    if (node!=NULL)
    {
        foreach (node->link, params, func);
        func (node->datum, params);
    }
}

template <class T>
bool LinkedList<T>::sort (LinkedList<T> *node)
{
    bool changed=false;
    if (node!=NULL && node->link!=NULL)
    {
        if (node->datum < node->link->datum)
        {
            T datum = node->datum;
            node->datum = node->link->datum;
            node->link->datum = datum;
            changed = true;
        }
        changed = changed || sort (node->link);
        if (changed)
            sort (node);
    }
    return changed;
}

void Empleado::print (Empleado &empleado, int *counter)
{
    cout << "codigo del empleado: " << empleado.codigo_empleado.c_str() << endl;
    cout << "apellidos       : " << empleado.apellidos.c_str() << endl;
    cout << "nombres         : " << empleado.nombres.c_str() << endl;
    cout << "departamento    : " << empleado.departamento.c_str() << endl;
    cout << "puesto          : " << empleado.puesto.c_str() << endl;
    cout << "sueldo          : " << empleado.sueldo << endl;
    cout << endl;
    (*counter)++;
}

void Empleado::print_on_file (Empleado &empleado, ostream *file_stream)
{
    *file_stream << empleado.codigo_empleado.c_str() << "\t";
    *file_stream << empleado.apellidos.c_str() << "\t";
    *file_stream << empleado.nombres.c_str() << "\t";
    *file_stream << empleado.departamento.c_str() << "\t";
    *file_stream << empleado.puesto.c_str() << "\t";
    *file_stream << empleado.sueldo << endl;
}

string read_str (const char *message)
{
    char string_var[255];
    cout << message << ": ";
    cin.get (string_var, sizeof (string_var), '\n');
    cin.get();
    string str(string_var);
    return str;
}

int read_integer (const char *message)
{
    int integer_var;
    cout << message << ": ";
    cin >> integer_var;
    cin.get();
    return integer_var;
}

float read_decimal (const char *message)
{
    float decimal_var;
    cout << message << ": ";
    cin >> decimal_var;
    cin.get();
    return decimal_var;
}

bool read_field (istream &file_stream, char *str, char delimiter)
{
    file_stream.get (str, 255, delimiter);
    file_stream.get();
    return !file_stream.eof();
}

void pause_screen (const char *message)
{
    cout << message << endl << "Presiona una tecla para continuar . . . ";
    _getch ();
}

#ifdef __linux__

int _getch ()
{
    int ch;
    struct termios oldt, newt;
    tcgetattr (STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr (STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr (STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

#endif
