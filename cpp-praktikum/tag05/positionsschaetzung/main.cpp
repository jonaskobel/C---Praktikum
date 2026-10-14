#include "pos_estimation.h"
#include <fstream>
#include <iostream>

int main()
{
    PosEstimation position;
    std::ifstream datei("PosEstimationInput.txt");
    double rechts;
    double links;
    double T;

    // Die drei Spalten enthalten rechts, links und den Zeitschritt.
    while (datei >> rechts >> links >> T)
    {
        position.PredictPosition(rechts, links, T);
        double* werte = position.GetPosition();

        // Ausgabe wie in der Vergleichsdatei: x, y und Winkel.
        std::cout << werte[0] << "\t" << werte[1] << "\t"
                  << werte[2] << std::endl;
    }

    datei.close();
    // Die Ausgabe auf der VM mit PosEstimationVgl.txt vergleichen.
    return 0;
}
