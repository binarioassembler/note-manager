/*
Cada alumno de una clase de licenciatura en Ciencias de la Computación tiene notas correspondientes a ocho asignaturas diferentes, pudiendo no tener calificación en alguna asignatura. A cada asignatura le corresponde un determinado coeficiente. Escribir un algoritmo que permita calcular la media de cada alumno.
Modificar el algoritmo para obtener las siguientes medias:
  * General de la clase.
  * De la clase en cada asignatura.
  * Porcentaje de faltas (no presentado a examen).
*/
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

const int TOTAL_DATA = 10, MAX_STUDENT = 3;
const std::string NAME_COLUMN[10] = {
  "DNI",
  "ESTUDIANTE",
  "PROGRAMACION I",
  "ALGORITMOS",
  "BASES DE DATOS",
  "MATEMATICAS DISCRETAS",
  "SISTEMAS OPERATIVOS",
  "DESARROLLO WEB",
  "INGENIERIA DE SOFTWARE",
  "REDES Y COMUNICACIONES"
};
const std::string LABEL_INPUT[10] = {
    "DNI del alumno: ",
    "nombre completo del alumno: ",
    "promedio final de Programacion I: ",
    "promedio final de Algoritmos: ",
    "promedio final de Bases de datos: ",
    "promedio final de Matematica Discretas: ",
    "promedio final de Sistemas Operativos: ",
    "promedio final de Desarrollo Web: ",
    "promedio final de Ingenieria de Software: ",
    "promedio final de Redes y Comunicaciones: "};
const int SETW_NUMBERS[4] = {0, 9, 25, 6};

// Funciones generales
std::string** registerStudent(std::string **matrix, int &size);
std::string dataValidator(std::string** &matrix, int size, int col);
void showTableStudents(std::string** matrix, int size);
void showTableHead();
void showFormatDataStudent(std::string **matrix, int row, int col);
std::string formatString(std::string &data, int lengthStr);
void freeMemory(std::string** &matrix, int size);
void selectProcess(std::string **&matrix, int &size, int op);
void displayMenu();

// Funciones auxiliares
bool successfulDataEntry(std::string** matrix, std::string &input, int size, int col);
bool validateIdentityDocument(std::string **matrix, std::string input, int size, int col);
std::string validateFullNameStudent(std::string &input);
bool validateStudentAverage(std::string input);
bool isEmptyString(std::string input);

int main()
{
  std::string **generalStudentInfo = nullptr;
  int row = 0, option = 0;

  do
  {
    displayMenu();
    std::cout << "> Elija una opcion (1-3): ";
    std::cin >> option;
    std::cin.ignore(100000,'\n');
    system("cls");

    selectProcess(generalStudentInfo, row, option);

  } while (option!=3);

  freeMemory(generalStudentInfo, row);

  return 0;
}

void displayMenu()
{
  std::cout << "\n========================================"
            << "\n=        GESTOR DE NOTAS (UPSB)        ="
            << "\n========================================\n"
            << "1. Registrar nuevo estudiante\n"
            << "2. Mostrar informacion completa\n"
            << "3. Salir del sistema\n"
            << "========================================" << std::endl;
}

void selectProcess(std::string** &matrix, int &size, int op)
{
  switch (op)
  {
  case 1:
    matrix = registerStudent(matrix, size);
    break;
  case 2:
    showTableStudents(matrix, size);
    break;
  case 3:
    std::cout << "Saliste del sistema..." << std::endl;
    break;
  default:
    std::cout << "Opcion no valida, fuera de rango." << std::endl;
    break;
  }
}

std::string** registerStudent(std::string** matrix, int &size)
{
  if (size==MAX_STUDENT)
  {
    std::cerr << "Limite alcanzado, usted ya no puede registrar mas estudiantes." << std::endl;
    return matrix;
  }

  std::string** newMatrix = new std::string*[size+1];

  // copiar valores a la nueva matriz
  for (int i = 0; i < size; i++)
  {
    newMatrix[i] = matrix[i];
  }


  // Reservar espacio para la nueva fila
  newMatrix[size] = new std::string[TOTAL_DATA];

  // Agregar nuevo alumno
  for (int i = 0; i < TOTAL_DATA; i++)
  {
    newMatrix[size][i] = dataValidator(newMatrix, size, i);
  }

  // liberar memoria de la matriz original
  delete[] matrix;

  // Actualizar tamaño de la matriz
  size++;

  return newMatrix; // devolver matriz actualizada
}

std::string dataValidator(std::string** &matrix, int size, int col)
{
  std::string input;
  bool value = false;
  do
  {
    std::cout << "Ingrese " << LABEL_INPUT[col];
    std::getline(std::cin, input);
    system("cls");
    value = successfulDataEntry(matrix, input, size, col);
  } while (!value);

  system("cls");
  return input;
}

bool successfulDataEntry(std::string** matrix, std::string &input, int size, int col)
{
  if (!isEmptyString(input))
  {
    std::cerr << "El dato ingresado no puede ser vacio" << std::endl;
    return false;
  }

  if (col==0)
  {
    // validar documento de indentidad
    return validateIdentityDocument(matrix, input, size, col);
  }

  if (col==1)
  {
    // validar nombre completo del alumno
    input = validateFullNameStudent(input);
  }

  if (col!=0 && col!=1)
  {
    // validar notas
    return validateStudentAverage(input);
  }

  return true;
}

