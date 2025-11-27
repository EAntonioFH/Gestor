#include <stdio.h>
#include <string.h>

#define ARCHUSUARIOS "usuarios.us"//Nombre del archivo binario donde van los usuarios
#define ARCHGESMENU "menuGestor.txt"//Nombre del archivo donde esta el menu principal
#define ARCHOPGESTOR "opcionesGestor.txt"//Nombre del archivo donde estan lasa opciones del gestor
#define OPC_SALIR 100 //Valor por defecto si se desea volver al menu segun las opciones
#define MAXARCHUSER 30//Valor maximo para el nombre del archivo generado del usuario
#define LIMP_BUFFER fseek(stdin,0,SEEK_END) //Limpia el buffer, puesto en duda

//Estructura que sirve para obtener los datos del archivo de los usuarios 
typedef struct
{
    char nom[21];//Nombre del usuario
    char clave[11];//contraseña del usuario
}User;

void gestionar(const char archUs[]);//Funcion que no devuelve nada y como parametro de entrada tiene una cadena constante
int menu(const char nomArch[],int nDatos,const char mensaje[]);//Funcion que devuelve un entero y como parametros de entrada dos cadenas constantes y un entero
int validacionEntrada( int minRang,int maxRang,char *auxTexto );//Funcion regresa un int y tiene como datos de entrada dos int y una cadena
User ingresar(void);//Funcion que devuelve una estructura del tipo User y no acepta ningun parametro
User crearUsuario(void);//Funcion que devuelve una estructura del tipo User y no acepta ningun parametro
User buscarUsuario(char nomUsuario[]);//Funcion que devuelve una estructura del tipo User y pide una cadena como dato de entrada

//Funcion principal (no acepta ningun dato)
int main(void)
{
    int menuOp;//VAriable int para almacenar la opcion del menu seleccionada
    char archUs[MAXARCHUSER];//Cadena para almacenar el nombre del archivo de un usuario

    //Ciclo para repetir el menu siempre y cuando no se elija salir
    while((menuOp = menu(ARCHGESMENU,3,"INGRESA O CREA UN USUARIO PARA CONTINUAR")) != 3)
    {
        switch(menuOp)//Switch que permite dirigir al usuario segun lo que elija
        {
            /*Guarda el nombre del usuario (proveniente de la funcion ingresar en el atributo nom) y le agrega la extencion .pdf 
              y luego llama a la funcion gestioinar pasandole como parametro de entrada archUs*/
            case 1:     sprintf(archUs,"%s.txt",ingresar().nom);
                        if(strcmp(archus.nom[0],".txt"))//Se asegura que se haya encontrado una cuenta
                            gestionar(archUs);
                break;
            /*Guarda el nombre del usuario (proveniente de la funcion crearUsuario en el atributo nom) y le agrega la extencion .pdf 
              y luego llama a la funcion gestioinar pasandole como parametro de entrada archUs*/
            case 2:     sprintf(archUs,"%s.txt",crearUsuario().nom);
                        gestionar(archUs);
                break;
        }
    }

    return 0;
}

//Funcion que despliega un menu u opciones de un archivo de texto (preexistente) con entradas validadas
int menu(const char nomArch[],int nDatos,const char mensaje[])
{
    FILE *menuArch;//Variable para gestionar el archivo
    char opcion[25];//Variable para obtener los datos del archivo

    printf("\n\n\t%s\n\n",mensaje);//Muestra un mensaje para dar a entender de que es el menu

    menuArch = fopen(nomArch,"r");//Abre el archivo de texto (este debe existir previamente)

    //Despliega lo contenido en el archivo (Solo si abrio corrrectamente el archivo)
    if(menuArch)
        while(fgets(opcion,24,menuArch))
            printf("%s",opcion);

    fclose(menuArch);//Cierra el archivo
    puts("\n");//Dos saltos de linea para un poco mas de presentacion

    //Regresa el valor que regresa la funcion validacionEntrada
    return  validacionEntrada(1,nDatos,"");
} 

