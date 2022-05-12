#include "consola.h"


typedef struct configuracion
{
    char* IP_KERNEL;
    int PUERTO_KERNEL;
}config;

typedef struct pseudoCodigo
{
    int NO_OP;
    int IO;
    int WRITE[2];
    int COPY[2];
    int READ;
    bool EXIT;

}pseudoCodigo;

t_config* iniciar_config ();
config leerConf (t_config* archConfig,config conf);
pseudoCodigo leerPseudo (FILE* archPseudo, pseudoCodigo psdCod);


int main(){
    t_log* logger = log_create("./logs/consola.log", "CONSOLA", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso consola! %s", mi_funcion_compartida());
    log_destroy(logger);

    t_config* archConfig = iniciar_config ();

    config conf;
    conf =leerConf (archConfig,conf);

    FILE* archPseudo = fopen("pseudocodigo.txt","r");
    pseudoCodigo pseCod;
    pseCod =leerPseudo (archPseudo,pseCod);
}

t_config* iniciar_config()
{
    t_config* nuevo_config;
    nuevo_config = config_create("./consola.config");
    if(nuevo_config == NULL){
        printf("No pude leer la config\n");
    }
    return nuevo_config;
}


config leerConf (t_config* archConfig,config conf)
{
    conf.IP_KERNEL = config_get_string_value (archConfig,"IP_KERNEL");
    conf.PUERTO_KERNEL = config_get_int_value (archConfig,"PUERTO_KERNEL");
    return conf;
}


pseudoCodigo leerPseudo (FILE* archPseudo, pseudoCodigo psdCod)
{
    char* codigo="";
    char lector;
    fread (&lector,sizeof(char),1,archPseudo);
    while (!feof(archPseudo))
    {
        while (lector!=' ')
        {
            string_append_with_format (codigo*,"%c!",lector);
            fread (&lector,sizeof(char),1,archPseudo);
        }
        if (lector==' ')
        {
            if (codigo == "NO_OP")
                {
                    fread (&lector,sizeof(char),1,archPseudo);
                }
            
        }
        fread (&lector,sizeof(char),1,archPseudo);
    }
    return psdCod;
}