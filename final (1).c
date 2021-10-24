#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<string.h>


typedef struct     // struct of birthday
{
	unsigned int Year;
	char Month;
	char Day;
}DateOfBirth;


typedef struct    // struct of person
{
	unsigned long long Id;
	char* Name;
	char* Family;
	DateOfBirth dateOfBirth;
	unsigned long long PartnerId;
	unsigned long long MotherId;
	unsigned long long FatherId;
	char NumOfChildren;
	long long* ChildrenPtr;
}Person;


typedef struct     // struct of array of person
{
	Person* personArray;
	int numOfRegisteredPersons;
	int sizeArray;

}db_mgr;

typedef struct            // strucr for array of Id 
{
	long long* childrens_ID;
	int arrSize;             //The amount of ID's stored in the array
	int gen;
}Gen;

void init_db(db_mgr* s);
void callFunction(const db_mgr* s);
int menu();
void add_person(db_mgr* s);
long long checkPositive();
void adjustLocation(db_mgr* s);
void shiftPersons(db_mgr* s, int startIndex);
void copyPersonArray(Person* newArray, const Person* existArray, int numOfRegisteredPersons);
void copyOnePerson(Person* newArray, const Person* existArray);
void search_person(const db_mgr* s);
Person* search_id(const db_mgr* s, long long num);
void search_parents(const db_mgr* s);
void delete_person(db_mgr* s);
void deleteKid(const db_mgr* s, Person* p, long long kid_id);
int findPersonIndex(const db_mgr* s, long long id);
void get_gen(const db_mgr* s);
void IdArray(const db_mgr* s, Gen* ID);
int cauntKidsNum(const db_mgr* s, const Gen* ID);
void print_db(const db_mgr* s);
void print_person(const Person* p);
void search_by_name(const db_mgr* s);
void freeAllAllocations(db_mgr* s);
void freeFirstNames(db_mgr* s);
void freeLastNames(db_mgr* s);
void freeChildren(db_mgr* s);
void quit(const db_mgr* s);

void main()
{
	db_mgr dataBase;
	init_db(&dataBase);

	while (1)
	{
		callFunction(&dataBase);
	}

	system("pause");
}

void init_db(db_mgr* s)           //  Initializing database
{
	int numOfcitizens;

	printf("insert the amount of the persons in the country please\n");
	printf("\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%d", &numOfcitizens);

	while (numOfcitizens < 1)
	{
		printf("\n\nError. please insert the amount of the country persons again\n");
		scanf("%d", &numOfcitizens);
	}

	s->personArray = (Person*)malloc(numOfcitizens * sizeof(Person));   //array of persons at the size of the ammount persons which scaned

	if (s->personArray == NULL)
	{
		printf("memory allocation failed\n");
		exit(0);
	}

	s->sizeArray = numOfcitizens;
	s->numOfRegisteredPersons = 0;
}


void callFunction(const db_mgr* s)     // call the main menu and choose the right one 
{
	int num = menu();
	switch (num)
	{

	case 1:
		add_person(s);
		break;

	case 2:
		search_person(s);
		break;

	case 3:
		search_parents(s);
		break;

	case 4:
		delete_person(s);
		break;

	case 5:
		get_gen(s);
		break;

	case 6:
		print_db(s);
		break;

	case 7:
		search_by_name(s);
		break;

	case 8:
		quit(s);
		break;


	}
}


int menu()     // main menu
{
	int num;

	printf("\n\nDatabase System Menu:\n\n1. Add person\n2. Search a person\n3. Search Parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n");
	printf("\nPlease press the number of the option you want to use!\n\n");

	fseek(stdin, 0, SEEK_END);
	scanf("%d", &num);

	while (num < 1 || num > 8)
	{
		printf("\n");
		printf("Error. The option you pressed doesn't exist!\n");
		printf("\n\nDatabase System Menu:\n\n1. Add person\n2. Search a person\n3. Search Parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n");
		printf("\nPlease press the number of the option you want to use!\n\n");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &num);
	}

	return num;
}


