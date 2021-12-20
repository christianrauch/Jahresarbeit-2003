/*
Jahresarbeit2003 - Jahresarbeit im Fach Informatik
Copyright (C) 2003  Christian Rauch
http://www.sdlgames.de.tf

Dieses Programm ist freie Software. Sie können es unter den Bedingungen der GNU General Public License,
wie von der Free Software Foundation veröffentlicht, weitergeben und/oder modifizieren, entweder gemäß
Version 2 der Lizenz oder (nach Ihrer Option) jeder späteren Version.

Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, daß es Ihnen von Nutzen sein wird, aber
OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT FÜR
EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben.
Falls nicht, schreiben Sie an die Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
MA 02111-1307, USA.
*/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_syswm.h>
#include <time.h>
#include <sys/stat.h>

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
void menu(SDL_Renderer *ausgabe_bild, int *field);
void hs_menue(SDL_Renderer *bild);
void hardware_info(SDL_Renderer *bild);
void free_game(SDL_Renderer *bildschirm);
setting game_setting();
void game(SDL_Renderer *bildschirm);
void random_matrix(int matrix[9][9]);
void draw_matrix(int matrix[9][9], SDL_Rect matrix_pos);
void draw_arrows(SDL_Rect Matrix_pos);
void change_matrix(int matrix[9][9], char *arrows, int nr);
int look_for_match(int matrix[9][9], int pos_x, int pos_y);
int whether_gameend(int m[9][9]);
void stat_menue(int time, int time2, int k_ges, int k_fla, int k_pfe, int max_anz);
void input_name(SDL_Renderer *prev_bild, char *t_name);
void write_hs(hs *hs_out);
void read_hs(hs *hs_in);
void hilfe_menue(SDL_Renderer *bild);
void in_hs(hs hs_input);
int pos_hs(hs hs_input);

int main()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	TTF_Init();

	program();

	TTF_Quit();
	SDL_Quit();
	return 0;
}

SDL_Window *window;

char *user_config_path;

char *data_path;

struct
{
	SDL_Surface *background;
	SDL_Surface *menu_text;
	SDL_Surface *menu_back;
	SDL_Surface *info_back;
	SDL_Surface *piece_set;
	SDL_Surface *direction_arrows;
	SDL_Surface *stat_back;
}imgs;

struct
{
	TTF_Font *copperplate2;
	TTF_Font *bloody;
	TTF_Font *bradley;
	TTF_Font *comic;
	TTF_Font *arial_bold;
	TTF_Font *arial;
	TTF_Font *copperplate;
	TTF_Font *marker;
	TTF_Font *courier;
}font;

char hilfe[20][200];

void load_resources()
{
	setting game_s;

	data_path = SDL_GetBasePath();
	game_s=game_setting();
	
	char path[4096];
	
	snprintf(path, 4096, "%s/%s", data_path, "grafik/hintergrund.jpg");
	imgs.background = IMG_Load(path);
	snprintf(path, 4096, "%s/%s", data_path, "grafik/menu_text.gif");
	imgs.menu_text = IMG_Load(path);
	snprintf(path, 4096, "%s/%s", data_path, "grafik/menu_back.jpg");
	imgs.menu_back = IMG_Load(path);
	snprintf(path, 4096, "%s/%s", data_path, "grafik/info_back.jpg");
	imgs.info_back = IMG_Load(path);
	snprintf(path, 4096, "%s/%s", data_path, game_s.piece_set);
	imgs.piece_set = IMG_Load(path);
	snprintf(path, 4096, "%s/%s", data_path, "grafik/pfeile.gif");
	imgs.direction_arrows = IMG_Load(path);
	snprintf(path, 4096, "%s/%s", data_path, "grafik/stat_back.jpg");
	imgs.stat_back = IMG_Load(path);
	
	snprintf(path, 4096, "%s/%s", data_path, "fonts/IndieFlower.ttf");
	font.copperplate2=TTF_OpenFont(path,30);
	snprintf(path, 4096, "%s/%s", data_path, "fonts/DoubleFeature.ttf");
	font.bloody=TTF_OpenFont(path,50);
	snprintf(path, 4096, "%s/%s", data_path, "fonts/IndieFlower.ttf");
	font.bradley=TTF_OpenFont(path,30);
	snprintf(path, 4096, "%s/%s", data_path, "fonts/PermanentMarker.ttf");
	font.comic=TTF_OpenFont(path,20);
	
	snprintf(path, 4096, "%s/%s", data_path, "fonts/SourceSansPro-Bold.ttf");
	font.arial_bold=TTF_OpenFont(path,30);
	snprintf(path, 4096, "%s/%s", data_path, "fonts/SourceSansPro-Regular.ttf");
	font.arial=TTF_OpenFont(path,20);
	
	snprintf(path, 4096, "%s/%s", data_path, "fonts/IndieFlower.ttf");
	font.copperplate=TTF_OpenFont(path,50);
	
	snprintf(path, 4096, "%s/%s", data_path, "fonts/PermanentMarker.ttf");
	font.marker=TTF_OpenFont(path,30);
	snprintf(path, 4096, "%s/%s", data_path, "fonts/SourceCodePro-Bold.ttf");
	font.courier=TTF_OpenFont(path,20);
	
	memset(hilfe, 0, sizeof(hilfe));
	
	snprintf(path, 4096, "%s/%s", data_path, "hilfe.txt");
	FILE *fp = fopen(path, "r");
	for(int i=0;i<15;i++)
	{
		fgets(hilfe[i], 200, fp);
		strtok(hilfe[i], "\r\n");
	}
	fclose(fp);
}

