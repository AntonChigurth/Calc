#include <iostream>

/******************************************************************************
Filename:           main.cpp
Version:            1.0
Author:             Valeri Navitaniuc
Purpose:            To interpolate and extrapolate transistor counts and the
                    rate of change of transistor counts in CPUs.
******************************************************************************/



#include <fstream>
#include <limits>
#include <ctgmath>


using namespace std;

void doLinearRegression();

void powerLawCalculations(double *array_t, double *array_T, int length, double *m, double *c);

void exponentialCalculations(double *array_t, double *array_T, int length, double *m, double *c);

void doPowerLawExtrapolation(double m, double c);

void doExponentialExtrapolation(double m, double c);

/******************************************************************************
Function Name:  main
Purpose:        Get the users choice, either quit or call the function to
                perform the linear regression.
In parameters:  none
Out parameters: EXIT_SUCCESS
Version:        1.0
Author:         Valeri Navitaniuc
******************************************************************************/
int main() {


  const int CHOICE_OPEN_FILE = 1;
  const int CHOICE_QUIT = 2;

  int choice;


  for (choice = -1;
       choice != CHOICE_QUIT;) {
    /* Print menu. */
    cout << "*******************************************************" << endl;
    cout << " Least-Sqares fit of Exponential and Power Law " << endl;
    cout << "1. Read Data from File" << endl;
    cout << "2. Quit" << endl;
    cout << "*******************************************************" << endl;
    cout << "Select an option: ";

    /* Get users choice. */
    choice = -1;
    cin >> choice;

    /* Skip the rest of the data in the stream. */
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    /* Do what the user asks. */
    switch (choice) {
      case CHOICE_OPEN_FILE:
        doLinearRegression();
        break;

      case CHOICE_QUIT:
        break;

      default:
        cout << "Invalid choice." << endl;
    }
  }

  system("pause");

  return EXIT_SUCCESS;
}


/******************************************************************************
Function Name:  doLinearRegression
Purpose:        Prompt the user for the file name, read that file and calulate
                the values a and c in the function y = a*exp(c*x), then asks
                the user what kind of interpolation/extrapolation they want to
                do.
In parameters:  none
Out parameters: none
Version:        1.3
Author:         Valeri Navitaniuc
******************************************************************************/
void doLinearRegression() {
  /* Get file path. */
  string filePath;
  filePath = R"(C:\Users\Valerian\Desktop\m.txt)";
  cout << "please enter the name of the file to open: ";

  /* Attempt to open file. */
  ifstream fileReader(filePath);

  /* File opened. */
  cout << endl << endl << "FILE OPENED FOR READING" << endl;

  /* Find out how many records there are. */
  int numRecords = 1;
  {
    char temp;
    for (fileReader.get(temp);
         !fileReader.eof();
         fileReader.get(temp)) {
      if ('\n' == temp) {
        ++numRecords;
      }
    }
  }


  double *array_t = new double[numRecords];
  double *array_T = new double[numRecords];


  cout << "There are " << numRecords << " records." << endl << endl;
  printf("%-16s%s\n", "YEAR", "COUNT");

  /* Rewind stream to the start. */
  fileReader.clear();
  fileReader.seekg(0);

  double m;
  double c;
  double year;
  int i = 0;
  {
    double transistorCount;

    while (fileReader >> year >> transistorCount) {
      printf("%-16.0f%g\n", year, transistorCount);
      array_t[i] = year;
      array_T[i] = transistorCount;
      i++;
    }
    /* Calculate a and c. */
  }
  printf("-------- %d\n", numRecords);


  cout << endl << "File read into memory" << endl << endl;
  cout << "EXPONENTIAL FUNCTION" << endl;

  const int EXPONENT = 1; /* User wants to Interpolation/Extrapolation transistor count. */
  const int POWER = 2; /* User wants to Interpolation/Extrapolation rate of increase of transistor count. */
  const int QUIT = 3; /* User wants to quit. */

  /* Keep interpolating and extrapolating until the user quits. */
  for (int choice = -1; choice != QUIT;) {
    /* Print out menu. */
    cout << endl;
    cout << "*******************************************************" << endl;
    cout << "Exponential Interpolation/Extrapolation" << endl;
    cout << "1. Exponential fit to data" << endl;
    cout << "2. Power Law fit to data " << endl;
    cout << "3. Quit " << endl;
    cout << "*******************************************************" << endl;
    cout << "Select an option: ";

    /* Get users choice. */
    cin >> choice;


    /* Clear the rest of the data in the stream. */
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {


      case EXPONENT:

        exponentialCalculations(array_t, array_T, numRecords, &m, &c);

        cout << "EXPONENTIAL " << endl;
        cout << " A = " << pow(M_E, c) << " *" << "exp(" << m << "*(Y-1939))" << endl;


        doExponentialExtrapolation(m, c);

        /* Clear the rest of the data in the stream. */
//        cin.clear();
//        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "transistor count at " << year << " = " << m * exp(c * (year - 1939)) << endl;
        break;

      case POWER:

        powerLawCalculations(array_t, array_T, numRecords, &m, &c);

        /* Calculate a and c. */

        cout << "this is second choice" << endl;

        cout << "POWER LAW" << endl;

        cout << " A = " << pow(M_E, c) << " * (Y-1935)^" << m << endl;

//        cout << "*******************************************************" << endl;
//        cout << "Power Law Interpolation/Extrapolation " << endl;
//        cout << "1. Interpolation/Extrapolation of Toad area " << endl;
//        cout << "2. Quit Interpolation/Extrapolation " << endl;
//        cout << "*******************************************************" << endl;
        doPowerLawExtrapolation(m, c);


        /* Clear the rest of the data in the stream. */
//        cin.clear();
//        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        break;

      case QUIT:
        cout << endl;
        break;

        /* Invalid choice. */
      default:
        cout << "ERROR: Choice is not valid." << endl;
    }
  }


}


