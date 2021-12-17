
// compile: gcc -Wall -o MasterMindV2 MasterMindV2.c $(pkg-config gtk+-3.0 --cflags --libs)

#include <gtk/gtk.h>
#include <stdlib.h>


GtkWidget *window;
GtkWidget *grid;
GtkWidget *drawing_area;

GDateTime *date_time;
gchar *strTime;
gchar *strKlokDatum;

gdouble grote;
int xArea, yArea;
int xWit, yWit, xZwart, yZwart, xGeel, yGeel, xGroen, yGroen, xRood, yRood, xBlauw, yBlauw;

char *validatietekst = " PLAY";
int kleurveld[7];
int kleur = 0;

int veld[9][14];    // vier voor pionnen en vier voor evaluatie
int xveld[5][14];   // voor aanklikpositie x
int yveld[5][14];   // voor aanklikpositie y
int rij = 1;        // aktieve rij

gdouble ngx1, ngx2, ngy1, ngy2;     // new game knop
gdouble okx1, okx2, oky1, oky2;     // OK knop
gboolean knopOKenable = FALSE;
gboolean laatstekans = FALSE;
gboolean codejuist = FALSE;
gboolean buttonOKMuisOver = FALSE;
gboolean buttonNewGameMuisOver = FALSE;
gboolean buttonNewGameIsIngedrukt = FALSE;

// voor animated weergave:
gdouble rgbR = 0.00;
gdouble rgbG = 0.00;
gdouble rgbB = 0.00;
int rgbRtel = 0;
int rgbGtel = 0;
int rgbBtel = 0;
gboolean rgbRood = TRUE;
gboolean rgbGroen = TRUE;
gboolean rgbBlauw = TRUE;
gboolean rgbRtelUp = TRUE;
gboolean rgbGtelUp = TRUE;
gboolean rgbBtelUp = TRUE;


