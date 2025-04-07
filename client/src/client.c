#include "client.h"

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

	logger = log_create("tp0.log", "Cliente", 1, LOG_LEVEL_INFO);
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	log_info(logger, "'cliente.config': INICIALIZANDO ARCHIVO DE CONFIGURACIONES");

	config = config_create("cliente.config");

	if (config == NULL) {
		log_info(logger, "'cliente.config': ERROR AL INICIALIZAR ARCHIVO DE CONFIGURACIONES");
		abort();
	}
	log_info(logger, "'cliente.config': LEYENDO VALOR 'IP'");
	ip = config_get_string_value(config, "IP");
	log_info(logger,"'cliente.config': IP = %s", ip);

	log_info(logger, "'cliente.config': LEYENDO VALOR 'PUERTO'");
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger,"'cliente.config': PUERTO = %s", puerto);

	log_info(logger, "'cliente.config': LEYENDO VALOR 'CLAVE'");
	valor = config_get_string_value(config, "CLAVE");
	log_info(logger,"'cliente.config': CLAVE = %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	log_info(logger, "LOGGEANDO ENTRADAS EN CONSOLA (PARA TERMINAR NO INGRESE VALORES):");
	
	char* leido = readline("> ");
	log_info(logger,"%s", leido);

	while (strcmp(leido, "") != 0) {

		free(leido);

		leido = readline("> ");
		log_info(logger,"%s", leido);
	}

	log_info(logger, "NO SE INGRESARON VALORES - TERMINANDO LOGGEO DE ENTRADAS EN CONSOLA");
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	printf("GUARDANDO ENTRADAS EN CONSOLA PARA SERVIDOR (PARA TERMINAR NO INGRESE VALORES):\n");
	while (1) {
		leido = readline("> ");

		if(strcmp(leido, "") == 0) break;

		printf("%s\n", leido);
		agregar_a_paquete(paquete, leido, strlen(leido));
		free(leido);
	}

	printf("NO SE INGRESARON VALORES - TERMINANDO GUARDADO DE ENTRADAS EN CONSOLA PARA SERVIDOR\n");

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
