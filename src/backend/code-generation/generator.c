#include "../support/logger.h"
#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

/**
 * Implementación de "generator.h".
 */

 #define ALL_PERMISSIONS 0777
 // TODO: Hacer que el path sea configurable.
 #define OUTPUT_DIR "output"

FILE * outputFile;

const char * getFileName(const char * path) {
	const char * fileName = strrchr(path, '/');
	if (fileName == NULL) {
		fileName = path;
	} else {
		fileName++;
	}
	return fileName;
}

void generate(Program * program, const char * inputFile) {
	LogInfo("Generando codigo...");
	mkdir("output", ALL_PERMISSIONS);
	char outputPath[1024];
	const char * fileName = getFileName(inputFile);
	sprintf(outputPath, "%s/%s", OUTPUT_DIR, fileName);
	outputFile = fopen(outputPath, "w");
	LogInfo("Generando codigo en %s", outputPath);
	fprintf(outputFile, "// output of %s\n", fileName);

	ProgramGenerate(program);

	LogInfo("Generacion de codigo finalizada.");	
	fclose(outputFile);
}