void add_person(db_mgr* s)       // add person to the array
{

	if (s->numOfRegisteredPersons == s->sizeArray)
	{
		Person* tmp;

		tmp = (Person*)malloc(((s->numOfRegisteredPersons) + 1) * sizeof(Person));  //new array of person which Functioning as realloc

		if (tmp == NULL)
		{
			printf("memory allocation failed\n");
			quit(s);
		}

		copyPersonArray(tmp, s->personArray, s->numOfRegisteredPersons); // copy the exist array to a new bigger array

		free(s->personArray);

		s->personArray = tmp;
		(s->sizeArray)++;
	}

	Person* currentPerson = &(s->personArray[s->numOfRegisteredPersons]);  //shorter name for the array

	printf("\nLet's start insert details please\n");
	printf("\nInsert your Id\n");   // get ID

	currentPerson->Id = checkPositive();

	char NAME[100];

	printf("\n\nInsert your first name please\n\n");     // get first name
	fseek(stdin, 0, SEEK_END);
	gets(NAME);

	currentPerson->Name = (char*)malloc((strlen(NAME) + 1) * sizeof(char));  // array for the first name

	if (currentPerson->Name == NULL)
	{
		printf("Memory allocation failed\n");
		quit(s);
	}

	strcpy(currentPerson->Name, NAME);

	char LASTNAME[100];

	printf("\n\nInsert your last name please\n\n");     // get last name
	fseek(stdin, 0, SEEK_END);
	gets(LASTNAME);

	currentPerson->Family = (char*)malloc((strlen(LASTNAME) + 1) * sizeof(char));  //array for the last name

	if (LASTNAME == NULL)
	{
		free(currentPerson->Name);
		printf("Memory allocation failed\n");
		quit(s);
	}

	strcpy(currentPerson->Family, LASTNAME);

	printf("\n\nInsert your date of birth please in format of yyyy/mm/dd\n\n");    // get date of birth
	fseek(stdin, 0, SEEK_END);
	scanf("%d/%d/%d",
		&(currentPerson->dateOfBirth.Year),
		&(currentPerson->dateOfBirth.Month),
		&(currentPerson->dateOfBirth.Day)
	);

	while
		(
		currentPerson->dateOfBirth.Day > 30 ||
		currentPerson->dateOfBirth.Day <= 0 ||
		currentPerson->dateOfBirth.Month > 12 ||
		currentPerson->dateOfBirth.Month <= 0 ||
		currentPerson->dateOfBirth.Year > 9999 ||
		currentPerson->dateOfBirth.Year < 1000
		)
	{

		printf("\nWrong date, please Insert again in format of yyyy/mm/dd\n\n");

		fseek(stdin, 0, SEEK_END);
		scanf("%d/%d/%d",
			&(currentPerson->dateOfBirth.Year),
			&(currentPerson->dateOfBirth.Month),
			&(currentPerson->dateOfBirth.Day)
		);
	}

	printf("\n\nDo you have a partner?\n");
	currentPerson->PartnerId = checkPositive(); // get partner ID

	printf("\n\nDo you have a mother?\n");
	currentPerson->MotherId = checkPositive();  // get mother ID

	printf("\n\nDo you have a father?\n");
	currentPerson->FatherId = checkPositive();   // get father ID

	printf("\n\nHow many kids do you have?\n\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%d", &(currentPerson->NumOfChildren));  // get number of kids

	while (currentPerson->NumOfChildren < 0)
	{
		printf("\nInsert a valid number!\n");
		scanf("%d", &(currentPerson->NumOfChildren));  
	}

	if ((currentPerson->NumOfChildren) > 0)
	{
		int size = currentPerson->NumOfChildren;
		currentPerson->ChildrenPtr = (long long*)malloc(size * sizeof(long long));   // array of children Id's

		if (currentPerson->ChildrenPtr == NULL)
		{
			free(currentPerson->Name);
			free(currentPerson->Family);
			printf("Memory allocation failed\n");
			quit(s);
		}

		for (int i = 0; i < currentPerson->NumOfChildren; i++)
		{
			printf("\nThe %dst kid:\n ", i + 1);
			currentPerson->ChildrenPtr[i] = checkPositive();
		}
	}

	(s->numOfRegisteredPersons)++; 
	adjustLocation(s);   // locate the person in its axact place in the array

	return;   //back to main menu
}


long long checkPositive()     //  check if the ID is correct
{
	long long num;

	printf("\n(Write the ID or 0 whether you don't have)\n\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%lld", &num);

	while (num < 1 && num != 0)
	{
		printf("\nError. please insert Id again\n\n");
		fseek(stdin, 0, SEEK_END);
		scanf("%lld", &num);
	}

	return num;
}


