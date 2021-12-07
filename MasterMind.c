
// compile: gcc -Wall -o MasterMind MasterMind.c $(pkg-config gtk+-3.0 --cflags --libs)

/*
Meestal wordt er gespeeld met 4 plaatsen op iedere rij.
Met pionnen van 6 verschillende kleuren krijgt de speler die de code probeert te kraken 12 kansen.
Er zijn dus 12 rijen met 4 plaatsen, matrix is 4x12 + 1 rij voor computerzet.
*/

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


GtkWidget *window;
GtkWidget *draw_matrix;
GtkWidget *draw_matrix_2;
GtkWidget *draw_matrix_3;
GtkWidget *grid;

GtkWidget *ButtonNieuwSpel;
GtkWidget *ButtonKleur[7];    // kleurkeuze voor pionnen op een rij
GtkWidget *ButtonRij[5];      // op welke plaats van de rij moet de kleur komen
GtkWidget *ButtonOK;          // bevestig de gekozen kleuren op de matrix

// deze Factor is +/- vastgelegd doormiddel van 'draw_matrix.width',
// en dient om een goede vulling te krijgen van deze matrix
int Factor = 42;        // minimum 36;


gboolean BordVol = FALSE;
gboolean CombinatieJuist = FALSE;

char *tekst1 = "";
char *tekst2 = "";
char *tekst3 = "";
char *tekst4 = "";

GDateTime *date_time;
gchar *strTime;
gchar *strKlokDatum;
int kleur = 1;
int rijteller = 1;
int veld[14][9];    // 13 rijen en 8 kollommen nodig (vier kollommen voor pionen + vier voor aanduiding juiste kleuren 0f plaatsen, telkens + 1, omdat base[] = 0)


