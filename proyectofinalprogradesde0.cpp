#include <iostream>
#include<fstream>
#include <conio.h>
#include<windows.h>
#include <cstring>
#include<time.h>
#include<stdlib.h>
#include <chrono>
#include <thread>
using namespace std;
#define T 15
#define TAM 30
#define RUTA 70
#define color_ASTEROIDE  14
#define color_Nave 11
#define color_NaveconEscudo 2
#define color_Naveenemiga  3
#define color_disparos 15
#define color_Pot 15
typedef struct{//coordenadas
    int x, y, direc;
}coordenadas;
typedef struct{//estado
    int vida;
    int mov;
    int show;
    bool principal;
}estado;
typedef struct{//partidas
    char nombre[TAM];
    int pts;
}partidas;
typedef struct{//objetos
    int tipo;// nave 1, nave enemiga 2, asteroide -1, disparos 0, escudo 3, vidaextra 4
    coordenadas pos;
    estado carac;
    char mascara[T];
}objetos;
typedef struct{//infoPartida
    int dificultad, puntaje;
    objetos navesita, navesita2;
    bool asociadaEnemigos[T];
    objetos enemigos[T];
    bool asociadadisp[T];
    objetos disparos[T];
    bool escudo;
}infoPartida;
void ocultarCursor(){
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void gotoxy(short x, short y){
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void color(WORD col){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
    //1 Azul
    //2 Verde
    //3 Celeste
    //4 Rojo
    //5 Morado
    //6 Amarillo
    //7 Blanco
    //8 Gris
    //9 Otro Azul
    //10 Otro verde
    //11 Celeste bonito
    //12 Rojo naranja
    //13 Morado rosado
    //14 Amarillo claro
    //15 Blanco
}
void imprimirMargen(int ancho, int alto){
    gotoxy(0,0);
    color(15);
    cout<<"+";
    for(int i=0;i<ancho-2;i++){cout<<"-";}
    cout<<"+"<<endl;
    for(int i=0;i<alto-2;i++){
            cout<<"|";
            for(int j=0; j<ancho-2; j++){cout<<" ";}
            cout<<"|"<<endl;
    }
    cout<<"+";
    for(int i=0;i<ancho-2;i++){cout<<"-";}
    cout<<"+";
    ocultarCursor();
}
void imprimirMargenDoble(int ancho, int alto){
    gotoxy(0,0);
    color(13);
    cout<<"+";
    for(int i=0;i<ancho-2;i++){cout<<"-";}
    cout<<"+"<<endl;
    for(int i=0;i<alto-2;i++){
            cout<<"|";
            int j;
            for(j=0; j<(ancho-3)/2; j++){cout<<" ";}
            cout<<"|";
            for(j;j<ancho-3;j++){cout<<" ";}
            cout<<"|"<<endl;
    }
    cout<<"+";
    for(int i=0;i<ancho-2;i++){cout<<"-";}
    cout<<"+";
    ocultarCursor();
}
void imprimir(int col, int fila, char palabra[TAM], int tinte){
    color(tinte);
    gotoxy(col, fila);
    cout<<palabra;
    ocultarCursor();
}
void borrarCuadro(int col, int fila, int ancho, int alto){
    for(int j=0;j<alto;j++){
        gotoxy(col, fila+j);
        color(0);
        for(int i=0;i<ancho;i++){cout<<" ";}
    }
    ocultarCursor();
}
void imprimirVida(infoPartida data){
    borrarCuadro(30,28, 110, 1);
    color(13);
    gotoxy(30, 28);
    cout<<"Vida: "<<data.navesita.carac.vida<<"        Puntaje:"<<data.puntaje<<"      Dificultad: "<< data.dificultad;
    if(data.escudo){cout<<"         ESCUDO ACTIVO";}
    if(data.navesita2.carac.show==-10){cout<<"              Vida: "<<data.navesita2.carac.vida;}
    ocultarCursor();
}
void mostrarMenu(int col, int fila, char titulo[TAM],int cantOp, int opcionSeleccionada, char lista[TAM][TAM]){
    borrarCuadro(col, fila, TAM, 7);
    char texto[TAM]="";
    imprimir(col,fila, titulo, 10);
    for(int i=0; i<cantOp;i++){
        if(opcionSeleccionada==i){
            strcat(texto, "-> ");
            strcat(texto, lista[i]);
        }
        else{
            strcat(texto, "   ");
            strcat(texto, lista[i]);
        }
        imprimir(col, fila+i+1, texto , 10);
        strcpy(texto, "");
    }
}
void selec(int col, int fila, char titulo[TAM],int cantOp, int& opcionSeleccionada, char lista[TAM][TAM]){
    int tecla;
    mostrarMenu(col, fila, titulo,cantOp,opcionSeleccionada, lista);
    while (true) {
        tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72) {
                opcionSeleccionada--;
                if (opcionSeleccionada < 0){opcionSeleccionada = cantOp-1;}
            }
            else if (tecla == 80) {
                opcionSeleccionada++;
                if (opcionSeleccionada > cantOp-1){opcionSeleccionada = 0;}
            }
            mostrarMenu(col, fila,titulo,cantOp,opcionSeleccionada, lista);
        }
        else if (tecla == 13) {
            imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
            tecla=_getch();
            borrarCuadro(25, 27, 100, 2);
            break;
        }
    }
}
void seleccionarCantJugadores(int& opcionSeleccionada){
    char lista[TAM][TAM];
    strcpy(lista[0], "Un Jugador");
    strcpy(lista[1], "Dos jugadores");
    selec(45, 11, "Cantidad de jugadores ", 2, opcionSeleccionada, lista);
}
void elejirModo(int& opcionSeleccionada){
    char lista[TAM][TAM];
    strcpy(lista[0], "VS");
    strcpy(lista[1], "Muerte Subita");
    strcpy(lista[2], "Retroceder");
    selec(45, 11, "Elija el modo de juego ", 3, opcionSeleccionada, lista);
}
void seleccionarDif(int& opcionSeleccionada){
    char lista[TAM][TAM];
    strcpy(lista[0], "Facil");
    strcpy(lista[1], "Intermedio");
    strcpy(lista[2], "Dificil");
    selec(45, 11, "Elija la dificultad de juego ", 3, opcionSeleccionada, lista);
}
void instruccionesjuegoPrincipal(){
    char lista[TAM][TAM];
    int op=0;
    strcpy(lista[1], "Omitir Tutorial");
    strcpy(lista[0], "Tutorial");
    selec(45, 11, "¿Desea ver el Tutorial? ", 2, op, lista);
    if(op==0){
        imprimir(40, 8, "El juego consiste en acumular ", 10);
        imprimir(40, 9, "la mayor cantidad de puntos antes de morir,", 10);
        imprimir(40, 10, "se consiguen puntos eliminando a los enemigos", 10);
        imprimir(40, 11, "al derribar un asteroide se consigue 100pts, ", 10);
        imprimir(40, 12, "para matar una nave enemiga se necesitan dos disparos", 10);
        imprimir(40, 13, "y te da 500pts, ademas hay potenciadores que te ayudaran", 10);
        imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
        getch();
        borrarCuadro(40, 7, 80, 8);
        borrarCuadro(40, 27, 50, 2);
        imprimir(40, 8, "Los potenciadores son simbolizados por: ", 10);
        imprimir(40, 9, "una O para activar el escudo y un @ para recuperar vida,", 10);
        imprimir(40, 10, "cuando el escudo es activado aparece en la parte inferior", 10);
        imprimir(40, 11, "de la pantalla y a su vez la nave cambia de color ", 10);
        imprimir(40, 12, "el escudo desaparece tras un cierto tiempo", 10);
        imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
        getch();
        borrarCuadro(40, 7, 80, 8);
        borrarCuadro(40, 27, 50, 2);
        imprimir(40, 8, "Inicias con una vida de 100  ", 10);
        imprimir(40, 9, "reduce en 5 si un asteroide lleva hasta el piso,", 10);
        imprimir(40, 10, "reduce en 20 si algun asteroide choca con la nave,", 10);
        imprimir(40, 11, "reduce 15 si algun disparo de las naves enemigas te da,", 10);
        imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
        getch();
        borrarCuadro(40, 7, 80, 8);
        borrarCuadro(40, 27, 50, 2);
        imprimir(40, 8, "El movimiento de la nave consiste en ", 10);
        imprimir(40, 9, "presionar la tecla a para ir a la izquierda, ", 10);
        imprimir(40, 10, "presionar la tecla d para ir a la derecha,", 10);
        imprimir(40, 11, "presionar el espacio para disparar,", 10);
        imprimir(40, 12, "Al aumentar la dificulta aumentara tu velocidad,", 10);
        imprimir(40, 13, "la cantidad de enemigos y la velocidad de estos", 10);
        imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
        getch();
    }
}
void guardarPartidaActual(infoPartida& data){
    char nombre[TAM];
    imprimir(40, 13, "Ingrese el nombre del jugador:  ", 12);
    char buscarPartidaen[RUTA]="D:\\Cato\\Segundo semestre\\Progra 1\\";
    cin>>nombre;
    partidas act;
    strcpy(act.nombre, nombre);
    act.pts=data.puntaje;
    strcat(buscarPartidaen, nombre);
    strcat(buscarPartidaen, ".dat");
    ofstream file;
    file.open("D:\\Cato\\Segundo semestre\\Progra 1\\partidasAnteriores.dat", ios::binary|ios::app);
    file.write((char*)& act, sizeof(act));
    file.close();
    fstream filesaver;
    filesaver.open(buscarPartidaen, ios::binary|ios::out);
    filesaver.write((char*)& data, sizeof(data));
    filesaver.close();
}
void guardarPuntajeActual(infoPartida& data){
    char nombre[TAM];
    imprimir(40, 13, "Ingrese el nombre del jugador:  ", 12);
    cin>>nombre;
    partidas aux;
    strcpy(aux.nombre, nombre);
    aux.pts=data.puntaje;
    ofstream file;
    file.open("D:\\Cato\\Segundo semestre\\Progra 1\\ranking.dat", ios::binary|ios::app);
    file.write((char*)& aux, sizeof(aux));
    file.close();
}
void revisarRanking(){
    partidas aux;
    int i=0;
    ifstream file;
    file.open("D:\\Cato\\Segundo semestre\\Progra 1\\ranking.dat", ios::binary);
    int n;
    partidas p[T];
    file.seekg(0,ios::end);
    n=file.tellg();
    file.seekg(0, ios::beg);
    n=n/sizeof(p[0]);
    for(i=0;i<n;i++){
        file.read((char*)& p[i], sizeof(p[i]));
    }
    for(int k=0;k<n;k++){
        for(int j=0;j<(n-1);j++){
            if(p[j].pts<p[j+1].pts){
                aux=p[j+1];
                p[j+1]=p[j];
                p[j]=aux;
            }
        }
    }
    file.close();
    ofstream files;
    files.open("D:\\Cato\\Segundo semestre\\Progra 1\\ranking.dat", ios::binary);
    if(n>T){n=T;}
    for(int j=0; j<n;j++){
        files.write((char*)&p[j], sizeof(p[j]));
    }
    files.close();
}
void verRanking(){
    partidas ranking[T];
    int i=0;
    ifstream file;
    file.open("D:\\Cato\\Segundo semestre\\Progra 1\\ranking.dat", ios::binary);
    int n;
    partidas p[T];
    file.seekg(0,ios::end);
    n=file.tellg();
    file.seekg(0, ios::beg);
    n=n/sizeof(p[0]);
    for(i=0;i<=n;i++){
        file.read((char*)& p[i], sizeof(p[i]));
    }
    file.close();
    borrarCuadro(40, 7, TAM, T+1);
    char texto[TAM]="";
    imprimir(40,7, "Top mejores puntajes:", 10);
    imprimir(40,8, "Nombre                     Puntaje", 10);
    for(int j=0; j<n;j++){
        strcpy(texto, p[j].nombre);
        imprimir(40, 9+j, texto , 10);
        for(int k=strlen(texto); k<TAM;k++){
                cout<<" ";
        }
        cout<<p[j].pts;
    }
}
void obtenerDatosPartidaAntigua(char buscar[RUTA], infoPartida&data){
    fstream filesaver;
    filesaver.open(buscar, ios::binary|ios::in);
    filesaver.read((char*)& data, sizeof(data));
    filesaver.close();
}
void elejirPartida(infoPartida& data){
    fstream file;
    file.open("D:\\Cato\\Segundo semestre\\Progra 1\\partidasAnteriores.dat", ios::binary|ios::in|ios::out);
    int n;
    partidas p[T];
    char lista[TAM][TAM];
    file.seekg(0,ios::end);
    n=file.tellg();
    file.seekg(0, ios::beg);
    n=n/sizeof(p[0]);
    for(int i=1;i<=n;i++){
        file.read((char*)& p[i], sizeof(p[i]));
        strcpy(lista[i],p[i].nombre);
    }
    strcpy(lista[0], "Retroceder");
    int op=0;
    selec(45, 11, "Partidas guardadas ", n+1, op, lista);
    if(op==0){
        file.close();
        data.dificultad=-1;
        return;
    }
    char buscarPartidaen[RUTA]="D:\\Cato\\Segundo semestre\\Progra 1\\";
    strcat(buscarPartidaen, lista[op]);
    strcat(buscarPartidaen, ".dat");
    obtenerDatosPartidaAntigua(buscarPartidaen, data);
    file.close();
}
void reestablecerInfo(infoPartida& data){
    data.escudo=false;
    data.puntaje=0;
    data.navesita.tipo=1;
    data.navesita.pos.x=20;
    data.navesita.pos.y=24;
    data.navesita.pos.direc=90;
    data.navesita.carac.vida=100;
    data.navesita.carac.mov=0;
    data.navesita.carac.show=0;
    strcpy(data.navesita.mascara ,"*");
    data.navesita2.tipo=1;
    data.navesita2.pos.x=80;
    data.navesita2.pos.y=24;
    data.navesita2.pos.direc=90;
    data.navesita2.carac.vida=100;
    data.navesita2.carac.mov=0;
    data.navesita2.carac.show=0;
    strcpy(data.navesita2.mascara ,"#");
}
void nuevaGuardada(int& opcionSeleccionada){
    char lista[TAM][TAM];
    strcpy(lista[0], "Nueva Partida");
    strcpy(lista[1], "Cargar Partida");
    strcpy(lista[2], "Ver Ranking");
    strcpy(lista[3], "Retroceder");
    selec(45, 11, "Elija una opcion ", 4, opcionSeleccionada, lista);
}
void obtenerDatosNuevaPartida(infoPartida& data){
    data.dificultad=0;
    data.navesita.carac.principal=true;
    reestablecerInfo(data);
    seleccionarDif(data.dificultad);
    for(int i=0; i<T;i++){data.asociadadisp[i]=true;data.asociadaEnemigos[i]=true;}
}
void imprimirObjeto(objetos obj){
    if(obj.carac.show==0||obj.carac.show==-10){
            if(obj.tipo==1){
                imprimir(obj.pos.x, obj.pos.y, obj.mascara, color_Nave);
                switch(obj.pos.direc){
                    case 90:imprimir(obj.pos.x, obj.pos.y+1, obj.mascara,color_Nave);
                            imprimir(obj.pos.x-1, obj.pos.y+1, obj.mascara,color_Nave);
                            imprimir(obj.pos.x+1, obj.pos.y+1, obj.mascara,color_Nave);
                            break;
                    case 0:imprimir(obj.pos.x-1, obj.pos.y-1, obj.mascara,color_Nave);
                            imprimir(obj.pos.x-1, obj.pos.y, obj.mascara,color_Nave);
                            imprimir(obj.pos.x-1, obj.pos.y+1, obj.mascara,color_Nave);
                            break;
                    case 180:imprimir(obj.pos.x+1, obj.pos.y-1, obj.mascara,color_Nave);
                            imprimir(obj.pos.x+1, obj.pos.y, obj.mascara,color_Nave);
                            imprimir(obj.pos.x+1, obj.pos.y+1, obj.mascara,color_Nave);
                            break;
                    case 270:imprimir(obj.pos.x-1, obj.pos.y-1, obj.mascara,color_Nave);
                            imprimir(obj.pos.x, obj.pos.y-1, obj.mascara,color_Nave);
                            imprimir(obj.pos.x+1, obj.pos.y-1, obj.mascara,color_Nave);
                            break;
                }
            }
            else if(obj.tipo==0){
                switch(obj.pos.direc){
                    case 90:imprimir(obj.pos.x, obj.pos.y, "|",color_disparos);
                            imprimir(obj.pos.x, obj.pos.y-1, "|",color_disparos);
                            break;
                    case 0: imprimir(obj.pos.x, obj.pos.y,"-",color_disparos);
                            imprimir(obj.pos.x+1, obj.pos.y, "-",color_disparos);
                            break;
                    case 180:imprimir(obj.pos.x-1, obj.pos.y, "-",color_disparos);
                            imprimir(obj.pos.x, obj.pos.y, "-",color_disparos);
                            break;
                    case 270:imprimir(obj.pos.x, obj.pos.y+1, "|",color_disparos);
                            imprimir(obj.pos.x, obj.pos.y, "|",color_disparos);
                            break;
                }
            }
            else if(obj.tipo==-1){
                imprimir(obj.pos.x, obj.pos.y,obj.mascara,color_ASTEROIDE);
                imprimir(obj.pos.x+1, obj.pos.y+1,obj.mascara,color_ASTEROIDE);
                imprimir(obj.pos.x-1, obj.pos.y+1,obj.mascara,color_ASTEROIDE);
                imprimir(obj.pos.x+1, obj.pos.y-1,obj.mascara,color_ASTEROIDE);
                imprimir(obj.pos.x-1, obj.pos.y-1,obj.mascara,color_ASTEROIDE);
            }
    }
}
void ocultarObjeto(objetos obj){
    int c=0;
    imprimir(obj.pos.x, obj.pos.y,obj.mascara,c);
    imprimir(obj.pos.x+1, obj.pos.y+1,obj.mascara,c);
    imprimir(obj.pos.x-1, obj.pos.y+1,obj.mascara,c);
    imprimir(obj.pos.x+1, obj.pos.y-1,obj.mascara,c);
    imprimir(obj.pos.x-1, obj.pos.y-1,obj.mascara,c);
    imprimir(obj.pos.x+1, obj.pos.y,obj.mascara,c);
    imprimir(obj.pos.x-1, obj.pos.y,obj.mascara,c);
    imprimir(obj.pos.x, obj.pos.y-1,obj.mascara,c);
    imprimir(obj.pos.x, obj.pos.y+1,obj.mascara,c);
}
void imprimirNavePrincipal(objetos& obj){
    int c=1;
    if(obj.carac.show==100){c=color_NaveconEscudo;}
    else{c=color_Nave;}
    imprimir(obj.pos.x, obj.pos.y, obj.mascara,c);
    imprimir(obj.pos.x, obj.pos.y+1, obj.mascara,c);
    imprimir(obj.pos.x-1, obj.pos.y+1, obj.mascara,c);
    imprimir(obj.pos.x+1, obj.pos.y+1, obj.mascara,c);
}
void imprimirNaveEnemiga(objetos& obj){
    imprimir(obj.pos.x, obj.pos.y, obj.mascara, color_Naveenemiga);
    imprimir(obj.pos.x, obj.pos.y-1, obj.mascara,color_Naveenemiga);
    imprimir(obj.pos.x-1, obj.pos.y-1, obj.mascara,color_Naveenemiga);
    imprimir(obj.pos.x+1, obj.pos.y-1, obj.mascara,color_Naveenemiga);
}
void imprimirPot(objetos obj){
    imprimir(obj.pos.x, obj.pos.y, obj.mascara, color_Pot);
}
void corregirPos(objetos& obj){
    if(obj.pos.x<4){
            obj.pos.x=4;
            if(obj.tipo==0){obj.carac.mov=0;}
            if(obj.tipo==2){obj.pos.direc=0;}
    }
    if(obj.pos.x>114){
            obj.pos.x=114;
            if(obj.tipo==0){obj.carac.mov=0;}
            if(obj.tipo==2){obj.pos.direc=180;}
    }
    if(obj.pos.y<2){
            obj.pos.y=2;
            if(obj.tipo==0){obj.carac.mov=0;}
    }
    if(obj.pos.y>24){
            obj.pos.y=24;
            if(obj.tipo==0 || obj.tipo==-1){obj.carac.mov=0;}
    }
}
void corregirPos1(objetos& obj){
    if(obj.pos.x<4){obj.pos.x=4;}
    if(obj.pos.x>53){obj.pos.x=53;}
    if(obj.pos.y<2){obj.pos.y=2;}
    if(obj.pos.y>25){obj.pos.y=25;}
}
void corregirPos2(objetos& obj){
    if(obj.pos.x<61){obj.pos.x=61;}
    if(obj.pos.x>114){obj.pos.x=114;}
    if(obj.pos.y<2){obj.pos.y=2;}
    if(obj.pos.y>25){obj.pos.y=25;}
}
void crearDisparo(objetos nav, objetos disp[T], bool asociada[T], infoPartida& data){
    for(int i=0; i<T; i++){
        if(asociada[i]){
            asociada[i]=false;
            disp[i].tipo=0;
            disp[i].pos=nav.pos;
            if(nav.tipo==2){disp[i].pos.direc=270;}
            disp[i].carac.show=1;
            disp[i].carac.mov=1;
            break;
        }
    }
}
void crearEnemigo(infoPartida& data){
    for(int i=0; i<T; i++){
        if(data.asociadaEnemigos[i]){
            data.asociadaEnemigos[i]=false;
            data.enemigos[i].tipo=-1;
            data.enemigos[i].pos.x=rand()%100+10;
            data.enemigos[i].pos.y=3;
            strcpy(data.enemigos[i].mascara,"*");
            data.enemigos[i].carac.show=1;
            data.enemigos[i].carac.mov=1;
            data.enemigos[i].carac.vida=50;
            break;
        }
    }
}
void crearVidaextraOEscudo(infoPartida& data){
    for(int i=0; i<T; i++){
        if(data.asociadaEnemigos[i]){
            data.asociadaEnemigos[i]=false;
            data.enemigos[i].tipo=rand()%2+3;
            data.enemigos[i].pos.x=rand()%100+10;
            data.enemigos[i].pos.y=24;
            if(data.enemigos[i].tipo==3){strcpy(data.enemigos[i].mascara,"O");}
            if(data.enemigos[i].tipo==4){strcpy(data.enemigos[i].mascara,"@");}
            data.enemigos[i].carac.show=1;
            data.enemigos[i].carac.mov=1;
            data.enemigos[i].carac.vida=50;
            break;
        }
    }
}
void crearEnemigo2(infoPartida& data){
    for(int i=0; i<T; i++){
        if(data.asociadaEnemigos[i]){
            data.asociadaEnemigos[i]=false;
            data.enemigos[i].tipo=2;
            data.enemigos[i].pos.x=rand()%100+10;
            data.enemigos[i].pos.y=3;
            strcpy(data.enemigos[i].mascara,"+");
            data.enemigos[i].carac.show=1;
            data.enemigos[i].carac.mov=1;
            data.enemigos[i].carac.vida=100;
            break;
        }
    }
}
void movimiento(objetos& nav, int dif){
    int velmed=1+dif;
    if(nav.carac.mov==0){return;}
    if(nav.tipo==1){
        imprimir(nav.pos.x-1, nav.pos.y-1, "   ", 0);
        imprimir(nav.pos.x-1, nav.pos.y, "   ", 0);
        imprimir(nav.pos.x-1, nav.pos.y+1, "   ", 0);
        switch(nav.pos.direc){
            case 90:nav.pos.y-=nav.carac.mov*velmed;
                    break;
            case 0:nav.pos.x+=nav.carac.mov*velmed;
                    break;
            case 180:nav.pos.x-=nav.carac.mov*velmed;
                    break;
            case 270:nav.pos.y+=nav.carac.mov*velmed;
                    break;
        }
        corregirPos(nav);
        if(nav.carac.show!=100){nav.carac.show=0;}
        if(nav.carac.principal){imprimirNavePrincipal(nav);}else{imprimirObjeto(nav);}
        if(nav.carac.show!=100){nav.carac.show=-10;}
        nav.carac.mov=0;
    }
    else if(nav.tipo==0){
        imprimir(nav.pos.x-1, nav.pos.y-1, "   ", 4);
        imprimir(nav.pos.x-1, nav.pos.y, "   ", 4);
        imprimir(nav.pos.x-1, nav.pos.y+1, "   ", 4);
        switch(nav.pos.direc){
            case 90:nav.pos.y-=nav.carac.mov*velmed;
                    break;
            case 0:nav.pos.x+=nav.carac.mov*velmed;
                    break;
            case 180:nav.pos.x-=nav.carac.mov*velmed;
                    break;
            case 270:nav.pos.y+=nav.carac.mov*velmed;
                    break;
        }
        corregirPos(nav);
        nav.carac.show=0;
        imprimirObjeto(nav);
        if(nav.carac.mov==0){
            imprimir(nav.pos.x-1, nav.pos.y-1, "   ", 4);
            imprimir(nav.pos.x-1, nav.pos.y, "   ", 4);
            imprimir(nav.pos.x-1, nav.pos.y+1, "   ", 4);
        }
    }
    else if(nav.tipo==-1){
        imprimir(nav.pos.x-1, nav.pos.y-1, "   ", 4);
        imprimir(nav.pos.x-1, nav.pos.y, "   ", 4);
        imprimir(nav.pos.x-1, nav.pos.y+1, "   ", 4);
        nav.pos.y+=nav.carac.mov*velmed;
        corregirPos(nav);
        nav.carac.show=0;
        imprimirObjeto(nav);
        if(nav.carac.mov==0){
            imprimir(nav.pos.x-1, nav.pos.y-1, "   ", 4);
            imprimir(nav.pos.x-1, nav.pos.y, "   ", 4);
            imprimir(nav.pos.x-1, nav.pos.y+1, "   ", 4);
        }
    }
    else if(nav.tipo==2){
        imprimir(nav.pos.x-1, nav.pos.y-1, "   ", 0);
        imprimir(nav.pos.x-1, nav.pos.y, "   ", 0);
        imprimir(nav.pos.x-1, nav.pos.y+1, "   ", 0);
        switch(nav.pos.direc){
            case 0:nav.pos.x+=nav.carac.mov*velmed;
                    break;
            case 180:nav.pos.x-=nav.carac.mov*velmed;
                    break;
        }
        corregirPos(nav);
        nav.carac.show=0;
        imprimirNaveEnemiga(nav);
    }
    else if(nav.tipo==3||nav.tipo==4){
        imprimirPot(nav);
    }
}
void movimientoNaves(objetos& nav, char teclas[5], objetos listadisparos[T], bool asociada[T], infoPartida& data){
        imprimirObjeto(nav);
        int delay=50-20*data.dificultad;
        Sleep(delay);
        char tecla;
        if (_kbhit()) {
            tecla = _getch();
            if(tecla== teclas[0]){
                nav.pos.direc=180;
                nav.carac.mov=1;
            }
            else if(tecla==teclas[1]){
                nav.pos.direc=270;
                nav.carac.mov=1;
            }
            else if(tecla==teclas[2]){
                nav.pos.direc=0;
                nav.carac.mov=1;
            }
            else if(tecla==teclas[3]){
                nav.pos.direc=90;
                nav.carac.mov=1;
            }
            else if(tecla==teclas[4]){
                crearDisparo(nav, listadisparos, asociada, data);
            }
            movimiento(nav, data.dificultad);
        }
}
void movimientoNavePrincipal(infoPartida& data, char teclas[3],objetos& nav){
        imprimirNavePrincipal(nav);
        int delay=50-20*data.dificultad;
        Sleep(delay);
        char tecla;
        if (_kbhit()) {
            tecla = _getch();
            if(tecla== teclas[0]){
                nav.pos.direc=180;
                nav.carac.mov=1;
                movimiento(nav, data.dificultad);
            }
            else if(tecla==teclas[1]){
                nav.pos.direc=0;
                nav.carac.mov=1;
                movimiento(nav, data.dificultad);
            }
            else if(tecla==teclas[2]){
                nav.pos.direc=90;
                crearDisparo(nav, data.disparos, data.asociadadisp, data);
            }
        }
}
void verificarColisionesVs(objetos& n1, objetos& n2, objetos ld1[T], objetos ld2[T], bool a1[T], bool a2[T]){
    for(int i=0; i<T;i++){
        if(!a1[i]){
            for(int j=0; j<T;j++){
                if(!a2[T]){
                    if(ld1[i].pos.x==ld2[j].pos.x && ld1[i].pos.y==ld2[j].pos.y){
                        ld1[i].carac.mov=0;
                        ld2[j].carac.mov=0;
                        borrarCuadro(ld1[i].pos.x-2, ld1[i].pos.y-2, 5, 5);
                    }
                }
            }
            if(n2.pos.direc==0){
                if((n2.pos.x==ld1[i].pos.x && n2.pos.y==ld1[i].pos.y)
                   ||((n2.pos.x-1)==ld1[i].pos.x && (n2.pos.y-1)==ld1[i].pos.y)
                   ||((n2.pos.x-1)==ld1[i].pos.x && n2.pos.y==ld1[i].pos.y)
                   ||((n2.pos.x-1)==ld1[i].pos.x && (n2.pos.y+1)==ld1[i].pos.y))
                    {n2.carac.vida-=20;ld1[i].carac.mov=0;}
            }
            else if(n2.pos.direc==90){
                if((n2.pos.x==ld1[i].pos.x && n2.pos.y==ld1[i].pos.y)
                   ||((n2.pos.x-1)==ld1[i].pos.x && (n2.pos.y+1)==ld1[i].pos.y)
                   ||(n2.pos.x==ld1[i].pos.x && (n2.pos.y+1)==ld1[i].pos.y)
                   ||((n2.pos.x+1)==ld1[i].pos.x && (n2.pos.y+1)==ld1[i].pos.y))
                    {n2.carac.vida-=20;ld1[i].carac.mov=0;}
            }
            else if(n2.pos.direc==180){
                if((n2.pos.x==ld1[i].pos.x && n2.pos.y==ld1[i].pos.y)
                   ||((n2.pos.x+1)==ld1[i].pos.x && (n2.pos.y-1)==ld1[i].pos.y)
                   ||((n2.pos.x+1)==ld1[i].pos.x && n2.pos.y==ld1[i].pos.y)
                   ||((n2.pos.x+1)==ld1[i].pos.x && (n2.pos.y+1)==ld1[i].pos.y))
                    {n2.carac.vida-=20;ld1[i].carac.mov=0;}
            }
            else if(n2.pos.direc==270){
                if((n2.pos.x==ld1[i].pos.x && n2.pos.y==ld1[i].pos.y)
                   ||((n2.pos.x-1)==ld1[i].pos.x && (n2.pos.y-1)==ld1[i].pos.y)
                   ||(n2.pos.x==ld1[i].pos.x && (n2.pos.y-1)==ld1[i].pos.y)
                   ||((n2.pos.x+1)==ld1[i].pos.x && (n2.pos.y-1)==ld1[i].pos.y))
                    {n2.carac.vida-=20;ld1[i].carac.mov=0;}
            }
        }
    }
    for(int i=0; i<T;i++){
        if(!a2[i]){
            if(n1.pos.direc==0){
                if((n1.pos.x==ld2[i].pos.x && n1.pos.y==ld2[i].pos.y)
                   ||((n1.pos.x-1)==ld2[i].pos.x && (n1.pos.y-1)==ld2[i].pos.y)
                   ||((n1.pos.x-1)==ld2[i].pos.x && n1.pos.y==ld2[i].pos.y)
                   ||((n1.pos.x-1)==ld2[i].pos.x && (n1.pos.y+1)==ld2[i].pos.y))
                    {n1.carac.vida-=20;ld2[i].carac.mov=0;}
            }
            else if(n1.pos.direc==90){
                if((n1.pos.x==ld2[i].pos.x && n1.pos.y==ld2[i].pos.y)
                   ||((n1.pos.x-1)==ld2[i].pos.x && (n1.pos.y+1)==ld2[i].pos.y)
                   ||(n1.pos.x==ld2[i].pos.x && (n1.pos.y+1)==ld2[i].pos.y)
                   ||((n1.pos.x+1)==ld2[i].pos.x && (n1.pos.y+1)==ld2[i].pos.y))
                    {n1.carac.vida-=20;ld2[i].carac.mov=0;}
            }
            else if(n1.pos.direc==180){
                if((n1.pos.x==ld2[i].pos.x && n1.pos.y==ld2[i].pos.y)
                   ||((n1.pos.x+1)==ld2[i].pos.x && (n1.pos.y-1)==ld2[i].pos.y)
                   ||((n1.pos.x+1)==ld2[i].pos.x && n1.pos.y==ld2[i].pos.y)
                   ||((n1.pos.x+1)==ld2[i].pos.x && (n1.pos.y+1)==ld2[i].pos.y))
                    {n1.carac.vida-=20;ld2[i].carac.mov=0;}
            }
            else if(n1.pos.direc==270){
                if((n1.pos.x==ld2[i].pos.x && n1.pos.y==ld2[i].pos.y)
                   ||((n1.pos.x-1)==ld2[i].pos.x && (n1.pos.y-1)==ld2[i].pos.y)
                   ||(n1.pos.x==ld2[i].pos.x && (n1.pos.y-1)==ld2[i].pos.y)
                   ||((n1.pos.x+1)==ld2[i].pos.x && (n1.pos.y-1)==ld2[i].pos.y))
                    {n1.carac.vida-=20;ld2[i].carac.mov=0;}
            }
        }
    }
}
void verificarColisionesBalas(infoPartida& data){
    for(int i=0;i<T;i++){
        if(!data.asociadadisp[i]){
            for(int j=i+1; j<T;j++){
                if(!data.asociadadisp[j]){
                    if(data.disparos[i].pos.direc!=data.disparos[j].pos.direc){
                        if(data.disparos[i].pos.x==data.disparos[j].pos.x && data.disparos[i].pos.y==data.disparos[j].pos.y){
                            data.disparos[i].carac.mov=0;
                            data.disparos[j].carac.mov=0;
                            data.asociadadisp[i]=true;
                            data.asociadadisp[j]=true;
                        }
                    }
                }
            }
        }
    }
}
void verificarColisionesPrincipal(infoPartida& data){
    verificarColisionesBalas(data);
    for(int i=0; i<T;i++){
        if(!data.asociadaEnemigos[i]){
            for(int j=0; j<T;j++){
                if(!data.asociadadisp[j]){
                    if(data.enemigos[i].pos.x>=(data.disparos[j].pos.x-1)
                        && data.enemigos[i].pos.y>=(data.disparos[j].pos.y-1)
                        && data.enemigos[i].pos.x<=(data.disparos[j].pos.x+1)
                        && data.enemigos[i].pos.y<=(data.disparos[j].pos.y+1)){
                        if(!(data.enemigos[i].tipo==2 && data.disparos[j].pos.direc==270)){
                            data.enemigos[i].carac.vida-=50;
                            data.disparos[j].carac.mov=0;
                            ocultarObjeto(data.disparos[j]);
                            data.asociadadisp[j]=true;
                            if(data.enemigos[i].carac.vida==0){
                                data.enemigos[i].carac.mov=0;
                                ocultarObjeto(data.enemigos[i]);
                                data.asociadaEnemigos[i]=true;
                                if(data.enemigos[i].tipo==2){data.puntaje+=500;}
                                if(data.enemigos[i].tipo==-1){data.puntaje+=100;}
                                imprimirVida(data);
                            }
                        }
                    }
                }
            }
            if(data.enemigos[i].pos.x>=(data.navesita.pos.x-2)
               && data.enemigos[i].pos.y>=(data.navesita.pos.y-1)
               && data.enemigos[i].pos.x<=(data.navesita.pos.x+2)
               && data.enemigos[i].pos.y<=(data.navesita.pos.y+1)){
                        data.enemigos[i].carac.mov=0;
                        data.asociadaEnemigos[i]=true;
                        data.navesita.carac.mov=0;
                        borrarCuadro(data.enemigos[i].pos.x-2, data.enemigos[i].pos.y-2, 4, 4);
                        if(data.enemigos[i].tipo==-1){data.navesita.carac.vida-=20;}
                        if(data.enemigos[i].tipo==4){data.navesita.carac.vida+=75;}
                        if(data.enemigos[i].tipo==3){data.escudo=true;data.navesita.carac.show=100;}
                        imprimirVida(data);
            }
            if(data.navesita2.carac.show==-10){
                if(data.enemigos[i].pos.x>=(data.navesita2.pos.x-2)
               && data.enemigos[i].pos.y>=(data.navesita2.pos.y-1)
               && data.enemigos[i].pos.x<=(data.navesita2.pos.x+2)
               && data.enemigos[i].pos.y<=(data.navesita2.pos.y+1)){
                        data.enemigos[i].carac.mov=0;
                        data.asociadaEnemigos[i]=true;
                        data.navesita2.carac.mov=0;
                        borrarCuadro(data.enemigos[i].pos.x-2, data.enemigos[i].pos.y-2, 4, 4);
                        data.navesita2.carac.vida-=20;
                        imprimirVida(data);
                }
            }
        }
    }
    for(int j=0; j<T;j++){
                if(!data.asociadadisp[j]){
                    if(data.disparos[j].pos.direc==270){
                        if((data.navesita.pos.x==(data.disparos[j].pos.x)
                        && data.navesita.pos.y==(data.disparos[j].pos.y))||(
                        data.navesita.pos.y==(data.disparos[j].pos.y+1)&&(
                        data.navesita.pos.x==(data.disparos[j].pos.x-1)
                        || data.navesita.pos.x==(data.disparos[j].pos.x)
                        || data.navesita.pos.x==(data.disparos[j].pos.x+1)))){
                            data.navesita.carac.vida-=15;
                            data.disparos[j].carac.mov=0;
                            ocultarObjeto(data.disparos[j]);
                            data.asociadadisp[j]=true;
                            imprimirVida(data);
                        }
                    }
                }
    }
    if(data.navesita.carac.vida<=0){data.navesita.carac.vida==0;imprimirVida(data);}
}
void juegovs(infoPartida& data){
    bool estado=true;
    char teclasnav1[5]={'a', 's', 'd', 'w', ' '};
    char teclasnav2[5]={'j', 'k', 'l', 'i', 'p'};
    objetos ldispnv1[T], ldispnv2[T];
    bool asociadanv1[T], asociadanv2[T];
    for(int i=0; i<T;i++){asociadanv1[i]=true;asociadanv2[i]=true;}
    while(estado){
        for(int i=0;i<T;i++){if(!asociadanv1[i]){movimiento(ldispnv1[i], data.dificultad);if(ldispnv1[i].carac.mov==0){asociadanv1[i]=true;}}}
        for(int i=0;i<T;i++){if(!asociadanv2[i]){movimiento(ldispnv2[i], data.dificultad);if(ldispnv2[i].carac.mov==0){asociadanv2[i]=true;}}}
        movimientoNaves(data.navesita, teclasnav1, ldispnv1, asociadanv1, data);
        movimientoNaves(data.navesita2, teclasnav2, ldispnv2, asociadanv2, data);
        verificarColisionesVs(data.navesita, data.navesita2, ldispnv1, ldispnv2, asociadanv1, asociadanv2);
        if(data.navesita.carac.vida==0 || data.navesita2.carac.vida ==0){break;}
    }
}
void juegoPrincipal(infoPartida& data){
    srand(time(NULL));
    int vidaaux=0;
    auto inicioasteroides = chrono::steady_clock::now();
    auto iniciodisparos = chrono::steady_clock::now();
    auto inicioenemigo = chrono::steady_clock::now();
    auto iniciodif = chrono::steady_clock::now();
    auto iniciogen = chrono::steady_clock::now();
    auto iniciopotenciadores = chrono::steady_clock::now();
    auto inicioescudo = chrono::steady_clock::now();
    auto tiempo_transcurridoescudo=0;
    int proba;
    char teclasnav1[3]={'a', 'd', ' '};
    imprimirVida(data);
    int velenemigos=200-20*data.dificultad;
    int veldisparos=40-10*data.dificultad;
    int velnavenemigos=800-50*data.dificultad;
    int velgeneradordisp=2800-100*data.dificultad;
    int timechangedif=50000;
    int timepoten=20000;
    int timeescudo=10000;
    while(true){
        char tec;
        vidaaux=data.navesita.carac.vida;
        auto ahora = chrono::steady_clock::now();
        if(!data.escudo){inicioescudo=ahora;}
        else{tiempo_transcurridoescudo = chrono::duration_cast<chrono::milliseconds>(ahora - inicioescudo).count();}
        auto tiempo_transcurridoa = chrono::duration_cast<chrono::milliseconds>(ahora - inicioasteroides).count();
        auto tiempo_transcurridod = chrono::duration_cast<chrono::milliseconds>(ahora - iniciodisparos).count();
        auto tiempo_transcurridoe = chrono::duration_cast<chrono::milliseconds>(ahora - inicioenemigo).count();
        auto tiempo_transcurridodif = chrono::duration_cast<chrono::milliseconds>(ahora -  iniciodif).count();
        auto tiempo_transcurridogen = chrono::duration_cast<chrono::milliseconds>(ahora - iniciogen).count();
        auto tiempo_transcurridopotenciadores = chrono::duration_cast<chrono::milliseconds>(ahora - iniciopotenciadores).count();
        movimientoNavePrincipal(data, teclasnav1, data.navesita);
        if(tiempo_transcurridopotenciadores > timepoten){
            crearVidaextraOEscudo(data);
            iniciopotenciadores = chrono::steady_clock::now();
        }
        if(tiempo_transcurridodif > timechangedif){
            data.dificultad+=1;
            iniciodif = chrono::steady_clock::now();
        }
        if(tiempo_transcurridogen > velgeneradordisp){
            for(int i=0; i<T; i++){
                if(!data.asociadaEnemigos[i]){
                    if(data.enemigos[i].tipo==2){crearDisparo(data.enemigos[i], data.disparos, data.asociadadisp, data);}
                }
            }
            iniciogen = chrono::steady_clock::now();
        }
        if(tiempo_transcurridod > veldisparos){
            for(int i=0;i<T;i++){if(!data.asociadadisp[i]){movimiento(data.disparos[i], data.dificultad);if(data.disparos[i].carac.mov==0){data.asociadadisp[i]=true;}}}
            iniciodisparos = chrono::steady_clock::now();
        }
        if(tiempo_transcurridoa > velenemigos){
            proba=rand()%100;
            if(proba<(15+5*data.dificultad)){crearEnemigo(data);}
            for(int i=0;i<T;i++){if(!data.asociadaEnemigos[i]){movimiento(data.enemigos[i], data.dificultad);if(data.enemigos[i].carac.mov==0){data.navesita.carac.vida-=5;imprimirVida(data);data.asociadaEnemigos[i]=true;}}}
            inicioasteroides = chrono::steady_clock::now();
        }
        if(tiempo_transcurridoe > velnavenemigos){
            proba=rand()%100;
            if(proba<(1+2*data.dificultad)){crearEnemigo2(data);}
            inicioenemigo = chrono::steady_clock::now();
        }
        verificarColisionesPrincipal(data);
        if(data.escudo){
            if(vidaaux>data.navesita.carac.vida){
                data.navesita.carac.vida=vidaaux;
                imprimirVida(data);
            }
        }
        if(data.escudo && tiempo_transcurridoescudo > timeescudo){data.escudo=false;imprimirVida(data);data.navesita.carac.show=1;tiempo_transcurridoescudo=0;}
        if(data.navesita.carac.vida<=0){break;}
        if (_kbhit()){
            tec = _getch();
            if(tec==27){
                guardarPartidaActual(data);
                break;
            }
        }
    }
}
void juegoMuertesubita(infoPartida& data){
    srand(time(NULL));
    auto inicio = chrono::steady_clock::now();
    auto inicio2 = chrono::steady_clock::now();
    bool estado=true;
    int proba;
    char teclasnav1[3]={'a', 'd', ' '};
    char teclasnav2[3]={'j', 'l', 'p'};
    imprimirVida(data);
    int velenemigos=200-20*data.dificultad;
    int veldisparos=40-10*data.dificultad;
    while(estado){
        char tec;
        auto ahora = chrono::steady_clock::now();
        auto tiempo_transcurrido = chrono::duration_cast<chrono::milliseconds>(ahora - inicio).count();
        auto ahora2 = chrono::steady_clock::now();
        auto tiempo_transcurrido2 = chrono::duration_cast<chrono::milliseconds>(ahora - inicio).count();
        movimientoNavePrincipal(data, teclasnav1, data.navesita);
        movimientoNavePrincipal(data, teclasnav2, data.navesita2);
        if(tiempo_transcurrido > velenemigos){
            proba=rand()%100;
            if(proba<15){crearEnemigo(data);}
            for(int i=0;i<T;i++){if(!data.asociadaEnemigos[i]){movimiento(data.enemigos[i], data.dificultad);if(data.enemigos[i].carac.mov==0){data.asociadaEnemigos[i]=true;}}}
            inicio = chrono::steady_clock::now();
        }
        if(tiempo_transcurrido2 > veldisparos){
            for(int i=0;i<T;i++){if(!data.asociadadisp[i]){movimiento(data.disparos[i], data.dificultad);if(data.disparos[i].carac.mov==0){data.asociadadisp[i]=true;}}}
            inicio2 = chrono::steady_clock::now();
        }
        corregirPos1(data.navesita);
        corregirPos2(data.navesita2);
        verificarColisionesPrincipal(data);
        if(data.navesita.carac.vida==0||data.navesita2.carac.vida==0){break;}
    }
}
void estadoPantalla(int &n){
    int cj=0;
    infoPartida data;
    imprimirMargen(118,27);
    if(n==0){
        seleccionarCantJugadores(cj);
        borrarCuadro(45, 7, TAM, 8);
        borrarCuadro(40, 27, 50, 2);
        if(cj==0){
            nuevaGuardada(cj);
            if(cj==3){
                n=0;
                estadoPantalla(n);
            }
            else{
                if(cj==0){
                    borrarCuadro(45, 7, TAM, 7);
                    borrarCuadro(40, 27, 50, 2);
                    obtenerDatosNuevaPartida(data);
                    borrarCuadro(45, 9, TAM, 8);
                    instruccionesjuegoPrincipal();
                }
                else if(cj==2){
                    borrarCuadro(45, 7, TAM, 7);
                    borrarCuadro(40, 27, 50, 2);
                    verRanking();
                    imprimir(40,28, "Aprete cualquier tecla para volver al menu", 10);
                    getch();
                    borrarCuadro(25, 27, 100, 2);
                    cj=0;
                    estadoPantalla(cj);
                }
                else{
                    elejirPartida(data);
                    if(data.dificultad==-1){
                        cj=0;
                        estadoPantalla(cj);
                    }
                }
            }
            borrarCuadro(45, 9, TAM, 8);
            borrarCuadro(40, 27, 50, 2);
            imprimirMargen(118, 27);
            bool estado=true;
            juegoPrincipal(data);
            if(data.navesita.carac.vida==0){
                guardarPuntajeActual(data);
                revisarRanking();
            }
            borrarCuadro(25, 27, 100, 2);
            imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
            getch();
            borrarCuadro(40, 27, 50, 2);
            cj=0;
            estadoPantalla(cj);
        }
        else{
            elejirModo(cj);\
            if(cj==2){
                n=0;
                estadoPantalla(n);
            }
            else{
                if(cj==0){
                    borrarCuadro(45, 9, TAM, 8);
                    borrarCuadro(40, 27, 50, 2);
                    imprimir(40, 13, "Se eligio VERSUS", 12);
                    imprimirMargen(118, 27);
                    data.dificultad=0;
                    reestablecerInfo(data);
                    data.navesita.carac.principal=false;
                    data.navesita2.carac.principal=false;
                    juegovs(data);
                    imprimirMargen(118,27);
                    if(data.navesita.carac.vida==0){
                        imprimir(40, 13, "Gano el jugador numero 2", 12);
                    }
                    else{

                        imprimir(40, 13, "Gano el jugador numero 1", 12);
                    }
                    imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
                    getch();
                    borrarCuadro(40, 27, 50, 2);
                    estadoPantalla(cj);
                }
                else{
                    borrarCuadro(45, 9, TAM, 8);
                    borrarCuadro(40, 27, 50, 2);
                    imprimir(40, 13, "Se eligio MUERTE SUBITA", 12);
                    imprimirMargenDoble(118, 27);
                    data.dificultad=0;
                    data.puntaje=0;
                    reestablecerInfo(data);
                    data.navesita.carac.principal=true;
                    data.navesita2.carac.show=-10;
                    data.navesita2.carac.principal=true;
                    juegoMuertesubita(data);
                    imprimirMargen(118,27);
                    borrarCuadro(40, 27, 55, 2);
                    if(data.navesita.carac.vida==0){
                        imprimir(40, 13, "Gano el jugador numero 2", 12);
                    }
                    else{

                        imprimir(40, 13, "Gano el jugador numero 1", 12);
                    }
                    imprimir(40,28, "Aprete cualquier tecla para continuar", 10);
                    getch();
                    borrarCuadro(40, 27, 55, 2);
                    cj=0;
                    estadoPantalla(cj);
                }
            }
        }
    }
}
int main(){
    int n=0;
    imprimirMargen(118, 27);
    imprimir(50, 13, "Bienvenidos", 12);
    getch();
    estadoPantalla(n);
    gotoxy(1, 26);
    color(0);
    return 0;
}
