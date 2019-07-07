/*
Jahresarbeit2003 - Jahresarbeit im Fach Informatik
Copyright (C) 2003  Christian Rauch
http://www.sdlgames.de.tf

Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der GNU General Public License,
wie von der Free Software Foundation ver�ffentlicht, weitergeben und/oder modifizieren, entweder gem��
Version 2 der Lizenz oder (nach Ihrer Option) jeder sp�teren Version. 

Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, da� es Ihnen von Nutzen sein wird, aber
OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT F�R
EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben.
Falls nicht, schreiben Sie an die Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
MA 02111-1307, USA.
*/

#include<windows.h>
#include"sdl\include\sdl.h"
#include"sdl\SDL_image\include\SDL_image.h"
#include"sdl\SDL_ttf\include\SDL_ttf.h"
#include<time.h>
#include<fstream.h>

typedef struct
{
	char* piece_set;
}setting;

typedef struct
{
	char name[30];
	int punkte;
	int gr_fla;
	int zeit;
}hs;


void program();
int menu(SDL_Surface *ausgabe_bild, int field);
void hs_menue(SDL_Surface *bild);
void hardware_info(SDL_Surface *bild);
void free_game(SDL_Surface *bildschirm);
setting game_setting();
void game(setting game);
void random_matrix(int matrix[9][9]);
void draw_matrix(int matrix[9][9], SDL_Rect matrix_pos, char* piece_set_path);
void draw_arrows(SDL_Rect Matrix_pos);
void change_matrix(int matrix[9][9], char *arrows, int nr);
int look_for_match(int matrix[9][9], int pos_x, int pos_y);
int whether_gameend(int m[9][9]);
void stat_menue(int time, int time2, int k_ges, int k_fla, int k_pfe, int max_anz);
char *input_name(SDL_Surface *prev_bild);
void write_hs(hs *hs_out);
void read_hs(hs *hs_in);
void hilfe_menue();
void in_hs(hs hs_input);
int pos_hs(hs hs_input);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	TTF_Init();
	SDL_WM_SetCaption("JA2003",0);

	program();

	TTF_Quit();
	SDL_Quit();
	return 0;
}

void program()
{
	SDL_Surface *bildschirm;
	int field=2;

	bildschirm=SDL_SetVideoMode(800,600,0,SDL_HWSURFACE);	
	
	while(1)
	{
		switch(menu(bildschirm, field))
		{
		case 2:
			free_game(bildschirm);
			field=2;
			break;
		case 3:
			hs_menue(bildschirm);
			field=3;
			break;
		case 4:
			hardware_info(bildschirm);
			field=4;
			break;
		case 5:
			hilfe_menue();
			field=5;
			break;
		case 6:
			return;
		}
	}
	
	SDL_FreeSurface(bildschirm);
}

int menu(SDL_Surface *ausgabe_bild, int field=2)
{
	SDL_ShowCursor(SDL_DISABLE);

	SDL_Surface *background;
	SDL_Surface *menu_text;
	SDL_Surface *menu_back;

	SDL_Event menu_event;

	background=SDL_DisplayFormat(IMG_Load("grafik/hintergrund.jpg"));
	menu_text=SDL_DisplayFormat(IMG_Load("grafik/menu_text.gif"));
	menu_back=SDL_DisplayFormat(IMG_Load("grafik/menu_back.jpg"));
	SDL_SetColorKey(menu_text,SDL_SRCCOLORKEY | SDL_RLEACCEL,SDL_MapRGB(menu_text->format,255,255,255));

	SDL_Rect area;
	SDL_Rect ausgabe_area;
	SDL_Rect menu_pos;
	SDL_Rect menu_back_pos;

	menu_pos.x=250;
	menu_pos.y=150;
	menu_pos.w=300;
	menu_pos.h=350;
	
	area.x=0;
	area.w=300;
	area.h=50;

	menu_back_pos.x=0;
	menu_back_pos.y=0;
	menu_back_pos.w=300;
	menu_back_pos.h=50;

	ausgabe_area.x=menu_pos.x;

	while(menu_event.key.keysym.sym!=SDLK_RETURN)
	{
		for(int i=2;i<7;i++)
		{
			if(field==i)
			{
				area.y=i*50;
				ausgabe_area.y=menu_pos.y+area.y;
			}
			else
				continue;
		}

		SDL_BlitSurface(background,0,ausgabe_bild,0);
		SDL_BlitSurface(menu_back,&menu_back_pos,ausgabe_bild,&menu_pos);
		SDL_BlitSurface(menu_back,&area,ausgabe_bild,&ausgabe_area);
		SDL_BlitSurface(menu_text,0,ausgabe_bild,0);
		SDL_UpdateRect(ausgabe_bild,0,0,0,0);

		while(SDL_PollEvent(&menu_event))
		{
			switch(menu_event.type)
			{
			case SDL_KEYDOWN:
				switch(menu_event.key.keysym.sym)
				{
				case SDLK_UP:
					field--;
					break;
				case SDLK_DOWN:
					field++;
					break;
				case SDLK_RETURN:
					return field;
					break;
				case SDLK_i:					
					break;
				case SDLK_ESCAPE:
					exit(0);
				}
			}

			switch(field)
			{
			case 1: field=6; break;			
			case 7: field=2; break;
			}
		}
	}

	SDL_FreeSurface(ausgabe_bild);
	SDL_FreeSurface(background);
	SDL_FreeSurface(menu_text);
	SDL_FreeSurface(menu_back);
}