// declaretions:
gchar *str(int x);
int machtxy(int x, int y);
void computer_plaats_pionen();
void nieuw_spel();
void enable_buttonOK();
void disable_buttonOK();
void enable_ABCD_buttons();
void disable_ABCD_buttons();
void enable_kleur_buttons();
void disable_kleur_buttons();
void buttonRij1();
void buttonRij2();
void buttonRij3();
void buttonRij4();
void buttonKleur1();
void buttonKleur2();
void buttonKleur3();
void buttonKleur4();
void buttonKleur5();
void buttonKleur6();
gboolean checkrij(int R);
gboolean checkcombinatie(int R);
gboolean get_Tijd();
gboolean draw_matrix_callback (GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean draw_matrix_2_callback (GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean draw_matrix_3_callback (GtkWidget *widget, cairo_t *cr, gpointer data);


gchar *str(int x)
{
    gchar *y = g_strdup_printf("%i", x);        //  integer --> string
    return y;
}

void buttonKleur1()  // wit
{
    kleur = 1;
}

void buttonKleur2()  // zwart
{
    kleur = 2;
}

void buttonKleur3()  // geel
{
    kleur = 3;
}

void buttonKleur4()  // groen
{
    kleur = 4;
}

void buttonKleur5()  // rood
{
    kleur = 5;
}

void buttonKleur6()  // blauw
{
    kleur = 6;
}

void buttonRij1()
{
    veld[rijteller][1] = kleur;
    if (checkrij(rijteller))
        enable_buttonOK();
}

void buttonRij2()
{
    veld[rijteller][2] = kleur;
    if (checkrij(rijteller))
        enable_buttonOK();
}

void buttonRij3()
{
    veld[rijteller][3] = kleur;
    if (checkrij(rijteller))
        enable_buttonOK();
}

void buttonRij4()
{
    veld[rijteller][4] = kleur;
    if (checkrij(rijteller))
        enable_buttonOK();
}

void buttonOK()
{
    disable_buttonOK();
    CombinatieJuist = checkcombinatie(rijteller);

    if (CombinatieJuist)
    {
        tekst1 = "MasterMind";
        tekst2 = "";
        tekst3 = "! WINNER !";
        tekst4 = "";
    }

    rijteller++;
    if (rijteller > 12)
    {
        BordVol = TRUE;
        tekst1 = "MasterMind";
        tekst2 = "nog eens";
        tekst3 = "proberen";
        tekst4 = "????";
    }
    if ((BordVol) | (CombinatieJuist))
    {
        disable_ABCD_buttons();
        disable_kleur_buttons();
    }
}

void nieuw_spel()
{
    for (int i=1; i<13; i++)
    {
        for (int j=1; j<9; j++)
        {
            veld[i][j] = 0;
        }
    }
    rijteller = 1;
    buttonKleur1();
    computer_plaats_pionen();
    BordVol = FALSE;
    CombinatieJuist = FALSE;
    disable_buttonOK();
    enable_ABCD_buttons();
    enable_kleur_buttons();
    tekst1 = "MasterMind";
    tekst2 = "Kies een kleur";
    tekst3 = "en plaats ze met";
    tekst4 = "knop A, B, C of D";
}

int machtxy(int x, int y)  // machtsverheffing van integers,  x tot de macht y
{
    int m = x;
    for (int i=0; i<y-1; i++)
    {
        m = m * x;
    }
    return m;
}

void computer_plaats_pionen()
{
    int randnr, randnr1, randnr2, randnr3, randnr4;
    srand(time(0));
    randnr = rand();
    char *strrandnr = str(randnr);
    //g_print("randnr = %s\n", strrandnr); // TEST
    int strl = strlen(strrandnr);
    randnr1 = randnr / machtxy(10, strl-1);
    randnr2 = randnr / machtxy(10, strl-2) - randnr1 * 10;
    randnr3 = randnr / machtxy(10, strl-3) - randnr1 * 100 - randnr2 * 10;
    randnr4 = randnr / machtxy(10, strl-4) - randnr1 * 1000 - randnr2 * 100 - randnr3 * 10;
    if (randnr1 > 6){ randnr1 = randnr1 - 6; }
    if (randnr2 > 6){ randnr2 = randnr2 - 6; }
    if (randnr3 > 6){ randnr3 = randnr3 - 6; }
    if (randnr4 > 6){ randnr4 = randnr4 - 6; }
    if (randnr1 < 1){ randnr1 = 1; }
    if (randnr2 < 1){ randnr2 = 1; }
    if (randnr3 < 1){ randnr3 = 1; }
    if (randnr4 < 1){ randnr4 = 1; }
    veld[13][1] = randnr1;
    veld[13][2] = randnr2;
    veld[13][3] = randnr3;
    veld[13][4] = randnr4;
    g_print("A = %i\n", randnr1);  // TEST
    g_print("B = %i\n", randnr2);  // TEST
    g_print("C = %i\n", randnr3);  // TEST
    g_print("D = %i\n", randnr4);  // TEST
}

void enable_buttonOK()
{
    gtk_widget_set_sensitive( ButtonOK, TRUE );
}

void disable_buttonOK()
{
    gtk_widget_set_sensitive( ButtonOK, FALSE );
}

void enable_ABCD_buttons()
{
    for (int i=1; i<5; i++)
    {
        gtk_widget_set_sensitive( ButtonRij[i], TRUE );
    }
}

void disable_ABCD_buttons()
{
    for (int i=1; i<5; i++)
    {
        gtk_widget_set_sensitive( ButtonRij[i], FALSE );
    }
}

void enable_kleur_buttons()
{
    for (int i=1; i<7; i++)
    {
        gtk_widget_set_sensitive( ButtonKleur[i], TRUE );
    }
}

void disable_kleur_buttons()
{
    for (int i=1; i<7; i++)
    {
        gtk_widget_set_sensitive( ButtonKleur[i], FALSE );
    }
}

gboolean checkrij(int R)     // R is rij 1 tot 12
{
    gboolean butOK_Enable = TRUE; 
    for (int i=1; i<5; i++)
    {
        if (veld[R][i] == 0)
            butOK_Enable = FALSE;
    }
    return butOK_Enable;
}

gboolean checkcombinatie(int R)     // R is rij 1 tot 12
{
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
    gboolean combinatie = FALSE;
    A = veld[R][1];
    B = veld[R][2];
    C = veld[R][3];
    D = veld[R][4];
    Ac = veld[13][1];
    Bc = veld[13][2];
    Cc = veld[13][3];
    Dc = veld[13][4];
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
            A = veld[R][i];
            B = veld[13][x];
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
        veld[R][i] = 1;  // WIT
    }
    for (int i= 5; i<5+juisteplaats; i++)
    {
        veld[R][i] = 2;  // ZWART
    }
    return combinatie;
}

gboolean get_Tijd()
{
    date_time = g_date_time_new_now_local();                            // get local time
    strKlokDatum = g_date_time_format(date_time, "%d:%m:%y");           // datum
    strTime = g_date_time_format(date_time, "%A %e %B %Y - %H:%M:%S");  // 24hr time format
    gtk_window_set_title (GTK_WINDOW (window), strTime);
    gtk_widget_queue_draw(draw_matrix);
    gtk_widget_queue_draw(draw_matrix_2);
    gtk_widget_queue_draw(draw_matrix_3);
    return TRUE;
}

gboolean draw_matrix_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    gdouble width, height;
    gdouble x, y;
    gdouble A, B;
    int R, K;
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);

    //g_print("width = %f\n", width);  // TEST om Factor vast te stellen, Factor = width / 4

    // kader inkleuren
    cairo_set_source_rgb(cr, 0.7, 0.45, 0.0);
    cairo_rectangle (cr, 0, 0, Factor*6, Factor*13);
    cairo_fill (cr);

    // computer rij inkleuren
    cairo_set_source_rgb(cr, 0.8, 0.45, 0.0);
    cairo_rectangle (cr, 0, 0, Factor*6, Factor*0.9);
    cairo_fill (cr);

    // kader tekenen:
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 0,0);
    cairo_line_to(cr, Factor*6, 0);
    cairo_line_to(cr, Factor*6, Factor*13);
    cairo_line_to(cr, 0, Factor*13);
    cairo_line_to(cr, 0, 0);
    cairo_stroke(cr);

    // toon pionnen van speler:
    A = width / 6;
    B = (height-8) / 13;
    R = 13;
    K = 0;
    for (y = B*1.5; y < B * 13; y = y + B)
    {
        R--;
        for (x = A / 2; x < A * 8; x = x + A)
        {
            K++;
            if (K > 8)
                K = 1;
            if (veld[R][K] == 1)       // kleur is WIT
            {
                cairo_set_source_rgb(cr, 0.99, 0.99, 0.99);
            }
            else if (veld[R][K] == 2)  // kleur is ZWART
            {
                cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);
            }
            else if (veld[R][K] == 3)  // kleur is GEEL
            {
                cairo_set_source_rgb(cr, 0.85, 0.85, 0.00);
            }
            else if (veld[R][K] == 4)  // kleur is GROEN
            {
                cairo_set_source_rgb(cr, 0.00, 0.80, 0.00);
            }
            else if (veld[R][K] == 5)  // kleur is ROOD
            {
                cairo_set_source_rgb(cr, 0.80, 0.00, 0.00);
            }
            else if (veld[R][K] == 6)  // kleur is BLAUW
            {
                cairo_set_source_rgb(cr, 0.00, 0.00, 0.80);
            }
            else                       // kleur is GEEN 
            {
                //cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
                cairo_set_source_rgb(cr, 0.6, 0.4, 0.0);
            }

            if (K < 5)
            {
                cairo_arc (cr, x, y, MIN (width, height) / ((width+height)/(Factor*1.2)), 0, 2 * G_PI);
                cairo_fill (cr);
            }

            gdouble Fx;
            if (K == 5){ Fx = x+Factor*0.3; }

            if (K == 5)
            {
                cairo_arc (cr, Fx, (y-Factor*0.15), MIN (width, height) / ((width+height)/(Factor*0.5)), 0, 2 * G_PI);
                cairo_fill (cr);
            }

            if (K == 6)
            {
                cairo_arc (cr, Fx+Factor*0.4, (y-Factor*0.15), MIN (width, height) / ((width+height)/(Factor*0.5)), 0, 2 * G_PI);
                cairo_fill (cr);
            }

            if (K == 7)
            {
                cairo_arc (cr, Fx, (y+Factor*0.25), MIN (width, height) / ((width+height)/(Factor*0.5)), 0, 2 * G_PI);
                cairo_fill (cr);
            }

            if (K == 8)
            {
                cairo_arc (cr, Fx+Factor*0.4, (y+Factor*0.25), MIN (width, height) / ((width+height)/(Factor*0.5)), 0, 2 * G_PI);
                cairo_fill (cr);
            }

        }
    }
    if ((BordVol) | (CombinatieJuist))
    {
        // toon pionnen van computer
        K = 0;
        R = 13;
        y = B*0.5;
        for (x = A / 2; x < A * 4; x = x + A)
        {
            K++;
            if (veld[R][K] == 1)       // kleur is WIT
            {
                cairo_set_source_rgb(cr, 0.99, 0.99, 0.99);
            }
            else if (veld[R][K] == 2)  // kleur is ZWART
            {
                cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);
            }
            else if (veld[R][K] == 3)  // kleur is GEEL
            {
                cairo_set_source_rgb(cr, 0.85, 0.85, 0.00);
            }
            else if (veld[R][K] == 4)  // kleur is GROEN
            {
                cairo_set_source_rgb(cr, 0.00, 0.80, 0.00);
            }
            else if (veld[R][K] == 5)  // kleur is ROOD
            {
                cairo_set_source_rgb(cr, 0.80, 0.00, 0.00);
            }
            else if (veld[R][K] == 6)  // kleur is BLAUW
            {
                cairo_set_source_rgb(cr, 0.00, 0.00, 0.80);
            }
            else                       // kleur is GEEN 
            {
                cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
            }
            cairo_arc (cr, x, y, MIN (width, height) / ((width+height)/(Factor*1.2)), 0, 2 * G_PI);
            cairo_fill (cr);
        }
        cairo_set_source_rgb(cr, 0.5, 0.25, 0.00);
        cairo_set_font_size(cr, Factor/2);
        cairo_move_to(cr, A*4.6, B*0.7);
        if (BordVol)
        {
            cairo_show_text(cr, "oOo");
        }
        else
        {
            cairo_show_text(cr, "OK!");
        }
    }
    else
    {
        // Dek pionnen van computer af
        cairo_set_source_rgb(cr, 0.5, 0.25, 0.00);
        cairo_set_font_size(cr, Factor/2);
        cairo_move_to(cr, A*0.25, B*0.7);
        cairo_show_text(cr, "A");
        cairo_move_to(cr, A*1.25, B*0.7);
        cairo_show_text(cr, "B");
        cairo_move_to(cr, A*2.25, B*0.7);
        cairo_show_text(cr, "C");
        cairo_move_to(cr, A*3.25, B*0.7);
        cairo_show_text(cr, "D");
        cairo_set_font_size(cr, Factor/3);
        cairo_move_to(cr, A*4.5, B*0.4);
        cairo_show_text(cr, "Master");
        cairo_move_to(cr, A*4.5, B*0.8);
        cairo_show_text(cr, " Mind ");
    }
   
    return FALSE;
}

