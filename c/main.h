#pragma once

#define HOME_TAB_TITLE "⌂"
#define KEY_CLOSETAB GDK_KEY_w
#define KEY_MOVETABLEFT GDK_KEY_underscore
#define KEY_MOVETABRIGHT GDK_KEY_plus
#define KEY_NEWTAB GDK_KEY_t
#define KEY_NEXTTAB GDK_KEY_equal
#define KEY_OPEN GDK_KEY_o
#define KEY_PREVIOUSTAB GDK_KEY_minus

GtkNotebook *notebook;
GtkWidget *button_toolbar_back;
GtkWidget *button_toolbar_forward;
GtkWidget *button_toolbar_reload;
GtkWidget *button_toolbar_stop;
GtkWidget *entry_toolbar_path;

void activate(GtkApplication* app, gpointer data);
void menu_closetab(void);
void menu_movetab(const gint movement);
void menu_newtab(const gchar *title, gint type);
void menu_openfile(void);
GtkWidget* new_scrolled_window(void);
GtkWidget* new_text_view(void);
void startup(GtkApplication* app, gpointer data);
GtkWidget* tab_new_default(void);
GtkWidget* tab_new_files(void);
GtkWidget* tab_new_text(void);
GtkWidget* tab_new_web(void);
void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data);
void toolbar_back(void);
void toolbar_forward(void);
void toolbar_path(void);
void toolbar_reload(void);
void toolbar_stop(void);
