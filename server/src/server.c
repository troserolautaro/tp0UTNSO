#include "server.h"


int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");

	while (1) {
			   pthread_t thread;
			   int *socket_cliente = malloc(sizeof(int));
			   *socket_cliente = accept(server_fd, NULL, NULL);
			   log_info(logger,"Socket creado");
			   if(pthread_create(&thread,NULL, recibir(),(void*)socket_cliente)!=0){
				   perror("Error al crear el hilo");
			   }
			   log_info(logger,"Hilo creado");
			   pthread_detach(thread);
		}



	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
void recibir(int cliente_fd){
	t_list* lista;
				while (1) {
					int cod_op = recibir_operacion(cliente_fd);
					switch (cod_op) {
					case MENSAJE:
						recibir_mensaje(cliente_fd);
						break;
					case PAQUETE:
						lista = recibir_paquete(cliente_fd);
						log_info(logger, "Me llegaron los siguientes valores:\n");
						list_iterate(lista, (void*) iterator);
						break;
					case -1:
						log_error(logger, "Un cliente se desconecto.");
						log_destroy(logger);
						break;
					default:
						log_warning(logger,"Operacion desconocida. No quieras meter la pata");
						break;
					}
				}
}
