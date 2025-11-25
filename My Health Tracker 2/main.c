//created on 15 january 2024
//Beatriz Anak Anthony and the friends
//introduction to programming
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int arraySize = 0;
struct PersonalDetails {
    char name[50];
    char dob[11];
    char sex[10];
    float weight;
    float height;
    char healthGoals[50];
};

struct DailyHealthData {
    char date[100][11];
    int foodChoice;
    char foodItems[100][20];
    char exerciseDetails[100][20];
    float sleepDuration[100];
    int exerciseChoice;
    int sleepChoice;
    float caloriesConsumed[100];
    float exerciseDuration[100];
    
};

struct WeeklyHealthSummary {
    float totalCalories;
    float totalExerciseDuration;
    float averageSleepDuration;
};

void loadUserAndDetails(struct PersonalDetails *details, int *numEntries);
void displayMenu();
void enterPersonalDetails(struct PersonalDetails *details);
void logHealthData( struct DailyHealthData *data);
void saveToFile(struct PersonalDetails *details, struct DailyHealthData *data, int nd);
void displayWeeklyHealthSummary(struct DailyHealthData *data);
void displayMonthlyHealthSummary(struct DailyHealthData *data);
void displayHealthRecommendations(float caloricIntake, struct DailyHealthData *data, float sleepDuration, int arraySize);
void viewFileData(struct PersonalDetails *details);

int main(void) {
    struct PersonalDetails details = {""};
    struct DailyHealthData data[28];

    int choice;

    do {
        displayMenu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                int choose;
                int c; while ((c = getchar()) != '\n' && c != EOF);
                
                do {
                    printf("\n[1] - Enter Personal Details   [2] - Log Daily Health Data  [3] - Back to Main Menu\n" );
                    printf("\nChoose which detail to enter: ");
                    scanf("%d",&choose);
                    
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if(choose==1){
                        enterPersonalDetails(&details);
                        // The user's personal details are saved inside the function if new
                    }
                    else if(choose==2){
                        loadUserAndDetails(&details, &arraySize);
                        
                        if (details.name[0] != '\0') {
                            logHealthData(data);
                            saveToFile(&details, data, arraySize);
                        } else {
                            printf("\nOperation cancelled.\n");
                        }
                    }
                    else if(choose==3){
                        printf("\nBack to main Menu");
                    }
                    else{
                        printf("\nInvalid input.");
                    }
                } while (choose != 3);
                break;
            case 2:
                displayWeeklyHealthSummary(data);
                break;
            case 3:
                displayMonthlyHealthSummary(data);
                break;
            case 4:
                 viewFileData(&details);
                break;
            case 5:
                 printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                
        }
    } while (choice != 5);

    return 0;
}

