/* Preprocessor Directives */
#include <windows.h>  // Win32 API
#include "resource.h"  // resources for your application

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
	char emails[100];  // array to hold last name
	char phnums[100];  // array to hold last name
};


#define ID_ADDBUTTON	1001
#define ID_LIST			1010
#define ID_EDIT			1020
#define ID_DELEBUTTON	1030

/* Function Prototypes */
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );  //the window procedure
/* A callback function is passed (by reference) to another function */

/* Global variables */
contact contacts[100];
int in_cnt=0;

/* Functions */
/*****************************************************************
 * Function    | WinMain()
 * Description | Entry point for our application, we create and 
 *             |  register a window class and then call CreateWindow
 * Inputs      | None
 * Output      | Integer value 0
 *****************************************************************/ 
int WINAPI WinMain( HINSTANCE hInstance, 
				    HINSTANCE hPrevInstance,
                    LPSTR szCmdLine, 
					int iCmdShow) 
{
	static char szAppName[] = "My Window";
	
	HWND        hwnd, lhWnd;
       WNDCLASSEX  wndclass;  // This is our new windows class
	MSG msg;

	/*  Fill in WNDCLASSEX struct members  */
       wndclass.cbSize         = sizeof(wndclass);
       wndclass.style          = CS_HREDRAW | CS_VREDRAW;
       wndclass.lpfnWndProc    = WndProc;
       wndclass.cbClsExtra     = 0;
       wndclass.cbWndExtra     = 0;
       wndclass.hInstance      = hInstance;
       wndclass.hIcon          = LoadIcon(GetModuleHandle(NULL),
                                 MAKEINTRESOURCE(IDI_MYICON));
       wndclass.hIconSm        = (HICON)LoadImage(GetModuleHandle(NULL),
                                 MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);
       wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
       wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
       wndclass.lpszClassName  = szAppName;
       wndclass.lpszMenuName   = NULL;

    /*  Register a new window class with Windows  */
    RegisterClassEx(&wndclass);

    /*  Create a window based on our new class  */
    hwnd = CreateWindow(szAppName,  // class name the window is to be based on 
			   "My 1st Window",  // the title of the window that will 
					       //  appear in the bar at the top
		          WS_OVERLAPPEDWINDOW,  // window style (a window that 
						    //  has a caption, a system menu, 
						    //  a thick frame and a minimise 
						    //  and maximise box)
			/* Use default starting location for window */
			CW_USEDEFAULT, // initial x position (top left corner)
			CW_USEDEFAULT, // initial y position (top left corner)
			850, // initial width
			440, // initial height
			NULL, // window parent (NULL for not a child window)
			NULL, // menu (NULL to use class menu)
			hInstance, // the program instance passed to us
			NULL);  // pointer to any parameters wished to be 
	                     //  passed to the window producer when the window
	                     //  is created
 
	if(hwnd == NULL)  // check to see if an error occurred in creating the window
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    /* Show and update our window  */
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

 
    /*  Retrieve and process any queued messages until we get WM_QUIT  */
    /* Recall that Windows uses a messaging system to notify window of */
    /*  user actions												   */
    while ( GetMessage(&msg, NULL, 0, 0) ) 
    {
	TranslateMessage(&msg);  // for certain keyboard messages 
	DispatchMessage(&msg);  // send message to WndProc
    } 

    /*  Exit with status specified in WM_QUIT message  */
    return msg.wParam;
} // end WinMain()


/*****************************************************************
 * Function    | WinProc()
 * Description | Whenever anything happens to your window, Windows 
 *             |  will call this function telling you what has happened. 
 *             |  The message parameter contains the message sent
 * Inputs      | None
 * Output      | Integer value 0
 *****************************************************************/ 
