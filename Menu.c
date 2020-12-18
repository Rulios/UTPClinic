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

int EntradaDiaSeleccionado(nDias);
int EntradaHorarioSeleccionado(char iTextoHorario[][MAX_S], int reservado[], int prevenirReservado);

void MostrarError(char texto[MAX_S]);

int ListaYSeleccionMultiple(int nElementos, char elementos[][MAX_S]);

void Titulo();
void Subtitulo(char texto [MAX_S]);

int existeReservacion(struct Dia * dias, int nDias);

void MenuPrincipal();
void MenuNuevaCita();
void MenuVerCitas();
void RegresarAPrincipal();

int estaElDiaEnRango(int diaSeleccionado, int nDias);
int estaElHorarioEnRango(int horarioSeleccionado, int iTextoHorario);
int estaElHorarioDisponibleYEnRango(int horarioSeleccionado, int iTextoHorario, int reservado);
int estaElHorarioReservadoYEnRango(int horarioSeleccionado, int iTextoHorario, int reservado);
char * SeleccionarRol();
char * SeleccionarServicio();

void ImprimirPaciente(struct Paciente paciente);
void ImprimirCita(struct Dia dia, struct Paciente paciente, int iHorarioSeleccionado);

void ImprimirTablaDeHorario(struct Dia dias[], int nDias, int mostrarDisponibles, struct Paciente pacientes[], int nPacientes);
void ImprimirLineaDeFila(int anchoTabla);
void ImprimirContenidoDeTabla(struct Dia dias[], int nDias, int mostrarDisponibles, struct Paciente pacientes[], int nPacientes);

int main(){
	
	setlocale(LC_ALL, "spanish"); /* Permite imprimir caracteres con tilde */
	//OBLIGATORIO, SE DEBE INICIALIZAR EL PROGRAMA PRINCIPAL 
	//PARA HACER USO DE COLORES
	IniciarColores();
	MenuPrincipal();
	return 0;
}



void MenuPrincipal(){
	char selecciones[3][MAX_S];
	int opcion = 0;
	SaltoDeLinea(4);
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
			system("cls");
			MenuVerCitas();
		break;
		
		case 3: 
			exit(0);
		break;
	}
	
}