void loadUserAndDetails(struct PersonalDetails *details, int *numEntries) {
    *numEntries = 0;
    details->name[0] = '\0';
    
    FILE *indexFile = fopen("user_index.txt", "r");
    if (indexFile == NULL) {
        printf("\nNo registered users found. Please use Option 1 to enter personal details first.\n");
        return;
    }
    
    printf("\n--- Registered Users ---\n");
    char nameList[100][50];
    char lineBuffer[50];
    int userCount = 0;
    int selection;
    int c;

    while (fgets(lineBuffer, sizeof(lineBuffer), indexFile) != NULL && userCount < 100) {
        lineBuffer[strcspn(lineBuffer, "\n")] = 0;
        
        int isDuplicate = 0;
        for(int i = 0; i < userCount; i++) {
            if (strcmp(nameList[i], lineBuffer) == 0) {
                isDuplicate = 1;
                break;
            }
        }

        if (!isDuplicate && lineBuffer[0] != '\0') {
            strcpy(nameList[userCount], lineBuffer);
            printf("[%d] - %s\n", userCount + 1, nameList[userCount]);
            userCount++;
        }
    }
    fclose(indexFile);
    
    if (userCount == 0) {
        printf("No users found in index file.\n");
        return;
    }

    printf("\nEnter the number of the user to log data for: ");
    //true or false
    if (scanf("%d", &selection) != 1) {
        printf("Invalid input.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }
    
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (selection >= 1 && selection <= userCount) {
        strcpy(details->name, nameList[selection - 1]);
        printf("User '%s' selected successfully. New logs will be appended to their file.\n", details->name);
    } else {
        printf("Invalid user selection.\n");
    }
}

void displayMenu() {
    printf("\n--- My Health Tracker ---\n");
    printf("1. Enter Personal Details and Daily Health Data\n");
    printf("2. Display Weekly Health Summary\n");
    printf("3. Display Monthly Health Summary\n");
    printf("4. View File Data\n");
    printf("5. Exit");
}

void enterPersonalDetails(struct PersonalDetails *details) {
    char tempName[50];
    
    printf("\nEnter Personal Details:\n");
    
    printf("Name: ");
    fgets(tempName, sizeof(tempName), stdin);
    tempName[strcspn(tempName, "\n")] = '\0';
    
    if (tempName[0] == '\0') {
        printf("User name cannot be empty.\n");
        details->name[0] = '\0';
        return;
    }

    // --- CHECK FOR EXISTENCE ---
    int userExists = 0;
    FILE *indexFileRead = fopen("user_index.txt", "r");
    if (indexFileRead != NULL) {
        char lineBuffer[50];
        while (fgets(lineBuffer, sizeof(lineBuffer), indexFileRead) != NULL) {
            lineBuffer[strcspn(lineBuffer, "\n")] = '\0';
            if (strcmp(tempName, lineBuffer) == 0) {
                userExists = 1;
                break;
            }
        }
        fclose(indexFileRead);
    }

    if (userExists) {
        printf("User name existed.\n");
        details->name[0] = '\0';
        return;
    }
    // --- END CHECK ---
    
    // If new user: Copy the name and proceed with data entry.
    strcpy(details->name, tempName);

    // Collect remaining details
    printf("Date of Birth (dd/mm/yyyy): ");
    scanf("%s", details->dob);
    
    while(getchar() != '\n');
    
    printf("Sex: ");
    fgets(details->sex,sizeof(details->sex), stdin);
    details->sex[strcspn(details->sex, "\n")]= '\0';
    
    printf("Weight (kg): ");
    scanf("%f", &details->weight);
    printf("Height (cm): ");
    scanf("%f", &details->height);
    
    while(getchar() != '\n');
    
    printf("Health Goals: ");
    fgets(details->healthGoals, sizeof(details->healthGoals), stdin);
    details ->healthGoals[strcspn(details->healthGoals, "\n")] = '\0';

    // --- SAVE NEW PERSONAL DETAILS USING APPEND ---
    // 1. Add name to the user index (for listing)
    FILE *nameIndexFile = fopen("user_index.txt", "a");
    if (nameIndexFile != NULL) {
        fprintf(nameIndexFile, "%s\n", details->name);
        fclose(nameIndexFile);
    }
    
    // 2. Append the full details to the central details file
    FILE *detailsFile = fopen("personal_details_index.txt", "a");
    if (detailsFile != NULL) {
        fprintf(detailsFile, "--- USER: %s ---\n", details->name);
        fprintf(detailsFile, "DOB: %s\n", details->dob);
        fprintf(detailsFile, "Sex: %s\n", details->sex);
        fprintf(detailsFile, "Weight: %.2f kg\n", details->weight);
        fprintf(detailsFile, "Height: %.2f cm\n", details->height);
        fprintf(detailsFile, "Goals: %s\n\n", details->healthGoals);
        fclose(detailsFile);
        printf("\nNew user personal details saved successfully.\n");
    }
}

void logHealthData(struct DailyHealthData *data) {
    int index = 1;
    printf("\nLog Daily Health Data:\n");

    while(index != 0){

    printf("Date (dd/mm/yyyy): ");
    scanf("%s", data->date[arraySize]);
    
     while(getchar() != '\n');
     
    
    printf("\nFood Items: ");
    printf("\n1. Eggs\n2. Chicken\n3. Whole Grain Toast\n4. Banana\n5. Oatmeal\n6. Salad\n7. Fish\n");
    printf("Enter your choice:");
    scanf("%d", &data->foodChoice);
    
     if (data->foodChoice == 1){
        strcpy(data->foodItems[arraySize], "Eggs"); }
    else if(data->foodChoice == 2){
        strcpy(data->foodItems[arraySize], "Chicken"); }
    else if (data->foodChoice == 3){
        strcpy(data->foodItems[arraySize], "Whole Grain Toast");}
    else if (data->foodChoice == 4){
        strcpy(data->foodItems[arraySize], "Banana");}
    else if (data->foodChoice == 5){
        strcpy(data->foodItems[arraySize], "Oatmeal");}
    else if (data->foodChoice == 6){
        strcpy(data->foodItems[arraySize], "Salad");}
    else if (data->foodChoice == 7){
        strcpy(data->foodItems[arraySize], "Fish");}
    else {
        printf ("Invalid choice for food items.\n");
     }
    printf("Calories Consumed: ");
    scanf("%f", &data->caloriesConsumed[arraySize]);
    
    
    printf("\nExercise Details:\n");
    printf("1. Running\n2. Jogging\n3. Brisk Walk\n");
    printf("Enter your choice:");
    scanf("%d", &data->exerciseChoice);

    if(data->exerciseChoice == 1){
      strcpy(data->exerciseDetails[arraySize], "Running"); }
      
    else if (data->exerciseChoice == 2){
      strcpy(data->exerciseDetails[arraySize], "Jogging"); }
    else if (data->exerciseChoice == 3){
      strcpy(data->exerciseDetails[arraySize], "Brisk Walk"); }
    else{
        printf("Invalid choice\n"); }
        
    
    printf("\nEnter your exercise duration in minutes:");
    scanf("%f", &data->exerciseDuration[arraySize]);
    
    
    
    printf("\nSleep Duration (hours):\n");
    printf("1. 6 hours\n2. 7 hours\n3. 8 hours\n");
    printf("Enter your choice: ");
    scanf("%d", &data->sleepChoice);
    
     if(data->sleepChoice == 1){
      data->sleepDuration[arraySize] = 6;}
    else if (data->sleepChoice == 2){
      data->sleepDuration[arraySize] = 7;  }
    else if (data->sleepChoice ==3){
      data->sleepDuration[arraySize] = 8;  }
    else{
        printf("Invalid choice\n"); }
     
    arraySize++;
    displayHealthRecommendations(data->caloriesConsumed[arraySize - 1], data, data->sleepDuration[arraySize - 1], arraySize);
    index = 0;
    }
}

void saveToFile(struct PersonalDetails *details, struct DailyHealthData *data, int numEntries) {
    
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_health_data.txt", details->name);
    
    FILE *file;

    // Open file in append mode ("a") to ONLY add the new daily log data
    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: Could not open file %s for appending logs.\n", filename);
        return;
    }

    // Only write the entry that was just logged (the last entry in the array)
    if (numEntries > 0) {
        int i = numEntries - 1; // Index of the last entry logged
        fprintf(file, "\n--- NEW DAILY LOG ---\n");
        fprintf(file, "Entry Date: %s\n", data->date[i]);
        fprintf(file, "Food: %s (%.2f Calories)\n", data->foodItems[i], data->caloriesConsumed[i]);
        fprintf(file, "Exercise: %s (%.2f minutes)\n", data->exerciseDetails[i], data->exerciseDuration[i]);
        fprintf(file, "Sleep: %.2f hours\n", data->sleepDuration[i]);
        fprintf(file, "\n");
    } else {
         fprintf(file, "\n(No new data logged in this session)\n");
    }

    fclose(file);

    printf("Daily log appended successfully for user: %s\n", details->name);
}

void displayWeeklyHealthSummary(struct DailyHealthData *data) {
    struct WeeklyHealthSummary weeklySummary = {0};

    for (int i = 0; i < 7; i++) {
        weeklySummary.totalCalories += data->caloriesConsumed[i];
        weeklySummary.totalExerciseDuration += data->exerciseDuration[i];
        weeklySummary.averageSleepDuration += data->sleepDuration[i];
    }

    weeklySummary.averageSleepDuration /= 7;

    printf("\nWeekly Health Summary:\n");
    printf("Total Calories Consumed: %.2f\n", weeklySummary.totalCalories);
    printf("Total Exercise Duration: %.2f minutes\n", weeklySummary.totalExerciseDuration);
    printf("Average Sleep Duration: %.2f hours\n", weeklySummary.averageSleepDuration);
}

void displayMonthlyHealthSummary(struct DailyHealthData *data) {
    float totalCalories = 0, totalExerciseDuration = 0, totalSleepDuration = 0;

    for (int i = 0; i < 28; i++) {
        totalCalories += data->caloriesConsumed[i];
        totalExerciseDuration += data->exerciseDuration[i];
        totalSleepDuration += data->sleepDuration[i];
    }

    float averageCalories = totalCalories / 28;
    float averageExerciseDuration = totalExerciseDuration / 28;
    float averageSleepDuration = totalSleepDuration / 28;

    printf("\nMonthly Health Summary:\n");
    printf("Average Calories Consumed: %.2f\n", averageCalories);
    printf("Average Exercise Duration: %.2f minutes\n", averageExerciseDuration);
    printf("Average Sleep Duration: %.2f hours\n", averageSleepDuration);
}



void displayHealthRecommendations(float caloricIntake, struct DailyHealthData *data, float sleepDuration, int arraySize) {
    printf("\nHealth Recommendations:\n");

    if (caloricIntake < 800) {
        printf("Caloric Intake: Below recommended limit for weight loss.\n");
    } else if (caloricIntake > 800 && caloricIntake <= 2400) {
        printf("Caloric Intake: Within recommended limit for weight maintenance.\n");
    } else {
        printf("Caloric Intake: Exceeds recommended limit for weight gain.\n");
    }
 
    if (sleepDuration < 7) {
        printf("Sleep Duration: Below recommended limit for optimal sleep.\n");
    } else if (sleepDuration >= 7 && sleepDuration <= 9) {
        printf("Sleep Duration: Within recommended limit for optimal sleep.\n");
    } else {
        printf("Sleep Duration: Exceeds recommended limit for excessive sleep.\n");
    }
    
    float exerciseDurationPerWeek = 0;
    
    for(int i=0; i<arraySize; i++){
      exerciseDurationPerWeek += data->exerciseDuration[i];
      
      }
      
    if (exerciseDurationPerWeek < 150) {
        printf("Exercise Duration: %.2f minutes, Below recommended limit for optimal exercise.\n", exerciseDurationPerWeek );
    } else if (exerciseDurationPerWeek >= 150 && exerciseDurationPerWeek <= 300) {
        printf("Exercise Duration: %.2f minutes, Within recommended limit for optimal exercise.\n", exerciseDurationPerWeek);
    } else {
        printf("Exercise Duration: %.2f minutes, Exceeds recommended limit for optimal exercise.\n", exerciseDurationPerWeek);
    }
    
    return;

}

// CORRECTED FUNCTION TO LIST USERS AND VIEW DATA
void viewFileData(struct PersonalDetails *details) {
    FILE *indexFile = fopen("user_index.txt", "r");
    if (indexFile == NULL) {
        printf("\nNo registered users found to view. Please use Option 1 to enter personal details first.\n");
        return;
    }

    printf("\n--- Registered Users for Viewing ---\n");
    char nameList[100][50];
    char lineBuffer[50];
    int userCount = 0;
    int selection;
    int c;

    // 1. List all unique users
    while (fgets(lineBuffer, sizeof(lineBuffer), indexFile) != NULL && userCount < 100) {
        lineBuffer[strcspn(lineBuffer, "\n")] = 0;
        
        int isDuplicate = 0;
        for(int i = 0; i < userCount; i++) {
            if (strcmp(nameList[i], lineBuffer) == 0) {
                isDuplicate = 1;
                break;
            }
        }

        if (!isDuplicate && lineBuffer[0] != '\0') {
            strcpy(nameList[userCount], lineBuffer);
            printf("[%d] - %s\n", userCount + 1, nameList[userCount]);
            userCount++;
        }
    }
    fclose(indexFile);
    
    if (userCount == 0) {
        printf("No users found in index file.\n");
        return;
    }

    // 2. Select a user
    printf("\nEnter the number of the user to view their data: ");
    if (scanf("%d", &selection) != 1) {
        printf("Invalid input.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }
    
    while ((c = getchar()) != '\n' && c != EOF);
    
    if (selection < 1 || selection > userCount) {
        printf("Invalid user selection.\n");
        return;
    }
    
    char selectedName[50];
    strcpy(selectedName, nameList[selection - 1]);
    
    // 3. Find and Display Personal Details from personal_details_index.txt
    
    char searchTag[60];
    snprintf(searchTag, sizeof(searchTag), "--- USER: %s ---", selectedName);
    
    FILE *detailsFile = fopen("personal_details_index.txt", "r");
    if (detailsFile == NULL) {
        printf("\nERROR: Could not open the personal details database.\n");
        return;
    }
    
    printf("\n============================================\n");
    printf("--- Personal Details for %s ---\n", selectedName);

    int found = 0;
    int printingDetails = 0;
    char detailBuffer[256];
    
    // Read the personal details file line by line
    while (fgets(detailBuffer, sizeof(detailBuffer), detailsFile) != NULL) {
        if (printingDetails) {
            // Stop printing if we hit the next user's details marker or an empty line marker
            if (strstr(detailBuffer, "--- USER:") || strcmp(detailBuffer, "\n") == 0) {
                printingDetails = 0;
                break;
            }
            printf("%s", detailBuffer);
        } else {
            // Check for the start of the selected user's details block
            // Use strstr() to find the searchTag within the line, ignoring the trailing \n
            if (strstr(detailBuffer, searchTag) != NULL) {
                found = 1;
                printingDetails = 1;
                printf("%s", detailBuffer); // Print the USER marker line
            }
        }
    }
    fclose(detailsFile);
    
    if (!found) {
        printf("Personal details could not be retrieved.\n");
    }

    printf("============================================\n");


    // 4. Find and Display Daily Logs from [Name]_health_data.txt
    
    char logFilename[100];
    snprintf(logFilename, sizeof(logFilename), "%s_health_data.txt", selectedName);
    
    FILE *logFile = fopen(logFilename, "r");
    
    printf("\n--- Daily Health Logs from %s ---\n", selectedName);
    
    if (logFile == NULL) {
        printf("** No daily health logs found for this user. **\n");
    } else {
        char logCh;
        while ((logCh = fgetc(logFile)) != EOF) {
            putchar(logCh);
        }
        fclose (logFile);
    }
    
    printf("\n--------------------------------------------\n");
}
