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
#include "Paciente.c"/*
	struct Paciente
	struct * Paciente ObtenerPacientes(int * nPacientes)
	int AgregarPaciente(struct Paciente paciente)
	int EliminarPaciente(struct Paciente paciente)
	int BuscarPaciente(struct Paciente pacientes[], int nPacientes, struct Paciente paciente)
*/

#define MAX_S 256
#define ANCHO_DE_TABLA 98

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
void RegresarAPrincipal();

int estaElDiaEnRango(int diaSeleccionado, int nDias);
int estaElHorarioDisponibleYEnRango(int horarioSeleccionado, int iTextoHorario, int reservado);
char * SeleccionarRol();

void ImprimirTablaDeHorario(struct Dia * dias, int nDias, int mostrarDisponibles);
void ImprimirLineaDeFila(int anchoTabla);
void ImprimirContenidoDeTabla(struct Dia dias[], int nDias, int mostrarDisponibles);

int main(){
	
	setlocale(LC_ALL, "spanish"); /* Permite imprimir caracteres con tilde */
	//OBLIGATORIO, SE DEBE INICIALIZAR EL PROGRAMA PRINCIPAL 
	//PARA HACER USO DE COLORES
	
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
	int exito = 0;
	int nDias = 0;
	int iDiaSeleccionado = -1;
	int iHorarioSeleccionado = -1;
	struct Dia * dias = ObtenerDias(&nDias);
	struct Paciente nuevoPaciente;
	
	Titulo();
	Subtitulo("MENÚ PARA AÑADIR UNA CITA");
	SaltoDeLinea(2);
	ImprimirTablaDeHorario(dias, nDias, 1);

	
	do{
		Tabulaciones(2);
		printf("Ingrese el día (índice) para la cita: ");
		EntradaEntero(&iDiaSeleccionado);
		iDiaSeleccionado--; //lo que se muestra no concuerda con los índices del arreglo del mismo
		if(!estaElDiaEnRango(iDiaSeleccionado, nDias)){ //mostrar error
			Tabulaciones(2);
			MostrarError("DEBE SELECCIONAR UN DÍA EN EL RANGO MOSTRADO");
			SaltoDeLinea(1);
			iDiaSeleccionado = -1; //reiniciar el valor
		}
	}while(!estaElDiaEnRango(iDiaSeleccionado, nDias)); 
	
	do{
		Tabulaciones(2);
		printf("Ingrese el horario (índice) de la cita: ");
		EntradaEntero(&iHorarioSeleccionado);
		iHorarioSeleccionado--; //lo que se muestra no concuerda con los índices del arreglo del mismo
		if(!estaElHorarioDisponibleYEnRango(iHorarioSeleccionado, dias[iDiaSeleccionado].iTextoHorario, dias[iDiaSeleccionado].reservado[iHorarioSeleccionado])){ //mostrar error
			Tabulaciones(2);
			MostrarError("DEBE SELECCIONAR UN HORARIO DISPONIBLE EN EL RANGO MOSTRADO");
			SaltoDeLinea(1);
			iHorarioSeleccionado = -1; //reiniciar el valor seleccionado
		}
	}while(!estaElHorarioDisponibleYEnRango(iHorarioSeleccionado, dias[iDiaSeleccionado].iTextoHorario, dias[iDiaSeleccionado].reservado[iHorarioSeleccionado]));
	
	Tabulaciones(2);
	printf("Ingrese el nombre completo del paciente: ");
	EntradaTexto(&nuevoPaciente.nombre);
	
	Tabulaciones(2);
	printf("Ingrese la cédula del paciente: ");
	EntradaTexto(&nuevoPaciente.cedula);
	
	sprintf(nuevoPaciente.rol, "%s", SeleccionarRol());

	if(!AgregarPaciente(nuevoPaciente)){
		Tabulaciones(2);
		MostrarError("ERROR AL GUARDAR NUEVO PACIENTE");
	}
	exito++;
	
	//marcar como reservado el día
	dias[iDiaSeleccionado].reservado[iHorarioSeleccionado] = 1;
	
	if(!ModificarDias(dias, nDias)){
		Tabulaciones(2);
		MostrarError("ERROR AL GUARDAR LA CITA");
	}
	exito++;
	
	if(exito == 2){ //se completaron los 2 pasos críticos
		Tabulaciones(2);
		print_green("SE HA INSERTADO EL PACIENTE Y LA CITA CON ÉXITO");
	}
	RegresarAPrincipal();
}

