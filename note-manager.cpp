/*
Cada alumno de una clase de licenciatura en Ciencias de la Computación tiene notas correspondientes a ocho asignaturas diferentes, pudiendo no tener calificación en alguna asignatura. A cada asignatura le corresponde un determinado coeficiente. Escribir un algoritmo que permita calcular la media de cada alumno.
Modificar el algoritmo para obtener las siguientes medias:
  * General de la clase.
  * De la clase en cada asignatura.
  * Porcentaje de faltas (no presentado a examen).
*
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

// VALORES CONSTANTES
const int TOTAL_DATA = 10, MAX_STUDENT = 6;
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
    "promedio final de 'Programacion I': ",
    "promedio final de 'Algoritmos': ",
    "promedio final de 'Bases de datos': ",
    "promedio final de 'Matematica Discretas': ",
    "promedio final de 'Sistemas Operativos': ",
    "promedio final de 'Desarrollo Web': ",
    "promedio final de 'Ingenieria de Software': ",
    "promedio final de 'Redes y Comunicaciones': "
};
const std::string NAME_COLUMN_BOOKLET[3]{
  "ASIGNATURA",
  "PROMEDIO",
  "CREDITOS"
};
const int SETW_NUMBERS[5] = {0, 9, 25, 6, 10};
const int CREDITS[4] = {1,2,3,4};

/// MODULO 1: registrar estudiante
std::string **registerStudent(std::string **matrix, int &size);

/// MODULO 2: consultar notas
void selectProcessQueries(std::string **&matrix, int &size);
void selectOptionView(std::string **&matrix, int &size, int op);
void queryProcess(void (*query)(std::string **matrix, int size), std::string **&matrix, int &size);
void showAverageStudent(std::string **matrix, int size);
void showFacultyAverage(std::string **matrix, int size);
void showAveragesGeneralStudents(std::vector<std::vector<double>> avg, std::string **matrix, int size, int t_avg, int t_courses, double sum_avg);
void displayTableHeadAllAverages();
void displayTableBodyAllAverages(std::vector<std::vector<double>> avg, std::string **matrix, int size, int t_courses);
void calculateTotalAverages(std::vector<std::vector<double>> avg, int size, int t_courses);
std::vector<std::vector<double>> assignValues(std::vector<std::vector<double>> &avg, std::string **matrix, int row, int columns, int index);
double addAllAverages(std::vector<std::vector<double>> avg, int row, int columns);
void showHeadBooklet();
void formatDataForBooklet(std::string **matrix, int index, int column, std::vector<double> &container);
void calculateIndividualAverage(std::vector<double> container);
int searchFaults(double number, int &faults);

/// MODULO 3: modificar datos
std::string **analyzeModification(std::string **(*operate)(std::string **&matrix, int size, int index, int col), std::string **&matrix, int size, int op);
std::string **modifyData(std::string **&matrix, int size, int index, int col);
std::string **modifyAverage(std::string **&matrix, int size, int index, int col);
std::string **modifyStudent(std::string **&matrix, int &size);
std::string **selectProcessModifyAverage(std::string **&matrix, int size, int index, int op);
std::string **selectProcessModify(std::string **&matrix, int size, int op);
std::string** modifyAllFields(std::string **&matrix, int size, int index, int col);
void messageModifyData(int column, std::string dataBefore, std::string dataAfter);

/// MODULO 4: dar de baja
std::string **removeStudent(std::string **&matrix, int &size);
bool searchDocument(std::string **matrix, int size, int &key);
std::string **modifiyAndDeleteArray(std::string **&matrix, int &size, int &key);

// MENUS
void displayMenu();
void displayMenuQueries();
void displayMenuModify();
void displaySubMenuModify(int col);

/// FUNCIONES GENERALES
void selectProcess(std::string** &matrix, int &size, int op);
std::string dataValidator(std::string **&matrix, int size, int col);
bool successfulDataEntry(std::string** matrix, std::string &input, int size, int col);
bool validateIdentityDocument(std::string** matrix, std::string input, int size, int col);
std::string validateFullNameStudent(std::string &input);
bool validateStudentAverage(std::string input);
bool isEmptyString(std::string input);
double sendCredits(int index, double &credit);
std::string sendCredits(int column);
bool findStudent(std::string **matrix, int size, std::string &input, int &key);
void showIndividualData(std::string **matrix, int index, int op);
void showIndividualDataTextPlane(std::string **matrix, int index);
void showTableHead();
void showTableStudents(std::string** matrix, int size);
void showFormatDataStudent(std::string** matrix, int row, int col);
std::string formatString(std::string &data, int lengthStr);
void freeMemory(std::string** &matrix, int size);

int main()
{
  std::string **generalStudentInfo = nullptr;
  int row = 0, option = 0;

  do
  {
    displayMenu();
    std::cout << "> Elija una opcion (1-5): ";
    std::cin >> option;
    std::cin.ignore(100000,'\n');
    system("cls");

    selectProcess(generalStudentInfo, row, option);

  } while (option!=5);

  freeMemory(generalStudentInfo, row);

  return 0;
}

void displayMenu()
{
  std::cout << "\n========================================"
            << "\n=        GESTOR DE NOTAS (UPSB)        ="
            << "\n========================================\n"
            << "1. Registrar nuevo estudiante\n"
            << "2. Realizar consultas\n"
            << "3. Modificar datos del estudiante\n"
            << "4. Eliminar Estudiante\n"
            << "5. Salir del sistema (*)\n"
            << "========================================" << std::endl;
}

void displayMenuQueries()
{
  std::cout << "\n==============================================="
            << "\n=           MODULO: CONSULTAS (UPSB)           ="
            << "\n===============================================\n"
            << "1. Ver listado completo de los estudiantes\n"
            << "2. Ver media del estudiante\n"
            << "3. Ver media general\n"
            << "4. Salir del modulo (*)\n"
            << "===============================================\n"
            << "> Seleccione, la consulta a realizar (1-4): ";
}

void displayMenuModify()
{
  std::cout << "\n==============================================="
            << "\n=        MODULO: MODIFICAR DATOS (UPSB)        ="
            << "\n===============================================\n"
            << "1. Cambiar documento del estudiante\n"
            << "2. Cambiar nombres completos del estudiante\n"
            << "3. Cambiar promedio final del estudiante\n"
            << "4. Cambiar todos los datos del estudiante\n"
            << "5. Salir del modulo (*)\n"
            << "===============================================\n"
            << "> Seleccione, que dato desea modificar? (1-5): ";
}

void displaySubMenuModify(int col)
{
  int aux = col - 1, list = 1;

  std::cout << "\n==============================================="
            << "\n=   MODULO: MODIFICAR PROMEDIO FINAL (UPSB)    ="
            << "\n===============================================\n";
  for (int j = aux; j < TOTAL_DATA; j++)
  {
    std::cout << list << ". Cambiar promedio de '" << NAME_COLUMN[j] << "'\n";
    list++;
  }
  std::cout << list << ". Salir del modulo (*)\n";
  std::cout << "===============================================\n"
            << "> Que promedio desea modificar? (1-" << list << "): ";
}

void selectProcess(std::string** &matrix, int &size, int op)
{
  switch (op)
  {
  case 1:
    matrix = registerStudent(matrix, size);
    break;
  case 2:
    selectProcessQueries(matrix, size);
    break;
  case 3:
    matrix = modifyStudent(matrix, size);
    break;
  case 4:
    matrix = removeStudent(matrix, size);
    break;
  case 5:
  std::cout << "Saliste del sistema..." << std::endl;
  break;
  default:
    std::cout << "Opcion no valida, fuera de rango." << std::endl;
    break;
  }
}

/// @brief Modulo 1: Registrar nuevo estudiante
/// @param matrix
/// @param size
/// @return matrix
std::string **registerStudent(std::string **matrix, int &size)
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

/// @brief Modulo 2: Realizar consultas a la BD
/// @param matrix
/// @param size
void selectProcessQueries(std::string **&matrix, int &size)
{
  int option = 0;

  if (size == 0 || matrix == nullptr)
  {
    std::cout << "No hay estudiantes en la base de datos para hacer consultas." << std::endl;
    return;
  }

  do
  {
    displayMenuQueries();
    std::cin >> option;
    std::cin.ignore(100000, '\n');
    system("cls");

    selectOptionView(matrix, size, option);

  } while (option != 4);
}

void selectOptionView(std::string **&matrix, int &size, int op)
{
  switch (op)
  {
  case 1:
    showTableStudents(matrix, size);
    break;
  case 2:
    queryProcess(showAverageStudent, matrix, size);
    break;
  case 3:
    queryProcess(showFacultyAverage, matrix, size);
    break;
  case 4:
    std::cout << "Saliste del modulo de consultas..." << std::endl;
    break;
  default:
    std::cout << "Opcion no valida, fuera del rango." << std::endl;
    break;
  }
}

void queryProcess(void (*query)(std::string **matrix, int size), std::string **&matrix, int &size)
{
  // Calcular la media requerida
  query(matrix, size);
}

void showAverageStudent(std::string **matrix, int size)
{
  std::string input = "";
  int key = -1; // Indice del estudiante encontrado
  bool value = false;

  value = findStudent(matrix, size, input, key);

  if (!value)
  {
    return;
  }
  else
  {
    system("cls");
  }

  std::cout << "*** DATOS DEL ESTUDIANTE ***" << std::endl;
  std::vector<double> containerAverages;
  for (int i = 0; i < TOTAL_DATA; i++)
  {
    if (i == 0 || i == 1)
    {
      if (i == 1)
      {
        std::cout << NAME_COLUMN[i] << ": " << matrix[key][i] << "\n";
        showHeadBooklet();
      }
      else
      {
        std::cout << NAME_COLUMN[i] << ": " << matrix[key][i] << "\n";
      }
    }
    else
    {
      formatDataForBooklet(matrix, key, i, containerAverages);
    }
  }

  // Calcular media general del estudiante y su porcentaje de faltas
  calculateIndividualAverage(containerAverages);
}

void showFacultyAverage(std::string **matrix, int size)
{
  int init = 2, totalCourses = TOTAL_DATA - init;
  double sum = 0.0;
  std::vector<std::vector<double>> container(size, std::vector<double>(totalCourses, 0.0));
  int totalAvg = container.size() * container[0].size();

  container = assignValues(container, matrix, size, totalCourses, init); // guardar todos los promedios de la clase
  sum = addAllAverages(container, size, totalCourses);                   // sumar todos los promedios de la clase
  showAveragesGeneralStudents(container, matrix, size, totalAvg, totalCourses, sum);
}

void showAveragesGeneralStudents(std::vector<std::vector<double>> avg, std::string **matrix, int size, int t_avg, int t_courses, double sum_avg)
{
  displayTableHeadAllAverages();
  displayTableBodyAllAverages(avg, matrix, size, t_courses);
  // Calcular todas las medias
  calculateTotalAverages(avg, size, t_courses);
  std::cout << "-> Promedio General: " << sum_avg / t_avg << std::endl;
  std::cout << "-> Total de promedios: " << t_avg << std::endl;
  std::cout << "-> Total de columnas: " << t_courses << std::endl;
  std::cout << "-> Total de estudiantes: " << size << std::endl;
}

void displayTableHeadAllAverages()
{
  std::string header;
  const int totalLetters = 8;

  std::cout << "*** Vista de todos los promedios de la facultad ***\n";
  std::cout << "\n|---------|---------|---------|---------|---------|---------|---------|---------|---------|\n";
  for (int i = 0; i < TOTAL_DATA; i++)
  {
    header = NAME_COLUMN[i];
    if (i != 1)
    {
      if (i + 1 == TOTAL_DATA)
      {
        std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                  << std::setw(SETW_NUMBERS[1]) << formatString(header, totalLetters)
                  << std::setw(SETW_NUMBERS[0]) << "|" << std::endl;
      }
      else
      {
        std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                  << std::setw(SETW_NUMBERS[1]) << formatString(header, totalLetters);
      }
    }
  }
  std::cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|\n";
}

void displayTableBodyAllAverages(std::vector<std::vector<double>> avg, std::string **matrix, int size, int t_courses)
{
  // Formatear tabla
  std::string dni;
  for (int i = 0, j = 0; i < size; i++)
  {
    dni = matrix[i][j];
    std::cout << std::setw(SETW_NUMBERS[0]) << "|"
              << std::setw(SETW_NUMBERS[1]) << formatString(dni, SETW_NUMBERS[1]);
    for (int k = 0; k < t_courses; k++)
    {
      if (k + 1 == t_courses)
      {
        std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                  << std::setw(SETW_NUMBERS[1]) << std::fixed << std::setprecision(2) << avg[i][k]
                  << std::setw(SETW_NUMBERS[0]) << "|\n"
                  << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << std::endl;
      }
      else
      {
        std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                  << std::setw(SETW_NUMBERS[1]) << std::fixed << std::setprecision(2) << avg[i][k];
      }
    }
  }
}

void calculateTotalAverages(std::vector<std::vector<double>> avg, int size, int t_courses)
{
  std::cout << std::setw(SETW_NUMBERS[0]) << "|" << std::setw(SETW_NUMBERS[1]) << "MEDIA";

  for (int i = 0; i < t_courses; i++) // 3 (0,1,2,3,4,5,6,7)
  {
    double total = 0.0;

    for (int j = 0; j < size; j++) // (0,1,2,3)
    {
      // (0,1),(1,1),(2,1),(3,1)
      total += avg[j][i];
    }

    double average = total / static_cast<double>(size);

    if (i + 1 == t_courses)
    {
      std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                << std::setw(SETW_NUMBERS[1]) << std::fixed << std::setprecision(2) << average
                << std::setw(SETW_NUMBERS[0]) << "|\n"
                << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << std::endl;
    }
    else
    {
      std::cout << std::setw(SETW_NUMBERS[0]) << "|"
                << std::setw(SETW_NUMBERS[1]) << std::fixed << std::setprecision(2) << average;
    }
  }
}

std::vector<std::vector<double>> assignValues(std::vector<std::vector<double>> &avg, std::string **matrix, int row, int columns, int index)
{
  double value = 0.0;
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      value = std::stod(matrix[i][j + index]); // convertir dato de string  a double
      avg[i][j] = value;
    }
  }

  return avg;
}

double addAllAverages(std::vector<std::vector<double>> avg, int row, int columns)
{
  double add = 0.0;
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      add += avg[i][j];
    }
  }

  return add;
}

void showHeadBooklet()
{
  std::cout << "\n|-------------------------|---------|---------|\n"
            << std::setw(SETW_NUMBERS[0]) << "|"
            << std::setw(SETW_NUMBERS[2]) << NAME_COLUMN_BOOKLET[0]
            << std::setw(SETW_NUMBERS[0]) << "|"
            << std::setw(SETW_NUMBERS[1]) << NAME_COLUMN_BOOKLET[1]
            << std::setw(SETW_NUMBERS[0]) << "|"
            << std::setw(SETW_NUMBERS[1]) << NAME_COLUMN_BOOKLET[2]
            << std::setw(SETW_NUMBERS[0]) << "|\n"
            << "|-------------------------|---------|---------|" << std::endl;
}

void formatDataForBooklet(std::string **matrix, int index, int column, std::vector<double> &container)
{
  double average = 0.0;                     // valor para guardar en el vector (media del estudiante)
  std::string credit = sendCredits(column); // Buscar crédito
  std::string prom = matrix[index][column]; // valor del promedio de la asignatura
  std::string head = NAME_COLUMN[column];

  std::cout << std::setw(SETW_NUMBERS[0]) << "|"
            << std::setw(SETW_NUMBERS[2]) << head
            << std::setw(SETW_NUMBERS[0]) << "|"
            << std::setw(SETW_NUMBERS[1]) << prom
            << std::setw(SETW_NUMBERS[0]) << "|"
            << std::setw(SETW_NUMBERS[1]) << credit
            << std::setw(SETW_NUMBERS[0]) << "|\n"
            << "|-------------------------|---------|---------|" << std::endl;

  // Agregar el promedio al contenedor de tipo vector
  average = std::stod(prom);
  container.push_back(average);
}

void calculateIndividualAverage(std::vector<double> container)
{
  double avgFinal = 0.0, add = 0.0, avgFaults = 0.0, numCredit = 0.0, addCredits = 0.0;
  std::string strCredit;
  int size = container.size(), count_f = 0;

  for (int i = 0; i < size; i++)
  {
    numCredit = sendCredits(i, numCredit);         // buscar el número de créditos de la asignatura
    add += container[i] * numCredit;               // calcular todos los promedios multiplicándolo por cada crédito que corresponda según la asignatura
    addCredits += numCredit;                       // sumar totos los créditos
    count_f = searchFaults(container[i], count_f); // contabilizar total de promedios en 0
  }

  // Cacular promedio general del estudiante
  avgFinal = add / addCredits;
  // Calcular porcentaje de faltas del estudiante
  avgFaults = 100.0 / size * count_f;
  // Imprimir resultados
  std::cout << "-> Media: " << std::fixed << std::setprecision(2) << avgFinal << std::endl;
  std::cout << "-> Porcentaje de faltas: " << avgFaults << "%" << std::endl;
}

int searchFaults(double number, int &faults)
{
  if (number < 1.0)
  {
    faults++;
  }

  return faults;
}

/// @brief Modulo 3: Modificar datos
/// @param matrix
/// @param size
/// @return matrix
std::string **modifyStudent(std::string **&matrix, int &size)
{
  if (size == 0 || matrix == nullptr)
  {
    std::cout << "No hay estudiantes en la base de datos para hacer modificaciones." << std::endl;
    return matrix;
  }

  int option = 0;
  do
  {
    displayMenuModify();
    std::cin >> option;
    std::cin.ignore(100000, '\n');
    system("cls");

    matrix = selectProcessModify(matrix, size, option);

  } while (option != 5);

  return matrix;
}

std::string **selectProcessModify(std::string **&matrix, int size, int op)
{
  switch (op)
  {
  case 1:
  case 2:
    matrix = analyzeModification(modifyData, matrix, size, op);
    break;
  case 3:
    matrix = analyzeModification(modifyAverage, matrix, size, op);
    break;
  case 4:
    matrix = analyzeModification(modifyAllFields, matrix, size, op);
    break;
  case 5:
    std::cout << "Volver al menu de Inicio..." << std::endl;
    break;
  default:
    std::cout << "Opcion no valida, fuera de rango." << std::endl;
    break;
  }

  return matrix;
}

std::string **analyzeModification(std::string **(*operate)(std::string **&matrix, int size, int index, int col), std::string **&matrix, int size, int op)
{
  std::string input = "";
  int key = -1;
  bool value = false;

  value = findStudent(matrix, size, input, key); // buscar al estudiante por DNI

  // Si no existe el DNI en la BD devolver el arreglo completo
  if (!value)
  {
    return matrix;
  }

  // Si se detecta el DNI actualizar la información que corresponda
  showIndividualData(matrix, key, op);
  return operate(matrix, size, key, op);
}

std::string **modifyData(std::string **&matrix, int size, int index, int col)
{
  int dataIndex = col - 1;
  std::string newData = "", oldData = matrix[index][dataIndex];
  bool value = false;

  // Verificar que el dato modificado no este vacío
  while (!isEmptyString(newData))
  {
    std::cout << "Ingrese el nuevo dato a modificar en el campo '" << NAME_COLUMN[dataIndex] << "': ";
    std::getline(std::cin, newData);
    system("cls");
  }

  // En caso la opción sea cambiar el DNI se deberá validar los requisitos para dicha modificación
  if (col - 1 == 0)
  {
    value = validateIdentityDocument(matrix, newData, size, dataIndex);
    // Si los requisitos no son correctos, retornar la matriz original
    if (!value)
    {
      return matrix;
    }
  }

  // Modificar dato requerido
  matrix[index][dataIndex] = validateFullNameStudent(newData);
  messageModifyData(dataIndex, oldData, newData);
  return matrix;
}

std::string **modifyAverage(std::string **&matrix, int size, int index, int col)
{
  int option = 0, maxOption = TOTAL_DATA - 1;

  do
  {
    displaySubMenuModify(col);
    std::cin >> option;
    std::cin.ignore(100000, '\n');
    system("cls");

    if (option >= 1 && option <= maxOption - 1)
    {
      // Modificando un promedio (opciones 1-8)
      matrix = selectProcessModifyAverage(matrix, size, index, option);
    }
    else if (option != maxOption)
    {
      std::cerr << "Opcion no valida, intente de nuevo." << std::endl;
    }

  } while (option != TOTAL_DATA - 1);

  std::cout << "Saliendo del sub-modulo de promedios..." << std::endl;
  return matrix;
}

std::string **selectProcessModifyAverage(std::string **&matrix, int size, int index, int op)
{
  (void)size;
  int courseIndex = op + 1;
  std::string newAverage = "", oldAverage = matrix[index][courseIndex];

  while (!isEmptyString(newAverage))
  {
    std::cout << "Ingrese el nuevo promedio de '" << NAME_COLUMN[courseIndex] << "' a modificar: ";
    std::getline(std::cin, newAverage);
  }

  if (validateStudentAverage(newAverage))
  {
    matrix[index][courseIndex] = newAverage;
    messageModifyData(courseIndex, oldAverage, newAverage);
  }

  return matrix;
}

std::string** modifyAllFields(std::string **&matrix, int size, int index, int col)
{
  (void) col;
  // Actualizar datos del alumno
  for (int j = 0; j < TOTAL_DATA; j++)
  {
    matrix[index][j] = dataValidator(matrix, size, j);
  }

  std::cout << "Se hicieron todos los cambios." << std::endl;
  return matrix;
}

void messageModifyData(int column, std::string dataBefore, std::string dataAfter)
{
  std::cout << "El campo '" << NAME_COLUMN[column] << "', fue modificado con exito!" << std::endl;
  std::cout << dataBefore << " --> " << dataAfter << std::endl;
}

/// @brief Modulo 4: Eliminar estudiante
/// @param matrix: array de dos dimensiones de tipo string
/// @param size: tamaño del array de dos dimensiones de tipo string
/// @return matrix
std::string **removeStudent(std::string **&matrix, int &size)
{
  if (size == 0 || matrix == nullptr)
  {
    std::cout << "No hay estudiantes en la base de datos para eliminar." << std::endl;
    return matrix;
  }

  // verificar si el DNI se encuentra en la BD
  int key = -1;
  if (!searchDocument(matrix, size, key))
  {
    return matrix;
  }

  return modifiyAndDeleteArray(matrix, size, key);
}

bool searchDocument(std::string **matrix, int size, int &key)
{
  std::string dni, student;
  std::cout << "Ingrese el numero de DNI del estudiante que quiere dar de baja: ";
  std::getline(std::cin, dni);

  for (int i = 0, j = 0; i < size; i++)
  {
    if (dni == matrix[i][j])
    {
      student = matrix[i][j];
      key = i;
      break;
    }
  }

  if (key == -1)
  {
    std::cerr << "DNI: " << dni << " no ha sido encontrado en la base de datos." << std::endl;
    return false;
  }
  return true;
}

std::string **modifiyAndDeleteArray(std::string **&matrix, int &size, int &key)
{
  char answer;
  std::string student = matrix[key][1], document = matrix[key][0];
  showIndividualDataTextPlane(matrix, key);
  std::cout << "Desea dar de baja al estudiante?(S/N): ";
  std::cin.get(answer);
  std::cin.ignore(100000, '\n');

  if (answer != 'S' && answer != 's')
  {
    std::cerr << "Operacion cancelada, no se elimino al estudiante" << std::endl;
    return matrix;
  }

  // Crear una nueva matriz con menor tamaño
  std::string **newMatrix = new std::string *[size - 1];

  // Copiar los elementos excepto el que se va a eliminar
  for (int i = 0, j = 0; i < size; i++)
  {
    if (i != key)
    {
      newMatrix[j] = matrix[i];
      j++;
    }
  }

  // Liberar memoria del estudiante eliminado
  delete[] matrix[key];

  // liberar la memoria del arreglo principal
  delete[] matrix;

  // Actualizar la matriz y su tamaño
  matrix = newMatrix;
  size--;
  std::cout << "Estudiante " << student << " identificado con DNI #" << document << " ha sido dado de baja con exito!" << std::endl;
  return matrix;
}

/// Módulos auxiliares: Validación de datos
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

double sendCredits(int index, double &credit)
{
  if (index == 0)
  {
    credit = CREDITS[3];
  }
  else if (index >= 1 && index <= 2)
  {
    credit = CREDITS[2];
  }
  else if (index >= 3 && index <= 6)
  {
    credit = CREDITS[1];
  }
  else
  {
    credit = CREDITS[0];
  }

  return credit;
}

std::string sendCredits(int column)
{
  int value = 0;
  std::string val;
  if (column == 2)
  {
    value = CREDITS[3];
  }

  if (column > 2 && column <= 4)
  {
    value = CREDITS[2];
  }

  if (column > 4 && column <= 8)
  {
    value = CREDITS[1];
  }

  if (column == 9)
  {
    value = CREDITS[0];
  }

  return val = std::to_string(value);
}

bool findStudent(std::string **matrix, int size, std::string &input, int &key)
{
  // verificar que el input no este vacío e ingresar DNI del estudiante
  while (!isEmptyString(input))
  {
    std::cout << "Ingresa " << NAME_COLUMN[0] << " del estudiante: ";
    std::getline(std::cin, input);
  }

  // Buscar el DNI en la base de datos
  for (int i = 0, j = 0; i < size; i++)
  {
    if (matrix[i][j] == input)
    {
      key = i;
      break;
    }
  }

  // Si no existe el DNI en la BD devolver el arreglo completo
  if (key == -1)
  {
    std::cerr << "El DNI " << input << " no se encuentra en la base de datos." << std::endl;
    return false;
  }

  return true;
}

void showIndividualData(std::string **matrix, int index, int op)
{
  if (op != 1 && op != 2)
  {
    showTableHead();
    for (int k = 0; k < TOTAL_DATA; k++)
    {
      showFormatDataStudent(matrix, index, k);
    }
  }
  else
  {
    showIndividualDataTextPlane(matrix, index);
  }
}

void showIndividualDataTextPlane(std::string **matrix, int index)
{
  int k = 0;
  std::cout << "Datos del estudiante:\n"
            << "-> DNI: " << matrix[index][k] << "\n"
            << "-> Nombres completos: " << matrix[index][k + 1] << std::endl;
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

  std::cout << "*** Todos los promedios del estudiante ***\n";
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
