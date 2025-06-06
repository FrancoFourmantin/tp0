#include "client.h"
#include "string.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if (config == NULL) {
		// ¡No se pudo crear el config!
		// Terminemos el programa
		abort();
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	if ( config_has_property(config , "IP") ){
		ip = config_get_string_value(config, "IP");

		// log_info(logger, ip);
	}

	if ( config_has_property(config , "PUERTO") ){
		puerto = config_get_string_value(config, "PUERTO");

		// log_info(logger, puerto);
	}

	if ( config_has_property(config , "CLAVE") ){
		valor = config_get_string_value(config, "CLAVE");

		// log_info(logger, valor);
	}



	/* ---------------- LEER DE CONSOLA ---------------- */

	// leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	size_t bytes;

	int32_t handshake = 1;
	int32_t result;

	bytes = send(conexion, &handshake, sizeof(int32_t), 0);

	bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

	if (result == 0) {
		log_info(logger,"Enviando mensaje");
		enviar_mensaje(valor,conexion);
	} else {
		log_info(logger,"F");
	}
	
	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	paquete(conexion, logger);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log" , "TP0" , true , LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while( strcmp(leido, "") != 0 )
	{
		log_info(logger , leido);

		free(leido);

		leido = readline("> ");
	}

	free(leido);
}

void paquete(int conexion, t_log* logger)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();

	leido = readline("> ");

	while( strcmp(leido, "") != 0 )
	{
		log_info(logger,leido);

		agregar_a_paquete(paquete, leido, (strlen(leido)+1));

		free(leido);

		leido = readline("> ");
	}

	free(leido);

	enviar_paquete(paquete , conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);

	log_destroy(logger);

	config_destroy(config);
}