void hs_menue(SDL_Surface *bild)
{
	SDL_Surface *highscore_back;	
	highscore_back=SDL_DisplayFormat(IMG_Load("grafik/info_back.jpg"));
	TTF_Font *copperplate2=TTF_OpenFont("fonts/copperplate.ttf",20);
	TTF_Font *bloody=TTF_OpenFont("fonts/bloodfea.ttf",50);
	TTF_Font *bradley=TTF_OpenFont("fonts/bradley_hand.ttf",30);
	TTF_Font *comic=TTF_OpenFont("fonts/comic.ttf",20);

	SDL_Color black={0,0,0};
	SDL_Color red={255,0,0};

	SDL_Rect headline_pos={200,60,0,0};
	SDL_Rect name_pos={120,170,0,0};
	SDL_Rect punkte_pos={470,170,0,0};
	SDL_Rect gr_fla_pos={580,170,0,0};
	SDL_Rect zeit_pos={690,170,0,0};
	SDL_Rect ende_pos={150,550,0,0};

	hs highscorer[10];

	char punkte[10][50];
	char gr_fla[10][50];
	char zeit[10][50];

	int z_abst=30;

	SDL_ShowCursor(0);

	read_hs(highscorer);

	for(int i=0;i<10;i++)
	{
		_itoa(highscorer[i].punkte,punkte[i],10);
		_itoa(highscorer[i].gr_fla,gr_fla[i],10);
		_itoa(highscorer[i].zeit,zeit[i],10);
	}

	SDL_BlitSurface(highscore_back,0,bild,0);

	SDL_BlitSurface(TTF_RenderText_Solid(bloody,"HIGHSCORE",red),0,bild,&headline_pos);

	SDL_BlitSurface(TTF_RenderText_Solid(bradley,"Name",black),0,bild,&name_pos);
	SDL_BlitSurface(TTF_RenderText_Solid(bradley,"Punkte",black),0,bild,&punkte_pos);
	SDL_BlitSurface(TTF_RenderText_Solid(bradley,"Fl�che",black),0,bild,&gr_fla_pos);
	SDL_BlitSurface(TTF_RenderText_Solid(bradley,"Zeit",black),0,bild,&zeit_pos);

	name_pos.x=100, name_pos.y=200;
	punkte_pos.x=500, punkte_pos.y=200;
	gr_fla_pos.x=600, gr_fla_pos.y=200;
	zeit_pos.x=700, zeit_pos.y=200;

	for(int j=0;j<10;j++)
	{
		SDL_BlitSurface(TTF_RenderText_Solid(comic,highscorer[j].name,black),0,bild,&name_pos);
		name_pos.y+=z_abst;
		SDL_BlitSurface(TTF_RenderText_Solid(bradley,punkte[j],black),0,bild,&punkte_pos);
		punkte_pos.y+=z_abst;
		SDL_BlitSurface(TTF_RenderText_Solid(bradley,gr_fla[j],black),0,bild,&gr_fla_pos);
		gr_fla_pos.y+=z_abst;
		SDL_BlitSurface(TTF_RenderText_Solid(bradley,zeit[j],black),0,bild,&zeit_pos);
		zeit_pos.y+=z_abst;
	}
	
	SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,"<bel. Taste druecken um zurueck zu kehren>",black),0,bild,&ende_pos);
	SDL_UpdateRect(bild,0,0,0,0);

	SDL_Event highscore_event;

	while(highscore_event.type!=SDL_KEYDOWN)
	{
		SDL_PollEvent(&highscore_event);
	}

	TTF_CloseFont(copperplate2);
	TTF_CloseFont(bloody);
	TTF_CloseFont(bradley);
	TTF_CloseFont(comic);
	
	SDL_FreeSurface(bild);
	SDL_FreeSurface(highscore_back);
}

