/**
  Aplicația 12.2: Se cere un n strict pozitiv și patru valori reale:
                  ma, mb, m, d. ma și mb reprezintă masele a două corpuri
                  situate la distanța d unul de celălalt. m este masa unui corp
                  care pornește din a și ajunge în b, din n pași egali.
                  Să se calculeze în fiecare punct al traseului lui m forța F
                  de atracție gravitațională care acționează asupra lui. F va
                  fi cu semn: negativ înseamnă că m este atras către ma, iar
                  pozitiv către mb. Formula atracției gravitaționale dintre două
                  corpuri m1 și m2, situate la distanța d unul de celălalt este:
                  F=G*m1*m2/d2, unde G=6.674e-11.
                  Masele sunt exprimate în kilograme, distanțele în metri,
                  iar forțele în newtoni.
*/

#include "dependencies/utils.h"
#include <math.h>

// Macro for the value of the universal gravitational constant;
#define GCONST 6.674e-11

// Macro for computing the gravitational force between two bodies at a given distance;
/**
  USAGE:
    - computes the gravitational force, in newtons, between two bodies of
      'mass_a' kg and 'mass_b' kg, at a distance of 'distance' meters

  ARGUMENTS:
    - mass_a - the mass of the body A;
    - mass_b - the mass of the body B;
    - distance - the distance between them;
*/
#define gForce(mass_a, mass_b, distance) GCONST * (mass_a) * (mass_b) / pow((distance), 2)

// Driver program for the gForce macro;
// I/O handled by functions defined in utils.h;
int main()
{
  double mass_a; // < Mass of body A;
  double mass_b; // < Mass of body B;
  double mass_c; // < Mass of body C;
  double distance; // < Distance between body A and body B;
  unsigned step_number; // < Number of steps between A and B;

  // Getting the data on the masses, distance, and the number of segments the
  // distance will be split into;
  printf("Enter the mass of the body A (kg): ");
  if (!get_double(&mass_a, stdin))
  {
    err("I/O error when reading the mass of body A.\n");
  }
  printf("Enter the mass of the body B (kg): ");
  if (!get_double(&mass_b, stdin))
  {
    err("I/O error when reading the mass of body B.\n");
  }
  printf("Enter the mass of the body C, situated between body A and body B (kg): ");
  if (!get_double(&mass_c, stdin))
  {
    err("I/O error when reading the mass of body C.\n");
  }
  printf("Enter the distance between body A and body B (m): ");
  if (!get_double(&distance, stdin))
  {
    err("I/O error when reading the distance between body A and body B.\n");
  }
  printf("Enter the number of segments the distance should be split into: ");
  if (!get_unsigned(&step_number, stdin))
  {
    err("I/O error when reading the number of segments of the distance between body A and body B.\n");
  }

  // Computing the gravitational force acting on body C in a series of
  // equidistant points between body A and body B;
  double forces [step_number - 1];
  int iter;

  for (iter = 1; iter < step_number; iter++)
  {
    // By convention:
    // - if C is more attracted to A than to B, the g-force is negative;
    // - if C is more attracted to B than to A, the g-force is positive;
    forces [iter - 1] = gForce(mass_b, mass_c, distance / step_number * (step_number - iter)) -
                    gForce(mass_a, mass_c, distance / step_number * iter);
  }

  // Printing the computed g-force results in all the experimental points;
  for (iter = 1; iter < step_number; iter++)
  {
    printf("The g-force at distance %lf m from body A, and distance %lf m from body B is %lf N;\n",
           distance / step_number * iter,
           distance / step_number * (step_number - iter),
           forces [iter - 1]);
  }

  return 0;
}
