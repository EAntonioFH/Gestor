#include <stdio.h>
#include <string.h>
#include <ctype.h> 

#define ARCHUSUARIOS "usuarios.us"//Nombre del archivo binario donde van los usuarios
#define ARCHGESMENU "menuGestor.txt"//Nombre del archivo donde esta el menu principal
#define ARCHOPGESTOR "opcionesGestor.txt"//Nombre del archivo donde estan lasa opciones del gestor
#define OPC_SALIR 100 //Valor por defecto si se desea volver al menu segun las opciones
#define MAXARCHUSER 30//Valor maximo para el nombre del archivo generado del usuario
#define LIMP_BUFFER while (getchar() != '\n' && !feof(stdin)) //Limpia el buffer de entreda

#define CANT_PASSW 15
#define LONGMAX_PASSW 30
#define LONGMAX_TITULO 50
#define LONGMAX_LINK 100
#define LONGMAX_LINEA LONGMAX_LINK + 2

//Estructura que sirve para obtener los datos del archivo de los usuarios 
typedef struct
{
    char nom[21];//Nombre del usuario
    char clave[11];//contraseña del usuario
}User;


struct passwd {
    char titulo[LONGMAX_TITULO];
    char passw[LONGMAX_PASSW];
    char link[LONGMAX_LINK];
};

void gestionar(const char archUs[]);//Funcion que no devuelve nada y como parametro de entrada tiene una cadena constante
int menu(const char nomArch[],int nDatos,const char mensaje[]);//Funcion que devuelve un entero y como parametros de entrada dos cadenas constantes y un entero
int validacionEntrada( int minRang,int maxRang,char *auxTexto );//Funcion regresa un int y tiene como datos de entrada dos int y una cadena
User ingresar(void);//Funcion que devuelve una estructura del tipo User y no acepta ningun parametro
User crearUsuario(void);//Funcion que devuelve una estructura del tipo User y no acepta ningun parametro
User buscarUsuario(char nomUsuario[]);//Funcion que devuelve una estructura del tipo User y pide una cadena como dato de entrada
void caratula(void);
void leerPasswds(struct passwd contrasenas[],const char archPasswd[]);
int contarContrasenasGuardadas(struct passwd contrasenas[]);
int encontrarEspacioVacio(struct passwd contrasenas[]);
void agregarNuevaContrasena(struct passwd contrasenas[]);
void mostrarContrasenas( struct passwd contrasenas[], int isBorrar );
void modificarDatos( struct passwd contrasenas[],int indice );
void eliminarContrasena(struct passwd contrasenas[]);
void recorrerContrasena(struct passwd contrasenas[]);
void validarContra(const char *contrasena, int validaciones[5]);
void ingresaPassw(struct passwd *clave);
void cambioArchivo(struct passwd contrasenas[],const char archPasswd[],int cantidadPasswd);