gboolean draw_matrix_2_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    gdouble width, height;
    gdouble x, y;
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);

    /*
    // kader tekenen:
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 0,0);
    cairo_line_to(cr, width, 0);
    cairo_line_to(cr, width, height);
    cairo_line_to(cr, 0, height);
    cairo_line_to(cr, 0, 0);
    cairo_stroke(cr);
    */

    // bolletje voor gekozen kleur tonen:
    x = width / 2;
    y = height / 2;

    cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);
    cairo_arc (cr, x, y, MIN (width, height)/2.4, 0, 2 * G_PI);
    cairo_fill (cr);

    if (kleur == 1)       // kleur is WIT
    {
        cairo_set_source_rgb(cr, 0.99, 0.99, 0.99);
    }
    else if (kleur == 2)  // kleur is ZWART
    {
        cairo_set_source_rgb(cr, 0.00, 0.00, 0.00);
    }
    else if (kleur == 3)  // kleur is GEEL
    {
        cairo_set_source_rgb(cr, 0.85, 0.85, 0.00);
    }
    else if (kleur == 4)  // kleur is GROEN
    {
        cairo_set_source_rgb(cr, 0.00, 0.80, 0.00);
    }
    else if (kleur == 5)  // kleur is ROOD
    {
        cairo_set_source_rgb(cr, 0.80, 0.00, 0.00);
    }
    else if (kleur == 6)  // kleur is BLAUW
    {
        cairo_set_source_rgb(cr, 0.00, 0.00, 0.80);
    }
    else                       // kleur is GEEN 
    {
        cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
    }
    cairo_arc (cr, x, y, MIN (width, height)/2.5, 0, 2 * G_PI);
    cairo_fill (cr);

    return FALSE;
}

