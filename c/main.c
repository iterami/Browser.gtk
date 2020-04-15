#include <gtk/gtk.h>
#include "main.h"
#include "../../common/c/core.c"
#include "../../common/c/gtk.c"

void activate(GtkApplication* app, gpointer data){
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv){
    GtkApplication *app;

    app = gtk_application_new(
      "com.iterami.browsergtk",
      0
    );
    g_signal_connect(
      app,
      "activate",
      G_CALLBACK(activate),
      NULL
    );
    g_signal_connect(
      app,
      "startup",
      G_CALLBACK(startup),
      NULL
    );
    int status = g_application_run(
      G_APPLICATION(app),
      argc,
      argv
    );
    g_object_unref(app);

    return status;
}

void menu_closetab(void){
    int page = gtk_notebook_get_current_page(notebook);

    if(page <= 0){
        return;
    }

    gtk_notebook_remove_page(
      notebook,
      page
    );
}

void menu_movetab(const gint movement){
    int page = gtk_notebook_get_current_page(notebook);

    if(page == 0){
        return;
    }

    int position = page + movement;

    if(position <= 1){
        position = 1;

    }else{
        int pages = gtk_notebook_get_n_pages(notebook);

        if(position >= pages){
            position = pages - 1;
        }
    }

    gtk_notebook_reorder_child(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        gtk_notebook_get_current_page(notebook)
      ),
      position
    );
}

void menu_newtab(const gchar *title){
    // Append and show.
    gtk_notebook_append_page(
      notebook,
      new_scrolled_window(),
      gtk_label_new(title)
    );
    gtk_widget_show_all(window);
    gtk_notebook_set_current_page(
      notebook,
      gtk_notebook_get_n_pages(notebook) - 1
    );
    gtk_widget_grab_focus(entry_toolbar_path);
}

void menu_openfile(void){
}

GtkWidget* new_scrolled_window(void){
    GtkWidget *scrolled_window;

    scrolled_window = gtk_scrolled_window_new(
      NULL,
      NULL
    );
    gtk_scrolled_window_set_policy(
      GTK_SCROLLED_WINDOW(scrolled_window),
      GTK_POLICY_AUTOMATIC,
      GTK_POLICY_AUTOMATIC
    );

    return scrolled_window;
}

void startup(GtkApplication* app, gpointer data){
    GtkAccelGroup *accelgroup;
    GtkWidget *box;
    GtkWidget *menu_menu;
    GtkWidget *menubar;
    GtkWidget *menuitem_menu;
    GtkWidget *toolbar;

    gtk_init_gtk(
      app,
      "Browser.gtk"
    );

    // Setup scrollable notebook.
    notebook = GTK_NOTEBOOK(gtk_notebook_new());
    gtk_notebook_popup_enable(notebook);
    gtk_notebook_set_scrollable(
      notebook,
      TRUE
    );
    gtk_notebook_set_show_border(
      notebook,
      FALSE
    );
    g_signal_connect_after(
      notebook,
      "switch-page",
      G_CALLBACK(tab_switch),
      NULL
    );

    // Setup menu items.
    menubar = gtk_menu_bar_new();
    accelgroup = gtk_accel_group_new();
    gtk_window_add_accel_group(
      GTK_WINDOW(window),
      accelgroup
    );
    // Menu menu.
    menu_menu = gtk_menu_new();
    menuitem_menu = gtk_menu_item_new_with_mnemonic("☰");
    gtk_menu_item_set_submenu(
      GTK_MENU_ITEM(menuitem_menu),
      menu_menu
    );
    gtk_add_menuitem(
      menu_menu,
      "_New Tab",
      accelgroup,
      KEY_NEWTAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_newtab),
      "NEW TAB"
    );
    gtk_add_menuitem(
      menu_menu,
      "_Open File...",
      accelgroup,
      KEY_OPEN,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_openfile),
      NULL
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menu_menu),
      gtk_separator_menu_item_new()
    );
    gtk_add_menuitem(
      menu_menu,
      "_Next Tab",
      accelgroup,
      KEY_NEXTTAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(gtk_notebook_next_page),
      GTK_WIDGET(notebook)
    );
    gtk_add_menuitem(
      menu_menu,
      "_Previous Tab",
      accelgroup,
      KEY_PREVIOUSTAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(gtk_notebook_prev_page),
      GTK_WIDGET(notebook)
    );
    gtk_add_menuitem(
      menu_menu,
      "Move Tab _Left",
      accelgroup,
      KEY_MOVETABLEFT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK,
      G_CALLBACK(menu_movetab),
      (gpointer)-1
    );
    gtk_add_menuitem(
      menu_menu,
      "Move Tab _Right",
      accelgroup,
      KEY_MOVETABRIGHT,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK,
      G_CALLBACK(menu_movetab),
      (gpointer)1
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menu_menu),
      gtk_separator_menu_item_new()
    );
    gtk_add_menuitem(
      menu_menu,
      "_Close Tab",
      accelgroup,
      KEY_CLOSETAB,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_closetab),
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "_Quit",
      accelgroup,
      KEY_QUIT,
      GDK_CONTROL_MASK,
      G_CALLBACK(gtk_widget_destroy),
      window
    );
    gtk_menu_shell_append(
      GTK_MENU_SHELL(menubar),
      menuitem_menu
    );

    // Setup toolbar.
    toolbar = gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      GTK_WIDGET(menubar),
      FALSE,
      FALSE,
      0
    );
    button_toolbar_back = gtk_button_new_with_label("←");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_back,
      FALSE,
      FALSE,
      0
    );
    button_toolbar_forward = gtk_button_new_with_label("→");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_forward,
      FALSE,
      FALSE,
      0
    );
    button_toolbar_reload = gtk_button_new_with_label("⟳");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_reload,
      FALSE,
      FALSE,
      0
    );
    button_toolbar_stop = gtk_button_new_with_label("X");
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      button_toolbar_stop,
      FALSE,
      FALSE,
      0
    );
    entry_toolbar_path = gtk_entry_new();
    gtk_box_pack_start(
      GTK_BOX(toolbar),
      entry_toolbar_path,
      TRUE,
      TRUE,
      0
    );

    // Connect everything.
    g_signal_connect(
      entry_toolbar_path,
      "activate",
      G_CALLBACK(toolbar_path),
      NULL
    );
    g_signal_connect(
      button_toolbar_back,
      "clicked",
      G_CALLBACK(toolbar_back),
      NULL
    );
    g_signal_connect(
      button_toolbar_forward,
      "clicked",
      G_CALLBACK(toolbar_forward),
      NULL
    );
    g_signal_connect(
      button_toolbar_reload,
      "clicked",
      G_CALLBACK(toolbar_reload),
      NULL
    );
    g_signal_connect(
      button_toolbar_stop,
      "clicked",
      G_CALLBACK(toolbar_stop),
      NULL
    );

    // Add everything to a box.
    box = gtk_box_new(
      GTK_ORIENTATION_VERTICAL,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(box),
      toolbar,
      FALSE,
      FALSE,
      0
    );
    gtk_box_pack_start(
      GTK_BOX(box),
      GTK_WIDGET(notebook),
      TRUE,
      TRUE,
      0
    );
    gtk_container_add(
      GTK_CONTAINER(window),
      box
    );

    // Setup home tab.
    menu_newtab(HOME_TAB_TITLE);

    gtk_widget_show_all(window);
}

void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data){
}

void toolbar_back(void){
}

void toolbar_forward(void){
}

void toolbar_path(void){
}

void toolbar_reload(void){
}

void toolbar_stop(void){
}
