/* 
 * Wrapper for ethtool to flash Microchip LAN7800 EEPROM
 */

/*
 * Includes
 */
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

/*
 * Global variables and compiler definitions
 */
#define magicNumber "0x78A5"
#define fileName "LAN7800.hex"

/*
 * Function prototypes 
 */
int getValidByte(char*);
int flash(char*);
int flash1(char*, char*);
int check(char*);

int main(int argc, char *argv[])
{
    int status=1, ret=0;
    bool permission=false;
    char response;

    // Check argument list validity
    if (argc>4) {
        ret = E2BIG;
        printf("\nUsage - %s *interface* *MAC(optional)*\n",argv[0]);
        goto end;
    }
    if (argc<2) {
        ret = EINVAL;
        goto end;
    }

    status = getValidByte(argv[1]);

    if (status < 0){
        ret = status;
        goto end;
    } else if (status) {
        printf("\nAre you sure you want to rewrite valid EEPROM data?(y/N)");
        scanf("%c",&response);
        if (response == 'y' || response == 'Y') {
            permission = true;
        } else {
            permission = false;
        }
    } else {
        permission = true;
    }
    
    if (permission) {
        if (argc == 3) {
            status = flash1(argv[1], argv[2]);
            printf("\nNot implemented yet\n");
            ret = ENOSYS;
            goto end;
        } else if (argc == 2) {
            status = flash(argv[1]);
        }
        
    }
end:
    return ret;
}

int getValidByte(char *iface)
{
    char cmd[45] = "ethtool -e ";
    strcat(cmd, iface);
    strcat(cmd, " raw on offset 0 length 512");
    char* rawDump =(char*)malloc(512*sizeof(char));
    if (rawDump == NULL) {
        return ENOMEM;
    }
    rawDump = system(cmd);
    char firstByte = rawDump[0];
    free(rawDump);
    if (firstByte == 0xA5) {
        return 1;
    }
    return 0;
}

int flash(char *iface)
{
    char cmd[75] = "ethtool -E ";
    strcat(cmd, iface);
    strcat(cmd, " magic 0x78A5 offset 0 length 512 < ");
    strcat(cmd, fileName);
    system(cmd);
    return check(iface);
}

int flash1(char *iface, char *Mac)
{
    // Modify original hex file with new MAC, and call flash
}