/* Preprocessor Directives */
#include <iostream>  // cin, cout, endl
#include <string.h>  // strcpy()
#include <stdio.h>  // fgets()
#define ROWS 100
#define COLS 100

using namespace std;  // using the standard namespace

struct contact
{
	char fnames[100];  // array to hold first name
	char lnames[100];  // array to hold last name
	char emails[100];  // array to hold email address
	char phnums[100];  // array to hold phone number
};


/* Function Prototypes */
void add_data( contact [], int &cnt );
void read_contacts(int&,int&,int&,int&);

/* Functions */
/*****************************************************************
 * Function    | main()
 * Description | Main thread of execution, used for examples 
 *                performed in class
 * Inputs      | None
 * Output      | Integer value 0
 *****************************************************************/            
int main(int argc, char *argv[])
{
	contact contacts[100];
	char new_contact;
	char line[COLS];
	int in_cnt = 0, out_cnt, file_limit; // in_cnt:how many i read from file
	
	FILE *file_ptr;
	
	/* Read command line information */
	

	file_ptr = fopen("contacts.txt", "r");  // open the contacts.txt file for 
										   //  reading only
	if( file_ptr == NULL ) // failed to open file
	{
		cout << "ERROR OPENING FILE" << endl;
		exit( 0 );  // exit program
	}

	/* Read file line-by-line, storing each line into a seperate row in */
	/*  the names[][] string array									    */
	while( fgets( line, COLS, file_ptr) != NULL ) //no infromation i got from file.
	{
		printf("%s\n",line);
		sscanf(line, "%s\t%s\t%s\t%s", contacts[in_cnt].fnames, contacts[in_cnt].lnames, contacts[in_cnt].emails, contacts[in_cnt].phnums);
        in_cnt++;  // increment to next row in first and last name arrays 
	}
	file_limit = in_cnt;  // store location of any new data entered from user
	fclose(file_ptr);  //close file since all data has been extracted

	/* Create new contacts if desired */
	cout << "Would you like to add a new contact (Y/N): ";
	cin >> new_contact;
	

	while(new_contact == 'Y' || new_contact == 'y')
	{
		add_data(contacts, in_cnt);  // call add_data() to enter new 
										   //  contact information
		cout << "Would you like to add a another new contact (Y/N): ";
		cin >> new_contact;
	}

	if(in_cnt != file_limit) //check for new data added
	{
		file_ptr = fopen("contacts.txt", "a");  // open the contacts.txt file for 
												//  appending new information
		if( file_ptr == NULL ) // failed to open file
		{
			cout << "ERROR OPENING FILE" << endl;
			exit( 0 );  // exit program
		}
		fputc('\n', file_ptr);
		/* create new contact line for file */
		for( out_cnt = file_limit; out_cnt < in_cnt; out_cnt++ )
		{
			strcpy(line, contacts[out_cnt].fnames);
			strcat(line, "\t");
			strcat(line, contacts[out_cnt].lnames);
			strcat(line, "\t");
			strcat(line, contacts[out_cnt].emails);
			strcat(line, "\t");
			strcat(line, contacts[out_cnt].phnums);
			strcat(line, "\n");

			/* Update file with added contacts */
			fputs(line, file_ptr);  // write new contact information to file
		}
	}
	for( out_cnt = 0; out_cnt < in_cnt; out_cnt++ )
	{
	printf("%s\t%s\t%s\t%s\n", contacts[out_cnt].fnames, contacts[out_cnt].lnames, contacts[out_cnt].emails, contacts[out_cnt].phnums);
	}
	return 0;
	
}


void add_data( contact contacts[], int &cnt )
{
	cout << "Enter new FIRST name: ";
	cin >> contacts[cnt].fnames;  // store new first name
	cout << "Enter new LAST name: ";
	cin >> contacts[cnt].lnames;  // store new last name
	cout << "Enter new email address: ";
	cin >> contacts[cnt].emails;  // store new last name
	cout << "Enter new phone number: ";
	cin >> contacts[cnt].phnums;  // store new last name

	cnt++;  // update to next row in local storage arrys

	return;
}

