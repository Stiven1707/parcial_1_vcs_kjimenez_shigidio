#include <limits.h>
#include "versions.h"

/**
 * @brief Obtiene el hash de un archivo.
 * @param filename Nombre del archivo a obtener el hash
 * @param hash Buffer para almacenar el hash (HASH_SIZE)
 * @return Referencia al buffer, NULL si ocurre error
 */
char *get_file_hash(char * filename, char * hash);

/**
 * @brief Copia un archivo
 *
 * @param source Archivo fuente
 * @param destination Destino
 *
 * @return 1 en ca"hash"so de exito, 0 si existe error.
 */
int copy(char * source, char * destination);



return_code add(char * filename, char * comment) {
	//TODO implementar
	char* new;
	new = (char*)malloc(strlen(VERSIONS_DIR)+strlen("hash"));
	strcpy(new,VERSIONS_DIR);
	strcat(new,"/hash.txt");
	if(copy(filename,new)==0){
		return VERSION_ADDED;
	}
	return VERSION_ERROR;
}


void list(char * filename) {
	//TODO implementar
}

 /*
 *@brief Se encarga de copiar el archivo indicado al sub directorio versions
 *@param source la direccion del archivo fuente
 *@param destination la direccion donde de va a copiar +/hash
 *@return 1 si fue exitoso y 0 en caso contrario
 */
int copy(char * source, char * destination) {
	
	char *buff;
	FILE *fsource;
	FILE *fdestination;
	//Abrir el archivo fuente
	fsource=fopen(source,"r");

	//Verificar si se pudo abrir
	if(fsource!=NULL){
		//Abrir el archivo destino
		fdestination=fopen(destination,"w");
		//Verificar si se pudo abrir
		if(fdestination!=NULL){
			buff = (char*)malloc(LINEA_SIZE);
			fread(&buff, sizeof(buff), 1, fsource);
			//printf("%s -> LEN=%d\n",&buff,strlen(buff));
			while(!feof(fsource)){

				fwrite(&buff, sizeof(buff), 1, fdestination);
        		buff = (char*)malloc(LINEA_SIZE);
				fread(&buff, sizeof(buff), 1, fsource);
				//printf("%s\n",&buff);
			}
		}

		free(buff);
		fclose(fsource);
		fclose(fdestination);
	}else printf("\nError de apertura del archivo. \n\n");

	return 0;
}


return_code get(char * filename, int version) {

	//TODO implementar
	return VERSION_DOES_NOT_EXIST;
}

char *get_file_hash(char * filename, char * hash) {
	char *comando;
	FILE * fp;

	struct stat s;

	//Verificar que el archivo existe y que se puede obtener el hash
	if (stat(filename, &s) < 0 || !S_ISREG(s.st_mode)) {
		perror("stat");
		return NULL;
	}

	sha256_hash_file_hex(filename, hash);

	return hash;

}