void hardware_info(SDL_Surface *bild)
{
	SDL_Surface *about;
	const SDL_VideoInfo *video_hardware;
	SDL_Color black={0,0,0};
	TTF_Font *arial_bold=TTF_OpenFont("fonts/arial_bold.ttf",30);
	TTF_Font *arial=TTF_OpenFont("fonts/arial.ttf",20);
	about=SDL_DisplayFormat(IMG_Load("grafik/info_back.jpg"));
	char text_hardware[11][100];
	char t_h2[11][100];
	SDL_Rect headline_pos={50,25};
	SDL_Rect text_pos={50,100,0,0};
	SDL_Rect t_h2_pos={650,100,0,0};
	SDL_Rect ende_pos={300,500,0,0};

	video_hardware=SDL_GetVideoInfo();

	SDL_ShowCursor(0);

	strcpy(text_hardware[0],"Hardwareoberfl�chen:");
	strcpy(text_hardware[1],"Windowmanager:");
	strcpy(text_hardware[2],"Beschleunigung von Hard- zu Hardware �bertragungen:");
	strcpy(text_hardware[3],"Beschleunigung von Hard- zu Hardware Transparet�bertragungen:");
	strcpy(text_hardware[4],"Beschleunigung von Hard- zu Hardware Alphakanal�bertragungen:");
	strcpy(text_hardware[5],"Beschleunigung von Soft- zu Hardware �bertragungen:");
	strcpy(text_hardware[6],"Beschleunigung von Soft- zu Hardware Tranzparent�bertragungen:");
	strcpy(text_hardware[7],"Beschleunigung von Soft- zu Hardware Alphakanal�bertragungen:");
	strcpy(text_hardware[8],"Beschleunigung von Fl�chenf�llungen:");
	strcpy(text_hardware[9],"Verf�gbarer Videospeicher:");
	strcpy(text_hardware[10],"Farben:");

	strcpy(t_h2[0],video_hardware->hw_available ? "Ja" : "Nein");
	strcpy(t_h2[1],video_hardware->wm_available ? "Ja" : "Nein");
	strcpy(t_h2[2],video_hardware->blit_hw ? "Ja" : "Nein");
	strcpy(t_h2[3],video_hardware->blit_hw_CC ? "Ja" : "Nein");
	strcpy(t_h2[4],video_hardware->blit_hw_A ? "Ja" : "Nein");
	strcpy(t_h2[5],video_hardware->blit_sw ? "Ja" : "Nein");
	strcpy(t_h2[6],video_hardware->blit_sw_CC ? "Ja" : "Nein");
	strcpy(t_h2[7],video_hardware->blit_sw_A ? "Ja" : "Nein");
	strcpy(t_h2[8],video_hardware->blit_fill ? "Ja" : "Nein");
	_itoa(video_hardware->video_mem, t_h2[9], 10);
	strcat(t_h2[9]," KB");
	_itoa(video_hardware->vfmt->BitsPerPixel, t_h2[10], 10);
	strcat(t_h2[10]," Bit");

	SDL_BlitSurface(about,0,bild,0);
	SDL_BlitSurface(TTF_RenderText_Solid(arial_bold,"Informationen �ber Videohardware:",black),0,bild,&headline_pos);
	for(int j=0;j<11;j++)
	{
		SDL_BlitSurface(TTF_RenderText_Solid(arial,text_hardware[j],black),0,bild,&text_pos);
		SDL_BlitSurface(TTF_RenderText_Solid(arial,t_h2[j],black),0,bild,&t_h2_pos);
		text_pos.y+=30;
		t_h2_pos.y+=30;
	}
	SDL_BlitSurface(TTF_RenderText_Solid(arial,"<beliebige Taste>",black),0,bild,&ende_pos);
	SDL_UpdateRect(bild,0,0,0,0);
	
	SDL_Event about_event;

	while(about_event.type!=SDL_KEYDOWN)
	{
		SDL_PollEvent(&about_event);
	}

	TTF_CloseFont(arial_bold);
	TTF_CloseFont(arial);
	
	SDL_FreeSurface(about);
	SDL_FreeSurface(bild);
}

