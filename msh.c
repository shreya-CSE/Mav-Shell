// The MIT License (MIT)
//
// Copyright (c) 2016 Trevor Bakker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*
    Name: Shreya Chindepalli
    ID: 1001845703
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 11     // Mav shell only supports four arguments

#define MAX_HISTORY_COUNT 15     // Maximum commands stored and shown as  history

#define MAX_NUM_CHILD_PIDS 100     // Number of Child PIDs to track


void utils_mem_free(char **tokens, int token_cnt);
void utils_change_dir(char *dir_path);

// Function Name: utils_mem_free
//
// arg1: char **tokens : array of strings for which memory to be freed
// arg2: int token_cnt : number of elements in the tokens string array
//
// return value: no return value (void)
//
// Description: Functional will loop through and free the memory of each string
// in the array.
//
void utils_mem_free(char **tokens, int token_cnt)
{
  int cnt;

  for(cnt=0; cnt<token_cnt; cnt++)
  {
    if(tokens[cnt] != NULL)
    {
      free(tokens[cnt]);
    }
  }
  return;
}

// Function Name: utils_change_dir
//
// arg1: char *dir_path : target directory path to change to from current path
//
// return value: no return value (void)
//
// Description: Function will take directory path as input and using chdir()
// function will change the directory to input directory. This utility is to
// implement cd, cd .. and ch <path> requirements.
//
void utils_change_dir(char *dir_path)
{
  int ret_cd = 0;
  ret_cd = chdir(dir_path);
	if (ret_cd == -1) {
    printf("cd %s error\n", dir_path);
	}
}