void free_resources()
{
	SDL_FreeSurface(imgs.background);
	SDL_FreeSurface(imgs.menu_text);
	SDL_FreeSurface(imgs.menu_back);
	SDL_FreeSurface(imgs.info_back);
	SDL_FreeSurface(imgs.piece_set);
	SDL_FreeSurface(imgs.direction_arrows);
	SDL_FreeSurface(imgs.stat_back);
	
	TTF_CloseFont(font.copperplate2);
	TTF_CloseFont(font.bloody);
	TTF_CloseFont(font.bradley);
	TTF_CloseFont(font.comic);
	TTF_CloseFont(font.arial_bold);
	TTF_CloseFont(font.arial);
	TTF_CloseFont(font.copperplate);
	TTF_CloseFont(font.marker);
	TTF_CloseFont(font.courier);
	
	free(data_path);
}

void program()
{
	int field=2;
	
	window = SDL_CreateWindow("JA2003",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, 0);
	
	SDL_Renderer *bildschirm = SDL_CreateRenderer(window, -1, 0);
	
	load_resources();
	
	user_config_path = strcat(getenv("HOME"), "/.config/");
	mkdir(user_config_path, S_IRWXU | S_IRWXG);
	strcat(user_config_path, "jahresarbeit2003/");
	mkdir(user_config_path, S_IRWXU | S_IRWXG);

	int running=1;
	while(running)
	{
		menu(bildschirm, &field);
		switch(field)
		{
		case 2:
			free_game(bildschirm);
			break;
		case 3:
			hs_menue(bildschirm);
			break;
		case 4:
			hardware_info(bildschirm);
			break;
		case 5:
			hilfe_menue(bildschirm);
			break;
		case 6:
			running=0;
			break;
		}
	}
	
	free_resources();
	
	SDL_DestroyRenderer(bildschirm);
	SDL_DestroyWindow(window);
}

void menu(SDL_Renderer *ausgabe_bild, int *field)
{
	SDL_ShowCursor(SDL_DISABLE);

	SDL_Texture *background;
	SDL_Texture *menu_text;
	SDL_Texture *menu_back;

	SDL_Event menu_event;

	SDL_Surface *surf;
	surf = SDL_ConvertSurfaceFormat(imgs.background, SDL_PIXELFORMAT_RGB24, 0);
	background=SDL_CreateTextureFromSurface(ausgabe_bild, surf);
	SDL_FreeSurface(surf);
	surf = SDL_ConvertSurfaceFormat(imgs.menu_text, SDL_PIXELFORMAT_RGB24, 0);
	SDL_SetColorKey(surf,SDL_TRUE | SDL_RLEACCEL,SDL_MapRGB(surf->format,255,255,255));
	menu_text=SDL_CreateTextureFromSurface(ausgabe_bild, surf);
	SDL_FreeSurface(surf);
	surf = SDL_ConvertSurfaceFormat(imgs.menu_back, SDL_PIXELFORMAT_RGB24, 0);
	menu_back=SDL_CreateTextureFromSurface(ausgabe_bild, surf);
	SDL_FreeSurface(surf);


	SDL_Rect area;
	SDL_Rect ausgabe_area;
	SDL_Rect menu_pos;
	SDL_Rect menu_back_pos;

	menu_pos.x=250;
	menu_pos.y=150;
	menu_pos.w=300;
	menu_pos.h=50;

	area.x=0;
	area.w=300;
	area.h=50;

	menu_back_pos.x=0;
	menu_back_pos.y=0;
	menu_back_pos.w=300;
	menu_back_pos.h=50;

	ausgabe_area.x=menu_pos.x;
	ausgabe_area.w=300;
	ausgabe_area.h=50;

	int running=1;
	while(running)
	{
		for(int i=2;i<7;i++)
		{
			if(*field==i)
			{
				area.y=i*50;
				ausgabe_area.y=menu_pos.y+area.y;
			}
			else
				continue;
		}
		
		SDL_RenderCopy(ausgabe_bild, background, 0, 0);
		SDL_RenderCopy(ausgabe_bild, menu_back, &menu_back_pos, &menu_pos);
		SDL_RenderCopy(ausgabe_bild, menu_back, &area, &ausgabe_area);
		SDL_RenderCopy(ausgabe_bild, menu_text, 0, 0);
		SDL_RenderPresent(ausgabe_bild);

		SDL_WaitEvent(&menu_event);

		if(menu_event.type==SDL_KEYDOWN)
		{
			switch(menu_event.key.keysym.sym)
			{
			case SDLK_UP:
				(*field)--;
				break;
			case SDLK_DOWN:
				(*field)++;
				break;
			case SDLK_RETURN:
				running=0;
				break;
			case SDLK_ESCAPE:
				exit(0);
			}

			switch(*field)
			{
			case 1: *field=6; break;
			case 7: *field=2; break;
			}
		}
	}
	
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(menu_text);
	SDL_DestroyTexture(menu_back);
}

