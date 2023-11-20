#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "backend/support/linkedListADT.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include <stdio.h>

// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.succeed = false;
	state.elementsToFree = createLinkedListADT();
	state.modules = createLinkedListADT();
	state.errorMessages = createLinkedListADT();
	state.afterAll = NULL;
	state.beforeAll = NULL;

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}

	if (argumentCount < 2) {
		LogError("No se especifico el archivo de entrada.");
		return -1;
	}

	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
	const int result = yyparse();
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("La compilacion fue exitosa.");
				char * outputDir = argumentCount == 2? "output" : (char *) arguments[2];
				generate(state.program, arguments[1], outputDir);
			}
			else {
				LogError("Se produjo un error en la aplicacion.");
				begin(state.errorMessages);
				while (hasNext(state.errorMessages))
					LogError("%s", (char *) next(state.errorMessages));
				return -1;
			}
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).", result);
	}
	

	freeLinkedListADT(state.modules);
	freeLinkedListADTDeep(state.elementsToFree);
	freeLinkedListADTDeep(state.errorMessages);

	LogInfo("Fin.");
	return result;
}
