/* 
Name: Aryan Patel
CUID: ASP6
Course: ECE2220
Semester: Spring 2025
Assignment number: Project 2
Purpose: This program collects and displays player information using a "menu". It allows you to enter personal details, school details, player stats, and contact info.
Assumptions: None
Known Bugs: None
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Prompts for first, middle, and last names with input length check.
void enterName(char firstName[], char middleInitial[], char lastName[]){
    int valid = 0; // sets valid to 0 so that input is validated
    while (!valid) {
        printf("Enter First Name (max 32 characters): ");
        char buf[100]; // temporary buffer
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            int len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n') // removes new line character
                buf[len-1] = '\0';
            if (strlen(buf) > 32) // makes sure it 32 character long
                printf("Invalid Input, Input greater then 32 character\n");
            else {
                sscanf(buf, "%32s", firstName); // saves the first name to the variable first name
                valid = 1; // exits loop
            }
        }
    }

    valid = 0; // reset for next use
    char buffer[10];
    while (!valid) { // Loop for middle initial validation
        printf("Enter Middle Initial: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n')// removes new line character
                buffer[len-1] = '\0';
            if (strlen(buffer) != 1)
                printf("Invalid Input, Input should be 1 character\n");
            else {
                strcpy(middleInitial, buffer);
                valid = 1;
            }
        }
    }

    valid = 0;
    while (!valid) { //loop for last name validation
        printf("Enter Last Name: ");
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            int len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n')// removes new line character
                buf[len-1] = '\0';
            if (strlen(buf) > 32)
                printf("Invalid Input, Input greater then 32 character\n");
            else {
                sscanf(buf, "%32s", lastName);
                valid = 1;
            }
        }
    }
}

// Prompts for school and class information.
void schoolInfo(char school[], char class[]){
    int valid = 0;
    while (!valid) { // loop for school name validation
        printf("Enter School: ");
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            int len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n')// removes new line character
                buf[len-1] = '\0';
            if (strlen(buf) > 64)
                printf("Invalid Input, Input greater then 64 character\n");
            else {
                sscanf(buf, "%64s", school);
                valid = 1;
            }
        }
    }

    valid = 0;
    while (!valid) {//loop for class validation
        printf("Enter Class(FR, SO, JR, SR, GR, RFR, or RSO): ");
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            int len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n')// removes new line character
                buf[len-1] = '\0';
            if (strlen(buf) > 3)
                printf("Invalid Input, Input greater then 3 character\n");
            else {
                sscanf(buf, "%3s", class);
                valid = 1;
            }
        }
    }
}

// Prompts for and validates age (21–99). Return int age to main
int ageInfo(int age){
    int valid = 0;
    char buf[100];
    int temp;
    while (!valid) { // loop to enter age validation
        printf("Enter Age: ");
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &temp) != 1){
                printf("Invalid Input, age should be between 21 and 99\n");
            }else if (temp < 21 || temp > 99){
                printf("Invalid Input, age should be between 21 and 99\n");
            }else {
                age = temp;
                valid = 1;
            }
        }
    }
    return age; // returns age to be saved in main function
}

// Prompts for height in feet (4–7). 
int heightFeetinfo(int heightFeet){
    int valid = 0;
    char buf[100];
    int temp;
    while (!valid) { // loop to enter feet portion of height and validation of feet
        printf("Enter Height (Feet): ");
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &temp) != 1){
                printf("Invalid Input, Height should be between 4 and 7\n");
            }else if (temp < 4 || temp > 7){
                printf("Invalid Input, Height should be between 4 and 7\n");
            }else {
                heightFeet = temp;
                valid = 1;
            }
        }
    }
    return heightFeet;// returns feet to be saved in main function
}

// Prompts for height in inches (0–11).
int heightInchesInfo(int heightInches){
    int valid = 0;
    char buf[100];
    int temp;
    while (!valid) {
        printf("Enter Height (Inches): "); // loop to enter inches portion of height and validation of inches
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &temp) != 1){
                printf("Invalid Input, Height should be between 0 and 11\n");
            }else if (temp < 0 || temp > 11){
                printf("Invalid Input, Height should be between 0 and 11\n");
            }else {
                heightInches = temp;
                valid = 1;
            }
        }
    }
    return heightInches;// returns inches to be saved in main function
}

// Prompts for weight (100–500).
int weightinfo(int weight){
    int valid = 0;
    char buf[100];
    int temp;
    while (!valid) { // loop for weight validation
        printf("Enter Weight: ");
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &temp) != 1){
                printf("Invalid Input, Weight should be between 100 and 500\n");
            }else if (temp < 100 || temp > 500){
                printf("Invalid Input, Weight should be between 100 and 500\n");
            }else {
                weight = temp;
                valid = 1;
            }
        }
    }
    return weight; // returns weight to be saved in main function
}

// Prompts for player position (up to 3 uppercase letters).
void positionInfo(char position[]){
    int valid = 0;
    while (!valid) {
        printf("Enter Position: ");
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            int len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n'){ // removes new line character
                buf[len-1] = '\0';
            }
            sscanf(buf, "%3s", position);
            if (strlen(position) > 3){
                printf("Invalid Input, Input greater then 3 character\n");
            }else if (!isupper(position[0]) || !isupper(position[1])){ //check for capital letters
                printf("Invalid Input, must use all capital letter\n");
            }else{
                valid = 1;
            }
        }
    }
}

// Prompts for NIL money (0 to 1e50).
long double NILinfo(long double nilMoney){
    int valid = 0;
    char buf[100];
    long double temp;
    while (!valid) { // loop for nilMoney validation
        printf("Enter NIL Money: ");
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%Lf", &temp) != 1){
                printf("Invalid Input, NIL Money should be between 0 and 1e50\n");
            }else if (temp < 0 || temp > 1e50){
                printf("Invalid Input, NIL Money should be between 0 and 1e50\n");
            }else {
                nilMoney = temp;
                valid = 1;
            }
        }
    }
    return nilMoney;// returns nilMoney to be saved in main function
}

// Prompts for star rating (0–5).
int starRatingInfo(int starRating){
    int valid = 0;
    char buf[100];
    int temp;
    while (!valid) { // loop for star rating validation
        printf("Enter Star Rating(number of stars): ");
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &temp) != 1){
                printf("Invalid Input, Star Rating should be between 0 and 5\n");
            }else if (temp < 0 || temp > 5){
                printf("Invalid Input, Star Rating should be between 0 and 5\n");
            }else {
                starRating = temp;
                valid = 1;
            }
        }
    }
    return starRating; // returns starRating to be saved in main function
}

// Prompts for Twitter handle and phone number.
void contacts(char twitter[], char phoneNumber[]){
    int valid = 0;
    while (!valid) { // loop for twitter user name validation
        printf("Enter Twitter(start with '@'): ");
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            int len = strlen(buf);
            if (len > 0 && buf[len-1]=='\n'){ // removes new line character
                buf[len-1] = '\0';
            }
            sscanf(buf, "%15s", twitter);
            if (strlen(twitter) > 15){
                printf("Invalid Input, Input greater then 15 character\n");
            }else if (twitter[0] != '@'){
                printf("Invalid Input, Twitter should start with '@'\n");
            }else{
                valid = 1;
            }
        }
    }

    valid = 0;
    while (!valid) { // loop for phone number validation
        printf("Enter Phone Number: ");
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            int len = strlen(buf);
            if (len > 0 && buf[len-1]=='\n'){
                buf[len-1] = '\0';
            }
            sscanf(buf, "%10s", phoneNumber);
            if (strlen(phoneNumber) != 10){
                printf("Invalid Input, Input has to be 10 character\n");
            }else{
                valid = 1;
            }
        }
    }
}

// Displays the all data.
void displayData(char firstName[], char middleInitial[], char lastName[], char school[], char class[], int age, int heightFeet, int heightInches, int weight, char position[], int starRating, long double nilMoney, char twitter[], char phoneNumber[]){
    if (strlen(middleInitial) == 0){ // checks to know if Middle intial is null
        printf("Name: %s %s\n", firstName, lastName);
    }else{
        printf("Name: %s %s %s\n", firstName, middleInitial, lastName); 
    }
    printf("School: %s\n", school);
    if (strcmp(class, "Unkn") == 0){ 
        printf("Class: Unknown\n"); // prints unknown if the value is not set
    }else{
        printf("Class: %s\n", class);
    }
    if (age == -1){
        printf("Age: Unknown\n");
    }else{
        printf("Age: %d\n", age);
    }
    if (heightFeet == -1 || heightInches == -1){
        printf("Height: Unknown\n");
    }else{
        printf("Height: %d' %d\"\n", heightFeet, heightInches);
    }
    if (weight == -1){
        printf("Weight: Unknown\n");
    }else{
        printf("Weight: %d\n", weight);
    }
    if (strcmp(position, "\0") == 0){
        printf("Position: Unknown\n");
    }else{
        printf("Position: %s\n", position);
    }
    if (starRating == -1){
        printf("Star Rating: Unknown");
    }else {
        printf("Star Rating: "); // prints "*" 
        for (int i = 0; i < starRating; i++){ // prints star rating as "*"
            printf("*");
        }
    }
    
    if (nilMoney == -1){
        printf("\nNIL Money: Unknown\n");
    }else{
        printf("\nNIL Money: $%.2Lf\n", nilMoney);
    }
    printf("Twitter: %s\n", twitter);
    
    if (strcmp(phoneNumber, "Unknown") == 0){
        printf("Phone Number: Unknown\n");
    }else{
        printf("Formatted Phone Number: (%c%c%c)%c%c%c-%c%c%c%c\n", // print number in the correct format of (***)***-****
            phoneNumber[0], phoneNumber[1], phoneNumber[2],
            phoneNumber[3], phoneNumber[4], phoneNumber[5],
            phoneNumber[6], phoneNumber[7], phoneNumber[8], phoneNumber[9]);
    }
}

// Resets data char data.
void clearData(char firstName[], char middleInitial[], char lastName[], char school[], char class[], char position[], char twitter[], char phoneNumber[]){
    strcpy(firstName, "Unknown");    
    strcpy(middleInitial, "");      
    strcpy(lastName, "Unknown");        
    strcpy(school, "Unknown");        
    strcpy(class, "Unkn");            
    strcpy(position, "");            
    strcpy(twitter, "Unknown");     
    strcpy(phoneNumber, "Unknown");   

}

// prints menu 
void printMenu(void){
        printf("1. Enter Name \n");
        printf("2. Enter School Infomration: \n");
        printf("3. Enter Age, Height, and Weight: \n");
        printf("4. Player Information: \n");
        printf("5. Enter Contacts: \n");
        printf("6. Display the Data: \n");
        printf("7. Clear all Data: \n");
        printf("8. Quit: \n");
}

// main function
int main(void){
    char firstName[33] = "Unknown";
    char middleInitial[2] = "\0";
    char lastName[33] = "\0";
    char school[65] = "Unknown";
    char class[4] = "Unkn";
    int age = -1;
    int heightFeet = -1;
    int heightInches = -1;
    int weight = -1;
    char position[4] = "\0";
    int starRating = -1;
    long double nilMoney = -1;
    char twitter[16] = "Unknown";
    char phoneNumber[11] = "Unknown";
    int quit = 0;
    char choice;
    
    do {// Begin menu loop
        printMenu();
        printf("Enter Choice: ");
        char tempbuf[100];
        choice = '\0';

        // Reads input
        if (fgets(tempbuf, sizeof(tempbuf), stdin) != NULL) {
            // Remove trailing newline if present
            tempbuf[strcspn(tempbuf, "\n")] = '\0';

            // Check if user typed "quit"
            if (strcmp(tempbuf, "quit") == 0 || strcmp(tempbuf,"Quit") == 0) {
                quit = 1;
            } 
            else {
                // Otherwise, take the first character
                if (strlen(tempbuf) == 1) {
                    choice = tempbuf[0];
                switch(choice) {
                    case '1':
                        enterName(firstName, middleInitial, lastName);
                        break;
                    case '2':
                        schoolInfo(school, class);
                        break;
                    case '3':
                        age = ageInfo(age);
                        heightFeet = heightFeetinfo(heightFeet);
                        heightInches = heightInchesInfo(heightInches);
                        weight = weightinfo(weight);
                        break;
                    case '4':
                        positionInfo(position);
                        starRating = starRatingInfo(starRating);
                        nilMoney = NILinfo(nilMoney);
                        break;
                    case '5':
                        contacts(twitter, phoneNumber);
                        break;
                    case '6':
                        displayData(firstName, middleInitial, lastName, school, class, 
                                    age, heightFeet, heightInches, weight, position, 
                                    starRating, nilMoney, twitter, phoneNumber);
                        quit = 1; //exit switch compleatly 
                        break;
                    case '7':
                        clearData(firstName, middleInitial, lastName, school, class, 
                                  position, twitter, phoneNumber);
                        age = -1;
                        heightFeet = -1;
                        heightInches = -1;
                        weight = -1;
                        starRating = -1;
                        nilMoney = -1;
                        break;
                    case '8':
                        quit = 1; //exit switch compleatly
                        break;
                    default:
                        printf("Invalid Input! Choice options 1-8\n");
                        break;
                    }
                }else{
                   printf("Invalid Input! Choice options 1-8\n"); 
                }
            }
        }
    } while (quit != 1);//switch keeps going until quit is true

    return 0;
}
