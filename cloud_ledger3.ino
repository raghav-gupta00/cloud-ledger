#include "TRIGGER_WIFI.h"               
#include "TRIGGER_GOOGLESHEETS.h"       
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


char column_name_in_sheets[ ][6] = {"val1","val2","val3"};                        /*1. The Total no of column depends on how many value you have created in Script of Sheets;2. It has to be in order as per the rows decided in google sheets*/
String Sheets_GAS_ID = "AKfycbyxlhfIIF8hp5hh0IHMjM50mYGeO-IdoR0DWkqzGvJ56lRztggfAAah0AFYreib5Qi2";                                         /*This is the Sheets GAS ID, you need to look for your sheets id*/
int No_of_Parameters = 3; 
int data_button=16; 


const byte ROWS = 4;

const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'C', '0', '=', '/'}

};
byte rowPins[ROWS] = {0, 2, 14, 12};
byte colPins[COLS] = {13, 15, 3, 1};

// Create keypad object
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Define LCD display settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Calculator variables
String input = ""; // String to store user input
float num1 = 0; // First operand
float num2 = 0; // Second operand
float a = 0, b = 0, c = 0; 
//float result=0;
char operatorSymbol = ' '; // Operator symbol
bool resultShown = false; // Flag to indicate if result is shown



void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  pinMode(data_button, INPUT);

   lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Cloud Ledger");
  delay(1000);
  lcd.clear();

  // Wait for serial connection to be established
  while (!Serial);

  // Connect to Wi-Fi
  WIFI_Connect("SamsungA10", "raghu007"); // Provide your Wi-Fi SSID and password
  
  // Initialize Google Sheets communication
  Google_Sheets_Init(column_name_in_sheets, Sheets_GAS_ID, No_of_Parameters); // Set column names, GAS ID, and number of parameters
  
  // Initialize LCD display
 
}

void loop() {
  // put your main code here, to run repeatedly:

                               

  // Data_to_Sheets(No_of_Parameters,  a,  b, c);         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/

  // Serial.println();
  // delay(10000); 


  char key = keypad.getKey();

  // If a key is pressed
  if (key != NO_KEY) {
    // Check if the result is shown, if so, clear the display
    if (resultShown) {
      lcd.clear();
      resultShown = false;
    }

    // Check if the key is a digit or a decimal point
    if (isdigit(key) ) {
      // Append the key to the input string
      input += key;
      lcd.print(key);
    }
    // Check if the key is an operator (+, -, *, /)
    else if (key == '+' || key == '-' ) {
      // Store the input string as the first operand
      num1 = input.toFloat();
      // Store the operator symbol
      operatorSymbol = key;
      // Reset the input string
      input = "";
      // Print the operator symbol on the LCD display
      // lcd.setCursor(15, 0);
      // lcd.print(operatorSymbol);
      lcd.setCursor(0,1);
      lcd.print(key);
    }
    // Check if the key is the equals sign (=)
    else if (key == '=') {
      // Store the input string as the second operand
      num2 = input.toFloat();
      // Reset the input string
      input = "";
      // Perform the calculation based on the operator symbol
      float result;
      switch (operatorSymbol) {
        case '+':
          a=result = num1 + num2;
          break;
        case '-':
          a=result = num1 - num2; 
          break;
        case '*':
         a= result = num1 * num2;
          break;
        case '/':
         a= result = num1 / num2;
          break;
        default:
          // Display error message if an invalid operator is used
          lcd.setCursor(0, 1);
          lcd.print("Error");
          delay(1000);
          lcd.clear();
          break;
      }
      // Print the result on the LCD display
            lcd.setCursor(0, 1);
      lcd.print("Result: ");
      lcd.print(result);
      Data_to_Sheets(No_of_Parameters,  a,  b, c); 
      resultShown = true;
    }
    


    // else if (key == 'C') {
    //   // Clear the input string and reset the calculator
    //   input = "";
    //   num1 = 0;
    //   num2 = 0;
    //   operatorSymbol = ' ';
    //   resultShown = false;
    //   lcd.clear();
    // }

    else if(key=='C'){


      
        input = "";
      num1 = 0;
      num2 = 0;
      operatorSymbol = ' ';
      resultShown = false;
      lcd.clear();

    }

    else if(digitalRead(data_button)==HIGH){

       lcd.setCursor(0, 1);
      lcd.print("Result: ");
      lcd.print(a);
      Data_to_Sheets(No_of_Parameters,  a,  b, c); 
      resultShown = true;

    }
  }
  

}