gboolean draw_matrix_3_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    gdouble width, height;
    gdouble y;
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);

    /*
    // kader tekenen:
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 0,0);
    cairo_line_to(cr, width, 0);
    cairo_line_to(cr, width, height);
    cairo_line_to(cr, 0, height);
    cairo_line_to(cr, 0, 0);
    cairo_stroke(cr);
    */

    // plaats teksten:

    y = height / 6;

    cairo_set_source_rgb(cr, 0.00, 0.00, 0.80);
    cairo_set_font_size(cr, 18);

    cairo_text_extents_t extents;                      // tekst in center plaatsen met 'cairo_text_extents_t'

    cairo_text_extents(cr, tekst1, &extents);
    cairo_move_to(cr, width/2 - extents.width/2, y);
    cairo_show_text(cr, tekst1);

    cairo_text_extents(cr, tekst2, &extents);
    cairo_move_to(cr, width/2 - extents.width/2, y*2.5);
    cairo_show_text(cr, tekst2);

    if (CombinatieJuist)
    {
        cairo_set_font_size(cr, 24);

        cairo_text_extents(cr, tekst3, &extents);
        cairo_move_to(cr, width/2 - extents.width/2, y*3.5);
        cairo_show_text(cr, tekst3);
    }
    else
    {
        cairo_set_font_size(cr, 18);

        cairo_text_extents(cr, tekst3, &extents);
        cairo_move_to(cr, width/2 - extents.width/2, y*3.5);
        cairo_show_text(cr, tekst3);

        cairo_text_extents(cr, tekst4, &extents);
        cairo_move_to(cr, width/2 - extents.width/2, y*4.5);
        cairo_show_text(cr, tekst4);
    }

    return FALSE;
}