bool validateIdentityDocument(std::string** matrix, std::string input, int size, int col)
{
  int lenght = input.size();

  // filtro 1: verificar el total de dígitos del dni
  if (lenght!=8)
  {
    std::cerr << "El DNI debe tener 8 digitos. Vuelva a intentarlo por favor." << std::endl;
    return false;
  }

  // filtro 2: verificar si el dni esta compuesto por dígitos
  for (int i = 0; i < lenght; i++)
  {
    if (!isdigit(input[i]))
    {
      std::cerr << "El DNI solo debe contener digitos, intente de nuevo por favor." << std::endl;
      return false;
    }
  }

  // filtro 3: verificar que el DNI no exista
  for (int i = 0; i < size; i++)
  {
    if (matrix[i][col]==input)
    {
      std::cout << "Este DNI " << matrix[i][col] << " ya existe en la base de datos. Intente de nuevo." << std::endl;
      return false;
    }
  }

  // filtro 3: retornar verdadero si el DNI es nuevo
  return true;
}

std::string validateFullNameStudent(std::string &input)
{
  // convertir todo el nombre a mayúsculas
  for (char &c : input)
  {
    c = toupper(c);
  }

  return input;
}

bool validateStudentAverage(std::string input)
{
  int length = input.size(), counter = 0;

  for (int i = 0; i < length; i++)
  {
    if (input[i] == '.')
    {
      counter++; // contabilizar los puntos decimales
    }else {
      // filtro 1: verificar si son digitos las notas
      if (!isdigit(input[i]))
      {
        std::cerr << "Los promedios son de tipo decimal, debe tener solo digitos. Vuelva a intentarlo." << std::endl;
        return false;
      }
    }

    // filtro 2: detectar si hay mas de un punto decimal
    if (counter>1)
    {
      std::cerr << "Se ha detectado un caracter, no valido para el formato de promedio de nota. Vuelva a intentarlo." << std::endl;
      return false;
    }
  }

  // filtro 3: verificar si el promedio está dentro del rango 1 a 20
  double value = std::stod(input);
  if (value<0.0 || value>20.0)
  {
    std::cerr << "Promedio ingresado fuera del rango (1 a 20), intente de nuevo." << std::endl;
    return false;
  }

  return true;
}

bool isEmptyString(std::string input)
{
  // Verificar si la cadena contiene al menos un carácter no blanco
  // std::string::find_first_not_of: verifica si la cadena contiene algo que no sea un espacio, tabulación, o salto de línea
  return input.find_first_not_of(" \t\n\r") != std::string::npos;
}

void showTableStudents(std::string** matrix, int size)
{
  if (size == 0 || matrix == nullptr)
  {
    std::cout << "No hay estudiantes registrados, debe registrar uno como minimo." << std::endl;
    return;
  }

  showTableHead();
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < TOTAL_DATA; j++)
    {
      showFormatDataStudent(matrix, i, j);
    }
  }
}

void showTableHead()
{
  std::string header;
  std::cout << "*** Promedios de todos los alumnos de la facultad de Ingenieria y Ciencias de la Computacion - I Ciclo ***" << std::endl;
  std::cout << "\n|---------|-------------------------|------|------|------|------|------|------|------|------|\n";
  for (int i = 0; i < TOTAL_DATA; i++)
  {
    header = NAME_COLUMN[i];
    if (i==0)
    {
      std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                << std::setw(SETW_NUMBERS[1]) << formatString(header, 8);
    }else if(i==1){
      std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                << std::setw(SETW_NUMBERS[2]) << formatString(header, 10);
    }else if(i+1==TOTAL_DATA){
      std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                << std::setw(SETW_NUMBERS[3]) << formatString(header, 6)
                << std::setw(SETW_NUMBERS[0]) << "|" << std::endl;
    }else {
      std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                << std::setw(SETW_NUMBERS[3]) << formatString(header, 6);
    }
  }
  std::cout << "|---------|-------------------------|------|------|------|------|------|------|------|------|\n";
}

void showFormatDataStudent(std::string** matrix, int row, int col)
{
  std::string data = matrix[row][col];

  if (col == 0) {
    std::cout << std::setw(SETW_NUMBERS[0]) << "|"
              << std::setw(SETW_NUMBERS[1]) << formatString(data, SETW_NUMBERS[1]);
  }else if (col == 1) {
    std::cout << std::setw(SETW_NUMBERS[0]) << "|"
              << std::setw(SETW_NUMBERS[2]) << formatString(data, SETW_NUMBERS[2]);
  }else if (col + 1 == TOTAL_DATA) {
    std::cout << std::setw(SETW_NUMBERS[0]) << "|"
              << std::setw(SETW_NUMBERS[3]) << formatString(data, SETW_NUMBERS[3])
              << std::setw(SETW_NUMBERS[0]) << "|" << std::endl;
    std::cout << "|---------|-------------------------|------|------|------|------|------|------|------|------|\n";
  }else {
    std::cout << std::setw(SETW_NUMBERS[0]) << "|"
              << std::setw(SETW_NUMBERS[3]) << formatString(data, SETW_NUMBERS[3]);
  }
}

std::string formatString(std::string &data, int lengthStr)
{
  std::string newString;
  int sizeStr = data.size();

  if (sizeStr>lengthStr)
  {
    for (int i = 0; i < lengthStr; i++)
    {
      newString += data[i];
    }
  }else {
    return data;
  }

  return newString;
}

void freeMemory(std::string** &matrix, int size)
{
  for (int i = 0; i < size; i++)
  {
    delete matrix[i];
  }
  delete[] matrix;

  matrix = nullptr;
}