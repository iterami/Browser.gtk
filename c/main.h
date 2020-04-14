#pragma once

GtkNotebook *notebook;
GtkWidget *button_toolbar_back;
GtkWidget *button_toolbar_forward;
GtkWidget *button_toolbar_reload;
GtkWidget *button_toolbar_stop;
GtkWidget *entry_toolbar_address;

void activate(GtkApplication* app, gpointer data);
void startup(GtkApplication* app, gpointer data);
void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data);