// declaretions:
gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean get_Tijd();
gboolean mouse_moved(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean checkrij(int R);
gboolean checkcombinatie(int R);
gchar *str(int x);
void plaats_te_zoeken_code();
void reset_velden();
void init_kleuren();
void kies_kleur();
void animatie();


gchar *str(int x)
{
    gchar *y = g_strdup_printf("%i", x);        //  maak een string van en integer
    return y;
}

void init_kleuren()
{
    for (int i=1; i<7; i++)
    {
        kleurveld[i] = i;
    }
}

gboolean checkrij(int R)     // R is rij 1 tot 12
{
    gboolean butOK_Enable = TRUE; 
    for (int i=1; i<5; i++)
    {
        if (veld[i][R] == 0)
            butOK_Enable = FALSE;
    }
    return butOK_Enable;
}

gboolean checkcombinatie(int R)     // R is rij 1 tot 12
{
    gboolean combinatie = FALSE;
    int A, B, C, D;
    int Ac, Bc, Cc, Dc;
    int juist = 0;
    int juisteplaats = 0;
    int chkI[5];
    int chkX[5];
    for (int j=1; j<5; j++)
    {
        chkI[j] = 0;
        chkX[j] = 0;
    }
    A = veld[1][R];
    B = veld[2][R];
    C = veld[3][R];
    D = veld[4][R];
    Ac = veld[1][13];
    Bc = veld[2][13];
    Cc = veld[3][13];
    Dc = veld[4][13];
    // hoeveel op juiste plaats ?
    if (A == Ac){ juisteplaats++; }
    if (B == Bc){ juisteplaats++; }
    if (C == Cc){ juisteplaats++; }
    if (D == Dc){ juisteplaats++; }
    // alle plaatsen juist, dan combinatie TRUE
    if (juisteplaats == 4)
            combinatie = TRUE;
    // hoeveel kleuren juist ?
    for (int x=1; x<5; x++)
    {
        for (int i=1; i<5; i++)
        {
            A = veld[i][R];
            B = veld[x][13];
            C = chkI[i];
            D = chkX[x];
            if ((A == B) & (C == 0) & (D == 0))
            {
                chkI[i] = i;
                chkX[x] = x;
                juist++;
            }
        }
    }
    // zet witte en zwarte blokjes in matrix voor aantal kleuren en juiste plaatsen ( kollommen 5 6 7 en 8 )
    for (int i= 5; i<5+juist; i++)
    {
        veld[i][R] = 1;  // WIT
    }
    for (int i= 5; i<5+juisteplaats; i++)
    {
        veld[i][R] = 2;  // ZWART
    }
    return combinatie;
}

void plaats_te_zoeken_code()
{
	srand(time(NULL));
    g_print("Code = ");
    for (int i=1; i<5; i++)
    {
	    veld[i][13] = (rand()%6) + 1;
        g_print("%i ", veld[i][13]);
    }
    g_print("\n");
}

void reset_velden()
{
    for (int x=1; x<9; x++)
    {
        for (int y=1; y<14; y++)
        {
            veld[x][y] = 0;
        }
    }
}

void kies_kleur()
{
    if ((xArea < xWit+grote) & (xArea > xWit-grote) & (yArea < yWit+grote) & (yArea > yWit-grote))
        kleur = 1;
    else if ((xArea < xZwart+grote) & (xArea > xZwart-grote) & (yArea < yZwart+grote) & (yArea > yZwart-grote))
        kleur = 2;
    else if ((xArea < xGeel+grote) & (xArea > xGeel-grote) & (yArea < yGeel+grote) & (yArea > yGeel-grote))
        kleur = 3;
    else if ((xArea < xGroen+grote) & (xArea > xGroen-grote) & (yArea < yGroen+grote) & (yArea > yGroen-grote))
        kleur = 4;
    else if ((xArea < xRood+grote) & (xArea > xRood-grote) & (yArea < yRood+grote) & (yArea > yRood-grote))
        kleur = 5;
    else if ((xArea < xBlauw+grote) & (xArea > xBlauw-grote) & (yArea < yBlauw+grote) & (yArea > yBlauw-grote))
        kleur = 6;
    else
        kleur = 0;
}

gboolean mouse_moved(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    if (event->type == GDK_MOTION_NOTIFY)
    {
        GdkEventMotion *e = (GdkEventMotion*)event;
        xArea = (guint)e->x;
        yArea = (guint)e->y;
        if ((xArea > okx1) & (xArea < okx2) & (yArea > oky1) & (yArea < oky2))
            buttonOKMuisOver = TRUE;
        else
            buttonOKMuisOver = FALSE;
        if ((xArea > ngx1) & (xArea < ngx2) & (yArea > ngy1) & (yArea < ngy2)) // New Game knop
            buttonNewGameMuisOver = TRUE;
        else
            buttonNewGameMuisOver = FALSE;
    }
    return TRUE;
}

gboolean button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == GDK_BUTTON_PRIMARY)
    {
        usleep(200000);
        buttonNewGameIsIngedrukt = FALSE;
    }
    return TRUE;
}

