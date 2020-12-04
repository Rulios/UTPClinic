#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_S 256

struct Paciente{
	char cedula[MAX_S];
	char rol[MAX_S];
	char nombre[MAX_S];
};

int BuscarPaciente(struct Paciente pacientes[], int nPacientes, struct Paciente paciente);



struct * Paciente ObtenerPacientes(int * nPacientes){
	FILE * archivo;
	struct * Paciente pacientes;
	struct Paciente paciente;
	int i = 0;
	
	archivo = fopen("./src/pacientes.txt", "rb");
	
	if(archivo == NULL){
		//crear un nuevo archivo llamado pacientes
		archivo = fopen("./src/pacientes.txt", "wb");
		*nPacientes = 0;
		pacientes = NULL;
	}else{
		//posicionar el puntero al final
		fseek(archivo, 0, SEEK_END);
		
		//nPacientes = bytes de archivo / bytes de estructura unitaria
		*nPacientes = ftell(archivo) / sizeof(struct Paciente);
		
		//alojar memoria
		pacientes = (struct * Paciente)malloc(*nPacientes * sizeof(struct Paciente));
		
		//posicionar el puntero al comienzo
		fseek(archivo, 0, SEEK_SET);
		
		//Condicionar las iteraciones a la cantidad de elementos leídos.
		//Me debe dar uno, puesto que es lo pedido
		//Si me da 0, significa que la lectura está al final del archivo
		while(1 == fread(&paciente, sizeof(struct Paciente), 1 , archivo)){
			
			//añadirlo al arreglo
			paciente = pacientes[i];
			i++;
		}
		
	}
	
	fclose(archivo);
	return pacientes;
}

int AgregarPaciente(struct Paciente paciente){
	FILE *archivo;
	int nPacientes = 0;
	struct * Paciente pacientes = ObtenerPacientes(&nPacientes);
	int existePaciente = BuscarPaciente(pacientes, nPacientes, paciente);
	
	//modo de añadir al final
	archivo = fopen("./src/pacientes.txt", "ab");
	
	if(archivo == NULL){
		return 0;
	}else{
		if(existePaciente == -1){ //paciente nuevo
			
			//mover puntero al final
			fseek(archivo, 0, SEEK_END);
			//añadir estructura de paciente al final de archivo
			fwrite(&paciente, sizeof(paciente), 1, archivo);
			fclose(archivo);
		}
		return 1;
	}
}

int EliminarPaciente(struct Paciente paciente){
	FILE *archivo;
	int nPacientes = 0;
	struct * Paciente pacientes = ObtenerPacientes(&nPacientes);
	int existePaciente = BuscarPaciente(pacientes, nPacientes, paciente);
	int i = 0;
	
	if(existePaciente == -1){ //paciente no existe para eliminar
		return -1;
	}else{
		//sobreescribir el archivo y añadir la estructura menos el señalado
		archivo = fopen("./src/pacientes.txt", "wb");
		
		if(archivo == NULL){
			return 0;
		}else{
			//mover puntero al final
			fseek(archivo, 0, SEEK_SET);
			for(i = 0; i < nPacientes; i++){
				//añadir al archivo menos el paciente con la cédula a eliminar
				if(pacientes[i].cedula != paciente.cedula){
					fwrite(&pacientes[i], sizeof(paciente), 1, archivo);
				}
			}
			fclose(archivo);
		}
		
	}
	return 1;
}

int BuscarPaciente(struct Paciente pacientes[], int nPacientes, struct Paciente paciente){
	int i = 0;
	for(i = 0; i < nPacientes ; i++){
		//comparar por cédula
		if(strcmp(pacientes[i].cedula, paciente.cedula) == 0) return i;
	}
	return -1;
}



int main(){
	
	return 0;
}