void MenuNuevaCita(){
	int exito = 0;
	int nDias = 0, nPacientes =0 ;
	int iDiaSeleccionado = -1;
	int iHorarioSeleccionado = -1;
	struct Dia * dias = ObtenerDias(&nDias);
	struct Paciente nuevoPaciente;
	struct Paciente * pacientes = ObtenerPacientes(&nPacientes);
	
	Titulo();
	Subtitulo("MENÚ PARA AÑADIR UNA CITA");
	SaltoDeLinea(2);
	ImprimirTablaDeHorario(dias, nDias, 1, NULL, NULL);

	iDiaSeleccionado = EntradaDiaSeleccionado(nDias);
	iHorarioSeleccionado = EntradaHorarioSeleccionado(dias[iDiaSeleccionado].iTextoHorario, dias[iDiaSeleccionado].reservado, 1);
	
	Tabulaciones(2);
	printf("Ingrese el nombre completo del paciente: ");
	EntradaTexto(&nuevoPaciente.nombre);
	
	do{
		Tabulaciones(2);
		printf("Ingrese la cédula del paciente: ");
		EntradaTexto(&nuevoPaciente.cedula);
		if(BuscarPaciente(pacientes, nPacientes, nuevoPaciente.cedula) > -1){
			Tabulaciones(2);
			MostrarError("LA CÉDULA EXISTE EN UN PACIENTE EXISTENTE"); //mostrar error
			SaltoDeLinea(1);
		}
	}while(BuscarPaciente(pacientes, nPacientes, nuevoPaciente.cedula) > -1); //debe ser una cédula que no exista
	
	
	//asignar el rol del paciente
	sprintf(nuevoPaciente.rol, "%s", SeleccionarRol());
	//guardar el servicio a usar
	sprintf(dias[iDiaSeleccionado].servicio[iHorarioSeleccionado], "%s", SeleccionarServicio());
	//asignar el paciente a la cita en el horario
	sprintf(dias[iDiaSeleccionado].cedPaciente[iHorarioSeleccionado], "%s", nuevoPaciente.cedula);

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

void MenuVerCitas(){
	int exito = 0;
	int nDias = 0, nPacientes = 0;
	int iDiaSeleccionado = -1, iHorarioSeleccionado = -1;
	int iPaciente = -1;
	char opciones[4][MAX_S];
	char cedula[MAX_S];
	int opcion =0 , iOpcion = 0;
	struct Dia * dias = ObtenerDias(&nDias);
	struct Paciente * pacientes = ObtenerPacientes(&nPacientes);
	
	//inicializar las opciones
	sprintf(opciones[0], "%s", "Ver datos de un paciente");
	sprintf(opciones[1], "%s", "Ver detalles de una cita");
	sprintf(opciones[2], "%s", "Eliminar cita");
	sprintf(opciones[3], "%s", "Regresar");
	
	Titulo();
	Subtitulo("TODAS LAS CITAS RESERVADAS");
	SaltoDeLinea(2);
	ImprimirTablaDeHorario(dias, nDias, 0, pacientes, nPacientes);
	
	opcion = ListaYSeleccionMultiple(4, opciones);
	
	switch(opcion){
		case 1: //Ver datos de un paciente
			if(nPacientes > 0){
				do{
					Tabulaciones(2);
					printf("Ingrese la cédula del paciente: ");
					EntradaTexto(&cedula);
					iPaciente = BuscarPaciente(pacientes, nPacientes, cedula);
					if(iPaciente == -1){
						Tabulaciones(2);
						MostrarError("NO EXISTE UN PACIENTE CON ESA CÉDULA"); //mostrar error
						SaltoDeLinea(1);
					}
				}while(iPaciente == -1); //debe ser una cédula que exista
	
				system("cls");
				Titulo();
				ImprimirPaciente(pacientes[iPaciente]);
			}else{
				//Regresar al menú de citas
				system("cls");
				print_red("NO EXISTE NINGÚN PACIENTE PARA BUSCAR");
				SaltoDeLinea(2);
				MenuVerCitas();
			}
			
		break;
		
		case 2: //ver detalles de una cita
			if(existeReservacion(dias, nDias)){
				iDiaSeleccionado = EntradaDiaSeleccionado(nDias);
				iHorarioSeleccionado = EntradaHorarioSeleccionado(dias[iDiaSeleccionado].iTextoHorario, dias[iDiaSeleccionado].reservado, 0);
				iPaciente = BuscarPaciente(pacientes, nPacientes, dias[iDiaSeleccionado].cedPaciente[iHorarioSeleccionado]);
				
				system("cls");
				ImprimirCita(dias[iDiaSeleccionado], pacientes[iPaciente], iHorarioSeleccionado);
			}else{
				//Regresar al menú de citas
				system("cls");
				print_red("NO EXISTE NINGUNA CITA RESERVADA PARA VER");
				SaltoDeLinea(2);
				MenuVerCitas();
			}
		break;
		
		case 3: //eliminar cita
		
			if(existeReservacion(dias, nDias)){
				iDiaSeleccionado = EntradaDiaSeleccionado(nDias);
				iHorarioSeleccionado = EntradaHorarioSeleccionado(dias[iDiaSeleccionado].iTextoHorario, dias[iDiaSeleccionado].reservado, 0);
				dias[iDiaSeleccionado].reservado[iHorarioSeleccionado] = 0; //liberar la reservación
				sprintf(dias[iDiaSeleccionado].servicio[iHorarioSeleccionado], "%s", ""); //reiniciar valor del servicio a usar
				
				if(EliminarPaciente(dias[iDiaSeleccionado].cedPaciente[iHorarioSeleccionado]) == -1){
					Tabulaciones(2);
					MostrarError("ERROR AL ELIMINAR EL PACIENTE");
				}
				exito++;
				
				sprintf(dias[iDiaSeleccionado].cedPaciente[iHorarioSeleccionado], "%s", ""); //reiniciar la cédula que referencia la cita
				
				if(!ModificarDias(dias, nDias)){
					Tabulaciones(2);
					MostrarError("ERROR AL ELIMINAR LA CITA");
				}
				exito++;
				
				if(exito == 2){
					Tabulaciones(2);
					print_green("SE HA ELIMINADO LA CITA CORRECTAMENTE");
				}
			}else{
				//Regresar al menú de citas
				system("cls");
				print_red("NO EXISTE NINGUNA CITA RESERVADA PARA ELIMINAR");
				SaltoDeLinea(2);
				MenuVerCitas();
			}
			
			
		break;
		
		case 4:
			RegresarAPrincipal();
		break;
	}
	
	if(opcion != 4){
		RegresarAPrincipal();
	}
	
}

int estaElDiaEnRango(int diaSeleccionado, int nDias){
	//el día seleccionado debe caer dentro del rango (4 días)
	return diaSeleccionado >= 0 && diaSeleccionado < nDias;
}
int estaElHorarioEnRango(int horarioSeleccionado, int iTextoHorario){
	//verificar si el horario está en rango
	return horarioSeleccionado >= 0 && horarioSeleccionado + 1 <= iTextoHorario;
}

int estaElHorarioDisponibleYEnRango(int horarioSeleccionado, int iTextoHorario, int reservado){
	//el índice del horario escogido debe ser mayor al índice 0
	//Además, debe caer dentro del rango de los horarios mostrados que están formulados
	// 		iTextoHorario muestra el entero comenzando desde el índice 1. No es relativo al índice 0 de un arreglo
	//Y no debe estar reservado
	return estaElHorarioEnRango(horarioSeleccionado, iTextoHorario) && !reservado;
}

int estaElHorarioReservadoYEnRango(int horarioSeleccionado, int iTextoHorario, int reservado){
	return estaElHorarioEnRango(horarioSeleccionado, iTextoHorario) && reservado;
}

int existeReservacion(struct Dia * dias, int nDias){
	int i = 0,  y = 0;

	for(i = 0; i < nDias; i++){
		for(y = 0; y < 7; y++){
			if(dias[i].reservado[y]) return 1; //existe reservación
		}
	}
	return 0; // no existe reservación
}



void ImprimirPaciente(struct Paciente paciente){
	Subtitulo("DATOS DEL PACIENTE:");
	Tabulaciones(2);
	print_yellow("Nombre: ");
	printf("%s", paciente.nombre);
	SaltoDeLinea(1);
	Tabulaciones(2);
	print_yellow("Cédula: ");
	printf("%s", paciente.cedula);
	SaltoDeLinea(1);
	Tabulaciones(2);
	print_yellow("Rol: ");
	printf("%s", paciente.rol);
	SaltoDeLinea(2);
}

void ImprimirCita(struct Dia dia, struct Paciente paciente, int iHorarioSeleccionado){
	Titulo();
	Subtitulo("Cita");
	Tabulaciones(2);
	print_yellow("Día de la cita: ");
	printf("%s", dia.nombre);
	SaltoDeLinea(1);
	Tabulaciones(2);
	print_yellow("Horario de la cita: ");
	printf("%s", dia.textoHorario[iHorarioSeleccionado]);
	SaltoDeLinea(1);
	Tabulaciones(2);
	print_yellow("Servicio a usar en la cita: ");
	printf("%s", dia.servicio[iHorarioSeleccionado]);
	SaltoDeLinea(2);
	ImprimirPaciente(paciente);
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

char * SeleccionarServicio(){
	char servicios[12][MAX_S];
	int iServicioSeleccionado = -1;
	
	sprintf(servicios[0], "%s", "Consultas y evaluaciones medicas con previa cita");
	sprintf(servicios[1], "%s", "Consultas y evaluaciones de urgencias");
	sprintf(servicios[2], "%s", "Referencias a especialidades médicas");
	sprintf(servicios[3], "%s", "Certificado de buena salud");
	sprintf(servicios[4], "%s", "Solicitudes de estudios de gabinete");
	sprintf(servicios[5], "%s", "Administración gratuita de medicamentos básicos");
	sprintf(servicios[6], "%s", "Curaciones y corte de puntos");
	sprintf(servicios[7], "%s", "Control de peso y talla");
	sprintf(servicios[8], "%s", "Control de presión arterial");
	sprintf(servicios[9], "%s", "Inhaloterapias");
	sprintf(servicios[10], "%s", "Aplicación de medicamentos inyectables");
	sprintf(servicios[11], "%s", "Toma de glicemia capilar");
	
	Tabulaciones(2);
	printf("INGRESE EL SERVICIO QUE VA A USAR EL PACIENTE:");
	SaltoDeLinea(1);
	
	iServicioSeleccionado = ListaYSeleccionMultiple(12, servicios);
	
	return servicios[iServicioSeleccionado- 1]; //ajustar el índice para el retorno
}

void ImprimirTablaDeHorario(struct Dia dias[], int nDias, int mostrarDisponibles, struct Paciente pacientes[], int nPacientes){
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
	ImprimirContenidoDeTabla(dias, nDias, mostrarDisponibles, pacientes, nPacientes);
}

void ImprimirLineaDeFila(int anchoTabla){
	int i = 0;
	SaltoDeLinea(1);
	for(i = 0; i < anchoTabla; i++){
		printf("-");
	}
	SaltoDeLinea(1);
}

void ImprimirContenidoDeTabla(struct Dia dias[], int nDias, int mostrarDisponibles, struct Paciente pacientes[], int nPacientes){
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
					sprintf(temp, "%20s", "RESERVADO");
					print_red(temp); //mostrar texto de no disponible con letra roja
				}else{
					sprintf(temp, "%20s", dias[y].textoHorario[i]);
					print_green(temp); //mostrar el horario disponible
				}
			}else{ //mostrar los horarios reservados
				if(dias[y].reservado[i]){ //se debe mostrar las cédulas de los pacientes
					int iPaciente = BuscarPaciente(pacientes, nPacientes, dias[y].cedPaciente[i]);
					if(iPaciente >= 0){
						sprintf(temp, "%20s", dias[y].cedPaciente[i]); //mostrar la cédula del paciente
						print_green(temp);
					}else{
						sprintf(temp, "%20s","ERROR PACIENTE", iPaciente); //el paciente no se encuentra en la base de datos (hubo un error)
						print_red(temp);
					}
				}else{
					sprintf(temp, "%20s", "HORARIO LIBRE");
					print_yellow(temp);
					
				}
			}
			printf("|");
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

int EntradaDiaSeleccionado(nDias){
	int iDiaSeleccionado = -1;
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
	return iDiaSeleccionado;
}

int EntradaHorarioSeleccionado(char iTextoHorario[][MAX_S], int reservado[], int prevenirReservado){
	int iHorarioSeleccionado = -1;
	int repetir = 1;
	do{
		Tabulaciones(2);
		printf("Ingrese el horario (índice) de la cita: ");
		EntradaEntero(&iHorarioSeleccionado);
		iHorarioSeleccionado--; //lo que se muestra no concuerda con los índices del arreglo del mismo
		if(prevenirReservado){
			repetir = !estaElHorarioDisponibleYEnRango(iHorarioSeleccionado, iTextoHorario, reservado[iHorarioSeleccionado]);
			if(repetir){ //mostrar error
				Tabulaciones(2);
				MostrarError("DEBE SELECCIONAR UN HORARIO DISPONIBLE EN EL RANGO MOSTRADO");
				SaltoDeLinea(1);
				iHorarioSeleccionado = -1; //reiniciar el valor seleccionado
			}
		}else{
			repetir = !estaElHorarioReservadoYEnRango(iHorarioSeleccionado, iTextoHorario, reservado[iHorarioSeleccionado]);
			if(repetir){ //mostrar error
				Tabulaciones(2);
				MostrarError("DEBE SELECCIONAR UN HORARIO RESERVADO EN EL RANGO MOSTRADO");
				SaltoDeLinea(1);
				iHorarioSeleccionado = -1; //reiniciar el valor seleccionado
			}
		}
		
	}while(repetir);
	return iHorarioSeleccionado;
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






