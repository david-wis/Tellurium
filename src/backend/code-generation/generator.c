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

#define TEMPLATE_PATH "src/backend/domain-specific/template.js"
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

void generate(Program * program, const char * inputFile, const char * outputDir) {
	LogInfo("Generando codigo...");
	mkdir("output", ALL_PERMISSIONS);
	char outputPath[1024];
	const char * fileName = getFileName(inputFile);
	sprintf(outputPath, "%s/%s", outputDir, fileName);
	outputFile = fopen(outputPath, "w");
	LogInfo("Generando codigo en %s", outputPath);
	
	fprintf(outputFile, "// output of %s\n", fileName);
	
	// copy content of template
	FILE * templateFile = fopen(TEMPLATE_PATH, "r"); 
	if (templateFile == NULL) {
		LogError("No se pudo abrir el archivo de template.");
		exit(1);
	}
	char c;
	while ((c = fgetc(templateFile)) != EOF) {
		fputc(c, outputFile);
	}

	ProgramGenerate(program);
	fputc('}', outputFile);

	LogInfo("Generacion de codigo finalizada.");	
	fclose(outputFile);
}