void free_game(SDL_Surface *bildschirm)
{
	SDL_ShowCursor(SDL_ENABLE);
	setting game_s;
	game_s=game_setting();

	game(game_s);

	SDL_FreeSurface(bildschirm);
}

setting game_setting()
{
	setting standart;

	standart.piece_set="grafik/piece_set.gif";

	return standart;
}

void game(setting game_s)
{
	SDL_Surface *bildschirm;
	SDL_Surface *hintergrund;
	int matrix[9][9];
	long time1, time2, time_dif, time_gr2=0, time_gre;
	int gr_anzahl=0, akt_anzahl=0;
	int klicks_ges=0, klicks_fl=0, klicks_pf=0;	
	
	
	SDL_Rect matrix_pos;

	SDL_Event event_m;

	bildschirm=SDL_GetVideoSurface();
	hintergrund=SDL_DisplayFormat(IMG_Load("grafik/hintergrund.jpg"));

	matrix_pos.x=75;
	matrix_pos.y=75;
	matrix_pos.w=450;
	matrix_pos.h=450;
	
	random_matrix(matrix);

	SDL_BlitSurface(hintergrund,0,bildschirm,0);

	draw_arrows(matrix_pos);
	
	time(&time1);
	
	while(!whether_gameend(matrix))
	{
		draw_matrix(matrix,matrix_pos,game_s.piece_set);
		SDL_PollEvent(&event_m);
		switch(event_m.type)
		{
		case SDL_KEYDOWN: 			
			if(event_m.key.keysym.sym==SDLK_ESCAPE)
				return;			
			break;		
		case SDL_MOUSEBUTTONUP:
			klicks_ges++;			
			SDL_BlitSurface(hintergrund,&matrix_pos,bildschirm,&matrix_pos);			

			if(event_m.button.x>=matrix_pos.x && event_m.button.x<=matrix_pos.w+matrix_pos.x
				&& event_m.button.y>=matrix_pos.y && event_m.button.y<=matrix_pos.h+matrix_pos.y)
			{			
				klicks_fl++;
				akt_anzahl=look_for_match(matrix, int(event_m.button.x-matrix_pos.x)/50, int(event_m.button.y-matrix_pos.y)/50);
				if(akt_anzahl>gr_anzahl)
				{
					gr_anzahl=akt_anzahl;					
					time(&time_gr2);
				}
			}
			
			
			if(event_m.button.x>matrix_pos.x && event_m.button.x<matrix_pos.x+matrix_pos.w
				&& event_m.button.y>matrix_pos.y-50 && event_m.button.y<matrix_pos.y)
			{			
				klicks_pf++;
				change_matrix(matrix, "oben",int((event_m.button.x-matrix_pos.x)/50));
			}			
			if(event_m.button.x>matrix_pos.w+matrix_pos.x && event_m.button.x<matrix_pos.w+matrix_pos.x+50
				&& event_m.button.y>matrix_pos.y && event_m.button.y<matrix_pos.h+matrix_pos.y)
			{			
				klicks_pf++;
				change_matrix(matrix, "rechts",int((event_m.button.y-matrix_pos.y)/50));
			}
			if(event_m.button.x>matrix_pos.x && event_m.button.x<matrix_pos.w+matrix_pos.x
				&& event_m.button.y>matrix_pos.y+matrix_pos.h && event_m.button.y<matrix_pos.y+matrix_pos.h+50)
			{			
				klicks_pf++;
				change_matrix(matrix, "unten",int((event_m.button.x-matrix_pos.x)/50));
			}
			if(event_m.button.x>matrix_pos.x-50 && event_m.button.x<matrix_pos.x
				&& event_m.button.y>matrix_pos.y && event_m.button.y<matrix_pos.h+matrix_pos.y)
			{			
				klicks_pf++;
				change_matrix(matrix, "links",int((event_m.button.y-matrix_pos.y)/50));
			}
			
			break;
		}
	}
	
	time(&time2);

	time_dif=(time2-time1)*1000;
	time_gre=(time_gr2-time1)*1000;	

	stat_menue(time_dif,time_gre,klicks_ges,klicks_fl,klicks_pf,gr_anzahl);	

	SDL_FreeSurface(bildschirm);
	SDL_FreeSurface(hintergrund);
}

