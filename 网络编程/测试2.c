#include<stdio.h>
#include<stdlib.h>
#include"cJSON.c"
int
main(void)
{
    char name[20];
    char password[30];
    char buf[50];
    scanf("%s %s",name,password);
    sprintf(buf,"{\"sign_up\":{\"name\":\"%s\",\"password\":\"%s\"}}",name,password);
    printf("%s",buf);
    char * jsonStr = buf;
    printf("%s\n",jsonStr);
    cJSON *root =NULL;
    cJSON *item = NULL;
    cJSON *next = NULL;
    root = cJSON_Parse(jsonStr);
    item =  cJSON_GetObjectItem(root,"sign_up");
    next =  cJSON_GetObjectItem(item,"name");
    printf("%s\n",next->valuestring);
    next = cJSON_GetObjectItem(item,"password");
    printf("%s\n",next->valuestring);
    return 0;
}
