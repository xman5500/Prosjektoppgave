// include as5600, wire, and tca9548 libraries
#include <Wire.h>
#include <AS5600.h>
// Calibrating the load cell
#include "HX711.h"



// Define three as5600 objects as X, Y and Z for further use
AS5600 stpA;
AS5600 stpB;
AS5600 stpC;

// HX711 circuit wiring
const int LOADCELL1_DOUT_PIN = 2;
const int LOADCELL1_SCK_PIN = 3;
const int LOADCELL2_DOUT_PIN = 4;
const int LOADCELL2_SCK_PIN = 5;

// Define the scale class for the load cells
HX711 scale1;
HX711 scale2;

// Define the multiplexer bus values for the three encoders
int I2CA = 2;
int I2CB = 3;
int I2CC = 1;

// Define the variables for the encoder readings
float radA = 0;
float radB = 0;
float radC = 0;
float newradA;
float newradB;
float newradC;
float initA;
float initB;
float initC;
float dradA = 0, dradB = 0, dradC = 0;
float X = 0, Y = 0, Z = 0;
rge
// Reading variables
long reading1 = 0;
long reading2 = 0;


//for prusa mk2
// Calibration for 500 g load cell
float smal_load_cal = 244000/77.3;
// Calibration for 1 KG horizontal load cell
float large_load_cal = (42471 + 42498 + 42495 + 42504)/(4*23.54);

//for VORON 0.1 1KG load cell
// float smal_load_cal = 134150/76.48;

//timer variable
unsigned long load_cell_refresh =0;


//movement variables for the axes
//for prusa mk2
float P = 8 / (2 * M_PI); // mm/rad of stepper motor
float R =  32 / ( 2 * M_PI); // mm/rad of stepper motor

// //for VORON 0.1
// float P = 8 / (2*M_PI);  // mm/degreee of stepper motor
// float R = 12.73 / 2;    // mm/rad of stepper motor


void setup()
{

    // Start serial communication
    Serial.print("setup initializing");
    Serial.begin(115200);
    while (!Serial)
    {
    // wait for serial port to connect.
    }
    Wire.begin();
    // Set up and check if A is connected
    Serial.println("X setup initializing");
    TCA9548(I2CA);
    stpA.begin(4);
    stpA.setDirection(AS5600_CLOCK_WISE);
    int b = stpA.isConnected();
    Serial.print("Connect: ");
    Serial.println(b);
    Serial.println("X setup done");
    initA = stpA.getCumulativePosition() * AS5600_RAW_TO_RADIANS;
    radA = 0;
    delay(20);
    // Set up and check if B is connected
    TCA9548(I2CB);
    stpB.begin(4);
    stpB.setDirection(AS5600_CLOCK_WISE);
    int c = stpB.isConnected();
    Serial.print("Connect: ");
    Serial.println(c);
    initB = stpB.getCumulativePosition() * AS5600_RAW_TO_RADIANS;
    radB = 0;
    delay(20);
    // Set up and check if C is connected
    TCA9548(I2CC);
    stpC.begin(4);
    stpC.setDirection(AS5600_CLOCK_WISE);
    int d = stpC.isConnected();
    Serial.print("Connect: ");
    Serial.println(d);
    initC = stpC.getCumulativePosition() * AS5600_RAW_TO_RADIANS;
    radC = 0;
    delay(20);

    X = 0;
    Y = 0;
    Z = 0;
    // Extra delay to make sure everything is set up
    delay(5000);
    Serial.println("encoder setup done");

    // Load cell setup
    scale1.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);
    scale2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);
    scale1.tare();
    scale1.set_scale(30.5/54.3);
   
    scale2.tare();
    scale2.set_scale(714/690);
    
    load_cell_refresh = millis();

    // Extra delay to make sure everything is set up
    delay(1000);
    Serial.println("load cell setup done");
}
void loop()
{
    static uint32_t lastTime = 0;

    if (millis() - lastTime >= 5) // attempt to read position every 5 milliseconds
    {
        // Read position in radians and calculate the updated position for A, B and C
        TCA9548(I2CA);
        newradA = stpA.getCumulativePosition() * AS5600_RAW_TO_RADIANS - initA; 
        dradA = newradA - radA;
        radA = newradA;
        TCA9548(I2CB);
        newradB = stpB.getCumulativePosition() * AS5600_RAW_TO_RADIANS - initB;
        dradB = newradB - radB;
        radB = newradB;

        TCA9548(I2CC);
        newradC = stpC.getCumulativePosition() * AS5600_RAW_TO_RADIANS - initC;
        dradC = newradC - radC; 
        radC = newradC;
        
        //Update X, Y and Z position using new readings


        //for prusa mk2
        X = X + dradB * P ;
        Y = Y + dradC * P ;
        Z = Z + dradA * P ;


        //for VORON 0.1
        
        // X = X + (dradA + dradB) * R / 2;
        // Y = Y + (dradA - dradB) * R/ 2;
        // Z = Z + dradC * P;


        if (load_cell_refresh + 13  < millis()) //Modified load cell amplifier refresh rateis 13 ms
        {
            reading1 = scale1.get_units(1);
            reading2 = scale2.get_units(1);
            load_cell_refresh = millis();
        }     

        //Print time, X, Y, Z and load cell readings through the serial port
        Serial.print(millis());
        Serial.print(",");
        Serial.print(X, 3);
        Serial.print(",");
        Serial.print(Y, 3); 
        Serial.print(",");
        Serial.print(Z, 3);
        Serial.print(",");
        Serial.print(float(reading1)/large_load_cal, 3);
        Serial.print(",");
        Serial.println(float(reading2)/large_load_cal, 3);
        
    }
}
void TCA9548(int bus)
{
    Wire.beginTransmission(0x70); // establishes I2C connectiont through the multiplexer
    Wire.write(1 << bus);         // read from the correct encoder, given by the bus values
    Wire.endTransmission();       // close the transmission
}