void random_matrix(int matrix[9][9])
{
	srand(SDL_GetTicks());
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			matrix[i][j]=(rand()%5)+1;
		}
	}
}

void draw_matrix(int matrix[9][9], SDL_Rect matrix_pos, char* piece_set_path)
{
	SDL_Surface *bildschirm;
	SDL_Surface *pieces;

	SDL_Rect piece_pos;
	SDL_Rect piece_ko;

	bildschirm=SDL_GetVideoSurface();
	pieces=SDL_DisplayFormat(IMG_Load(piece_set_path));

	SDL_SetColorKey(pieces,SDL_SRCCOLORKEY | SDL_RLEACCEL,SDL_MapRGB(pieces->format,255,0,255));

	for(int i=0;i<=8;i++)
	{
		for(int j=0;j<=8;j++)
		{
			piece_pos.x=matrix_pos.x+i*50;
			piece_pos.y=matrix_pos.y+j*50;

			piece_ko.x=matrix[i][j]*50;
			piece_ko.y=0;
			piece_ko.w=((matrix[i][j]+1)*50)-piece_ko.x;
			piece_ko.h=50;

			SDL_BlitSurface(pieces,&piece_ko,bildschirm,&piece_pos);
		}
	}

	SDL_UpdateRect(bildschirm,0,0,0,0);

	SDL_FreeSurface(bildschirm);
	SDL_FreeSurface(pieces);
}

void draw_arrows(SDL_Rect matrix_pos)
{
	SDL_Surface *bildschirm;
	SDL_Surface *direction_arrows;

	SDL_Rect pfeil_ko;
	SDL_Rect pfeil_pos;

	bildschirm=SDL_GetVideoSurface();
	direction_arrows=SDL_DisplayFormat(IMG_Load("grafik/pfeile.gif"));
	SDL_SetColorKey(direction_arrows,SDL_RLEACCEL | SDL_SRCCOLORKEY,SDL_MapRGB(direction_arrows->format,255,255,255));
	
	pfeil_ko.x=0;
	pfeil_ko.y=0;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int i=0;i<9;i++)
	{
		pfeil_pos.x=matrix_pos.x+i*50;
		pfeil_pos.y=matrix_pos.y-50;

		SDL_BlitSurface(direction_arrows,&pfeil_ko,bildschirm,&pfeil_pos);
	}

	pfeil_ko.x=50;
	pfeil_ko.y=0;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int j=0;j<9;j++)
	{
		pfeil_pos.x=matrix_pos.x+matrix_pos.w;
		pfeil_pos.y=matrix_pos.y+j*50;

		SDL_BlitSurface(direction_arrows,&pfeil_ko,bildschirm,&pfeil_pos);
	}

	pfeil_ko.x=50;
	pfeil_ko.y=50;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int k=0;k<9;k++)
	{
		pfeil_pos.x=matrix_pos.x+k*50;
		pfeil_pos.y=matrix_pos.y+matrix_pos.h;

		SDL_BlitSurface(direction_arrows,&pfeil_ko,bildschirm,&pfeil_pos);
	}

	pfeil_ko.x=0;
	pfeil_ko.y=50;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int l=0;l<9;l++)
	{
		pfeil_pos.x=matrix_pos.x-50;
		pfeil_pos.y=matrix_pos.y+l*50;

		SDL_BlitSurface(direction_arrows,&pfeil_ko,bildschirm,&pfeil_pos);
	}

	SDL_FreeSurface(bildschirm);
	SDL_FreeSurface(direction_arrows);
}

void change_matrix(int matrix[9][9], char *arrows, int nr)
{
	int temp;
	
	if(arrows=="oben")
	{
		temp=matrix[nr][0];
		for(int i=0;i<9;i++)
		{
			matrix[nr][i]=matrix[nr][i+1];
		}
		matrix[nr][8]=temp;
	}

	if(arrows=="unten")
	{
		temp=matrix[nr][8];
		for(int i=8;i>=0;i--)
		{
			matrix[nr][i]=matrix[nr][i-1];
		}
		matrix[nr][0]=temp;
	}

	if(arrows=="links")
	{
		temp=matrix[0][nr];
		for(int i=0;i<9;i++)
		{
			matrix[i][nr]=matrix[i+1][nr];
		}
		matrix[8][nr]=temp;
	}

	if(arrows=="rechts")
	{
		temp=matrix[8][nr];
		for(int i=8;i>=0;i--)
		{
			matrix[i][nr]=matrix[i-1][nr];
		}
		matrix[0][nr]=temp;
	}
}

