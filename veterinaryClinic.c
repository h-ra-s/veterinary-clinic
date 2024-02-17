/*
Heather Slanec
Veterinary clinic final exam practice

tools/compiler options/second "linker" box/ -std=c99
*/

#include<stdio.h>
#include<string.h>//for strings
#include<stdlib.h>//for files

//structures-----------------------------------------------------
struct Services
{
	int serviceID;
	char service[50];
	int petType;
	float price;
};
struct Pet
{
	char petName[30];
	int petType;
	char gender;
	char ownerName[40];
};

//function prototypes---------------------------------------------
void readServices(struct Services allServices[]);
int readPets(struct Pet allPets[]);
int locatePetRecord(char inputPetName[], char inputOwnerName[], struct Pet allPets[], int numOfPets);
struct Pet newPetRecord(char inputPetName[], char inputOwnerName[]);
																		//change name from allPets to onePet
int selectServices(struct Services allServices[], struct Services custServices[], struct Pet onePet);
void paymentInfo(int *cardNumber, int *cardMonth, int *cardYear, int *cardCvv);
float printRecipt(int cardNumber, int cardMonth, int cardYear, int cardCvv, struct Services custServices[], struct Pet onePet, int numOfServices);

//main------------------------------------------------------------------------------------------------------------
int main()
{
	//array of 15 strings for data file services
	struct Services allServices[15];
	
	//array for services customer chooses
	struct Services custServices[4];
	
	//array to store 30 pets from data file
	struct Pet allPets[30];
	
	//declare vars
	int numOfPets, recordLocation, numOfServices, customersHelped = 0;
	char inputPetName[30], inputOwnerName[40], anotherCust;
	int cardNumber, cardMonth, cardYear, cardCvv;
	float totalPrice = 0;
	
	do{
		//function to read services from data file
		readServices(allServices);
		
		//function to read pets from data file
		numOfPets = readPets(allPets);
		
		
		//ask user for pet name
		printf("\nEnter the pet's name: ");
		fgets(inputPetName, 30, stdin);
		//remove enter keystroke from user input & insert null character in its place		
		inputPetName[strlen(inputPetName) - 1] = '\0';
		
		//ask user for owner name
		printf("\nEnter the owner's name: ");
		fgets(inputOwnerName, 40, stdin);
//CAREFUL WITH REMOVING ENTER KEYSTROKE it bugs the program when put here:
		//inputOwnerName[strlen(inputOwnerName) - 1] = '\0';
		
		
		//function to locate pet record
		recordLocation = locatePetRecord(inputPetName, inputOwnerName, allPets, numOfPets);
		
		//if pet record location isn't found:
		if(recordLocation == -1)
		{
			//set index location to current number of pets in the data
			recordLocation = numOfPets;
			
			//call function to create new pet record in blank location
			allPets[recordLocation] = newPetRecord(inputPetName, inputOwnerName);
			
			//increment number of pets
			numOfPets++;
		}
		
		//function to select services - send record location of ONE PET, aka a variable, no brackets
		numOfServices = selectServices(allServices, custServices, allPets[recordLocation]);
		
		//function to get payment info - send memory locations
		paymentInfo(&cardNumber, &cardMonth, &cardYear, &cardCvv);
		//test print
		//printf("\n%d %d %d %d", cardNumber, cardMonth, cardYear, cardCvv);
		
		//function to print recipt
		totalPrice += printRecipt(cardNumber, cardMonth, cardYear, cardCvv, custServices, 
			allPets[recordLocation], numOfServices);
			
		//accumulate customers helped
		customersHelped ++;
		
		//ask if there is another customer
		printf("\nIs there another customer? Y/N: ");
		scanf("\n%c", &anotherCust);
		//get rid of enter keystroke
		getchar();
	}while(anotherCust == 'Y' || anotherCust == 'y');
	
	//display totals
	printf("\nTotal number of pets served: %d", customersHelped);
	printf("\nTotal amount collected: %.2f", totalPrice);

	
	return 0;
}
//functions-----------------------------------------------------------------------------------------------------------
float printRecipt(int cardNumber, int cardMonth, int cardYear, int cardCvv, 
	struct Services custServices[], struct Pet onePet, int numOfServices)
{
	int x;
	float totalPrice = 0;
	
	printf("\nPayment receipt for services on %s:", onePet.petName);
	
	//loop to find user-chosen service and services match
	for(x = 0; x < numOfServices; x++)
	{
		printf("\t\n%d %s $ %.2f", custServices[x].serviceID, custServices[x].service, 
		custServices[x].price);
			
		totalPrice += custServices[x].price;
	}
	
	printf("\n\n\tTotal paid: %.2f", totalPrice);
	
	printf("\n\n\tCard Number:       %d", cardNumber);
	printf("\n\tExpiration Date:   %d/%d", cardMonth, cardYear);
	printf("\n\tCVV Code:          %d", cardCvv);
	