//Funcion que pedira un usuario y una contraseña para ingresar al gestor
User ingresar(void)
{
    char usuario[21],clave[15];//VAriables para almacenar los datos del usuario ingresados por el usuario
    User actual;//Varibale para almacenar los datos de una cuenta que cumpla con lo ingresado por el usuario

    //Pide el nombre del usuario para luego a partir de la funcion buscarUsuario buscarlo
    printf("Ingresa usuario: ");
    fgets(usuario,20,stdin);
    usuario[strcspn(usuario,"\n")] = 0;
    actual = buscarUsuario(usuario);

    if(actual.nom[0])//Se asegura que se haya encontrado una cuenta que cumpla con lo ingresado por el usuario
    {
        do
        {
            //Le pide la calve al usuario
            printf("\nIngrese la clave: ");
            fgets(clave,14,stdin);
            clave[strcspn(clave,"\n")] = 0;
        }while(strcmp(clave,actual.clave));//Mientras que no se escriba la clave correcta no se sale del bucle
    }
    else
        puts("\nNo se encontro ninguna cuenta");//Si no se encontro la cuenta manda un mensaje de error

    return actual;//Regresa la cuenta del usuario más cercana a lo que se encontro
}  

//Crea una cuenta nueva
User crearUsuario(void)
{
    FILE *usuariosArch;//Variable para el archivo que almacena a todos los usuarios
    User nuevo;//Estructura User para posteriormente agregar al archivo de los usuarios

    //Pide los datos del usuario (nombre y contraseña)
    printf("\nIngrese el nombre del usuario: ");
    fgets(nuevo.nom,20,stdin);
    nuevo.nom[strcspn(nuevo.nom,"\n")] = 0;

    printf("\nIngrese la clave: ");
    fgets(nuevo.clave,10,stdin);
    nuevo.clave[strcspn(nuevo.clave,"\n")] = 0;

    //Abre el archivo(Binario) de usuarios (exista o no)
    usuariosArch = fopen(ARCHUSUARIOS,"ab");

    //Mete la información nueva al archivo y posteriormente se cierra
    fwrite(&nuevo,sizeof(User),1,usuariosArch);
    fclose(usuariosArch);

    //Mensaje de exito
    puts("Cuenta creada con exito");

    return nuevo;//Regresa la cuenta creada
}

//Funcion para buscar al usuario en el archivo de usuario tiene como parametro de entrada la cadena con el nombre del usuario a buscar
User buscarUsuario(char nomUsuario[])
{
    FILE *usuariosArch;//Variable para el archivo que almacena a todos los usuarios
    User buscador;//Estructura para buscar dentro del archivo
    int cont=1,crear;//Variables para controlar la busqueda en el archivo y la creacion de una cuenta (de ser necesario)

    usuariosArch = fopen(ARCHUSUARIOS,"rb");//Abre el archivo(binario) de las cuentas

    if(usuariosArch)//si se abrio correctamente
    {
        //Busca con un while las coincidencias en el archivo
        while(cont && fread(&buscador,sizeof(User),1,usuariosArch))
            if(!strcmp(buscador.nom,nomUsuario))
                cont = 0;

        fclose(usuariosArch);//Cierra el archivo

        //Si no se encontro el usuario da la opcion de crear un nuevo usuario (con entradas validadas)
        if(cont)
        {
            puts("\nNo se encontro usuario");
            
            puts("\nCrear usuario?");
            puts("[1]Si");
            puts("[2]No");
            crear = validacionEntrada(1,2,"");

            //Si se opcta por crear un nuevo usuario lo manda a crear, sino se le da un valor al arreglo para determinar que no se encontró el usuario
            if(crear-2 && crear != OPC_SALIR)
                buscador = crearUsuario();
            else
                strcpy(buscador.nom,"");
        }
        else
            puts("Cuenta encontrada");//Mensaje de exito si si se encuentra el usuario
    }
    //Sino abre correctamente el archivo (o no existe) da la opcion para crear un usuario como excusa para crear el archivo (Es la misma forma que la anterior opcion de crear cuenta)
    else
    {
            puts("\nNo se encontro usuario");
            
            puts("\nCrear usuario?");
            puts("[1]Si");
            puts("[2]No");
            crear = validacionEntrada(1,2,"");

            if(crear-2 && crear != OPC_SALIR)
                buscador = crearUsuario();
            else
                strcpy(buscador.nom,"");
    }

    return buscador;//Regresa el usuario que se encontro, que se creo o en su defecto una forma para identificar que no se encontro nada
}

