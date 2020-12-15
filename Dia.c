#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_S 256

struct Dia{
	char nombre[MAX_S];
	int reservado[7];
	int VENTANAS_DISPONIBLES;
	char textoHorario[7][MAX_S]; //almacenar todos los horarios
	int iTextoHorario; //variable para rastrear la cantidad de elementos en textoHorario
	char servicio[7][MAX_S]; //anotar el servicio de visita
};

struct Dia * ObtenerDias(int * nDias){
	FILE * archivo;
	struct Dia * dias;
	struct Dia dia;
	int i = 0;
	
	archivo = fopen("./src/dias.txt", "rb");
	
	if(archivo == NULL){
		//rellenar el archivo (primer ingreso)
		dias = (struct Dia*)malloc(4 * sizeof(struct Dia));
		InicializarHorario(dias);
		ModificarDias(dias, 4);
		fclose(archivo);
		*nDias = 4;

	}else{
		//posicionar el puntero al final
		fseek(archivo, 0, SEEK_END);
		
		//nDias = bytes de archivo / bytes de estructura unitaria
		*nDias = ftell(archivo) / sizeof(struct Dia);

		//alojar memoria
		dias = (struct Dia *)malloc(*nDias * sizeof(struct Dia));
		
		//posicionar el puntero al comienzo
		fseek(archivo, 0, SEEK_SET);
		
		//Condicionar las iteraciones a la cantidad de elementos leídos.
		//Me debe dar uno, puesto que es lo pedido
		//Si me da 0, significa que la lectura está al final del archivo
		while(1 == fread(&dia, sizeof(dia), 1 , archivo)){
			
			//añadirlo al arreglo
			dias[i] = dia;
			i++;
		}
		fclose(archivo);
	}
	return dias;
}

int AgregarDias(struct Dia dias[], int nDias){
	//Agrega los días al archivo binario
	FILE *archivo;
	int i = 0;
	if(archivo == NULL){
		return 0;
	}else{
		//añadir al final
		archivo = fopen("./src/dias.txt", "ab");
		//mover puntero al final
		fseek(archivo, 0 , SEEK_END);
		
		//añadir todos los días
		for(i = 0; i < nDias;i ++){
			//añadir estructura de paciente al final de archivo
			fwrite(&dias[i], sizeof(dias[i]), 1, archivo);
		}
		
		fclose(archivo);
	
		return 1;
	}
}

int ModificarDias(struct Dia dias[], int nDias){
	
	if(dias != NULL){
		FILE *archivo;
	
		archivo = fopen("./src/dias.txt", "wb");
		fclose(archivo);
		AgregarDias(dias, nDias);
		return 1;
	}else{
		return 0;
	}
}

char * DeterminarCiclo(int hora){
	//ATENCIÓN: LA LÓGICA DE ESTA FUNCIÓN NO ES LA CORRECTA
	//Pero es útil en el contexto del problema, puesto que nada
	//más satisface los rangos de hora mostrados en el problema.
	
	//Explicación: Para determinar el ciclo de una hora, es necesario que 
	//			tenga el formato de 24h. Pero para no complicar tanto
	//			el desarrollo, nada más se va a hacer uso de 12h como 
	//			valores desplegables. Estos valores de 12h, satisfacen 
	//			con los valores que nos muestra el problema, puesto que
	//			no existe duplicidad de 12h en diferentes ciclos (AM/PM)
	
	//Si la hora es 12 o si cae dentro del rango entre 1 a 4. Entonces 
	//el ciclo de hora es PM
	if(hora == 12 || hora >= 1 && hora <= 4){
		return "PM";
	}else{
		return "AM";
	}
}

void RellenarTextoHorario(struct Dia * dia, int inicio,  int final){
	
	int i = 0;
	
	for(; inicio < final; inicio++){
		char tiempoHorario[MAX_S];
		
		//ej formato: 8:00AM-9:00AM
		sprintf(tiempoHorario, "%i:00%s-%i:00%s", inicio, DeterminarCiclo(inicio), inicio + 1, DeterminarCiclo(inicio+1));
		//copiarlo a la estructura
		strcpy(dia->textoHorario[dia->iTextoHorario], tiempoHorario);
		
		//printf("%s %i : %s\n", dia->nombre,dia->iTextoHorario,dia->textoHorario[dia->iTextoHorario]);
		
		dia->iTextoHorario++;
	}
}

void InicializarHorario(struct Dia dias[]){
	//inicializar el nombre de los días
	sprintf(dias[0].nombre, "%s", "Lunes");
	sprintf(dias[1].nombre, "%s", "Martes");
	sprintf(dias[2].nombre, "%s", "Jueves");
	sprintf(dias[3].nombre, "%s", "Viernes");
	
	dias[0].iTextoHorario = 0; //inicializar el índice de rastreo del relleno de horario
	RellenarTextoHorario(&dias[0], 8, 12); //lunes 8AM - 12PM
	
	dias[1].iTextoHorario = 0;//inicializar el índice de rastreo del relleno de horario
	RellenarTextoHorario(&dias[1], 1, 4); //martes 1PM - 4PM
	
	dias[2].iTextoHorario = 0; //inicializar el índice de rastreo del relleno de horario
	RellenarTextoHorario(&dias[2], 8, 12); //jueves 8AM - 12PM
	RellenarTextoHorario(&dias[2], 1, 4); // jueves 1PM - 4PM
	
	dias[3].iTextoHorario = 0;//inicializar el índice de rastreo del relleno de horario
	RellenarTextoHorario(&dias[3], 8, 12); //viernes 8AM - 12PM
}

//int main(){
//	int nDias = 0;
//	struct Dia * dias = ObtenerDias(&nDias);
//
//	
//	int i = 0, y = 0;
//	for(i = 0; i < 4; i++){
//		printf("%i: ", i +1);
//		for(y = 0; y < 7; y++){
//			printf("\t%s\n", dias[i].textoHorario[y]);
//		}
//	}
//	
//	return 0;
//}