	return totalPrice;
}

 
void paymentInfo(int *cardNumber, int *cardMonth, int *cardYear, int *cardCvv)
{
	printf("\nEnter Payment Information:");
	
	printf("\nEnter credit card number:  ");
	scanf("%d", cardNumber);
	
	printf("\nEnter card expiration Month:  ");
	scanf("%d", cardMonth);
	
	printf("\nEnter card expiration Year:  ");
	scanf("%d", cardYear);
	
	printf("\nEnter card cvv code:  ");
	scanf("%d", cardCvv);
	
	//test print
	//printf("\n%d %d %d %d", *cardNumber, *cardMonth, *cardYear, *cardCvv);
}


int selectServices(struct Services allServices[], struct Services custServices[], struct Pet onePet)
{
	int x, y, code;
	
	//outer loop for custServices
	for(y = 0; y < 4; y++)
	{
		for(x = 0; x < 15; x++)
		{
			if(onePet.petType == allServices[x].petType || allServices[x].petType == 3)
			{
				printf("\n%d %s $ %.2f", allServices[x].serviceID, allServices[x].service, 
				allServices[x].price);
			}
		}
		
		//get user input
		printf("\nPlease choose a service code or 0 to quit: ");
		scanf("%d", &code);
		//if quit is chosen, break
		if(code == 0)
			break;
	
		//loop to find user-chosen service and services match
		for(x = 0; x < 15; x++)
		{
			if(code == allServices[x].serviceID)
			{
				//put code (15 possible) and related info from allServices to 
				//correct slot (1-4) in custServices
				custServices[y] = allServices[x];
			}
		}
	}
	return y;
}


struct Pet newPetRecord(char inputPetName[], char inputOwnerName[])
{
	//create structure variable
	struct Pet newPet;
	
	//declare more vars
	int petType;
	char petGender;
	
	printf("\nCreating new pet account...");
	
	//assign vars from argument to structure
	strcpy(newPet.petName, inputPetName);
	strcpy(newPet.ownerName, inputOwnerName);
	
	//get user input
	printf("\nEnter 1 for Dog or 2 for Cat: ");
	scanf("%d", &petType);
	
	//validate input
	while(petType != 1 && petType != 2)
	{
		printf("\nError! Try again.");
		
		//get user input
		printf("\nEnter 1 for Dog or 2 for Cat: ");
		scanf("%d", &petType);
	}
	
	//get user input
	printf("\nEnter M for Male or F for Female: ");
	scanf("\n%c", &petGender);
	
	//validate input
	while(petGender != 'F' && petGender != 'f' && petGender != 'M' && petGender != 'm')
	{
		printf("\nError! Try again.");
		
		//get user input
		printf("\nEnter M for Male or F for Female: ");
		scanf("\n%c", &petGender);
	}
	
	//return structure variable
	return newPet;
}


int locatePetRecord(char inputPetName[], char inputOwnerName[], struct Pet allPets[], int numOfPets)
{
	int x;
	for(x = 0; x < numOfPets; x++)
	{
		//if they match:
		if(strcmp(inputPetName, allPets[x].petName) == 0 
		&& strcmp(inputOwnerName, allPets[x].ownerName) == 0)
		{
			//exit function with index location
			return x;
		}
	}
	//return fail if names don't match
	return -1;
}


int readPets(struct Pet allPets[])
{
	//1
	FILE *fp;
	
	//2
	fp = fopen("allPets.csv", "r");
	
	//3
	if(fp == NULL)
	{
		printf("Could not open allPets file...exiting program");
		exit(0);
	}
	
	//4
	char line[100];
	char *sp;
	
	int x = 0;
	while(fgets(line,100,fp) != NULL)
	{
		sp = strtok(line, ",");
		strcpy(allPets[x].petName, sp);
		
		sp = strtok(NULL, ",");
		allPets[x].petType = atoi(sp);
		
		sp = strtok(NULL, ",");
		allPets[x].gender = *sp;
		
		sp = strtok(NULL, ",");
		strcpy(allPets[x].ownerName, sp);
			
		//printf("\n%s %d %c %s",allPets[x].petName, allPets[x].petType, 
		//allPets[x].gender, allPets[x].ownerName);
		
		x++;
	}
	
	//5
	fclose(fp);
	
	//return # of pets
	return x;
}


void readServices(struct Services allServices[])
{
	//1
	FILE *fp;
	
	//2
	fp = fopen("allServices.csv", "r");
	
	//3
	if(fp == NULL)
	{
		printf("Could not open allServices file...exiting program");
		exit(0);
	}
	
	//4
	char line[100];
	char *sp;
	
	int x = 0;
	while(fgets(line,100,fp) != NULL)
	{
		sp = strtok(line, ",");
		allServices[x].serviceID = atoi(sp);
		
		sp = strtok(NULL, ",");
		strcpy(allServices[x].service, sp);
		
		sp = strtok(NULL, ",");
		allServices[x].petType = atoi(sp);
		
		sp = strtok(NULL, ",");
		allServices[x].price = atof(sp);
			
		//printf("\n%d %s %d %.2f",allServices[x].serviceID, allServices[x].service, 
		//allServices[x].petType, allServices[x].price);
		
		x++;
	}
	
	//5
	fclose(fp);
}