void hs_menue(SDL_Renderer *bild)
{
	SDL_Texture *highscore_back;

	SDL_Color black={0,0,0};
	SDL_Color red={255,0,0};

	hs highscorer[10];

	char punkte[10][50];
	char gr_fla[10][50];
	char zeit[10][50];

	int z_abst=30;

	SDL_ShowCursor(0);

	read_hs(highscorer);

	for(int i=0;i<10;i++)
	{
		sprintf(punkte[i],"%d", highscorer[i].punkte);
		sprintf(gr_fla[i],"%d", highscorer[i].gr_fla);
		sprintf(zeit[i],"%d", highscorer[i].zeit);
	}
	
	SDL_Surface *surf;
	surf = SDL_ConvertSurfaceFormat(imgs.info_back, SDL_PIXELFORMAT_RGB24, 0);
	highscore_back=SDL_CreateTextureFromSurface(bild, surf);
	SDL_FreeSurface(surf);

	SDL_RenderCopy(bild, highscore_back, 0, 0);

	surf = TTF_RenderUTF8_Solid(font.bloody,"HIGHSCORE",red);
	SDL_Texture *highscore_text=SDL_CreateTextureFromSurface(bild, surf);
	SDL_Rect headline_pos={200,60,surf->w,surf->h};
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, highscore_text, 0, &headline_pos);

	surf = TTF_RenderUTF8_Solid(font.bradley,"Name",black);
	SDL_Texture *name_text=SDL_CreateTextureFromSurface(bild, surf);
	SDL_Rect name_pos={120,170,surf->w,surf->h};
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, name_text, 0, &name_pos);
	
	surf = TTF_RenderUTF8_Solid(font.bradley,"Punkte",black);
	SDL_Texture *punkte_text=SDL_CreateTextureFromSurface(bild, surf);
	SDL_Rect punkte_pos={470,170,surf->w,surf->h};
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, punkte_text, 0, &punkte_pos);
	
	surf = TTF_RenderUTF8_Solid(font.bradley,"Fläche",black);
	SDL_Texture *fla_text=SDL_CreateTextureFromSurface(bild, surf);
	SDL_Rect gr_fla_pos={580,170,surf->w,surf->h};
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, fla_text, 0, &gr_fla_pos);
	
	surf = TTF_RenderUTF8_Solid(font.bradley,"Zeit",black);
	SDL_Texture *zeit_text=SDL_CreateTextureFromSurface(bild, surf);
	SDL_Rect zeit_pos={690,170,surf->w,surf->h};
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, zeit_text, 0, &zeit_pos);

	name_pos.x=100, name_pos.y=200;
	punkte_pos.x=500, punkte_pos.y=200;
	gr_fla_pos.x=600, gr_fla_pos.y=200;
	zeit_pos.x=700, zeit_pos.y=200;

	for(int j=0;j<10;j++)
	{
		surf = TTF_RenderUTF8_Solid(font.comic,highscorer[j].name,black);
		SDL_Texture *name_text=SDL_CreateTextureFromSurface(bild, surf);
		name_pos.w = surf->w;
		name_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, name_text, 0, &name_pos);
		name_pos.y+=z_abst;
		
		surf = TTF_RenderUTF8_Solid(font.bradley,punkte[j],black);
		SDL_Texture *punkte_text=SDL_CreateTextureFromSurface(bild, surf);
		punkte_pos.w = surf->w;
		punkte_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, punkte_text, 0, &punkte_pos);
		punkte_pos.y+=z_abst;
		
		surf = TTF_RenderUTF8_Solid(font.bradley,gr_fla[j],black);
		SDL_Texture *gr_fla_text=SDL_CreateTextureFromSurface(bild, surf);
		gr_fla_pos.w = surf->w;
		gr_fla_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, gr_fla_text, 0, &gr_fla_pos);
		gr_fla_pos.y+=z_abst;
		
		surf = TTF_RenderUTF8_Solid(font.bradley,zeit[j],black);
		SDL_Texture *zeit_text=SDL_CreateTextureFromSurface(bild, surf);
		zeit_pos.w = surf->w;
		zeit_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, zeit_text, 0, &zeit_pos);
		zeit_pos.y+=z_abst;
	}

	surf = TTF_RenderUTF8_Solid(font.copperplate2,"<bel. Taste druecken um zurueck zu kehren>",black);
	SDL_Texture *ende_text=SDL_CreateTextureFromSurface(bild, surf);
	SDL_Rect ende_pos={150,550,surf->w,surf->h};
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, ende_text, 0, &ende_pos);
	SDL_RenderPresent(bild);

	SDL_Event highscore_event;

	while(SDL_WaitEvent(&highscore_event) && highscore_event.type!=SDL_KEYDOWN);

	SDL_DestroyTexture(highscore_back);
	SDL_DestroyTexture(highscore_text);
}

