#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_NAME_LENGTH 100
#define MAX_FOOD_ITEMS 100

// Function to print the bar graph for each activity level
void printGraph(int calories[], int size) {
    const char* activityLevels[] = {"sedentary", "lightly active", "moderately active", "very active", "super active"};
    double multipliers[] = {1.0, 1.15, 1.35, 1.55, 1.75}; // Updated multipliers for more realistic values
    int numLevels = 5;
    int maxCalories = 2000; // Set a maximum calorie value for scaling
    int maxBarLength = 50; // Maximum length of the bar
    double baseBurnRate = 100.0; // Base burn rate for a sedentary activity level

    for (int level = 0; level < numLevels; level++) {
        double adjustedBurnRate = baseBurnRate * multipliers[level]; // Adjusted burn rate for each activity level
        printf("\n\e[32mActivity Level:\e[1;31m %s\e[0m\n", activityLevels[level]);
        for (int i = 0; i < size; i++) {
            int caloriesBurned = calories[i]; // No need to multiply by multipliers[level] here
            int barLength = (caloriesBurned * maxBarLength) / maxCalories; // Scale bar length
            double timeToBurn = (double)caloriesBurned / adjustedBurnRate; // Calculate time in hours

            printf("\e[32mMeal %d\e[34m❚\e[0m", i + 1);
            for (int j = 0; j < barLength; j++) {
                printf("\e[1;36m■\e[0m");
                fflush(stdout); // Ensure the output is printed immediately
                usleep(20000); // Sleep for 50 milliseconds
            }
            printf(" (\e[1;32m%d calories, %.2f hours to burn)\e[0m\n", caloriesBurned, timeToBurn);
        }
    }
}

void print_UTHM_logo() {
  char logo[] =
      "\n"
      " /$$   /$$       /$$$$$$$$       /$$   /$$       /$$      /$$ \n"
      "| $$  | $$      |__  $$__/      | $$  | $$      | $$$    /$$$ \n"
      "| $$  | $$         | $$         | $$  | $$      | $$$$  /$$$$ \n"
      "| $$  | $$         | $$         | $$$$$$$$      | $$ $$/$$ $$ \n"
      "| $$  | $$         | $$         | $$__  $$      | $$  $$$| $$ \n"
      "| $$  | $$         | $$         | $$  | $$      | $$\\  $ | $$ \n"
      "|  $$$$$$/         | $$         | $$  | $$      | $$ \\/  | $$ \n"
      "  \\______/          |__/         |__/  |__/      |__/     |__/  \n"
      "\n";
    char logo2[] ="\n ========================================================================================= \n";

  // Loop through each character of the logo
    for (int i = 0; i < strlen(logo); i += 2) {
        printf("\e[34m%c%c \e[0m", logo[i], logo[i + 1]);
        fflush(stdout);
        usleep(10000); // Or adjust as needed
    }
    for (int j = 0; j < strlen(logo2); j += 2) {
        printf("\e[31m%c \e[0m", logo2[j]);
        fflush(stdout);
        usleep(100000);
    }
}

typedef struct {
    char name[MAX_NAME_LENGTH];
    int calories_per_100g;
    float proteins;
    float fats;
    float carbohydrates;
} FoodItem;

FoodItem foodDatabase[MAX_FOOD_ITEMS];
int foodCount = 0;

void loadFoodData(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^,],%d,%f,%f,%f", foodDatabase[foodCount].name,
                   &foodDatabase[foodCount].calories_per_100g,
                   &foodDatabase[foodCount].proteins,
                   &foodDatabase[foodCount].fats,
                   &foodDatabase[foodCount].carbohydrates) == 5) {
            foodCount++;
        }
    }

    fclose(file);
}

void printFoodData() {
    printf("+-----+----------------------+-----------------+----------+---------+----------------+\n");
    printf("| No. | Food Name            | Calories (100g) | Proteins | Fats    | Carbohydrates  |\n");
    printf("+-----+----------------------+-----------------+----------+---------+----------------+\n");
    for (int i = 0; i < foodCount; i++) {
        printf("| %-3d | %-20s | %-15d | %-8.2f | %-7.2f | %-14.2f |\n", i + 1, foodDatabase[i].name, foodDatabase[i].calories_per_100g, foodDatabase[i].proteins, foodDatabase[i].fats, foodDatabase[i].carbohydrates);
    }
    printf("+-----+----------------------+-----------------+----------+---------+----------------+\n");
}

void printFoodDetails(int foodNumber, int quantity) {
    if (foodNumber < 1 || foodNumber > foodCount) {
        printf("Invalid food number.\n");
        return;
    }
    
    FoodItem food = foodDatabase[foodNumber - 1];
    float totalCalories = (food.calories_per_100g * quantity) / 100.0;
    printf("\nDetails for %s:\n", food.name);
    printf("+------------------+------------------+\n");
    printf("| Calories (100g)  | %-16d |\n", food.calories_per_100g);
    printf("| Total Calories   | %-16.2f |\n", totalCalories);    
    printf("| Proteins         | %-16.2f |\n", food.proteins);
    printf("| Fats             | %-16.2f |\n", food.fats);
    printf("| Carbohydrates    | %-16.2f |\n", food.carbohydrates);
    printf("+------------------+------------------+\n");
}

int main() {
    print_UTHM_logo();
    int input;
    do {
        printf("\n(1)Choose from the table:\n");
        printf("(2) Introduce your own food?\n");
        printf("Enter your choice: ");
        scanf("%d", &input);
        if (input == 1){
            loadFoodData("food_data.txt");
            printFoodData();
            
            int foodNumber;
            printf("\nEnter food number to get details: ");
            scanf("%d", &foodNumber);
            int quantity;
            printf("\nEnter the quantity in grams:");
            scanf("%d", &quantity);
            printFoodDetails(foodNumber, quantity);
        } else if (input == 2) {
            char foodname[100];
            int qantity2;
            int caloriesGRM;

            printf("Enter name food\n");
            scanf("%s", foodname);
            printf("\nEnter the quantity in grams:\n");
            scanf("%d", &qantity2);
            printf("\nEnter the number of calories");
            scanf("%d", &caloriesGRM);
            float totalCalories = (caloriesGRM * qantity2) / 100.0;
            printf("\nDetails for %s:\n", foodname);
            printf("+------------------+------------------+\n");
            printf("| Calories (100g)  | %-16d |\n", qantity2);
            printf("| Total Calories   | %-16.2f |\n", totalCalories);    
            printf("+------------------+------------------+\n");
        } else {
            printf("Invalid number entered. Please try again.\n");
        }
    } while (input != 1 && input != 2);
        
    int calories[3];
    int totalMeals = 3;
    int monitoring;

    // Ask the user if they are monitoring their calorie count
    printf("Are you monitoring your calorie count throughout the day? (1 for Yes, 2 for No): ");
    scanf("%d", &monitoring);

    if (monitoring == 1) {
        // Get calories for each meal
        for (int i = 0; i < totalMeals; i++) {
            printf("Enter calories for meal %d: ", i + 1);
            scanf("%d", &calories[i]);
        }

        // Print the graph for all activity levels
        printGraph(calories, totalMeals);
    } else {
        printf("Calorie monitoring is not enabled.\n");
    }
    
    return 0;
}