gboolean button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if (event->button == GDK_BUTTON_PRIMARY)
    {
        if ((kleur == 0) | (yArea > yWit-grote))
        {
            kies_kleur();
        }
        else
        {
            for (int x=1; x<5; x++)
            {
                for (int y=1; y<13; y++)
                {
                    if ((xArea < xveld[x][y]+grote) & (xArea > xveld[x][y]-grote) & 
                        (yArea < yveld[x][y]+grote) & (yArea > yveld[x][y]-grote) & 
                        (yArea > oky1) & (yArea < oky2))
                    {
                        veld[x][y] = kleur;
                        if (checkrij(rij))
                            knopOKenable = TRUE;
                        else
                            knopOKenable = FALSE;
                        kleur = 0;
                    }
                }
            }
        }
        if ((xArea > okx1) & (xArea < okx2) & (yArea > oky1) & (yArea < oky2) & (knopOKenable)) // OK knop
        {
            knopOKenable = FALSE;
            if (checkcombinatie(rij))
            {
                codejuist = TRUE;
                validatietekst = "WINNER";
            }
            else
            {
                rij++;
                if (rij > 12)
                {
                    rij = 12;
                    laatstekans = TRUE;
                    validatietekst = "TO BAD";
                }
            }
        }
        if ((xArea > ngx1) & (xArea < ngx2) & (yArea > ngy1) & (yArea < ngy2)) // New Game knop
        {
            buttonNewGameIsIngedrukt = TRUE;
            knopOKenable = FALSE;
            laatstekans = FALSE;
            codejuist = FALSE;
            validatietekst = " PLAY";
            kleur = 0;
            rij = 1;
            reset_velden();
            plaats_te_zoeken_code();
        }
    }
    else if (event->button == GDK_BUTTON_SECONDARY)
    {
        if (kleur == 0)
            kies_kleur();
        else
            kleur = 0;
    }
    return TRUE;
}

gboolean get_Tijd()
{
    date_time = g_date_time_new_now_local();                            // get local time
    strKlokDatum = g_date_time_format(date_time, "%d:%m:%y");           // datum
    strTime = g_date_time_format(date_time, "%e %b %Y - %H:%M:%S");     // 24hr time format
    gtk_window_set_title (GTK_WINDOW (window), strTime);

    animatie();    // voor animated weergave

    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}

