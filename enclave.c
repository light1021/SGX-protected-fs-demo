#include "sgx_trts.h"

#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */

#include "enclave.h"
#include "enclave_t.h" 
#include <string.h> 
#include <ctype.h>
#include <sgx_tseal.h>
#include <sgx_utils.h>
#include <sgx_tcrypto.h>
#include <stdlib.h>
#include "sgx_tprotected_fs.h"

/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */


void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
}


void ecall_enclaveString(char *s, size_t len)
{
	const char *secret = "Hello Enclave!";
	if (len > strlen(secret))
	{
		memcpy(s, secret, strlen(secret) + 1);
	}
	else
	{
		memcpy(s, "false", strlen("false") + 1);
	}
}

SGX_FILE* ecall_file_open(const char* filename, const char* mode) 
{
	SGX_FILE* a;
	a = sgx_fopen_auto_key(filename, mode);
	return a;
}

uint64_t ecall_file_get_file_size(SGX_FILE * fp)
{
	uint64_t file_size = 0;
	sgx_fseek(fp, 0, SEEK_END);
	file_size = sgx_ftell(fp);
	return file_size;
}

size_t ecall_file_write(SGX_FILE* fp, char data[100]) 
{
	size_t sizeofWrite;
	size_t len = strlen(data);
	sizeofWrite = sgx_fwrite(data, sizeof(char), len, fp);

	/*for (int i = 0; i < 5; i++) 
	{
		char buffer[] = { 'x' , 'c' };
		sizeofWrite += sgx_fwrite(buffer, sizeof(char), sizeof(buffer), fp);
	}*/
	return sizeofWrite;
}

size_t ecall_file_read(SGX_FILE* fp, char* readData, uint64_t size) 
{
	char *data;
	uint64_t startN = 1;
	sgx_fseek(fp, 0, SEEK_END);
	uint64_t finalN = sgx_ftell(fp);
	sgx_fseek(fp, 0, SEEK_SET);
        printf("file size%d\n", finalN);
	data = (char*)malloc(sizeof(char)*finalN);
	memset(data, 0, sizeof(char)*finalN);

	size_t sizeofRead = sgx_fread(data, startN, finalN, fp);
	int len = strlen(data);
	memcpy(readData, data, sizeofRead);
	memset(readData+sizeofRead, '\0', 1);
	printf("%s\n", readData);
	return sizeofRead;
}

int32_t ecall_file_close(SGX_FILE* fp)
{
	int32_t a;
	a = sgx_fclose(fp);
	return a;
}