void adjustLocation(db_mgr* s)   //Gets a data structure in which the latter is the person added. And looks for where to enter according to the ID and moves the array as needed
{

	for (int i = 0; i < s->numOfRegisteredPersons; ++i)
	{

		if (s->personArray[i].Id > s->personArray[(s->numOfRegisteredPersons) - 1].Id)
		{
			if (i == ((s->numOfRegisteredPersons) - 1))
			{
				return;   //back to main menu
			}
			Person temp;

			copyOnePerson(&temp, s->personArray + s->numOfRegisteredPersons - 1);     //save the array in local person
			shiftPersons(s, i);       // shift all the persons one index right
			copyOnePerson(s->personArray + i, &temp);       // replace the local person 'tmp' to its right place in the array according the ID

			return;   //back to main menu
		}
	}

}


void shiftPersons(db_mgr* s, int startIndex)             //Moves and copies each person to the left. From the index that the function received to the end
{
	for (int i = s->numOfRegisteredPersons - 1; i > startIndex; --i)
	{
		copyOnePerson(s->personArray + i, s->personArray + i - 1);
	}
}


void copyPersonArray(Person* newArray, const Person* existArray, int numOfRegisteredPersons)    // The function gets an existing array and a new array and the amount
{                                                                                                 //of people and copies the people from the old array to the new one
	for (int i = 0; i < numOfRegisteredPersons; ++i)
	{
		copyOnePerson(newArray + i, existArray + i);
	}
}


void copyOnePerson(Person* newArray, const Person* existArray)  //Copies all the data of person from an old set to a new one
{
	newArray->Name = existArray->Name;
	newArray->Family = existArray->Family;
	newArray->Id = existArray->Id;
	newArray->dateOfBirth.Year = existArray->dateOfBirth.Year;
	newArray->dateOfBirth.Month = existArray->dateOfBirth.Month;
	newArray->dateOfBirth.Day = existArray->dateOfBirth.Day;
	newArray->PartnerId = existArray->PartnerId;
	newArray->MotherId = existArray->MotherId;
	newArray->FatherId = existArray->FatherId;
	newArray->NumOfChildren = existArray->NumOfChildren;
	newArray->ChildrenPtr = existArray->ChildrenPtr;
}


void search_person(const db_mgr* s)   // search person by ID
{
	long long num;

	printf("\nInsert The Id num of the person you want to search: \n\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%lld", &num);

	Person* p = search_id(s, num);  //get pointer of the required ID 

	if (!p)
	{
		printf("\nThe Id doesn't exist\n");
		return;     //     back to main menu
	}

	else print_person(p);  // print the person
	return;     //     back to main menu

}


Person* search_id(const db_mgr* s, long long num)   // search ID and return pointer to that speciefic person in the array 
{
	int i = 0;

	for (int i = 0; i < s->numOfRegisteredPersons; i++)
	{

		if ((s->personArray[i].Id) == num)
		{
			return &(s->personArray[i]);
		}
	}

	return NULL;
}


void search_parents(const db_mgr* s)   // search parent by ID

{
	long long num;

	printf("\nInsert The Id num of the person you want to search: \n\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%lld", &num);

	Person* p = search_id(s, num);    //get pointer of the required ID 

	if (!p)
	{
		printf("\nThe Id doesn't exist\n");
		return;      //     back to main menu
	}

	long long num1 = p->MotherId;
	long long num2 = p->FatherId;
	Person* p1 = search_id(s, num1);       //get pointer of the mother person
	Person* p2 = search_id(s, num2);      //get pointer of the father person


	if (p1 == NULL && p2 == NULL)
	{
		printf("\nTwo of the Id's are'nt exist\n");
		return;   //     back to main menu
	}

	if (p1 != NULL)
	{
		printf("\nWe've found the mother!\n");
		print_person(p1);    //print the person
	}

	else printf("\nCant find mother\n");

	if (p2 != NULL)
	{
		printf("\nWe've found the father!\n");
		print_person(p2);   //print the person
	}

	else printf("\nCant find father\n");
	return;     //  back to main menu

}