void animatie()
{
    if (rgbRood)
    {
        rgbGroen = FALSE; rgbBlauw = FALSE;
        if (rgbRtelUp)
        {
            rgbRtel+=8;
            if (rgbRtel > 95){ rgbRtelUp = FALSE; }
        }
        else
        {
            rgbRtel--;
            if (rgbRtel < 5){ rgbRtelUp = TRUE; rgbRood = FALSE; rgbGroen = TRUE; }
        }
        rgbR = rgbRtel / 100.0;
    }
    else if (rgbGroen)
    {
        rgbRood = FALSE; rgbBlauw = FALSE;
        if (rgbGtelUp)
        {
            rgbGtel+=8;
            if (rgbGtel > 95){ rgbGtelUp = FALSE; }
        }
        else
        {
            rgbGtel--;
            if (rgbGtel < 5){ rgbGtelUp = TRUE; rgbGroen = FALSE; rgbBlauw = TRUE; }
        }
        rgbG = rgbGtel / 100.0;
    }
    else if (rgbBlauw)
    {
        rgbRood = FALSE; rgbGroen = FALSE;
        if (rgbBtelUp)
        {
            rgbBtel+=8;
            if (rgbBtel > 95){ rgbBtelUp = FALSE; }
        }
        else
        {
            rgbBtel--;
            if (rgbBtel < 5){ rgbBtelUp = TRUE; rgbBlauw = FALSE; rgbRood = TRUE; }
        }
        rgbB = rgbBtel / 100.0;
    }
}

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    int i, j;
    gdouble x, y;
    gdouble width, height;
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    grote = width / 16;   // waarde kiezen naargelang de instellingen van 'drawing_area'

    // kader tekenen:
    cairo_set_line_width(cr, 2.0);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 0,0);
    cairo_line_to(cr, width, 0);
    cairo_line_to(cr, width, height);
    cairo_line_to(cr, 0, height);
    cairo_line_to(cr, 0, 0);
    cairo_stroke(cr);

    // kader inkleuren:
    cairo_set_source_rgb(cr, 0.55, 0.35, 0.10);
    cairo_rectangle (cr, 1, 1, width-2, height-2);
    cairo_fill (cr);

    // lijntje tekenen tussen speelbord en kleuren keuze:
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 0, 13*grote*2.5+grote);
    cairo_line_to(cr, width, 13*grote*2.5+grote);
    cairo_stroke(cr);

    // plaats kleuren:
    y = 35.5*grote;
    i = 0;
    for (x = grote*1.5 + 4; x < 6*grote*2.5; x = x + grote*2.5)
    {
        i++;
        if (kleurveld[i] == 1)       // kleur is WIT
        {
            cairo_set_source_rgb(cr, 0.99, 0.99, 0.99); xWit = x; yWit = y;
        }
        else if (kleurveld[i] == 2)  // kleur is ZWART
        {
            cairo_set_source_rgb(cr, 0.00, 0.00, 0.00); xZwart = x; yZwart = y;
        }
        else if (kleurveld[i] == 3)  // kleur is GEEL
        {
            cairo_set_source_rgb(cr, 0.85, 0.85, 0.00); xGeel = x; yGeel = y;
        }
        else if (kleurveld[i] == 4)  // kleur is GROEN
        {
            cairo_set_source_rgb(cr, 0.00, 0.80, 0.00); xGroen = x; yGroen = y;
        }
        else if (kleurveld[i] == 5)  // kleur is ROOD
        {
            cairo_set_source_rgb(cr, 0.80, 0.00, 0.00); xRood = x; yRood = y;
        }
        else if (kleurveld[i] == 6)  // kleur is BLAUW
        {
            cairo_set_source_rgb(cr, 0.00, 0.00, 0.80); xBlauw = x; yBlauw = y;
        }
        else                       // kleur is GEEN 
        {
            cairo_set_source_rgb(cr, 0.65, 0.65, 0.65);
        }
        cairo_arc (cr, x, y, grote, 0, 2 * G_PI);
        cairo_fill (cr);
    }

    // plaats velden:
    i = 0;
    j = 14;
    for (y = grote*1.5; y < 13*grote*2.5; y = y + grote*2.5)
    {
        i = 0;
        j--;
        for (x = grote*1.5 + grote; x < 8*grote*2.5 + grote; x = x + grote*2.5)
        {
            i++;
            if (veld[i][j] == 1)       // kleur is WIT
            {
                cairo_set_source_rgb(cr, 0.99, 0.99, 0.99);
            }
            else if (veld[i][j] == 2)  // kleur is ZWART
            {
                cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);
            }
            else if (veld[i][j] == 3)  // kleur is GEEL
            {
                cairo_set_source_rgb(cr, 0.85, 0.85, 0.00);
            }
            else if (veld[i][j] == 4)  // kleur is GROEN
            {
                cairo_set_source_rgb(cr, 0.00, 0.80, 0.00);
            }
            else if (veld[i][j] == 5)  // kleur is ROOD
            {
                cairo_set_source_rgb(cr, 0.80, 0.00, 0.00);
            }
            else if (veld[i][j] == 6)  // kleur is BLAUW
            {
                cairo_set_source_rgb(cr, 0.00, 0.00, 0.80);
            }
            else                       // kleur is GEEN 
            {
                if ((j == rij) & (i < 5))
                {
                    cairo_set_source_rgb(cr, 0.65, 0.55, 0.35);  // (cr, 0.85, 0.85, 0.85);
                    oky1 = y-grote; oky2 = y+grote; // voor OK knop
                }
                else
                {
                    cairo_set_source_rgb(cr, 0.65, 0.45, 0.10);  // (cr, 0.65, 0.65, 0.65);
                }
            }
            if (i<5)                                // veld 1, 2, 3 en 4
            {
                xveld[i][j] = x; yveld[i][j] = y;
                cairo_arc (cr, x, y, grote, 0, 2 * G_PI);
                cairo_fill (cr);
            }
            if (j<13)
            {
                if (i == 5)                         // veld 5
                {
                    cairo_arc (cr, x+grote*0.5, y-grote*0.5, grote*0.4, 0, 2 * G_PI);
                    cairo_fill (cr);
                }
                else if (i == 6)                    // veld 6
                {
                    cairo_arc (cr, x-grote, y-grote*0.5, grote*0.4, 0, 2 * G_PI);
                    cairo_fill (cr);
                }
                else if (i == 7)                    // veld 7
                {
                    cairo_arc (cr, x-grote*4.5, y+grote*0.5, grote*0.4, 0, 2 * G_PI);
                    cairo_fill (cr);
                }
                else if (i == 8)                    // veld 8
                {
                    cairo_arc (cr, x-grote*6, y+grote*0.5, grote*0.4, 0, 2 * G_PI);
                    cairo_fill (cr);
                }
            }
        }
    }

    // bedek code tot ze geraden is, of tot na de laatste kans
    if ((!laatstekans) & (!codejuist))
    {
        gdouble xcl = grote*1.4;
        gdouble ycl = grote*0.4;
        gdouble clw = grote*9.7;
        gdouble clh = grote*2.2;
        cairo_set_source_rgb(cr, 0.35, 0.20, 0.05);     // bedekkingrand
        cairo_rectangle (cr, xcl-0.5, ycl-0.5, clw+3.5, clh+3.5);
        cairo_fill (cr);
        cairo_set_source_rgb(cr, 0.55, 0.30, 0.05);     // bedekking
        cairo_rectangle (cr, xcl, ycl, clw, clh);
        cairo_fill (cr);
        cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);     // tekst schaduw
        cairo_set_font_size(cr, grote*1.2);
        cairo_move_to(cr, xcl+grote*1.2+1, ycl+grote*1.5+2);
        cairo_show_text(cr, "Master Mind");
        cairo_set_source_rgb(cr, rgbR, rgbG, rgbB);     // tekst (animated)
        cairo_set_font_size(cr, grote*1.2);
        cairo_move_to(cr, xcl+grote*1.2, ycl+grote*1.5);
        cairo_show_text(cr, "Master Mind");
    }

    // tekst: ' Play ', 'to bad' of 'winner'
    cairo_set_source_rgb(cr, 0.99, 0.99, 0.99);     // tekst belichting
    cairo_set_font_size(cr, grote);
    cairo_move_to(cr, 11.5*grote-1, grote*1.9-2);
    cairo_show_text(cr, validatietekst);    
    cairo_set_source_rgb(cr, 0.00, 0.00, 0.85);     // tekst
    cairo_set_font_size(cr, grote);
    cairo_move_to(cr, 11.5*grote, grote*1.9);
    cairo_show_text(cr, validatietekst);    

    // knop ok:
    if (knopOKenable)
    {
        int okw = grote*3;
        int okh = grote*2;
        okx1 = 12*grote; okx2 = okx1 + okw;  // voor OK knop
        if (buttonOKMuisOver)
            cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
        else
            cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
        cairo_rectangle (cr, okx1, oky1, okw, okh);  // x, y, breedte, hoogte
        cairo_fill (cr);
        cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);
        cairo_set_font_size(cr, grote);
        cairo_move_to(cr, okx1+grote*0.5, oky1+grote*1.4);
        cairo_show_text(cr, "OK?");
    }

    // knop New Game:
    x = grote*4;
    y = grote*38;
    int ngw = grote*8;
    int ngh = grote*2;
    ngx1 = x; ngx2 = x + ngw; ngy1 = y; ngy2 = y + ngh;     // voor New Game knop
    cairo_set_source_rgb(cr, 0.35, 0.35, 0.35);     // knoprand
    cairo_rectangle (cr, x-1, y-1, ngw+3, ngh+3);
    cairo_fill (cr);
    if (buttonNewGameMuisOver)
    {
        if (buttonNewGameIsIngedrukt)
            cairo_set_source_rgb(cr, 0.65, 0.65, 0.65);     // knop zelf
        else
            cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);     // knop zelf
    }
    else
    {
        cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);     // knop zelf
    }
    cairo_rectangle (cr, x, y, ngw, ngh);
    cairo_fill (cr);
    cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);     // knop tekst
    cairo_set_font_size(cr, grote);
    cairo_move_to(cr, x+grote*1.2, y+grote*1.4);
    cairo_show_text(cr, "New Game");

    // kleurenbolletje van muispunt:
    if (kleur > 0)
    {
        if (kleur == 1)       // kleur is WIT
            cairo_set_source_rgb(cr, 0.99, 0.99, 0.99);
        else if (kleur == 2)  // kleur is ZWART
            cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);
        else if (kleur == 3)  // kleur is GEEL
            cairo_set_source_rgb(cr, 0.85, 0.85, 0.00);
        else if (kleur == 4)  // kleur is GROEN
            cairo_set_source_rgb(cr, 0.00, 0.80, 0.00);
        else if (kleur == 5)  // kleur is ROOD
            cairo_set_source_rgb(cr, 0.80, 0.00, 0.00);
        else if (kleur == 6)  // kleur is BLAUW
            cairo_set_source_rgb(cr, 0.00, 0.00, 0.80);
        cairo_arc (cr, xArea, yArea, grote, 0, 2 * G_PI);
        cairo_fill (cr);
    }

    // volgende is maar bijzaak, niet nodig!
    // figuurtje:
    // links:
    x = grote*1.5;
    y = height-grote*1.7;
    cairo_set_source_rgb(cr, 0.85-rgbR, 0.85, 0.00);
    cairo_arc (cr, x, y, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_arc (cr, x+grote*1.2, y, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_arc (cr, x+grote*0.6, y-grote*0.6, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_arc (cr, x+grote*0.6, y+grote*0.6, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_set_source_rgb(cr, 0.80, 0.50-rgbG*0.5, rgbB);
    cairo_arc (cr, x+grote*0.6, y, grote*0.3, 0, 2 * G_PI);
    cairo_fill (cr);
    // rechts:
    x = width-grote*2.7;
    cairo_set_source_rgb(cr, 0.85-rgbR, 0.85, 0.00);
    cairo_arc (cr, x, y, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_arc (cr, x+grote*1.2, y, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_arc (cr, x+grote*0.6, y-grote*0.6, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_arc (cr, x+grote*0.6, y+grote*0.6, grote*0.4, 0, 2 * G_PI);
    cairo_fill (cr);
    cairo_set_source_rgb(cr, 0.80, 0.50-rgbG*0.5, rgbB);
    cairo_arc (cr, x+grote*0.6, y, grote*0.3, 0, 2 * G_PI);
    cairo_fill (cr);

    //signatuur:
    cairo_set_source_rgb(cr, 0.00, 0.00, 0.85);
    cairo_set_font_size(cr, grote*0.7);
    cairo_move_to(cr, width-grote*1.7, height-grote*0.3);
    cairo_show_text(cr, "swa");    

    return FALSE;
}

gint main(int argc,char *argv[])
{
    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 8);

    drawing_area = gtk_drawing_area_new();
    grid         = gtk_grid_new ();
    gtk_grid_set_row_spacing       (GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing    (GTK_GRID(grid), 4);
    gtk_container_add              (GTK_CONTAINER(window), grid);

    gtk_widget_set_size_request (drawing_area, 235, 600);

    init_kleuren();
    reset_velden();
    plaats_te_zoeken_code();

    //grid opvullen                                            (x,    y,     h,    v)
    gtk_grid_attach (GTK_GRID(grid), drawing_area,              0,    0,    60,  120);


    g_signal_connect (G_OBJECT (drawing_area), "draw", G_CALLBACK (draw_callback), NULL);
    g_timeout_add(100, (GSourceFunc) get_Tijd, NULL);  // elke 100 usec functie uitvoeren
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_set_events(drawing_area, GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);   // | GDK_KEY_RELEASE_MASK | GDK_KEY_PRESS_MASK
    g_signal_connect(G_OBJECT(drawing_area), "motion-notify-event", G_CALLBACK(mouse_moved), NULL);
    g_signal_connect (drawing_area, "button-press-event", G_CALLBACK (button_press_event), NULL);
    g_signal_connect (drawing_area, "button-release-event", G_CALLBACK (button_release_event), NULL);


    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}