LRESULT CALLBACK WndProc( HWND hwnd, 
			     UINT iMsg,
			     WPARAM wParam, 
			     LPARAM lParam) 
{
    PAINTSTRUCT ps;
	HDC hdc;
	static HWND  addbutton, deletebutton, listbox, editbox_fnames, editbox_lnames,
              editbox_emails, editbox_phnums;
	
	int len;
	char line[COLS];
	int out_cnt;
	int del_idx;

	FILE *file_ptr;

	/*  Switch according to what type of message we have received  */
       switch ( iMsg ) 
	{
		case WM_PAINT:
		/* We receive WM_PAINT every time window is updated  */
			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 10, 350, "My First Window", 15);
			TextOut(hdc, 10, 133, "First Name", 10);
			TextOut(hdc, 10, 173, "Last Name", 9);
			TextOut(hdc, 10, 213, "Email Address", 13);
			TextOut(hdc, 10, 253, "Phone Number", 12);

			EndPaint(hwnd, &ps);
			break;
		case WM_CREATE:
              /* Operations to be performed whenthis window is created */ 
			/* Create a child window for a pushbutton */
			addbutton = CreateWindow(  "BUTTON",  // predefined class 
						"ADD",  // button text 
						WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
						// Size and position values are given 
						//  explicitly, because the CW_USEDEFAULT 
						//  constant gives zero values for buttons. 
						10,  // starting x position 
						40,  // starting y position 
						100, // button width 
						30,  // button height 
						hwnd, // parent window 
						(HMENU)ID_ADDBUTTON, // no menu 
						(HINSTANCE) 0,  // ignored for Windows XP
						NULL );  // pointer not needed 

			deletebutton = CreateWindow(  "BUTTON",  // predefined class 
						"DELETE",
						WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
						140,
						40,
						100,
						30,
						hwnd,
						(HMENU)ID_DELEBUTTON,
						(HINSTANCE) 0,
						NULL );  // pointer not needed 

			
			/* Create a list box to display values within the window */
			listbox = CreateWindow( "LISTBOX",  // predefined class
				  		"",  // initial lst box text (empty)
						// automatically sort items in list box 
						//  and include a vertical scroll bar 
						//  with a border
						WS_CHILD | WS_VISIBLE | LBS_NOTIFY | 
						WS_VSCROLL | WS_BORDER,  
						270,  // starting x position
						10,  // starting y position
						550,  // list box width
						400,  // list box height
						hwnd,  // parent window
						(HMENU)ID_LIST,  // no menu
						(HINSTANCE) 0,  // ignored for Windows XP
						NULL ); // pointer not needed

			/* Create edit box for adding and deleting items from list */
			editbox_fnames = CreateWindow( "EDIT",  // predefined class
				               "",  // initial lst box text (empty)
						// create an edit box with a border
						WS_CHILD | WS_VISIBLE | WS_BORDER, 
						10,  // starting x position
						150,  // starting y position
						250,  // list box width
						20,  // list box height
						hwnd,  // parent window
						(HMENU)ID_EDIT,  // no menu
						(HINSTANCE) 0,  // ignored for Windows XP
						NULL); // pointer not needed

			editbox_lnames = CreateWindow( "EDIT",  // predefined class
				             "",  // initial lst box text (empty)
						// create an edit box with a border
						WS_CHILD | WS_VISIBLE | WS_BORDER, 
						10,  // starting x position
						190,  // starting y position
						250,  // list box width
						20,  // list box height
						hwnd,  // parent window
						(HMENU)ID_EDIT,  // no menu
						(HINSTANCE) 0,  // ignored for Windows XP
						NULL); // pointer not needed

			editbox_emails = CreateWindow( "EDIT",  // predefined class
				              "",  // initial lst box text (empty)
						// create an edit box with a border
						WS_CHILD | WS_VISIBLE | WS_BORDER, 
						10,  // starting x position
						230,  // starting y position
						250,  // list box width
						20,  // list box height
						hwnd,  // parent window
						(HMENU)ID_EDIT,  // no menu
						(HINSTANCE) 0,  // ignored for Windows XP
						NULL); // pointer not needed

			editbox_phnums = CreateWindow( "EDIT",  // predefined class
				              "",  // initial lst box text (empty)
						// create an edit box with a border
						WS_CHILD | WS_VISIBLE | WS_BORDER, 
						10,  // starting x position
						270,  // starting y position
						250,  // list box width
						20,  // list box height
						hwnd,  // parent window
						(HMENU)ID_EDIT,  // no menu
						(HINSTANCE) 0,  // ignored for Windows XP
						NULL); // pointer not needed

		/* Read command line information */
		file_ptr = fopen("contacts.txt", "r");  // open the contacts.txt file for 
						   //  reading only
		if( file_ptr == NULL ) // failed to open file
		{
			MessageBox(NULL, "Error Opening File", "ERROR",
					   MB_ICONINFORMATION | MB_OK);
		}

		/* Read file line-by-line, storing each line into a seperate row in */
		/*  the names[][] string array					      */
		in_cnt;
		while( fgets( line, COLS, file_ptr) != NULL ) //no infromation i got from 
                                                             file.
		{
			sscanf(line, "%s\t%s\t%s\t%s", contacts[in_cnt].fnames, 
                           contacts[in_cnt].lnames, contacts[in_cnt].emails, 
                           contacts[in_cnt].phnums);
			sprintf(line, "%s  %s  %s  %s", contacts[in_cnt].fnames, 
                           contacts[in_cnt].lnames, contacts[in_cnt].emails, 
                           contacts[in_cnt].phnums);
			SendDlgItemMessage( hwnd, ID_LIST, 
						      LB_ADDSTRING, 0, 
				                   (LPARAM)line);
			in_cnt++;
		}
		fclose(file_ptr);  //close file since all data has been extracted		
		break;
	case WM_DESTROY:
	/* Window has been destroyed, so exit cleanly  */

		/* Read command line information */
		file_ptr = fopen("contacts.txt", "w");  // open the contacts.txt file for 
							//  writing only
		if( file_ptr == NULL ) // failed to open file
		{
			MessageBox(NULL, "Error Opening File", "ERROR",
					   MB_ICONINFORMATION | MB_OK);
		}
			
		/* create new contact line for file */
		for( out_cnt = 0; out_cnt < in_cnt; out_cnt++ )
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
			
		PostQuitMessage(0);

		break;
	case WM_COMMAND:
       /* User selected a command from a menu or a control sent a message */
           if (HIWORD(wParam) == BN_CLICKED) 
           { 
               switch (LOWORD(wParam)) 
               { 
		    case ID_ADDBUTTON:
			MessageBox(NULL, "Add Button Pressed", "ADD",
				   MB_ICONINFORMATION | MB_OK);
			/* Read length of string entered into the text box */
			len = GetWindowTextLength(editbox_fnames);
			len = GetWindowTextLength(editbox_lnames);
			len = GetWindowTextLength(editbox_emails);
			len = GetWindowTextLength(editbox_phnums);
			/* Read string from edit box and store to name[] */
			GetWindowText(editbox_fnames, contacts[in_cnt].fnames, len + 20);
			GetWindowText(editbox_lnames, contacts[in_cnt].lnames, len + 20);
			GetWindowText(editbox_emails, contacts[in_cnt].emails, len + 20);
			GetWindowText(editbox_phnums, contacts[in_cnt].phnums, len + 20);
			sprintf(line, "%s  %s  %s  %s", contacts[in_cnt].fnames, 
                             contacts[in_cnt].lnames, contacts[in_cnt].emails, 
                             contacts[in_cnt].phnums);
			in_cnt++;
			SendDlgItemMessage( hwnd, ID_LIST, 
      			                    LB_ADDSTRING, 0, 
						(LPARAM)line);
			break; 
		    case ID_DELEBUTTON:
			MessageBox(NULL, "Delete Button Pressed", "DELETE",
				   MB_ICONINFORMATION | MB_OK);						
			del_idx = SendDlgItemMessage( hwnd, ID_LIST, 
				                      LB_GETCURSEL, 
					              (WPARAM) 0, 0);
			SendDlgItemMessage( hwnd, ID_LIST, 
				            LB_DELETESTRING, 
					     (WPARAM) del_idx, 0);		    
			// update local array of structures
			for( out_cnt = del_idx; out_cnt < in_cnt; out_cnt++ )
			{
				contacts[out_cnt] = contacts[out_cnt+1];
			}
			in_cnt--;
			break; 
		  }
  }
  break;
  default:
  /* We do not want to handle this message so pass back to Windows */
  /*  to handle it in a default way                                */
      return DefWindowProc(hwnd, iMsg, wParam, lParam);
  }
  return 0;

} // end WndProc