int main()
{

  char *command_string = (char*) malloc( MAX_COMMAND_SIZE );
  int child_pids[MAX_NUM_CHILD_PIDS];
  int child_pids_count=0;
  char command_list[MAX_HISTORY_COUNT][MAX_COMMAND_SIZE];
  int command_count = 0;
  int cnt;
  char *token[MAX_NUM_ARGUMENTS];
  int token_count = 0;


  while( 1 )
  {
    token_count = 0;

    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    int hist_num=-1;  //capture command history number

    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr=NULL;

    //
    //remove new line character
    //
    if(command_string[strlen(command_string) - 1] == '\n')
    {
       command_string[strlen(command_string) - 1] = '\0';
    }

    //
    //blank input
    //shell will print another prompt
    if(strlen(command_string) == 0)
    {
      continue;
    }

    //
    //remove any end of line blank spaces
    //
    while(command_string[strlen(command_string) - 1] == ' ')
    {
      command_string[strlen(command_string) - 1] = '\0';
    }

    //code to handle history commands
    //history commands starts with exclamation (!) mark followed by history
    //number
    //
    if((command_string[0]=='!'))
    {
      if((strlen(command_string)==2)&&(isdigit(command_string[1])))
      {
        hist_num = atoi((command_string + 1));
      }

      if((strlen(command_string)==3)&&isdigit(command_string[1])&&isdigit(command_string[2]))
      {
        hist_num = atoi((command_string + 1));
      }

      if(hist_num >-1 && hist_num < command_count && hist_num < MAX_HISTORY_COUNT)
      {
        strcpy(command_string, command_list[hist_num]);
      }
      else
      {
        printf("Command not in history.\n");
      }
    }

    char *working_string  = strdup( command_string );


    // we are going to move the working_string pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *head_ptr = working_string;
    char *quit_ptr = "quit";
    char *exit_ptr = "exit";

    for(cnt=0; cnt<MAX_NUM_ARGUMENTS; cnt++)
    {
      token[cnt] = NULL;
    }

    // Tokenize the input strings with whitespace used as the delimiter
    while (((argument_ptr=strsep(&working_string, WHITESPACE)) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    free( working_string );
    free( head_ptr );

    if (token[0] != NULL)
    {
    	if ((strcmp(token[0], quit_ptr) == 0) || (strcmp(token[0], exit_ptr) == 0)) {
        free(command_string);
        utils_mem_free(token, MAX_NUM_ARGUMENTS);
	      return 0;
    	}

      // Add input command to command_list for history tracking
    	if (command_count == MAX_HISTORY_COUNT)
      {
	      int c = 0;
	      for (c = 0; c < MAX_HISTORY_COUNT - 1; c++) {
	    	  strcpy(command_list[c], command_list[c+1]);
	      }
	      strcpy(command_list[command_count - 1], command_string);
      }
    	else
      {
	      strcpy(command_list[command_count], command_string);
    	  command_count++;
    	}

      // print command history stored in command_list
      // Limit to MAX_HISOTRY_COUNT
      if(strcmp(token[0], "history") == 0)
      {
	      int k = 0;
	      for (k = 0; k < command_count; k++) {
		        if (command_list[k] != NULL) {
		        printf("%d: %s\n", k, command_list[k]);
		        }
	      }
        utils_mem_free(token, MAX_NUM_ARGUMENTS);
        continue;
	    }

      int ret_cd = 0;
    	if ((strcmp(token[0], "cd") == 0) && (token[1] ==  NULL))
      {
        //get home directory of logged in user from env
        //this will be used to change directory to home directory
        char *home_dir=getenv("HOME");

        utils_change_dir(home_dir);
        utils_mem_free(token, MAX_NUM_ARGUMENTS);
        continue;
      }
      else if ((strcmp(token[0], "cd") == 0) && (strcmp(token[1], "..") == 0))
      {
        utils_change_dir("..");
        utils_mem_free(token, MAX_NUM_ARGUMENTS);
        continue;
      }
      else if (strcmp(token[0], "cd") == 0)
      {
        utils_change_dir(token[1]);
        utils_mem_free(token, MAX_NUM_ARGUMENTS);
        continue;
      }

      if ((strcmp(token[0], "showpids") == 0) && (token[1] == NULL))
      {
        if(child_pids_count <= 0)
        {
          printf("No child processes forked\n");
        }
        else
        {
	        int i = 0;
          for(i=0; i<child_pids_count && i<15; i++)
          {
            printf("%d: %d\n", i, child_pids[i]);
          }
        }
        utils_mem_free(token, MAX_NUM_ARGUMENTS);
        continue;
      }

    }

    //new child process
    //
    pid_t pid = fork();
    int ret = 0;
    if (pid == -1)
    {
	    perror("Fork failed: ");
	    exit( EXIT_FAILURE );
    }
    else if (pid == 0)
    {
	    // child process to for shell like function
	    // TODO: exit if command count is greater than 10
	    int case_ = 0;
	    char command[100];

	    if (case_ == 0)
      {
		   // current working directory
		   strcpy(command, "./");
		   strcat(command, token[0]);
		   ret = execl(command, token[0], token[1], token[2],token[3], token[4], token[5], token[6], token[7], token[8], token[9], token[10], NULL);
		   if (ret == -1) {
		     case_++;
		   }
	   }

	   if (case_ == 1)
     {
		   // /usr/local/bin
		   strcpy(command, "/usr/local/bin/");
		   strcat(command, token[0]);
		   //ret = execl(command, token[0], token[1], token[2],NULL);
		   ret = execl(command, token[0], token[1], token[2],token[3], token[4], token[5], token[6], token[7], token[8], token[9], token[10], NULL);

		   if (ret == -1) {
		      case_++;
		   }
	   }

	   if (case_ == 2)
     {
		   // /usr/bin
		   strcpy(command, "/usr/bin/");
		   strcat(command, token[0]);
		   //ret = execl(command, token[0], token[1], token[2],NULL);
		   ret = execl(command, token[0], token[1], token[2],token[3], token[4], token[5], token[6], token[7], token[8], token[9], token[10], NULL);
		   if (ret == -1) {
		      case_++;
		   }
	   }

	   if (case_ == 3)
     {
		   // /bin
		   strcpy(command, "/bin/");
		   strcat(command, token[0]);
		   //ret = execl(command, token[0], token[1], token[2],NULL);
		   ret = execl(command, token[0], token[1], token[2],token[3], token[4], token[5], token[6], token[7], token[8], token[9], token[10], NULL);
		   if (ret == -1) {
		       case_++;
		   }
	   }
	   if (ret == -1)
     {
		   if (strcmp(token[0], "history") == 0)
       {
		      // continue;
		   }
		   else
       {
	        printf("%s: Command not found.\n", token[0]);
		   }
	   }

     //free up allocated memory for the token string array
     int cnt1;
     for(cnt1=0; cnt1<MAX_NUM_ARGUMENTS; cnt1++)
     {
       if(token[cnt1] != NULL)
       {
         free(token[cnt1]);
       }
     }
     free(command_string);
	   exit( EXIT_SUCCESS );
   }
   else
   {
	   // parent process
	   int status;

     //tracking child PIDs
     child_pids[child_pids_count]= pid;
     child_pids_count++;

	   waitpid(pid, &status, 0);

     utils_mem_free(token, MAX_NUM_ARGUMENTS);
   }
  }

  free(command_string);
  return 0;
}