//Funcion que valida que la entrada este en el rango especificado
int validacionEntrada( int minRang,int maxRang,char *auxTexto )
{
        int opcion,opcInvalida,repetirOpcInvalida=1;
        do
        {
                if(repetirOpcInvalida==1)//En caso de digitar dos sale del bucle y no vuelve a preguntar
                {
                        printf( "Digite la opcion entre %i y %i%s:  ",minRang,maxRang,auxTexto );
                        scanf("%i",&opcion);
                        LIMP_BUFFER;//limpiamos el buffer
                        opcInvalida=( opcion>maxRang || opcion<minRang);//Verifica si es valida la opcion digita dentro de un rango de valores 
                        if( opcInvalida )
                        {
                                do//Bucle que hace que el usuario escoja entre volver a digitar o volver al menu
                                {
                                        printf( "----------Opcion invalida-----------\n"  );
                                        printf( "----------¿Desea repetir?----------\n" );
                                        printf( "[1]Si\n[2]No,sair al menu\n-->" );
                                        scanf( "%i",&repetirOpcInvalida );
                                        LIMP_BUFFER;
                                }while( repetirOpcInvalida>2 || repetirOpcInvalida<1 );
                        }
                }
                else
                {
                        opcInvalida=0;//Se asigna el valor de 0 para salir del bucle
                        opcion=OPC_SALIR;//Este valor va a actuar como un indicador para volver al inicio
                }
        }while( opcInvalida );
        return opcion;
}

//Funcion que como parametro de entrada tiene una cadena que simbolisa el nombre del archivo del usuario
void gestionar(const char archUs[])
{
    int menuOp=0;//Variable para indicar la opcion digitada por el usuario

    //Mientras no se seleccione la opcion cuatro (o la opcion de salida al menu principal) se repetira el menu
    while(menuOp-4 && menuOp != OPC_SALIR)
    {
        /*Llama a la funcion menu (con los datos del nombre del archivo donde estan contenidas las opciones la cantidad de opciones y
          un mensaje para la presentacion dle menu) y guarda la opcion que regresa la funcion*/
        menuOp = menu(ARCHOPGESTOR,4,"BIENVENIDO, ELIGE UNA OPCION PARA CONTINUAR");

        //Segun la opcion digitada hará la funcionalidad digitada (se esperan las funciones respectivas para ejecutarlas)
        switch(menuOp)
        {
            case 1:     ;
                break;
            case 2:     ;
                break;
            case 3:     ;
                break;
        }
    }
}





////////////////////////////CAMBIOS PROXIMAMENTE (Talvez Recorte de funcional)
void recorrer(char recipiente[],char valor,int tam)
{
    int i,pos=0;
    char cupo=1;

    for(i=0;i<tam && cupo;i++)
    {
        if(!recipiente[i] && i!= tam-1)
        {
            pos = i;
            cupo = 0;
        }
        if(i==tam-1 && recipiente[i])
            cupo = 0;
    }

    if(!cupo)
    {
        recipiente[pos] = valor;
        recipiente[pos+1] = 0;
    }
}

char *operarX(char cad[])
{
    int lon = strlen(cad)/2,i;
    char prt1[lon+1]={},prt2[lon+1]={},res[7]={};
    char *mensaje;

    strncpy(prt1,cad,lon);
    strncpy(prt2,cad+lon,lon);

    for(i=1;i<lon-1;i++)
        recorrer(res,(prt1[i]*prt2[i+1])%94+32,sizeof(res));
    
    for(i=lon-2;i>0;i--)
        recorrer(res,(prt1[i]*prt2[i-1])%94+32,sizeof(res));

    mensaje=res;

    return mensaje;
}

void cambia(char cad[])
{
    int i, lon=strlen(cad);
    
    for(i=0;i<lon;i++)
    {
        if(cad[i+1])
            cad[i] = (cad[i]^cad[i+1])%94+32;
        else
            cad[i] = (cad[i]^cad[0])%94+32;
    }
}

void completar(char contra[])
{
    int lon=strlen(contra),falta = 10-lon,i;
    
    for(i=0;i<falta;i++)
        recorrer(contra,(contra[i]^contra[lon-1-i])%94+32,11);
}

void encripta(char contra[])
{
    int lon;
    char encrip[20]={},aux[10]={};

    if(strlen(contra) < 10)
        completar(contra);

    cambia(contra);
    strcpy(encrip,contra);
    strcpy(aux,operarX(encrip));
    strcat(encrip,aux);
    strcpy(aux,operarX(aux));
    strcat(encrip,aux);

    puts(encrip);
}