void hardware_info(SDL_Renderer *bild)
{
	SDL_Surface *about;
	SDL_RendererInfo* info = calloc(1, sizeof(SDL_RendererInfo));
	SDL_SysWMinfo *wm_info = calloc(1, sizeof(SDL_SysWMinfo));;
	SDL_Color black={0,0,0};
	about=SDL_ConvertSurfaceFormat(imgs.info_back, SDL_PIXELFORMAT_RGB24, 0);
	char text_hardware[12][100];
	char t_h2[12][100];
	SDL_Rect headline_pos={50,25};
	SDL_Rect text_pos={50,100,0,0};
	SDL_Rect t_h2_pos={600,100,0,0};
	SDL_Rect ende_pos={300,500,0,0};
	
	memset(text_hardware, 0, sizeof(text_hardware));
	memset(t_h2, 0, sizeof(t_h2));

	SDL_GetRendererInfo(bild, info);
	SDL_VERSION(&(wm_info->version));
	SDL_GetWindowWMInfo(window, wm_info);
	
	const char *subsystem;
	switch(wm_info->subsystem)
	{
	case SDL_SYSWM_UNKNOWN:   subsystem = "unknown";            break;
	case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows";  break;
	case SDL_SYSWM_X11:       subsystem = "X Window System";    break;
	case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";           break;
	case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";         break;
	case SDL_SYSWM_UIKIT:     subsystem = "UIKit";              break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
	case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";            break;
	case SDL_SYSWM_MIR:       subsystem = "Mir";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 3)
	case SDL_SYSWM_WINRT:     subsystem = "WinRT";              break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
	case SDL_SYSWM_ANDROID:   subsystem = "Android";            break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
	case SDL_SYSWM_VIVANTE:   subsystem = "Vivante";            break;
#endif
	case SDL_SYSWM_OS2:       subsystem = "OS/2";               break;
	}
	
	SDL_ShowCursor(0);
	
	strcpy(text_hardware[0],"Platform:");
	strcpy(text_hardware[1],"Windowmanager:");
	strcpy(text_hardware[2],"Renderer:");
	strcpy(text_hardware[3],"SDL version:");
	strcpy(text_hardware[4],"Software-Fallback:");
	strcpy(text_hardware[5],"Hardwarebeschleunigung:");
	strcpy(text_hardware[6],"Synchronisiert mit Bildwiederholfrequenz:");
	strcpy(text_hardware[7],"Unterstützt Wiedergabe auf Textur:");
	strcpy(text_hardware[8],"Anzahl Textur formate:");
	strcpy(text_hardware[9],"Maximale Textur Breite:");
	strcpy(text_hardware[10],"Maximale Textur Höhe:");
	strcpy(text_hardware[11],"Arbeitsspeicher (MB):");
	
	strcpy(t_h2[0],SDL_GetPlatform());
	strcpy(t_h2[1],subsystem);
	strcpy(t_h2[2],info->name);
	sprintf(t_h2[3],"%d.%d.%d", wm_info->version.major, wm_info->version.minor, wm_info->version.patch);
	strcpy(t_h2[4],info->flags & SDL_RENDERER_SOFTWARE ? "Ja" : "Nein");
	strcpy(t_h2[5],info->flags & SDL_RENDERER_ACCELERATED ? "Ja" : "Nein");
	strcpy(t_h2[6],info->flags & SDL_RENDERER_PRESENTVSYNC ? "Ja" : "Nein");
	strcpy(t_h2[7],info->flags & SDL_RENDERER_TARGETTEXTURE ? "Ja" : "Nein");
	sprintf(t_h2[8],"%d",info->num_texture_formats);
	sprintf(t_h2[9],"%d",info->max_texture_width);
	sprintf(t_h2[10],"%d",info->max_texture_height);
	sprintf(t_h2[11],"%d",SDL_GetSystemRAM());

	SDL_Texture *about_text=SDL_CreateTextureFromSurface(bild, about);
	SDL_RenderCopy(bild, about_text, 0, 0);
	SDL_Surface *surf;
	surf = TTF_RenderUTF8_Solid(font.arial_bold,"Informationen über Videohardware:",black);
	SDL_Texture *headline=SDL_CreateTextureFromSurface(bild, surf);
	headline_pos.w = surf->w;
	headline_pos.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, headline, 0, &headline_pos);
	for(int j=0;j<12;j++)
	{
		surf = TTF_RenderUTF8_Solid(font.arial,text_hardware[j],black);
		if(surf!=0) {
			SDL_Texture *text=SDL_CreateTextureFromSurface(bild, surf);
			text_pos.w = surf->w;
			text_pos.h = surf->h;
			SDL_FreeSurface(surf);
			SDL_RenderCopy(bild, text, 0, &text_pos);
		}
		surf = TTF_RenderUTF8_Solid(font.arial,t_h2[j],black);
		if(surf!=0) {
			SDL_Texture *t_h2=SDL_CreateTextureFromSurface(bild, surf);
			t_h2_pos.w = surf->w;
			t_h2_pos.h = surf->h;
			SDL_FreeSurface(surf);
			SDL_RenderCopy(bild, t_h2, 0, &t_h2_pos);
		}
		text_pos.y+=30;
		t_h2_pos.y+=30;
	}
	surf = TTF_RenderUTF8_Solid(font.arial,"<beliebige Taste>",black);
	SDL_Texture *ende=SDL_CreateTextureFromSurface(bild, surf);
	ende_pos.w = surf->w;
	ende_pos.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, ende, 0, &ende_pos);
	SDL_RenderPresent(bild);

	SDL_Event about_event;

	while(SDL_WaitEvent(&about_event) && about_event.type!=SDL_KEYDOWN);
	
	free(wm_info);
	free(info);

	SDL_FreeSurface(about);
}