void delete_person(db_mgr* s)   // delete person fron the arrary and make the necessary changes
{
	long long num;

	printf("\nInsert The Id num of the person you want to delete: \n\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%lld", &num);

	Person* p = search_id(s, num);    //get pointer of the required ID 

	if (!p)
	{
		printf("\nThe Id doesn't exist\n");
		return;   //bach to main menu
	}

	else
	{

		if (p->PartnerId != 0)          //delete the person fron his prtner as partner ID
		{
			Person* partner = search_id(s, p->PartnerId);     //get pointer of the required ID 
			if (partner != 0)
			{
				partner->PartnerId = 0;
			}
		}

		if (p->MotherId != 0)
		{
			Person* mother = search_id(s, p->MotherId);     //get pointer of the required ID 
			if (mother != 0)
			{
				deleteKid(s, mother, p->Id);
			}
		}

		if (p->FatherId != 0)
		{
			Person* father = search_id(s, p->FatherId);     //get pointer of the required ID 
			if (father != 0)
			{
				deleteKid(s, father, p->Id);
			}
		}

		if (p->NumOfChildren != 0)
		{

			for (int i = 0; i < p->NumOfChildren; i++)
			{

				Person* child = search_id(s, p->ChildrenPtr[i]);     //get pointer of the required ID 

				if (child != 0)
				{

					if (p->Id == child->FatherId)
					{
						child->FatherId = 0;
					}

					if (p->Id == child->MotherId)
					{
						child->MotherId = 0;
					}
				}
			}

			free(p->ChildrenPtr);
		}

		free(p->Name);
		free(p->Family);

		int index = findPersonIndex(s, p->Id);      //find person index in database

		shiftPersons(s, index);                    //copy every person one left from the index we found

		s->numOfRegisteredPersons--;

		Person* new = (Person*)malloc(((s->sizeArray) - 1) * sizeof(Person));  //Creates a new array -1 after we delete 1 person

		if (new == NULL)
		{
			printf("memory allocation failed\n");
			quit(s);
		}

		copyPersonArray(new, s->personArray, ((s->sizeArray) - 1));   //copy the old array to the new we create

		s->sizeArray--;

		free(s->personArray);

		s->personArray = new;
	}
}


void deleteKid(const db_mgr* s, Person* p, long long kid_id)          // get person and child ID, delete the child from the arrary. 
															   // create new array smaller in 1 than the current array tnd copy all the ID's to the new array.
{
	int j = 0;

	if (p->NumOfChildren == 1)
	{
		free(p->ChildrenPtr);
		p->NumOfChildren = 0;
		return;   //back to main menu
	}

	long long* newChildrenPtr = (long long*)malloc(((p->NumOfChildren) - 1) * sizeof(long long));

	if (newChildrenPtr == NULL)
	{
		quit(s);
	}

	for (int i = 0; i < p->NumOfChildren; i++)
	{

		if (kid_id != p->ChildrenPtr[i])
		{
			newChildrenPtr[j] = p->ChildrenPtr[i];
			j++;
		}
	}

	free(p->ChildrenPtr);

	p->ChildrenPtr = newChildrenPtr;
	p->NumOfChildren--;
}


int findPersonIndex(const db_mgr* s, long long id)       //find person index in database
{
	int i = 0;

	for (i = 0; i < s->numOfRegisteredPersons; i++)
	{

		if (id == s->personArray[i].Id)
		{
			return i;
		}
	}

	return s->numOfRegisteredPersons;
}


void get_gen(const db_mgr* s)          // search and print the number of generation which exist for scanned ID
{

	long long num;

	printf("\nInsert The Id num of the person you want to search: \n\n");
	fseek(stdin, 0, SEEK_END);
	scanf("%lld", &num);

	Person* p = search_id(s, num);   //get pointer of the required ID 

	if (!p)
	{
		printf("\nThe Id doesn't exist\n");
		return;      // back to main menu
	}

	if (p->NumOfChildren == 0)
	{
		printf("\nThe num of generation is 1\n");
		return;      // back to main menu
	}

	Gen ID_array;   //struct of generation details
	ID_array.childrens_ID = (long long*)malloc(1 * sizeof(long long));  // creat array of children ID's in the Gen struct 

	ID_array.childrens_ID[0] = p->Id;
	ID_array.arrSize = 1;
	ID_array.gen = 1;

	while (ID_array.arrSize != 0)          // while the number of children != 0 keep run to the next ganeration array
	{
		IdArray(s, &ID_array);
	}

	printf("\nthe num of generation is %d\n", ID_array.gen);
	free(ID_array.childrens_ID);
}