int look_for_match(int m[9][9], int x, int y)
{
	int farbe;
	int pos_x[80];
	int pos_y[80];
	int anzahl=0;

	int i=0;

	farbe=m[x][y];
	pos_x[0]=x;
	pos_y[0]=y;
	
	do
	{
		if(m[pos_x[i]][pos_y[i]-1]==farbe)
		{
			if(pos_y[i]-1>=0)
			{
				m[pos_x[i]][pos_y[i]-1]=0;
				anzahl++;
				pos_x[i+1]=pos_x[i];
				pos_y[i+1]=pos_y[i]-1;
				i++;
				continue;
			}
		}
		
		if(m[pos_x[i]+1][pos_y[i]]==farbe)
		{
			m[pos_x[i]+1][pos_y[i]]=0;
			anzahl++;
			pos_x[i+1]=pos_x[i]+1;
			pos_y[i+1]=pos_y[i];
			i++;
			continue;
		}
		
		if(m[pos_x[i]][pos_y[i]+1]==farbe)
		{
			if(pos_y[i]+1<=8)
			{
				m[pos_x[i]][pos_y[i]+1]=0;
				anzahl++;
				pos_x[i+1]=pos_x[i];
				pos_y[i+1]=pos_y[i]+1;
				i++;
				continue;
			}
		}
		
		if(m[pos_x[i]-1][pos_y[i]]==farbe)
		{
			m[pos_x[i]-1][pos_y[i]]=0;
			anzahl++;
			pos_x[i+1]=pos_x[i]-1;
			pos_y[i+1]=pos_y[i];		
			i++;
			continue;
		}
		
		if(i!=0)
			i--;
			
	}while(i!=0);

		return anzahl;	
}