void free_game(SDL_Renderer *bildschirm)
{
	SDL_ShowCursor(SDL_ENABLE);

	game(bildschirm);
}

setting game_setting()
{
	setting standart;

	standart.piece_set="grafik/piece_set.gif";

	return standart;
}

void game(SDL_Renderer *bildschirm)
{
	int matrix[9][9];
	long time1, time2, time_dif, time_gr2=0, time_gre;
	int gr_anzahl=0, akt_anzahl=0;
	int klicks_ges=0, klicks_fl=0, klicks_pf=0;


	SDL_Rect matrix_pos;

	SDL_Event event_m;

	matrix_pos.x=75;
	matrix_pos.y=75;
	matrix_pos.w=450;
	matrix_pos.h=450;

	random_matrix(matrix);

	time(&time1);

	while(!whether_gameend(matrix))
	{
		draw_matrix(matrix,matrix_pos);
		SDL_WaitEvent(&event_m);
		switch(event_m.type)
		{
		case SDL_KEYDOWN:
			if(event_m.key.keysym.sym==SDLK_ESCAPE)
				return;
			break;
		case SDL_MOUSEBUTTONUP:
			klicks_ges++;

			if(event_m.button.x>=matrix_pos.x && event_m.button.x<=matrix_pos.w+matrix_pos.x
				&& event_m.button.y>=matrix_pos.y && event_m.button.y<=matrix_pos.h+matrix_pos.y)
			{
				klicks_fl++;
				akt_anzahl=look_for_match(matrix, (event_m.button.x-matrix_pos.x)/50, (event_m.button.y-matrix_pos.y)/50);
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
				change_matrix(matrix, "oben", (event_m.button.x-matrix_pos.x)/50);
			}
			if(event_m.button.x>matrix_pos.w+matrix_pos.x && event_m.button.x<matrix_pos.w+matrix_pos.x+50
				&& event_m.button.y>matrix_pos.y && event_m.button.y<matrix_pos.h+matrix_pos.y)
			{
				klicks_pf++;
				change_matrix(matrix, "rechts", (event_m.button.y-matrix_pos.y)/50);
			}
			if(event_m.button.x>matrix_pos.x && event_m.button.x<matrix_pos.w+matrix_pos.x
				&& event_m.button.y>matrix_pos.y+matrix_pos.h && event_m.button.y<matrix_pos.y+matrix_pos.h+50)
			{
				klicks_pf++;
				change_matrix(matrix, "unten", (event_m.button.x-matrix_pos.x)/50);
			}
			if(event_m.button.x>matrix_pos.x-50 && event_m.button.x<matrix_pos.x
				&& event_m.button.y>matrix_pos.y && event_m.button.y<matrix_pos.h+matrix_pos.y)
			{
				klicks_pf++;
				change_matrix(matrix, "links", (event_m.button.y-matrix_pos.y)/50);
			}

			break;
		}
	}

	time(&time2);

	time_dif=(time2-time1)*1000;
	time_gre=(time_gr2-time1)*1000;

	stat_menue(time_dif,time_gre,klicks_ges,klicks_fl,klicks_pf,gr_anzahl);
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

void draw_matrix(int matrix[9][9], SDL_Rect matrix_pos)
{
	SDL_Renderer *bildschirm;
	SDL_Surface *hintergrund;
	SDL_Surface *pieces;

	SDL_Rect piece_pos = {.w=50, .h=50};
	SDL_Rect piece_ko;

	bildschirm=SDL_GetRenderer(window);
	hintergrund=SDL_ConvertSurfaceFormat(imgs.background, SDL_PIXELFORMAT_RGB24, 0);
	SDL_Texture *hintergrund_txt=SDL_CreateTextureFromSurface(bildschirm, hintergrund);
	SDL_RenderCopy(bildschirm, hintergrund_txt, 0, 0);
	draw_arrows(matrix_pos);
	pieces=SDL_ConvertSurfaceFormat(imgs.piece_set, SDL_PIXELFORMAT_RGB24, 0);
	SDL_SetColorKey(pieces,SDL_TRUE | SDL_RLEACCEL,SDL_MapRGB(pieces->format,255,0,255));
	SDL_Texture *pieces_txt=SDL_CreateTextureFromSurface(bildschirm, pieces);
	SDL_FreeSurface(pieces);

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

			SDL_RenderCopy(bildschirm, pieces_txt, &piece_ko, &piece_pos);
		}
	}

	SDL_RenderPresent(bildschirm);
	
	SDL_FreeSurface(hintergrund);
}