int main(int argc, char **argv)
{

    gtk_init (&argc,&argv);

    //Declarations
    window       = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    draw_matrix  = gtk_drawing_area_new();
    gtk_widget_set_size_request (draw_matrix, Factor*6, Factor*13);
    draw_matrix_2 = gtk_drawing_area_new();
    draw_matrix_3 = gtk_drawing_area_new();
    grid         = gtk_grid_new ();

    tekst1 = "MasterMind";
    tekst2 = "Druk op knop";
    tekst3 = "Nieuw Spel";
    tekst4 = "";

    ButtonNieuwSpel  = gtk_button_new_with_label ("Nieuw Spel");
    // Kleuren: 0 = leeg, 1 = wit, 2 = zwart, 3 = geel, 4 = groen, 5 = rood, 6 = blauw
    ButtonKleur[1]   = gtk_button_new_with_label ("Wit");
    ButtonKleur[2]   = gtk_button_new_with_label ("Zwart");
    ButtonKleur[3]   = gtk_button_new_with_label ("Geel");
    ButtonKleur[4]   = gtk_button_new_with_label ("Groen");
    ButtonKleur[5]   = gtk_button_new_with_label ("Rood");
    ButtonKleur[6]   = gtk_button_new_with_label ("Blauw");

    ButtonRij[1]    = gtk_button_new_with_label ("A");
    ButtonRij[2]    = gtk_button_new_with_label ("B");
    ButtonRij[3]    = gtk_button_new_with_label ("C");
    ButtonRij[4]    = gtk_button_new_with_label ("D");

    ButtonOK        = gtk_button_new_with_label ("OK");

    disable_buttonOK();
    disable_ABCD_buttons();
    disable_kleur_buttons();

    //Properties
    gtk_container_set_border_width (GTK_CONTAINER(window), 16);
    gtk_widget_set_size_request    (GTK_WIDGET(window), 80, 120);
    gtk_grid_set_row_spacing       (GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing    (GTK_GRID(grid), 4);
    gtk_container_add              (GTK_CONTAINER(window), grid);

    for (int i=0; i<5; i++)
    {
        for (int j=0; j<14; j++)
        {
            veld[i][j] = 0;   // 0 is kleur "geen"
        }
    }

    if (Factor < 36){ Factor = 36; }
    //grid opvullen                                            (x,    y,            h,        v)

    gtk_grid_attach (GTK_GRID(grid), draw_matrix,               0,    0,       Factor, Factor*3);
    gtk_grid_attach (GTK_GRID(grid), ButtonNieuwSpel,      Factor,    0,    80-Factor,        8);
    gtk_grid_attach (GTK_GRID(grid), draw_matrix_3,        Factor,    8,    80-Factor,       40);

    for (int i=0; i<6; i++)
    {
        gtk_grid_attach (GTK_GRID(grid), ButtonKleur[i+1], Factor, Factor*3-24-6*8+i*8,  80-Factor,   8);
    }

    gtk_grid_attach (GTK_GRID(grid), draw_matrix_2,        Factor, Factor*3-24,  80-Factor,       8);

    for (int i=0; i<4; i++)
    {
        gtk_grid_attach (GTK_GRID(grid), ButtonRij[i+1],   Factor+i*(80-Factor)/4, Factor*3-16,  (80-Factor)/4,   8);

    }

    gtk_grid_attach (GTK_GRID(grid), ButtonOK,             Factor, Factor*3-8, 80-Factor,     8);


    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_timeout_add(100, (GSourceFunc) get_Tijd, NULL);  // elke 100 msec functie uitvoeren
    g_signal_connect (G_OBJECT (draw_matrix), "draw", G_CALLBACK (draw_matrix_callback), NULL);
    g_signal_connect (G_OBJECT (draw_matrix_2), "draw", G_CALLBACK (draw_matrix_2_callback), NULL);
    g_signal_connect (G_OBJECT (draw_matrix_3), "draw", G_CALLBACK (draw_matrix_3_callback), NULL);

    g_signal_connect(G_OBJECT(ButtonNieuwSpel),  "clicked", G_CALLBACK(nieuw_spel), NULL);
    g_signal_connect(G_OBJECT(ButtonKleur[1]), "clicked", G_CALLBACK(buttonKleur1), NULL);
    g_signal_connect(G_OBJECT(ButtonKleur[2]), "clicked", G_CALLBACK(buttonKleur2), NULL);
    g_signal_connect(G_OBJECT(ButtonKleur[3]), "clicked", G_CALLBACK(buttonKleur3), NULL);
    g_signal_connect(G_OBJECT(ButtonKleur[4]), "clicked", G_CALLBACK(buttonKleur4), NULL);
    g_signal_connect(G_OBJECT(ButtonKleur[5]), "clicked", G_CALLBACK(buttonKleur5), NULL);
    g_signal_connect(G_OBJECT(ButtonKleur[6]), "clicked", G_CALLBACK(buttonKleur6), NULL);
    g_signal_connect(G_OBJECT(ButtonRij[1]), "clicked", G_CALLBACK(buttonRij1), NULL);
    g_signal_connect(G_OBJECT(ButtonRij[2]), "clicked", G_CALLBACK(buttonRij2), NULL);
    g_signal_connect(G_OBJECT(ButtonRij[3]), "clicked", G_CALLBACK(buttonRij3), NULL);
    g_signal_connect(G_OBJECT(ButtonRij[4]), "clicked", G_CALLBACK(buttonRij4), NULL);

    g_signal_connect(G_OBJECT(ButtonOK), "clicked", G_CALLBACK(buttonOK), NULL);

    gtk_widget_show_all (window);

    gtk_main();

    return 0;
}

