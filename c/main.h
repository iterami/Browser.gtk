#pragma once

GtkNotebook *notebook;

void activate(GtkApplication* app, gpointer data);
void startup(GtkApplication* app, gpointer data);
void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data);