void draw_arrows(SDL_Rect matrix_pos)
{
	SDL_Renderer *bildschirm;
	SDL_Surface *direction_arrows;

	SDL_Rect pfeil_ko;
	SDL_Rect pfeil_pos = {.w=50, .h=50};

	bildschirm=SDL_GetRenderer(window);
	direction_arrows=SDL_ConvertSurfaceFormat(imgs.direction_arrows, SDL_PIXELFORMAT_RGB24, 0);
	SDL_SetColorKey(direction_arrows,SDL_RLEACCEL | SDL_TRUE,SDL_MapRGB(direction_arrows->format,255,255,255));
	SDL_Texture *direction_arrows_txt=SDL_CreateTextureFromSurface(bildschirm, direction_arrows);

	pfeil_ko.x=0;
	pfeil_ko.y=0;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int i=0;i<9;i++)
	{
		pfeil_pos.x=matrix_pos.x+i*50;
		pfeil_pos.y=matrix_pos.y-50;

		SDL_RenderCopy(bildschirm, direction_arrows_txt, &pfeil_ko, &pfeil_pos);
	}

	pfeil_ko.x=50;
	pfeil_ko.y=0;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int j=0;j<9;j++)
	{
		pfeil_pos.x=matrix_pos.x+matrix_pos.w;
		pfeil_pos.y=matrix_pos.y+j*50;

		SDL_RenderCopy(bildschirm, direction_arrows_txt, &pfeil_ko, &pfeil_pos);
	}

	pfeil_ko.x=50;
	pfeil_ko.y=50;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int k=0;k<9;k++)
	{
		pfeil_pos.x=matrix_pos.x+k*50;
		pfeil_pos.y=matrix_pos.y+matrix_pos.h;

		SDL_RenderCopy(bildschirm, direction_arrows_txt, &pfeil_ko, &pfeil_pos);
	}

	pfeil_ko.x=0;
	pfeil_ko.y=50;
	pfeil_ko.w=50;
	pfeil_ko.h=50;

	for(int l=0;l<9;l++)
	{
		pfeil_pos.x=matrix_pos.x-50;
		pfeil_pos.y=matrix_pos.y+l*50;

		SDL_RenderCopy(bildschirm, direction_arrows_txt, &pfeil_ko, &pfeil_pos);
	}

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

	if(farbe==0)
	{
		return 0;
	}

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
	SDL_Renderer *bild=SDL_GetRenderer(window);

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
	char t_pos[55];
	char t_pos_int[5];

	hs hs_in;

	SDL_ShowCursor(0);

	p=(max_anz*1000000*100)/(time2*k_ges);

	sprintf(t_zeit_min, "%d", (int)time/60000);
	sprintf(t_zeit_sek, "%d", (time%60000/1000));

	strcpy(t_zeit,"Zeit: ");
	strcat(t_zeit,t_zeit_min);
	strcat(t_zeit," Min. ");
	strcat(t_zeit,t_zeit_sek);
	strcat(t_zeit," Sek.");

	sprintf(t_p, "%d", p);
	strcpy(t_punkte,"Punkte: ");
	strcat(t_punkte,t_p);

	sprintf(t_k[0], "%d", k_ges);
	sprintf(t_k[1], "%d", k_fla);
	sprintf(t_k[2], "%d", k_pfe);

	strcpy(t_stat1[0],"Statistik:");
	strcpy(t_stat1[1],"Klicks gesammt:");
	strcpy(t_stat1[2],"Klicks im Feld:");
	strcpy(t_stat1[3],"Klicks auf Pfeile:");

	sprintf(t_rest[0], "%d", max_anz);
	gcvt(k_ges/(time/1000),4,t_rest[1]);

	sprintf(t_rest[2], "%d", time2/1000);
	strcat(t_rest[2]," Sek.");

	strcpy(t_stat2[0],"Gr. Fläche:");
	strcpy(t_stat2[1],"Klicks pro Sekunde:");
	strcpy(t_stat2[2],"Zeit für gr. Fläche:");

	SDL_Texture *stat_back_txt=SDL_CreateTextureFromSurface(bild, imgs.stat_back);
	SDL_RenderCopy(bild, stat_back_txt, 0, 0);

	SDL_Surface *surf;
	surf = TTF_RenderUTF8_Solid(font.copperplate,"SPIELENDE",black);
	SDL_Texture *headline_txt=SDL_CreateTextureFromSurface(bild, surf);
	headline.w = surf->w;
	headline.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, headline_txt, 0, &headline);

	hs_in.punkte=p;
	hs_in.gr_fla=max_anz;
	hs_in.zeit=time2/1000;

	pos=pos_hs(hs_in);

	if(pos!=0)
	{
		surf = TTF_RenderUTF8_Solid(font.copperplate2,"Name: ",black);
		SDL_Texture *name=SDL_CreateTextureFromSurface(bild, surf);
		name_pos.w = surf->w;
		name_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, name, 0, &name_pos);
		input_name(bild, t_name);

		strcpy(hs_in.name,t_name);

		in_hs(hs_in);

		strcpy(t_pos,"Sie haben den ");
		sprintf(t_pos_int, "%d", pos);
		strcat(t_pos,t_pos_int);
		strcat(t_pos,"-ten Platz belegt");
	}
	else
	{
		strcpy(t_pos,"Ihre Leistung war nicht ausreichend für die Highscore");
	}

	surf = TTF_RenderUTF8_Solid(font.copperplate2,t_pos,black);
	SDL_Texture *pos_txt=SDL_CreateTextureFromSurface(bild, surf);
	pos_pos.w = surf->w;
	pos_pos.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, pos_txt, 0, &pos_pos);

	surf = TTF_RenderUTF8_Solid(font.copperplate2,t_zeit,black);
	SDL_Texture *time_txt=SDL_CreateTextureFromSurface(bild, surf);
	time_pos.w = surf->w;
	time_pos.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, time_txt, 0, &time_pos);
	surf = TTF_RenderUTF8_Solid(font.copperplate2,t_punkte,black);
	SDL_Texture *punkte=SDL_CreateTextureFromSurface(bild, surf);
	punkte_pos.w = surf->w;
	punkte_pos.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, punkte, 0, &punkte_pos);

	for(int i=0;i<4;i++)
	{
		surf = TTF_RenderUTF8_Solid(font.copperplate2,t_stat1[i],black);
		SDL_Texture *name=SDL_CreateTextureFromSurface(bild, surf);
		stat1_pos.w = surf->w;
		stat1_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, name, 0, &stat1_pos);
		stat1_pos.y+=30;
		if(i==3) continue;
		surf = TTF_RenderUTF8_Solid(font.copperplate2,t_k[i],black);
		SDL_Texture *k=SDL_CreateTextureFromSurface(bild, surf);
		k_pos.w = surf->w;
		k_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, k, 0, &k_pos);
		k_pos.y+=30;
	}

	for(int i=0;i<3;i++)
	{
		surf = TTF_RenderUTF8_Solid(font.copperplate2,t_stat2[i],black);
		SDL_Texture *stat2=SDL_CreateTextureFromSurface(bild, surf);
		stat2_pos.w = surf->w;
		stat2_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, stat2, 0, &stat2_pos);
		stat2_pos.y+=30;
		surf = TTF_RenderUTF8_Solid(font.copperplate2,t_rest[i],black);
		SDL_Texture *rest=SDL_CreateTextureFromSurface(bild, surf);
		rest_pos.w = surf->w;
		rest_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, rest, 0, &rest_pos);
		rest_pos.y+=30;
	}

	surf = TTF_RenderUTF8_Solid(font.copperplate2,"<bel. Taste druecken um zurueck zu kehren>",black);
	SDL_Texture *ende=SDL_CreateTextureFromSurface(bild, surf);
	ende_pos.w = surf->w;
	ende_pos.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, ende, 0, &ende_pos);
	SDL_RenderPresent(bild);

	while(SDL_WaitEvent(&stat_event) && stat_event.type!=SDL_KEYDOWN);
}

