#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main()
{
  char userinput;
  //line below: set the last character as the end of the buffer to signify end of buffer space
  static char buffer[BUFFER_SIZE] = {'\0'};
  //prompt user input (no input quite yet)
  /*printf("press 'r' to read, 's' to seek, or 'w' to write. If you want to exit, "
  "press 'e'.\n");*/
  int fileStruct = open("/dev/simple_character_device", O_RDWR);
  printf("press 'r' to read, 's' to seek, or 'w' to write. If you want to exit, "
  "press 'e'.\n");
  while(1)
  {
  	printf("enter command. \n");
    scanf("%c", &userinput);
    switch (userinput)
      {
      case 'r':
	{
	  int readbytes = 0;
	  printf("enter number of bytes to read. \n");
	  scanf("%d", &readbytes);
	  //printf("error message %d\n", error);
	  //char* buff = (char*)malloc(sizeof(char) * readbytes);
	  short count = read(fileStruct, buffer, readbytes);
	  printf("data read from device: %s\n", buffer);
	  //free the array
	  memset(buffer, 0, sizeof(buffer));
	  //free(buff);
	  //we dont want to include pressing "enter" as a character.
	  while (getchar() != '\n');
	  break;
	}
      case 'w':
	{
	  int byteswrite;
	  short error = 0;
	  printf("enter the data you want to write in. \n");
	  //char* buf = malloc(BUFFER_SIZE);
	  scanf("%s", buffer);
	  byteswrite = strlen(buffer);
	  write(fileStruct, buffer, byteswrite);
	  printf("data from buffer %s\n", buffer);
	  printf("number of bytes written %d\n", byteswrite);
	 // free(buf);
	  memset(buffer, 0, sizeof(buffer));
	  while(getchar() != '\n');
	  break;
	}
      case 's':
	{
	  int offset_int = 0;
	  printf("enter offset value \n");
	  scanf("%d", &offset_int);
	  int whence = 0;
	  printf("enter whence value \n");
	  scanf("%d", &whence);
	  llseek(fileStruct, offset_int, whence);
	  while(getchar() != '\n');
	  break;

	}
      case 'e':
	return 0;
	break;
      default:
	break;
      }
    
  } 
  close(fileStruct);
  return 0;
}