int whether_gameend(int m[9][9])
{	
	int farbe[6]={0,0,0,0,0,0};

	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(m[i][j]!=0)
			{				
				farbe[m[i][j]]++;
			}
			else
				continue;
		}
	}	

	if(farbe[1]<2 && farbe[2]<2 && farbe[3]<2 && farbe[4]<2 && farbe[5]<2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void stat_menue(int time, int time2, int k_ges, int k_fla, int k_pfe, int max_anz)
{
	SDL_Surface *bild=SDL_GetVideoSurface();
	SDL_Surface *stat_back=IMG_Load("grafik/stat_back.jpg");

	TTF_Font *copperplate=TTF_OpenFont("fonts/copperplate.ttf",50);
	TTF_Font *copperplate2=TTF_OpenFont("fonts/copperplate.ttf",20);

	SDL_Rect headline={120,50,0,0};
	SDL_Rect name_pos={130,110,0,0};
	SDL_Rect time_pos={100,200,0,0};
	SDL_Rect punkte_pos={500,200,0,0};
	SDL_Rect stat1_pos={80,400,0,0};
	SDL_Rect k_pos={290,430,0,0};
	SDL_Rect stat2_pos={410,430,0,0};
	SDL_Rect rest_pos={650,430,0,0};
	SDL_Rect pos_pos={100,310,0,0};
	SDL_Rect ende_pos={150,550,0,0};

	SDL_Color black={0,0,0};

	SDL_Event stat_event;
	
	int p=0;
	int pos=0;

	char t_name[30];
	char t_zeit[100], t_zeit_min[10], t_zeit_sek[10];
	char t_punkte[100], t_p[10];
	char t_stat1[4][60];
	char t_k[3][10];
	char t_stat2[3][60];
	char t_rest[3][10];
	char t_pos[50];
	char t_pos_int[5];

	hs hs_in;

	SDL_ShowCursor(0);
	
	p=(max_anz*1000000*100)/(time2*k_ges);

	_i64toa((int)time/60000,t_zeit_min,10);
	_i64toa((time%60000/1000),t_zeit_sek,10);

	strcpy(t_zeit,"Zeit: ");
	strcat(t_zeit,t_zeit_min);
	strcat(t_zeit," Min. ");
	strcat(t_zeit,t_zeit_sek);
	strcat(t_zeit," Sek.");

	_i64toa(p,t_p,10);
	strcpy(t_punkte,"Punkte: ");
	strcat(t_punkte,t_p);

	_i64toa(k_ges,t_k[0],10);
	_i64toa(k_fla,t_k[1],10);
	_i64toa(k_pfe,t_k[2],10);

	strcpy(t_stat1[0],"Statistik:");
	strcpy(t_stat1[1],"Klicks gesammt:");
	strcpy(t_stat1[2],"Klicks im Feld:");
	strcpy(t_stat1[3],"Klicks auf Pfeile:");

	_itoa(max_anz,t_rest[0],10);
	_gcvt(double(k_ges)/(time/1000),4,t_rest[1]);
	
	_itoa(time2/1000,t_rest[2],10);
	strcat(t_rest[2]," Sek.");

	strcpy(t_stat2[0],"Gr. Fl�che:");
	strcpy(t_stat2[1],"Klicks pro Sekunde:");
	strcpy(t_stat2[2],"Zeit f�r gr. Fl�che:");
	
	SDL_BlitSurface(stat_back,0,bild,0);

	SDL_BlitSurface(TTF_RenderText_Solid(copperplate,"SPIELENDE",black),0,bild,&headline);	
	
	hs_in.punkte=p;
	hs_in.gr_fla=max_anz;
	hs_in.zeit=time2/1000;
	
	pos=pos_hs(hs_in);

	if(pos!=0)
	{
		SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,"Name: ",black),0,bild,&name_pos);
		strcpy(t_name,input_name(bild));

		strcpy(hs_in.name,t_name);

		in_hs(hs_in);
		
		strcpy(t_pos,"Sie haben den ");
		strcat(t_pos,_itoa(pos,t_pos_int,10));
		strcat(t_pos,"-ten Platz belegt");
	}
	else
	{
		strcpy(t_pos,"Ihre Leistung war nicht ausreichend f�r die Highscore");
	}

	SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_pos,black),0,bild,&pos_pos);
	
	SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_zeit,black),0,bild,&time_pos);
	SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_punkte,black),0,bild,&punkte_pos);
	
	for(int i=0;i<4;i++)
	{
		SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_stat1[i],black),0,bild,&stat1_pos);
		stat1_pos.y+=30;
		if(i==3) continue;
		SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_k[i],black),0,bild,&k_pos);
		k_pos.y+=30;
	}

	for(i=0;i<3;i++)
	{
		SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_stat2[i],black),0,bild,&stat2_pos);
		stat2_pos.y+=30;
		SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_rest[i],black),0,bild,&rest_pos);
		rest_pos.y+=30;
	}

	SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,"<bel. Taste druecken um zurueck zu kehren>",black),0,bild,&ende_pos);
	SDL_UpdateRect(bild,0,0,0,0);

	do
	{
		SDL_PollEvent(&stat_event);
	}while(stat_event.type!=SDL_KEYDOWN);

	TTF_CloseFont(copperplate);
	TTF_CloseFont(copperplate2);

	SDL_FreeSurface(bild);
	SDL_FreeSurface(stat_back);
}

char *input_name(SDL_Surface *prev_bild)
{
	SDL_Surface *bild=SDL_GetVideoSurface();
	SDL_Event input_event;
	TTF_Font *copperplate2=TTF_OpenFont("fonts/copperplate.ttf",20);
	SDL_Rect name_pos={202,110,0,0};
	SDL_Color black={0,0,0};

	int i=0;
	int taste=0;

	char t_name[20]="";
	char *temp_data="grafik/input.bmp";

	SDL_SaveBMP(prev_bild,temp_data);
	prev_bild=SDL_LoadBMP(temp_data);
	remove(temp_data);

	do
	{
		if(SDL_PollEvent(&input_event))
		{
			SDL_BlitSurface(prev_bild,0,bild,0);			
			
			SDL_BlitSurface(TTF_RenderText_Solid(copperplate2,t_name,black),0,bild,&name_pos);		
		
			SDL_UpdateRect(bild,0,0,0,0);

			if(input_event.key.type==SDL_KEYDOWN)
				taste=input_event.key.keysym.sym;			
		}
		
		if((taste>=SDLK_0 && taste<=SDLK_9) || (taste>=SDLK_a && taste<=SDLK_z)) // || taste==SDLK_SPACE)
		{
			if(taste!=32)
			{
				if(taste=='z')
					taste='y';
				else if(taste=='y')
					taste='z';

				t_name[i]=(char)taste-32;
			}
			else
				t_name[i]=(char)taste;
			i++;
			t_name[i]='\0';
			taste=0;
			if(i==20)
			{
				i--;
				t_name[i]='\0';
			}
		}
		else if(taste==SDLK_BACKSPACE)
		{
			i--;
			t_name[i]='\0';
			taste=0;
		}

	}while(taste!=SDLK_RETURN);

	TTF_CloseFont(copperplate2);

	SDL_FreeSurface(bild);
	SDL_FreeSurface(prev_bild);

	return t_name;
}

