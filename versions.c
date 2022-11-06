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
void add_version(file_version info);
void imprimirDatos(file_version dversion, int version);


return_code add(char * filename, char * comment) {
	//TODO implementar
	file_version version;
	char hash[HASH_SIZE];
	char* new;
	
	FILE *fsource;
	fsource=fopen(VERSIONS_DB_PATH,"rb");
	int resp = 0;
	
	if(fsource!=NULL){
		file_version dversion;
		
		fread(&dversion, sizeof(file_version), 1, fsource);
		while(!feof(fsource)){
		
			if(EQUALS(filename, dversion.filename)){
				resp = 1;
				break; 
			}
			fread(&dversion, sizeof(file_version), 1, fsource);
		}
	}
	fclose(fsource);
	if(!resp){
		//Verifica si el archivo existe, se puede crear el hash y si es un archivo regular
		//Obtener el hash del archivo
		if(get_file_hash(filename,hash)==NULL){
			return VERSION_ERROR;
		}
		//Asigno memoria dinamicamente
		new = (char*)malloc(sizeof(char)*(strlen(VERSIONS_DIR)+strlen(hash)+2));
		//Nuevo nombre del archivo: .versions/ABCDEF....
		//Construir la cadena
		strcpy(new,VERSIONS_DIR);
		strcat(new,"/");
		strcat(new,hash);
		//Copia el archivo al directorio .versions, adiciona el registro
		if(copy(filename,new)==0){
			strcpy(version.filename,filename);
			strcpy(version.hash,hash);
			strcpy(version.comment,comment);
			//Adicionamos la version al archivo "versions.db"
			add_version(version);
			free(new);
			return VERSION_ADDED;
		}
		free(new);
	}
	return VERSION_ERROR;
}


void list(char * filename) {
	FILE *fsource;
	fsource=fopen(VERSIONS_DB_PATH,"rb");
	int bandera = 0;
	if(fsource!=NULL){
		file_version dversion;
		
		int contador = 1;
		fread(&dversion, sizeof(file_version), 1, fsource);
		while(!feof(fsource)){
			
			if(filename != NULL){
				if(EQUALS(dversion.filename, filename)){
					imprimirDatos(dversion, contador);
					bandera = 1;
				}
			}
				
			else
				imprimirDatos(dversion, contador);
					
			
			fread(&dversion, sizeof(file_version), 1, fsource);
			contador++;
		}
	}
	fclose(fsource);
	if(!bandera){
		printf("No hay registros del archivo '%s' en la base de datos!!\n", filename);
	}
}

void imprimirDatos(file_version dversion, int version){
	printf("Version #%d\n",version);
	printf("Nombre del archivo: %s\n",dversion.filename);
	printf("Hash: %s\n",dversion.hash);
	printf("Comentario: %s\n\n",dversion.comment);
}
 /*
 *@brief Se encarga de copiar el archivo indicado al sub directorio versions
 *@param source la direccion del archivo fuente
 *@param destination la direccion donde de va a copiar +/hash
 *@return 1 si fue exitoso y 0 en caso contrario
 */
int copy(char * source, char * destination) {
	
	char buff[BUFSIZ];
	size_t nread;
	size_t nwrite;
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
			while(!feof(fsource)){
				nread = fread(buff, sizeof(char), BUFSIZ, fsource);
				if (nread>0)
				{
					nwrite = fwrite(buff, sizeof(char), nread, fdestination);
				}
				
			}
		}

		fclose(fsource);
		fclose(fdestination);
	}else printf("\nError de apertura del archivo. \n\n");

	return 0;
}
 /*
 * @brief Adiciona la final del archivo binario(versions.db) la estructura que tiene la informacion del
 * archivo al que se le hizo la copia
 * @param file_version Estructura con la info del archivo
 */
void add_version(file_version info){
	size_t nwrite;
	FILE *fdestination;
	//Abrir el archivo destino
	fdestination=fopen(VERSIONS_DB_PATH,"a");
	//Verificar si se pudo abrir
	if(fdestination!=NULL){
		//Guardamos al final la extructura
		nwrite = fwrite(&info, sizeof(file_version), 1, fdestination);
		//TODO Verificamos si la escritura fue exitosa		
	}
	fclose(fdestination);

}

return_code get(char * filename, int version) {

	FILE *fsource;
	fsource=fopen(VERSIONS_DB_PATH,"rb");
	char* new;
	if(fsource!=NULL){
		file_version dversion;
		
		int contador = 1;
		fread(&dversion, sizeof(file_version), 1, fsource);
		while(!feof(fsource)){
			
			if(contador == version){
				new = (char*)malloc(sizeof(char)*(strlen(VERSIONS_DIR)+strlen(dversion.hash)+2));
				//Nuevo nombre del archivo: .versions/ABCDEF....
				//Construir la cadena
				strcpy(new,VERSIONS_DIR);
				strcat(new,"/");
				strcat(new,dversion.hash);

				if(EQUALS(filename, dversion.filename)){
					copy(new, dversion.filename);
					printf("La version %d del archivo %s se recupero correctamente.\n",version, dversion.filename);
					return VERSION_RETRIEVED;
				}else
					break;
			}
			
			fread(&dversion, sizeof(file_version), 1, fsource);
			contador++;
		}
	}
	fclose(fsource);
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
//hexddump -C archivo
