#include"bits/stdc++.h"
using namespace std;
const double P0 = 101325.0;               // Sea level pressure (Pa)
const double T0 = 288.15;                 // Sea level temp (K)
const double R = 287.05;                  // Gas constant (J/kg·K)
const double GAMMA = 1.4;                 // Specific heat ratio
const double G = 9.80665;                 // Gravity (m/s^2)
const double L = 0.0065;                  // Lapse rate (K/m)
const double RHO0 = 1.225;                // Sea level density (kg/m^3)
const double M_PER_S_TO_KNOTS = 1.94384;  // m/s to knots
const double M_TO_FT = 3.28084;           // Meters to feet
class AirDataComputer {
   private:
    double staticPressure;
    double totalPressure;
    double staticTemperature;

   public:
    AirDataComputer(double pStatic = P0, double pTotal = P0, double temp = T0)
        : staticPressure(pStatic),
          totalPressure(pTotal),
          staticTemperature(temp) {}

    void setStaticPressure(double pStatic) { staticPressure = pStatic; }
    void setTotalPressure(double pTotal) { totalPressure = pTotal; }
    void setStaticTemperature(double temp) { staticTemperature = temp; }

    double calculatePressureAltitude() const {  // Altitude in feet
        return (T0 / L) * (1.0 - pow(staticPressure / P0, (R * L) / G)) *
               M_TO_FT;
    }

    double calculateIndicatedAirspeed() const {  // IAS in knots
        double deltaP = totalPressure - staticPressure;
        return sqrt((2 * deltaP) / RHO0) * M_PER_S_TO_KNOTS;
    }

    double calculateTrueAirspeed() const {  // TAS in knots
        double rho = staticPressure / (R * staticTemperature);
        double deltaP = totalPressure - staticPressure;
        return sqrt((2 * deltaP) / rho) * M_PER_S_TO_KNOTS;
    }

    double calculateEquivalentAirspeed() const {  // EAS in knots
        double rho = staticPressure / (R * staticTemperature);
        double tas_mps = calculateTrueAirspeed() / M_PER_S_TO_KNOTS;
        return tas_mps * sqrt(rho / RHO0) * M_PER_S_TO_KNOTS;
    }

    double calculateMachNumber() const {  // Mach number
        double speedOfSound = sqrt(GAMMA * R * staticTemperature);
        return (calculateTrueAirspeed() / M_PER_S_TO_KNOTS) / speedOfSound;
    }

    double calculateTotalTemperature() const {  // Total temp (K)
        double mach = calculateMachNumber();
        return staticTemperature * (1 + 0.2 * mach * mach);
    }

    double calculateDensityAltitude() const {  // Density altitude in feet
        double rho = staticPressure / (R * staticTemperature);
        double T_std = T0 - L * (calculatePressureAltitude() / M_TO_FT);
        return (T_std / L) * (1 - pow(rho / RHO0, R * L / (G - R * L))) *
               M_TO_FT;
    }

    void displayParameters() const {
        cout << "Pressure Altitude: " << calculatePressureAltitude() << " ft\n";
        cout << "Density Altitude: " << calculateDensityAltitude() << " ft\n";
        cout << "Indicated Airspeed: " << calculateIndicatedAirspeed()
             << " knots\n";
        cout << "Equivalent Airspeed: " << calculateEquivalentAirspeed()
             << " knots\n";
        cout << "True Airspeed: " << calculateTrueAirspeed() << " knots\n";
        cout << "Mach Number: " << calculateMachNumber() << "\n";
        cout << "Static Temperature: " << staticTemperature << " K\n";
        cout << "Total Temperature: " << calculateTotalTemperature() << " K\n";
    }
};

class ConsoleInterface {
   private:
    AirDataComputer adc;

   public:
    ConsoleInterface() : adc() {}

    void run() {
        string choice;
        do {
            cout << "\nAir Data Computer\n";
            cout << "1. Set Static Pressure (Pa)\n";
            cout << "2. Set Total Pressure (Pa)\n";
            cout << "3. Set Static Temperature (K)\n";
            cout << "4. Display Air Data\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;
            handleChoice(choice);
        } while (choice != "5");
    }

   private:
    void handleChoice(const string& choice) {
        double value;
        if (choice == "1") {
            cout << "Enter static pressure (Pa): ";
            cin >> value;
            adc.setStaticPressure(value);
        } else if (choice == "2") {
            cout << "Enter total pressure (Pa): ";
            cin >> value;
            adc.setTotalPressure(value);
        } else if (choice == "3") {
            cout << "Enter static temperature (K): ";
            cin >> value;
            adc.setStaticTemperature(value);
        } else if (choice == "4") {
            adc.displayParameters();
        } else if (choice != "5") {
            cout << "Invalid choice. Try again.\n";
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);  
    cin.tie(nullptr);
    ConsoleInterface interface;
    interface.run();
    cout << "Exiting Air Data Computer program.\n";
    return 0;
}