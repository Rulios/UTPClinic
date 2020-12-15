#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> /* setlocale */
#include "p_salidas.c"
#include "Dia.c" /*
	struct Dia
	struct Dia * ObtenerDias(int * nDias)
	int ModificarDias(struct Dia dias[], int nDias)
*/


#define MAX_S 256

void IniciarColores();
void Tabulaciones(int tab);
void SaltoDeLinea(int saltos);
void BorrarSaltoDeLinea(char texto[]);

void EntradaTexto(char * destino);
void EntradaEntero(int * destino);
void EntradaDeOpcion(int *opcion, int nElementos);

void MostrarError(char texto[MAX_S]);

int ListaYSeleccionMultiple(int nElementos, char elementos[][MAX_S]);

void Titulo();
void Subtitulo(char texto [MAX_S]);

void MenuPrincipal();
void MenuNuevaCita();

int main(){
	
	setlocale(LC_ALL, "spanish"); /* Permite imprimir caracteres con tilde */
	//OBLIGATORIO, SE DEBE INICIALIZAR EL PROGRAMA PRINCIPAL 
	//PARA HACER USO DE COLORES


	printf("%20s", "|");
	
	SaltoDeLinea(4);
	
	IniciarColores();
	MenuPrincipal();
	return 0;
}



void MenuPrincipal(){
	char selecciones[3][MAX_S];
	int opcion = 0;
	
	//incializar las opciones
	sprintf(selecciones[0], "%s", "Añadir nueva cita");
	sprintf(selecciones[1], "%s", "Verificar citas reservadas");
	sprintf(selecciones[2], "%s", "Salir");
	
	Titulo();
	
	//imprimir la lista para la selección multiple
	opcion = ListaYSeleccionMultiple(3, selecciones);
	
	switch(opcion){
		case 1:
			system("cls");
			MenuNuevaCita();
		break;
		
		case 2:
			
		break;
		
		case 3: 
			return;
		break;
	}
	
}

void MenuNuevaCita(){
	int nDias = 0;
	struct Dia * dias = ObtenerDias(&nDias);
	
	Titulo();
	Subtitulo("MENÚ PARA AÑADIR UNA CITA");
	
	

}



void Titulo(){
	//Título principal
	Tabulaciones(4);
	print_cyan("CLÍNICA UNIVERSITARIA DE LA UTP");
	SaltoDeLinea(2);
}

void Subtitulo(char texto [MAX_S]){
	Tabulaciones(4);
	print_bright_magenta(texto);
	SaltoDeLinea(2);
}

int ListaYSeleccionMultiple(int nElementos, char elementos[][MAX_S]){
	//LAS LISTAS MOSTRADAS SERÁN PREDETERMINADAMENTE IMPRESAS
	//CON CARACTERES DE COLOR AMARILLO (VER REFERENCIA DE "ANSI ESCAPE CODE")
	int opcion = 0;
	
	int i = 0;
	char cadena[MAX_S];
	for(i = 0; i < nElementos; i++){
		Tabulaciones(2);
		sprintf(cadena, "%i. %s", i+1, elementos[i]);
		print_yellow(cadena);
		SaltoDeLinea(1);
	}
	
	EntradaDeOpcion(&opcion, nElementos);
	
	return opcion;
}

void MostrarError(char texto[MAX_S]){
	print_red(texto);
}

void IniciarColores(){
	system("COLOR");
}

void EntradaTexto(char * destino){
	fgets(destino, MAX_S, stdin);
	BorrarSaltoDeLinea(destino);
}

void EntradaEntero(int * destino){
	char entradaNum[MAX_S];
	//obtener entrada de caracter
	fgets(entradaNum, MAX_S, stdin);
	//borrar el salto de linea
	BorrarSaltoDeLinea(entradaNum);
	//convertir a entero 
	*destino = atoi(entradaNum);
}

void EntradaDeOpcion(int *opcion, int nElementos){ 
	//entrada de opcion después de haber mostrado una lista
	do{
		Tabulaciones(2);
		print_green("Ingrese la opción: ");
		EntradaEntero(opcion);
		if(*opcion < 1 || *opcion > nElementos){
			Tabulaciones(2);
			MostrarError("¡ERROR, DEBE SER UN NÚMERO QUE APARECE EN LA LISTA!");
			SaltoDeLinea(1);
		} 
	}while(*opcion < 1 || *opcion > nElementos); //validación de entrada
	
}

void Tabulaciones(int tab){
	int i = 0;
	for(i = 0; i< tab; i++){
		printf("\t");
	}
}

void SaltoDeLinea(int saltos){
	int i = 0;
	for(i = 0; i < saltos; i++){
		printf("\n");
	}
}
void BorrarSaltoDeLinea(char texto[]){
	texto[strlen(texto) - 1] = '\0';
}






