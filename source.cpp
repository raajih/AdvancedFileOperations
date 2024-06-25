//Raajih Roland
//CIS 1202.5T1
//June 25, 2024

#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;

//Struct
const int NAME_SIZE = 40;
struct Product
{
	long number;
	char name[NAME_SIZE];
	double price;
	int quantity;
};


//Fuction prototypes
int showMenu();
void createFile(fstream&);
void displayFile(fstream&);
void displayRecord(fstream&, int);
void modifyRecord(fstream&);

int main()
{
	int userChoice;
	int recNum;
	fstream dataFile;
	
	createFile(dataFile);

	do
	{
		userChoice = showMenu();

		switch (userChoice)
		{
		case 1:
			displayFile(dataFile);
			break;
		case 2:
			cout << "Enter record number of product: ";
			cin >> recNum;
			cin.ignore();
			displayRecord(dataFile, recNum);
			break;
		case 3:
			modifyRecord(dataFile);
			break;
		}
	} while (userChoice != 4);
	


	return 0;
}

int showMenu()
{
	int choice;
	cout << "1. Display the entire inventory\n"
		<< "2. Display a particular product\n"
		<< "3. Modify a product\n"
		<< "4. Exit program\n"
		<< "Enter choice (1-4): ";
	cin >> choice;
	
	while (choice < 1 || choice > 4)//Validate option
	{
		cout << "ERROR. INVALID CHOICE. Please enter valid menu option (1-4): ";
		cin >> choice;
	}

	return choice;
}

void createFile(fstream& dataFile)
{
	Product a, b, c;//three initial products

	//Create practice file
	dataFile.open("practice.txt", ios::out | ios::binary);

	//Initialize a with random values
	a.number = 1111;
	strcpy_s(a.name, "Random");
	a.price = 1000;
	a.quantity = 1;
	dataFile.write(reinterpret_cast<char*>(&a), sizeof(a));//write product a to file

	//Initialize b
	b.number = 124;
	strcpy_s(b.name, "Name");
	b.price = 20;
	b.quantity = 5;
	dataFile.write(reinterpret_cast<char*>(&b), sizeof(b));//write product b to file

	//Now c
	c.number = 24;
	strcpy_s(c.name, "Cat");
	c.price = 99;
	c.quantity = 0;
	dataFile.write(reinterpret_cast<char*>(&c), sizeof(c));//write product c to file

	dataFile.close();
}

void displayFile(fstream &dataFile)
{
	Product temp;
	int recNum = 0;
	
	dataFile.clear();//Start by clearing any bits
	dataFile.seekg(0L, ios::beg);//Start at beginning
	dataFile.open("practice.txt", ios::in | ios::binary);

	//Make sure file opens
	if (dataFile)
	{
		dataFile.read(reinterpret_cast<char*>(&temp), sizeof(temp));//Read first record from file
		while (!dataFile.eof())//Make sure we're not at the end of the file
		{
			cout << "Record number " << recNum << endl;
			cout << "Name: ";
			cout << temp.name << endl;
			cout << "Product number: ";
			cout << temp.number << endl;
			cout << "Price: $";
			cout << temp.price << endl;
			cout << "Quantity: ";
			cout << temp.quantity << endl << endl;
			recNum++;

			dataFile.read(reinterpret_cast<char*>(&temp), sizeof(temp));//Read next product
		}

		cout << "That's all the data in the file!\n";
	}
	else
		cout << "Error opening file.";

	dataFile.close();
	
	
}

void displayRecord(fstream &dataFile, int recNum)
{
	dataFile.clear();//Start by clearing any bits
	dataFile.open("practice.txt", ios::in | ios::binary);
	Product temp;
	long pos;

	if (!dataFile)
	{
		cout << "ERROR. Couldn't open file.";
	}
	else
	{
		//Find out how many records exist
		dataFile.seekg(0, ios::end);
		pos = dataFile.tellg();
		auto numRecords = pos / (sizeof(Product));
		
		//Validate recNum
		while (recNum < 0 || recNum >(numRecords - 1))
		{
			cout << "Error invalid record number. Please try again: ";
			cin >> recNum;
		}
		{
			dataFile.seekg(sizeof(Product) * recNum, ios::beg);//Go to the correct record number
			dataFile.read(reinterpret_cast<char*>(&temp), sizeof(temp));//Read in correct record
			//Display record
			cout << "Record number " << recNum << endl;
			cout << "Name: ";
			cout << temp.name << endl;
			cout << "Product number: ";
			cout << temp.number << endl;
			cout << "Price: $";
			cout << temp.price << endl;
			cout << "Quantity: ";
			cout << temp.quantity << endl << endl;
		}
	}
	dataFile.close();
}

void modifyRecord(fstream& dataFile)
{
	Product temp;
	int recNum;
	long pos;
	dataFile.clear();
	dataFile.open("practice.txt", ios::in | ios::out | ios::binary);

	cout << "What is the record number you would like to modify?: ";
	cin >> recNum;
	cin.ignore();

	//Validate recNum
	dataFile.seekg(0, ios::end);
	pos = dataFile.tellg();
	auto numRecords = pos / (sizeof(Product));
	while (recNum < 0 || recNum >(numRecords - 1))
	{
		cout << "Error invalid record number. Please try again: ";
		cin >> recNum;
		cin.ignore();
	}
	dataFile.seekg(recNum * sizeof(Product), ios::beg);//Find correct record
	dataFile.read(reinterpret_cast<char*>(&temp), sizeof(temp));//Read correct record into temp
	
	//Display old record
	cout << "Record number " << recNum << endl;
	cout << "Name: ";
	cout << temp.name << endl;
	cout << "Product number: ";
	cout << temp.number << endl;
	cout << "Price: $";
	cout << temp.price << endl;
	cout << "Quantity: ";
	cout << temp.quantity << endl << endl;

	//Get new record
	cout << "Enter new information \n";
	cout << "Name: ";
	cin.getline(temp.name, NAME_SIZE);
	cout << "Product Number: ";
	cin >> temp.number;
	cout << "Price: ";
	cin >> temp.price;
	cout << "Quantity: ";
	cin >> temp.quantity;

	//Input new record into file
	dataFile.seekp(recNum * sizeof(Product), ios::beg);//Find the right record for input
	dataFile.write(reinterpret_cast<char*>(&temp), sizeof(temp));//Write over record

	dataFile.close();
}