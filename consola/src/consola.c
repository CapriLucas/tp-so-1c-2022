#include "consola.h"

typedef struct pseudoCodigo
{
    int NO_OP;
    int IO;
    int WRITE[2];
    int COPY[2];
    int READ;
    bool EXIT;

}pseudoCodigo;

t_log* mainLog;
t_config_consola* mainConfig;
pseudoCodigo leerPseudo (FILE* archPseudo, pseudoCodigo psdCod);

static void initializeProcess(){
    mainLog = log_create("./logs/consola.log", "CONSOLA", true, LOG_LEVEL_INFO);
    mainConfig = malloc(sizeof(t_config_consola));
    if (!cargar_configuracion(mainConfig, mainLog)){
        exit(EXIT_FAILURE);
    }
}

int main(){
    initializeProcess();

    FILE* archPseudo = fopen("pseudocodigo.txt","r");
    pseudoCodigo pseCod;
    pseCod =leerPseudo (archPseudo,pseCod);

    cerrar_programa(mainConfig, mainLog);
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