void write_hs(hs *hs_out)
{
	ofstream highscore_out("highscore.txt",ios::out | ios::trunc | ios::nocreate);

	for(int i=0;i<10;i++)
	{
		highscore_out<<hs_out->name<<endl;
		highscore_out<<hs_out->punkte<<endl;
		highscore_out<<hs_out->gr_fla<<endl;
		highscore_out<<hs_out->zeit<<endl<<endl;

		hs_out++;
	}

	highscore_out.close();
}

void read_hs(hs *hs_in)
{
	ifstream highscore_in("highscore.txt",ios::in | ios::nocreate);
	
	for(int i=0;i<10;i++)
	{
		highscore_in>>hs_in->name;
		highscore_in>>hs_in->punkte;
		highscore_in>>hs_in->gr_fla;
		highscore_in>>hs_in->zeit;

		hs_in++;
	}

	highscore_in.close();	
}

void hilfe_menue()
{
	SDL_Surface *bild=SDL_GetVideoSurface();
	SDL_Surface *hilfe_back=IMG_Load("grafik/info_back.jpg");

	SDL_Rect hilfe_pos={50,50,0,0};
	SDL_Rect ende_pos={130,550,0,0};

	SDL_Event hilfe_event;

	TTF_Font *courier=TTF_OpenFont("fonts/courier_fett.ttf",20);

	SDL_Color black={0,0,0};

	char hilfe[20][200];

	SDL_ShowCursor(0);

	ifstream hilfe_in("hilfe.txt", ios::in | ios::nocreate);
	for(int i=0;i<15;i++)
	{
		hilfe_in.getline(hilfe[i],100);
	}
	hilfe_in.close();

	SDL_BlitSurface(hilfe_back,0,bild,0);
	for(int j=0;j<15;j++)
	{
		SDL_BlitSurface(TTF_RenderText_Solid(courier,hilfe[j],black),0,bild,&hilfe_pos);
		hilfe_pos.y+=25;
	}
	SDL_BlitSurface(TTF_RenderText_Solid(courier,"<belibige Taste dr�cken um zur�ck zu kehren>",black),0,bild,&ende_pos);
	SDL_UpdateRect(bild,0,0,0,0);

	do
	{
		SDL_PollEvent(&hilfe_event);
	}while(hilfe_event.type!=SDL_KEYDOWN);

	TTF_CloseFont(courier);

	SDL_FreeSurface(bild);
	SDL_FreeSurface(hilfe_back);
}

void in_hs(hs hs_input)
{
	hs akt_hs[10];
	hs new_hs[10];
	
	read_hs(akt_hs);
	
	if(hs_input.punkte>akt_hs[9].punkte)
	{
		for(int i=0;i<10;i++)
		{
			if(hs_input.punkte>akt_hs[i].punkte)
			{
				for(int j=i+1;j<10;j++)
				{
					new_hs[j]=akt_hs[j-1];
				}

				for(int k=0;k<i;k++)
				{
					new_hs[k]=akt_hs[k];
				}
				new_hs[i]=hs_input;
				write_hs(new_hs);
				return;
			}
		}
	}
}

int pos_hs(hs hs_input)
{
	hs akt_hs[10];
	int ret_n=0;

	read_hs(akt_hs);

	if(hs_input.punkte>akt_hs[9].punkte)
	{
		for(int i=0;i<10;i++)
		{
			if(hs_input.punkte>akt_hs[i].punkte)
			{
				ret_n=i+1;
				break;
			}
		}
	}
	else
		ret_n=0;
	
	return ret_n;
}