void input_name(SDL_Renderer *prev_bild, char *t_name)
{
	SDL_Renderer *bild=SDL_GetRenderer(window);
	SDL_Event input_event;
	SDL_Rect name_pos={205,110,0,0};
	SDL_Color black={0,0,0};

	int i=0;
	int taste=0;
	
	SDL_Texture *stat_back=SDL_CreateTextureFromSurface(bild, imgs.stat_back);

	memset(t_name, 0, 30);
	
	SDL_Rect max_name = {.x=name_pos.x, .y=name_pos.y};
	
	SDL_RenderPresent(bild);

	do
	{
		if(SDL_WaitEvent(&input_event))
		{
			if(input_event.key.state==SDL_PRESSED) {
				taste=input_event.key.keysym.sym;
			
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
					if(i<0) i=0;
					t_name[i]='\0';
					taste=0;
				}
				
				SDL_RenderCopy(bild, stat_back, &max_name, &max_name);
				
				SDL_Surface *surf = TTF_RenderUTF8_Solid(font.marker,t_name,black);
				if(surf!=0) {
					SDL_Texture *name=SDL_CreateTextureFromSurface(bild, surf);
					name_pos.w = surf->w;
					name_pos.h = surf->h;
					if(name_pos.w>max_name.w) max_name.w = name_pos.w;
					if(name_pos.h>max_name.h) max_name.h = name_pos.h;
					SDL_FreeSurface(surf);
					SDL_RenderCopy(bild, name, 0, &name_pos);
				}
				
				SDL_RenderPresent(bild);
				SDL_Delay(1);
			}
		}
	}while(taste!=SDLK_RETURN);
}