void IdArray(const db_mgr* s, Gen* ID)                 //  insert all this generation children's ID to arrary of ID's and return the array
{
	int kids_num = cauntKidsNum(s, ID);

	if (kids_num == 0)
	{
		ID->arrSize = 0;
		return;   // back to main menu
	}

	long long* childrens_ID = (long long*)malloc(kids_num * sizeof(long long));        //Creates a new array of children's ID. all the childrens of the persons in the ID->childrens_ID

	if (childrens_ID == NULL)
	{
		printf("Memory allocation failed\n");
		quit(s);
	}
	for (int i = 0; i < ID->arrSize; i++)
	{

		Person* temp = search_id(s, ID->childrens_ID[i]);    //get pointer of the required ID 

		if (temp != 0)
		{
			for (int j = 0; j < temp->NumOfChildren; j++)
			{
				childrens_ID[i] = temp->ChildrenPtr[j];         //copy the childrens ID to the new array. the next gen. 
			}
		}
	}

	free(ID->childrens_ID);

	ID->childrens_ID = childrens_ID;
	ID->arrSize = kids_num;
	ID->gen++;
}


int cauntKidsNum(const db_mgr* s, const Gen* ID)             //count current generation children
{
	int idx = 0;

	for (int i = 0; i < ID->arrSize; i++)
	{

		Person* temp = search_id(s, ID->childrens_ID[i]);    //get pointer of the required ID 

		if (temp != 0)
		{
			idx += temp->NumOfChildren;
		}
	}

	return idx;
}


void print_db(const db_mgr* s)          //Prints all the existing people in the array with all the data in order

{
	int size = s->numOfRegisteredPersons;

	for (int i = 0; i < size; i++)
	{
		printf("\nThe %dst person!\n", i + 1);
		print_person(&s->personArray[i]);
	}

}


void print_person(const Person* p)    // print all the person details
{
	printf("\nThe Id is: %lld\n", p->Id);
	printf("First name is: %s\n", (p->Name));
	printf("Last name is: %s\n", (p->Family));
	printf("The date of birth is: %d/%d/%d\n", (p->dateOfBirth.Day), (p->dateOfBirth.Month), (p->dateOfBirth.Year));
	printf("The Partner Id is: %lld\n", p->PartnerId);
	printf("The Mother Id is: %lld\n", p->MotherId);
	printf("The Father Id is: %lld\n", p->FatherId);
	printf("The num of children is: %d\n", p->NumOfChildren);

	for (int i = 0; i < p->NumOfChildren; i++)
	{
		printf("The Id of the %dth children is: %lld", i + 1, p->ChildrenPtr[i]);
		printf("\n");
	}
}


void search_by_name(const db_mgr* s)    // search person by name

{
	char name[100];

	printf("\nWright the *first* name of the person you want to find\nIf you have'nt insert 0\n\n");
	scanf("%s", name);

	char family[100];

	printf("\nWright the *last* name of the person you want to find\nIf you have'nt insert 0\n\n");
	scanf("%s", family);

	int flag = 0;

	for (int i = 0; i < s->numOfRegisteredPersons; ++i)
	{

		if (!strcmp(name, s->personArray[i].Name) &&
			!strcmp(family, s->personArray[i].Family))
		{
			flag = 1;
			print_person(s->personArray + i);
		}

	}

	if (!flag)
	{
		printf("\nSorry, we did'nt find ! try again\n");
	}
}


void freeAllAllocations(db_mgr* s)     //free all the dynamic memory we used in the program
{
	freeFirstNames(s);
	freeLastNames(s);
	freeChildren(s);
	free(s->personArray);
}


void freeFirstNames(db_mgr* s)         //free the first name of all the people in the array
{
	for (int i = 0; i < s->numOfRegisteredPersons; ++i)
	{
		free(s->personArray[i].Name);
	}
}


void freeLastNames(db_mgr* s)        //free the last name of all the people in the array
{
	for (int i = 0; i < s->numOfRegisteredPersons; ++i)
	{
		free(s->personArray[i].Family);
	}
}


void freeChildren(db_mgr* s)        //free the childrenPtr of all the people in the array if exist
{
	for (int i = 0; i < s->numOfRegisteredPersons; ++i)
	{

		if (s->personArray->NumOfChildren > 0)
		{
			free(s->personArray[i].ChildrenPtr);
		}
	}
}


void quit(db_mgr* s)    //quit the program and free all dynamic memory we used

{
	printf("\n\n\nThe program ended! have a good day!\n\n");

	freeAllAllocations(s);
	exit(0);
}