void exponentialCalculations(double *array_t, double *array_T, int length, double *m, double *c) {

  double Sx = 0;
  double Sy = 0;
  double Sxy = 0;
  double Sxx = 0;
  double delta = 0;

  for (int i = 0; i < length; i++) {
    double lnY = log(array_T[i]);
    double X = array_t[i] - 1939;
    Sx = Sx + X;
    Sy = Sy + lnY;
    Sxx = Sxx + X * X;
    Sxy = Sxy + X * lnY;
  }

  delta = length * Sxx - Sx * Sx;

  *m = (length * Sxy - Sx * Sy) / delta;
  *c = (Sxx * Sy - Sx * Sxy) / delta;


}


void powerLawCalculations(double *array_t, double *array_T, int length, double *m, double *c) {

  double Sx = 0;
  double Sy = 0;
  double Sxy = 0;
  double Sxx = 0;
  double delta = 0;

  for (int i = 0; i < length; i++) {
    double lnY = log(array_T[i]);
    double X = log(array_t[i] - 1935);
    Sx = Sx + X;
    Sy = Sy + lnY;
    Sxx = Sxx + X * X;
    Sxy = Sxy + X * lnY;
  }

  delta = length * Sxx - Sx * Sx;

  *m = (length * Sxy - Sx * Sy) / delta;
  *c = (Sxx * Sy - Sx * Sxy) / delta;
}

void doExponentialExtrapolation(double m, double c) {


  int choice = 0;
  double year;
  const int ENTER = 1;
  const int EXIT = 2;
  double A;

  for (choice = -1; choice != EXIT;) {

    /* Get users choice. */
    choice = -1;
    cin >> choice;

    /* Skip the rest of the data in the stream. */
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    /* Do what the user asks. */

    cout << "*******************************************************" << endl;
    cout << "Exponential Interpolation/Extrapolation " << endl;
    cout << "1. Interpolation/Extrapolation of Toad area " << endl;
    cout << "2. Quit Interpolation/Extrapolation " << endl;
    cout << "*******************************************************" << endl;


    switch (choice) {

      case ENTER:
        cin >> year;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        A = pow(M_E, c) * exp(m * (year - 1939));
        cout << A << endl;



        break;

      case EXIT:
        cout << endl;
        break;

      default:
        cout << "Invalid choice." << endl;
    }
  }

}


void doPowerLawExtrapolation(double m, double c) {

  int choice = 0;
  double year;
  const int ENTER = 1;
  const int EXIT = 2;

  double A = 0;

  for (choice = -1; choice != EXIT;) {

    cout << "*******************************************************" << endl;
    cout << "Power Law Interpolation/Extrapolation " << endl;
    cout << "1. Interpolation/Extrapolation of Toad area " << endl;
    cout << "2. Quit Interpolation/Extrapolation " << endl;
    cout << "*******************************************************" << endl;


    /* Get users choice. */
    choice = -1;
    cin >> choice;

    /* Skip the rest of the data in the stream. */
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    /* Do what the user asks. */
    switch (choice) {

      case ENTER:
        cin >> year;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        A = pow(M_E, c) * pow((year - 1935), m);
        cout << A << endl;

        break;

      case EXIT:
        cout << endl;
        break;

      default:
        cout << "Invalid choice." << endl;
    }
  }

}