int estaElDiaEnRango(int diaSeleccionado, int nDias){
	//el día seleccionado debe caer dentro del rango (4 días)
	return diaSeleccionado >= 0 && diaSeleccionado < nDias;
}

int estaElHorarioDisponibleYEnRango(int horarioSeleccionado, int iTextoHorario, int reservado){
	//el índice del horario escogido debe ser mayor al índice 0
	//Además, debe caer dentro del rango de los horarios mostrados que están formulados
	// 		iTextoHorario muestra el entero comenzando desde el índice 1. No es relativo al índice 0 de un arreglo
	//Y no debe estar reservado
	return horarioSeleccionado >= 0 && horarioSeleccionado + 1 <= iTextoHorario && !reservado;
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

char * SeleccionarRol(){
	char roles[4][MAX_S];
	int iRolSeleccionado = -1;
	
	sprintf(roles[0], "%s", "Administrativo");
	sprintf(roles[1], "%s", "Investigador");
	sprintf(roles[2], "%s", "Docente");
	sprintf(roles[3], "%s", "Estudiante");
	
	Tabulaciones(2);
	printf("INGRESE UN ROL DISPONIBLE:");
	SaltoDeLinea(1);
	
	iRolSeleccionado = ListaYSeleccionMultiple(4, roles);
	
	return roles[iRolSeleccionado- 1]; //ajustar el índice para el retorno
}

void ImprimirTablaDeHorario(struct Dia dias[], int nDias, int mostrarDisponibles){
	int i = 0;
	
	for(i = -1; i < nDias; i++){ //imprimir títulos horizontales de los campos
		char temp[MAX_S];
		if(i != -1){
			sprintf(temp, "%19s(%i)", dias[i].nombre, i + 1);
			//printf("%22s", dias[i].nombre);
			printf(temp);
			printf("|");
		}else{
			//espacio vacío para mostrar la diagonal de la tabla
			printf("     |"); // 5 espacios
		}
	}
	ImprimirLineaDeFila(ANCHO_DE_TABLA);
	ImprimirContenidoDeTabla(dias, nDias, mostrarDisponibles);
}

void ImprimirLineaDeFila(int anchoTabla){
	int i = 0;
	SaltoDeLinea(1);
	for(i = 0; i < anchoTabla; i++){
		printf("-");
	}
	SaltoDeLinea(1);
}

void ImprimirContenidoDeTabla(struct Dia dias[], int nDias, int mostrarDisponibles){
	int y = 0, i = 0, cFilas = 1;
	
	// iterar hasta el máximo de textoHorario, 
	//en este caso del problema, el máximo de textoHorario
	//que hay en un día son 7.
	for(i = 0; i < 7; i++){ //filas
		printf("%5i|", i+1); //imprimir la numeración de textoHorario (no se usa el índice del arreglo)
		for(y = 0; y < nDias; y++){ //columnas
			char temp[MAX_S];
			if(mostrarDisponibles){ // se debe mostrar los horarios disponibles
				if(dias[y].reservado[i]){ // 1 = reservado | 0 = no reservado
					sprintf(temp, "%20s", "NO DISPONIBLE");
					print_red(temp); //mostrar texto de no disponible con letra roja
					printf("|");
				}else{
					sprintf(temp, "%20s", dias[y].textoHorario[i]);
					print_green(temp); //mostrar el horario disponible
					printf("|");
				}
			}else{ //mostrar los horarios reservados
				if(dias[y].reservado[i]){
					
				}else{
				
				}
			}
			
		}
		ImprimirLineaDeFila(ANCHO_DE_TABLA);
	}
}


void MostrarError(char texto[MAX_S]){
	print_red(texto);
}

void IniciarColores(){
	system("COLOR");
}

void RegresarAPrincipal(){
	char temp[MAX_S];
	SaltoDeLinea(2);
	Tabulaciones(2);
	printf("REGRESAR AL MENÚ PRINCIPAL...presione ENTER para continuar");
	EntradaTexto(&temp);
	system("cls");
	MenuPrincipal();
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






