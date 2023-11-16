#include "shared.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>
#define BUFFSIZE 1024

void * gcCalloc(size_t size) {
	void * element = calloc(1, size);
	if (element == NULL) {
		LogError("No se pudo reservar memoria");
	}
	appendElement(state.elementsToFree, element);
	return element;
}

void addError(const char * msg) {
    char buffer[BUFFSIZE] = {0};
    sprintf(buffer, "%d: %s", yylineno, msg);
    char * message = gcCalloc(strlen(buffer) + 1);
    strcpy(message, buffer);
    appendElement(state.errorMessages, message);
}