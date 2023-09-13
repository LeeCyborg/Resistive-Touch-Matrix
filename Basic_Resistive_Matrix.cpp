
/*
Pressure Sensor Matrix Code

Create a grid of rows and columns to pinpoint what area is being pressed. Use INPUT A
and OUTPUT to define the column you are reading, and ANALOGREAD to read the rows in that
defined column.  
Print the analog readouts of each button and find the active button. 

Originally concept from https://www.kobakant.at/DIY/?p=7443. 
rewritten and modified by lee wilkins & kyle chisholm September 2023 
*/

// Define the number of rows and columns 
#define numRows 3
#define numCols 3

// Rows should be analog pins, columns should be digital pins. These are 
// for the Adafruit Flora
int rows[] = {A10, A9, A7};
int cols[] = {3, 2, 0};
// Use a 2D array to hold rows and columns
int incomingValues[numRows][numCols] = {};

void setup() {
    // All of the row (analog pins) use the PULLUP resistor inside your Arduino. 
    // You can also manually use one if your board doesn't have one. 
    for (int i = 0; i < numRows; i++) {
        pinMode(rows[i], INPUT_PULLUP);
    }
    // Columns all start off ans INPUTS
    for (int i = 0; i < numCols; i++) {
        pinMode(cols[i], INPUT);
    }
    Serial.begin(9600);
}
void loop() {
    // On every loop, iterate through the columns 
    for (int col = 0; col < numCols; col++) {
        // Set the entire column to output OUTPUT 
        // and write a LOW signal to it.  This lets us know what column we 
        // are reading. 
        pinMode(cols[col], OUTPUT);
        digitalWrite(cols[col], LOW);
        for (int row = 0; row < numRows; row++) {
            /// In that column, read the row. 
            incomingValues[row][col] = analogRead(rows[row]); 
        } // end row
        // Then set it back to input so we can look at the next column
        pinMode(cols[col], INPUT); 
    } // end col

    // Print out the entire grid, you can see how the rows and columns work
    printGrid();
    // Print out the active button
    findActiveButton();
    delay(10);
}

void printGrid() {
    // Print the incoming values of the grid by 
    // iterating through rows and columns and priting their value
    for (int col = 0; col < numCols; col++) {
        for (int row = 0; row < numRows; row++) {
            Serial.print(incomingValues[row][col]);
            Serial.print("\t");
        }
        Serial.println();
    }
    Serial.println();
}

void findActiveButton() {
    // Find the smallest value.
    int smallest = 1024; // Set to max so we know if it isn't working
    int smallestRow = -1;
    int smallestCol = -1;
    for (int col = 0; col < numCols; col++) {
        for (int row = 0; row < numRows; row++) {
            // Iterate through rows and columns, check each value if it is
            // smaller than the smallest, then make it the new smallest if it is. 
            // and set the rows and columns to t he current position
            if (incomingValues[row][col] < smallest) {
               smallest = incomingValues[row][col];
               smallestCol = col;
               smallestRow = row;
            }
        }
    }
    if (smallest < 100){
        // If there is a button being presed it will be less than
        // 100, so you can identify it. 
        Serial.print("There is a button being pressed in row: ");
        Serial.println(smallestRow);
        Serial.print("and column: ");
        Serial.println(smallestCol);
    } else {
        Serial.println("Nothing is being pressed");
    }
}