void write_hs(hs *hs_out)
{
	char fullpath[200];
	sprintf(fullpath, "%s%s", user_config_path, "highscore.txt");
	FILE *fp = fopen(fullpath, "w");

	for(int i=0;i<10;i++)
	{
		fprintf(fp, "%s\n", hs_out->name);
		fprintf(fp, "%d\n", hs_out->punkte);
		fprintf(fp, "%d\n", hs_out->gr_fla);
		fprintf(fp, "%d\n\n", hs_out->zeit);

		hs_out++;
	}

	fclose(fp);
}

void read_hs(hs *hs_in)
{
	FILE *fp;

	char fullpath[200];
	sprintf(fullpath, "%s%s", user_config_path, "highscore.txt");
	fp = fopen(fullpath, "r");
	if(fp==0) {
		// open template
		char path[4096];
		snprintf(path, 4096, "%s/%s", data_path, "highscore.txt");
		fp = fopen(path, "r");
	}

	for(int i=0;i<10;i++)
	{
		fscanf(fp, "%s", hs_in->name);
		fscanf(fp, "%d", &hs_in->punkte);
		fscanf(fp, "%d", &hs_in->gr_fla);
		fscanf(fp, "%d", &hs_in->zeit);

		hs_in++;
	}

	fclose(fp);
}

void hilfe_menue(SDL_Renderer *bild)
{
	SDL_Rect hilfe_pos={50,50,0,0};

	SDL_Event hilfe_event;

	SDL_Color black={0,0,0};

	SDL_ShowCursor(0);

	SDL_Texture *hilfe_back=SDL_CreateTextureFromSurface(bild, imgs.info_back);
	SDL_RenderCopy(bild, hilfe_back, 0, 0);

	for(int j=0;j<15;j++)
	{
		SDL_Surface *surf;
		surf=TTF_RenderUTF8_Solid(font.courier,hilfe[j],black);
		if(surf!=0) {
		SDL_Texture *hilfe_text=SDL_CreateTextureFromSurface(bild, surf);
		hilfe_pos.w = surf->w;
		hilfe_pos.h = surf->h;
		SDL_FreeSurface(surf);
		SDL_RenderCopy(bild, hilfe_text, 0, &hilfe_pos);
		}
		hilfe_pos.y+=25;
	}
	SDL_Surface *surf;
	surf=TTF_RenderUTF8_Solid(font.courier,"<belibige Taste drücken um zurück zu kehren>",black);
	SDL_Texture *ende_text=SDL_CreateTextureFromSurface(bild, surf);
	SDL_Rect ende_pos={130,550,surf->w,surf->h};
	SDL_FreeSurface(surf);
	SDL_RenderCopy(bild, ende_text, 0, &ende_pos);
	SDL_RenderPresent(bild);

	while(SDL_WaitEvent(&hilfe_event) && hilfe_event.type!=SDL_KEYDOWN);

	SDL_DestroyTexture(hilfe_back);
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
