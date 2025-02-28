
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
openlog(NULL,0,LOG_USER);
if(argc != 3)
{
 syslog(LOG_ERR,"Invalid number of arguments(not 2), it is %d", argc);
 return 1;
}
else {
FILE *file = fopen(argv[1],"w+");
if(file == NULL) {
 syslog(LOG_ERR,"Error opening file");
}
else {
 fputs(argv[2],file);
// syslog(LOG_DEBUG,"Writing <%s> to <%s>",argv[1],argv[2]);
}
}
return 0;
}