//Funcion principal (no acepta ningun dato)
int main(void)
{
    int menuOp;//VAriable int para almacenar la opcion del menu seleccionada
    char archUs[MAXARCHUSER];//Cadena para almacenar el nombre del archivo de un usuario

    caratula();

    //Ciclo para repetir el menu siempre y cuando no se elija salir
    while((menuOp = menu(ARCHGESMENU,3,"INGRESA O CREA UN USUARIO PARA CONTINUAR")) != 3)
    {
        switch(menuOp)//Switch que permite dirigir al usuario segun lo que elija
        {
            /*Guarda el nombre del usuario (proveniente de la funcion ingresar en el atributo nom) y le agrega la extencion .pdf 
              y luego llama a la funcion gestioinar pasandole como parametro de entrada archUs*/
            case 1:     sprintf(archUs,"%s.txt",ingresar().nom);
                        if(strcmp(archUs,".txt"))//Se asegura que se haya encontrado una cuenta
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

    printf("\n\t%s\n\n",mensaje);//Muestra un mensaje para dar a entender de que es el menu

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
    int menuOp=0, total;//Variable para indicar la opcion digitada por el usuario y para obtener el total actual de contraseñas
    struct passwd contrasenas[CANT_PASSW];

    leerPasswds(contrasenas,archUs);

    //Mientras no se seleccione la opcion cuatro (o la opcion de salida al menu principal) se repetira el menu
    while(menuOp-4 && menuOp != OPC_SALIR)
    {

        total = contarContrasenasGuardadas(contrasenas);
        printf("\n\nTienes %d contraseñas guardadas de %d disponibles.", total, CANT_PASSW);

        /*Llama a la funcion menu (con los datos del nombre del archivo donde estan contenidas las opciones la cantidad de opciones y
          un mensaje para la presentacion dle menu) y guarda la opcion que regresa la funcion*/
        menuOp = menu(ARCHOPGESTOR,4,"");

        //Segun la opcion digitada hará la funcionalidad digitada (se esperan las funciones respectivas para ejecutarlas)
        switch(menuOp)
        {
            case 1:     agregarNuevaContrasena(contrasenas);
                break;
            case 2:     mostrarContrasenas(contrasenas,1);
                break;
            case 3:     eliminarContrasena(contrasenas);
                break;
        }
    }

    cambioArchivo(contrasenas,archUs,total);
}

void caratula(void)
{
    FILE *arch;
    char lin[75];
    
    arch = fopen("caratula.txt","r");
    if(arch)
    {
        while(fgets(lin,75,arch))
            puts(lin);
    }
    else
        puts("No se encontro la caratula");

    fclose(arch);

    printf("\nPresiona ENTER para continuar...");
    while (getchar() != '\n');
}

//Funciones nuevas

void leerPasswds(struct passwd contrasenas[],const char archPasswd[]) {
    FILE *archivo = fopen(archPasswd, "r");
    char linea[LONGMAX_LINEA];
    int i;

    if (archivo == NULL) {
        archivo = fopen(archPasswd, "w");
        fclose(archivo);
        archivo = fopen(archPasswd, "r");
    }

    for (i = 0; i < CANT_PASSW; i++) {
        contrasenas[i].titulo[0] = '\0';
        contrasenas[i].passw[0] = '\0';
        contrasenas[i].link[0] = '\0';

        if (fgets(linea, LONGMAX_LINEA, archivo) != NULL) {
            linea[strcspn(linea, "\n")] = '\0';
            strncpy(contrasenas[i].titulo, linea, LONGMAX_TITULO);
        }
        if (fgets(linea, LONGMAX_LINEA, archivo) != NULL) {
            linea[strcspn(linea, "\n")] = '\0';
            strncpy(contrasenas[i].passw, linea, LONGMAX_PASSW);
        }
        if (fgets(linea, LONGMAX_LINEA, archivo) != NULL) {
            linea[strcspn(linea, "\n")] = '\0';
            strncpy(contrasenas[i].link, linea, LONGMAX_LINK);
        }
    }
    fclose(archivo);
}

int contarContrasenasGuardadas(struct passwd contrasenas[]) {
    int contador = 0;
    for (int i = 0; i < CANT_PASSW; i++) {
        if (strlen(contrasenas[i].titulo) > 0) contador++;
    }
    return contador;
}

int encontrarEspacioVacio(struct passwd contrasenas[]) {
    for (int i = 0; i < CANT_PASSW; i++) {
        if (contrasenas[i].titulo[0] == '\0')
            return i;
    }
    return -1;
}

void agregarNuevaContrasena(struct passwd contrasenas[]) {
    int espacio = encontrarEspacioVacio(contrasenas);

    if (espacio == -1) {
        printf("No hay espacio disponible para agregar más contraseñas.\n");
        return;
    }

    printf("\n=== AGREGAR NUEVA CONTRASEÑA ===\n");
    
    printf("Nombre o sitio: ");
    fgets(contrasenas[espacio].titulo, LONGMAX_TITULO, stdin);
    contrasenas[espacio].titulo[strcspn(contrasenas[espacio].titulo, "\n")] = 0;

    ingresaPassw(&contrasenas[espacio]);

    printf("Link o URL: ");
    fgets(contrasenas[espacio].link, LONGMAX_LINK, stdin);
    contrasenas[espacio].link[strcspn(contrasenas[espacio].link, "\n")] = 0;

    printf("Contraseña agregada exitosamente.\n");
    
    printf("\nPresiona ENTER para continuar...");
    while (getchar() != '\n');
}


void mostrarContrasenas( struct passwd contrasenas[], int isBorrar ) {
    int hayDatos = 0,opcion;

    printf("\n=== LISTADO DE CONTRASEÑAS ===\n");

    for (int i = 0; i < CANT_PASSW; i++) {
        if (strlen(contrasenas[i].titulo) > 0) {
            hayDatos = 1;
            printf("[%d] %s\n   Contraseña: %s\n   Link: %s\n",
                   i + 1, contrasenas[i].titulo, contrasenas[i].passw, contrasenas[i].link);
        }
    }
    if (!hayDatos)
        printf("(No hay contraseñas registradas)\n");

    if(isBorrar)
    {
            //Bucle que permite editar multiples contrasenas sin salir del menu
        do{
            printf("\n¿Deseas editar alguna contrasena especifica?\n");
            printf("[1] Si\n[2] No\n> ");
            opcion=validacionEntrada(1,2,"");
    
            if(opcion==1) {
                int indice;
                printf("Introduce el numero de la contrasena a editar: ");
                scanf("%d",&indice);
                LIMP_BUFFER;
    
                if(indice<1 || indice>CANT_PASSW || strlen(contrasenas[indice-1].titulo)==0)
                    printf("Opcion invalida o contrasena vacia.\n");
                    else 
                        modificarDatos(contrasenas,indice-1);
            }
            else if(opcion==OPC_SALIR)
                opcion==1;
        }while(opcion==1);
    }
}

//Funcion para modificar los datos de una contrasena seleccionada
void modificarDatos( struct passwd contrasenas[],int indice )
{
        int opcion;

        printf("\n=== EDITAR CONTRASENA ===\n");
        printf("Actualmente seleccionaste: %s\n",contrasenas[indice].titulo);
        printf("\n¿Qué deseas cambiar?\n");
        printf("[1] Nombre o titulo\n");
        printf("[2] Contrasena\n");
        printf("[3] Link o URL\n");
        printf("[4] Cancelar\n");
        opcion=validacionEntrada(1,4,"");

        if(opcion==1) {
                printf("Nuevo titulo: ");
                fgets(contrasenas[indice].titulo,LONGMAX_TITULO,stdin);
                contrasenas[indice].titulo[strcspn(contrasenas[indice].titulo,"\n")]=0;
                printf("Titulo actualizado correctamente.\n");
        }
        else if(opcion==2) {
                ingresaPassw(&contrasenas[indice]);
                printf("Contrasena actualizada correctamente.\n");
        }
        else if(opcion==3) {
                printf("Nuevo link/URL: ");
                fgets(contrasenas[indice].link,LONGMAX_LINK,stdin);
                contrasenas[indice].link[strcspn(contrasenas[indice].link,"\n")]=0;
                printf("Link actualizado correctamente.\n");
        }
        else
                printf("Operacion cancelada.\n");
}

void eliminarContrasena(struct passwd contrasenas[]) {
    int indice, confirm;
    printf("\n=== ELIMINAR CONTRASEÑA ===\n");
    mostrarContrasenas(contrasenas,0);
    printf("Selecciona el número de la contraseña que deseas eliminar: ");
    scanf("%d", &indice);
    LIMP_BUFFER;
    if (indice < 1 || indice > CANT_PASSW || strlen(contrasenas[indice - 1].titulo) == 0) {
        printf("Opción inválida o contraseña vacía.\n");
        return;
    }
    printf("¿Seguro que deseas eliminar '%s'?\n[1] Sí\n[2] No\n> ", contrasenas[indice - 1].titulo);
    scanf("%d", &confirm);
    LIMP_BUFFER;
    if (confirm == 1) {
        contrasenas[indice - 1].titulo[0] = '\0';
        contrasenas[indice - 1].passw[0] = '\0';
        contrasenas[indice - 1].link[0] = '\0';
        printf("Contraseña eliminada correctamente.\n");

        recorrerContrasena(contrasenas);
    } else {
        printf("Operación cancelada.\n");
    }

    printf("\nPresiona ENTER para continuar...");
    while (getchar() != '\n');
}

void recorrerContrasena(struct passwd contrasenas[])
{
	int i,j;
	int n=contarContrasenasGuardadas(contrasenas);
	for(i=0;i<CANT_PASSW-1;i++)
	{
		if(contrasenas[i].titulo[0] == '\0')
		{
			for(j=i;j<CANT_PASSW-1;j++)
			{
				strncpy(contrasenas[j].titulo,contrasenas[j+1].titulo,LONGMAX_TITULO);
				strncpy(contrasenas[j].passw,contrasenas[j+1].passw,LONGMAX_PASSW);
				strncpy(contrasenas[j].link,contrasenas[j+1].link,LONGMAX_LINK);
			}
			contrasenas[CANT_PASSW - 1].titulo[0] = '\0';
        	contrasenas[CANT_PASSW - 1].passw[0] = '\0';
        	contrasenas[CANT_PASSW - 1].link[0] = '\0';
		}
	}
}

void validarContra(const char *contrasena, int validaciones[5]) {
    enum { LONG, MAYUS, MINUS, NUMS, ESPECIALES };
    int i;
    for (i = 0; contrasena[i] != '\0'; i++) {
        if (!validaciones[MAYUS]) validaciones[MAYUS] = isupper(contrasena[i]);
        if (!validaciones[MINUS]) validaciones[MINUS] = islower(contrasena[i]);
        if (!validaciones[NUMS]) validaciones[NUMS] = isdigit(contrasena[i]);
        if (!validaciones[ESPECIALES]) validaciones[ESPECIALES] = !isalnum(contrasena[i]);
    }
    validaciones[LONG] = (i >= 8) ? 1 : 0;
}

void ingresaPassw(struct passwd *clave) {
    char ingreso[LONGMAX_PASSW];
    int valida[5] = {}, validar, i;

    do {
        validar = 0;
        memset(valida, 0, sizeof(valida));

        printf("Ingresa la nueva contraseña: ");
        fgets(ingreso, LONGMAX_PASSW, stdin);
        ingreso[strcspn(ingreso, "\n")] = 0;

        validarContra(ingreso, valida);

        for (i = 0; i < 5; i++)
            if (!valida[i]) {
                validar = 1;
                printf("No cumple con: %s\n",
                       (i == 0) ? "mínimo 8 caracteres"
                                : (i == 1) ? "una letra mayúscula"
                                : (i == 2) ? "una letra minúscula"
                                : (i == 3) ? "un número"
                                           : "un carácter especial");
            }

    } while (validar);

    strcpy(clave->passw, ingreso);
}

void cambioArchivo(struct passwd contrasenas[],const char archPasswd[],int cantidadPasswd) {
    FILE *archivo = fopen(archPasswd, "w");
    for (int i = 0; i < CANT_PASSW && i != cantidadPasswd-1; i++) {
        fprintf(archivo, "%s\n", contrasenas[i].titulo);
        fprintf(archivo, "%s\n", contrasenas[i].passw);
        fprintf(archivo, "%s\n", contrasenas[i].link);
    }
    fclose(archivo);
}