#include <gtk/gtk.h>
#include "main.h"
#include "../../common/c/core.c"
#include "../../common/c/gtk.c"

int main(int argc, char **argv){
    GtkApplication *app;

    app = gtk_application_new(
      "com.iterami.browsergtk",
      0
    );
    g_signal_connect(
      app,
      "activate",
      G_CALLBACK(gtk_activate),
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

    if(page < 1){
        return;
    }

    gtk_notebook_remove_page(
      notebook,
      page
    );
}

void menu_movetab(const gint movement){
    int page = gtk_notebook_get_current_page(notebook);

    if(page < 1){
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

void menu_newtab(const gchar *title, gint type){
    int page = gtk_notebook_get_n_pages(notebook);
    GtkWidget *tab_label_container;
    GtkWidget *tab_label_title;
    GtkWidget *tab_label_type;
    GtkWidget *tab_menu_title;

    tab_label_container = gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      5
    );
    tab_label_title = gtk_label_new(title);
    tab_label_type = gtk_list_box_new();
    tab_menu_title = gtk_label_new(title);

    gtk_box_pack_end(
      GTK_BOX(tab_label_container),
      tab_label_title,
      TRUE,
      TRUE,
      0
    );
    gtk_widget_show(tab_label_title);

    // Files tab.
    if(type == 0){
        tab_label_type = gtk_label_new("FILES");

        gtk_box_pack_start(
          GTK_BOX(tab_label_container),
          tab_label_type,
          TRUE,
          TRUE,
          0
        );

        gtk_notebook_append_page_menu(
          notebook,
          tab_new_files(),
          tab_label_container,
          tab_menu_title
        );

    // Text tab.
    }else if(type == 1){
        tab_label_type = gtk_label_new("TEXT");

        gtk_box_pack_start(
          GTK_BOX(tab_label_container),
          tab_label_type,
          TRUE,
          TRUE,
          0
        );

        gtk_notebook_append_page_menu(
          notebook,
          tab_new_text(),
          tab_label_container,
          tab_menu_title
        );

    // Web tab.
    }else if(type == 2){
        tab_label_type = gtk_label_new("WEB");

        gtk_box_pack_start(
          GTK_BOX(tab_label_container),
          tab_label_type,
          TRUE,
          TRUE,
          0
        );

        gtk_notebook_append_page_menu(
          notebook,
          tab_new_web(),
          tab_label_container,
          tab_menu_title
        );

    // Default tab.
    }else{
        tab_label_type = gtk_label_new("DEFAULT");

        gtk_box_pack_start(
          GTK_BOX(tab_label_container),
          tab_label_type,
          TRUE,
          TRUE,
          0
        );

        gtk_notebook_append_page_menu(
          notebook,
          tab_new_default(),
          tab_label_container,
          tab_menu_title
        );
    }

    gtk_widget_show_all(window);
    gtk_notebook_set_current_page(
      notebook,
      page
    );
    gtk_widget_grab_focus(entry_toolbar_path);
}

void menu_openfile(void){
    GtkFileChooser *chooser;
    GtkWidget *dialog_open;

    dialog_open = gtk_file_chooser_dialog_new(
      "Open File...",
      GTK_WINDOW(window),
      GTK_FILE_CHOOSER_ACTION_OPEN,
      "_Cancel",
      GTK_RESPONSE_CANCEL,
      "_Open",
      GTK_RESPONSE_ACCEPT,
      NULL
    );
    chooser = GTK_FILE_CHOOSER(dialog_open);
    gtk_file_chooser_set_show_hidden(
      chooser,
      TRUE
    );
    gtk_file_chooser_set_file(
      chooser,
      g_file_new_for_path(gtk_entry_get_text(GTK_ENTRY(entry_toolbar_path))),
      NULL
    );

    if(gtk_dialog_run(GTK_DIALOG(dialog_open)) == GTK_RESPONSE_ACCEPT){
        gchar *content;
        char *filename;
        gssize length;

        filename = gtk_file_chooser_get_filename(chooser);
        menu_newtab(
          "NEW TAB",
          1
        );

        if(g_file_get_contents(
            filename,
            &content,
            &length,
            NULL
          ) && g_utf8_validate(
            content,
            length,
            NULL
          )){
            GtkTextBuffer *textbuffer;
            int page = gtk_notebook_get_current_page(notebook);

            textbuffer = tab_get_text_buffer(page);

            tab_update_labels(
              -1,
              g_path_get_basename(filename),
              filename
            );
            gtk_entry_set_text(
              GTK_ENTRY(entry_toolbar_path),
              filename
            );
            gtk_window_set_title(
              GTK_WINDOW(window),
              g_path_get_basename(filename)
            );

            gtk_text_buffer_set_text(
              textbuffer,
              content,
              length
            );
        }

        g_free(content);
        g_free(filename);
    }

    gtk_widget_destroy(dialog_open);
}

void menu_save(void){
    int type = tab_get_type(-1);

    if(type != 1){
        return;
    }

    gchar *content;
    GtkTextIter end;
    GtkTextIter start;
    GtkTextBuffer *textbuffer;

    textbuffer = tab_get_text_buffer(-1);

    gtk_text_buffer_get_start_iter(
      textbuffer,
      &start
    );
    gtk_text_buffer_get_end_iter(
      textbuffer,
      &end
    );

    content = gtk_text_buffer_get_text(
      textbuffer,
      &start,
      &end,
      FALSE
    );
    g_file_set_contents(
      tab_get_menu_label_text(-1),
      content,
      -1,
      NULL
    );
    g_free(content);
}

void menu_saveas(void){
    int type = tab_get_type(-1);
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

GtkWidget* new_text_view(void){
    GtkWidget *text_view;

    text_view = gtk_text_view_new_with_buffer(gtk_text_buffer_new(NULL));

    gtk_text_view_set_monospace(
      GTK_TEXT_VIEW(text_view),
      TRUE
    );
    gtk_text_view_set_wrap_mode(
      GTK_TEXT_VIEW(text_view),
      GTK_WRAP_WORD
    );

    return text_view;
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
      "_Save",
      accelgroup,
      KEY_SAVE,
      GDK_CONTROL_MASK,
      G_CALLBACK(menu_save),
      NULL
    );
    gtk_add_menuitem(
      menu_menu,
      "Save _As...",
      accelgroup,
      KEY_SAVE,
      GDK_CONTROL_MASK | GDK_SHIFT_MASK,
      G_CALLBACK(menu_saveas),
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
    g_signal_connect_after(
      notebook,
      "switch-page",
      G_CALLBACK(tab_switch),
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
    menu_newtab(
      HOME_TAB_TITLE,
      0
    );

    gtk_widget_show_all(window);
}

GList* tab_get_children(int page){
    if(page < 0){
        page = gtk_notebook_get_current_page(notebook);
    }

    return gtk_container_get_children(
      GTK_CONTAINER(gtk_notebook_get_nth_page(
        notebook,
        page
      ))
    );
}

const gchar* tab_get_menu_label_text(int page){
    if(page < 0){
        page = gtk_notebook_get_current_page(notebook);
    }

    return gtk_notebook_get_menu_label_text(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        page
      )
    );
}

GtkTextBuffer* tab_get_text_buffer(int page){
    if(page < 0){
        page = gtk_notebook_get_current_page(notebook);
    }

    return gtk_text_view_get_buffer(
      GTK_TEXT_VIEW(gtk_bin_get_child(
        GTK_BIN(g_list_nth_data(
          tab_get_children(page),
          0
        ))
      ))
    );
}

int tab_get_type(int page){
    if(page < 0){
        page = gtk_notebook_get_current_page(notebook);
    }

    GtkLabel *tab_label_type;

    GList *tab_label_children = gtk_container_get_children(
      GTK_CONTAINER(gtk_notebook_get_tab_label(
        notebook,
        gtk_notebook_get_nth_page(
          notebook,
          page
        )
      ))
    );

    tab_label_type = g_list_nth_data(
      tab_label_children,
      0
    );

    if(g_strcmp0(
        gtk_label_get_text(tab_label_type),
        "FILES"
      ) == 0){
        return 0;
    }

    if(g_strcmp0(
        gtk_label_get_text(tab_label_type),
        "TEXT"
      ) == 0){
        return 1;
    }

    if(g_strcmp0(
        gtk_label_get_text(tab_label_type),
        "WEB"
      ) == 0){
        return 2;
    }

    return -1;
}

GtkWidget* tab_new_default(void){
    GtkWidget *default_tab_list;
    GtkWidget *default_tab_files;
    GtkWidget *default_tab_text;
    GtkWidget *default_tab_web;

    default_tab_list = gtk_list_box_new();
    default_tab_files = gtk_button_new_with_label("Files Tab");
    default_tab_text = gtk_button_new_with_label("Text Tab");
    default_tab_web = gtk_button_new_with_label("Web Tab");

    g_signal_connect(
      default_tab_files,
      "clicked",
      G_CALLBACK(tab_new_from_default),
      GINT_TO_POINTER(0)
    );
    g_signal_connect(
      default_tab_text,
      "clicked",
      G_CALLBACK(tab_new_from_default),
      GINT_TO_POINTER(1)
    );
    g_signal_connect(
      default_tab_web,
      "clicked",
      G_CALLBACK(tab_new_from_default),
      GINT_TO_POINTER(2)
    );

    gtk_container_add(
      GTK_CONTAINER(default_tab_list),
      default_tab_files
    );
    gtk_container_add(
      GTK_CONTAINER(default_tab_list),
      default_tab_text
    );
    gtk_container_add(
      GTK_CONTAINER(default_tab_list),
      default_tab_web
    );

    return default_tab_list;
}

void tab_new_from_default(GtkWidget *widget, GdkEventButton *event, gpointer data){
    menu_closetab();
    menu_newtab(
      "NEW TAB",
      GPOINTER_TO_INT(data)
    );
}

GtkWidget* tab_new_files(void){
    GtkWidget *box;
    GtkWidget *places;

    box = gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      1
    );

    places = gtk_places_sidebar_new();
    gtk_widget_set_hexpand(
      places,
      FALSE
    );
    gtk_places_sidebar_set_show_desktop(
      GTK_PLACES_SIDEBAR(places),
      TRUE
    );
    gtk_places_sidebar_set_show_recent(
      GTK_PLACES_SIDEBAR(places),
      FALSE
    );
    gtk_places_sidebar_set_show_trash(
      GTK_PLACES_SIDEBAR(places),
      TRUE
    );
    gtk_places_sidebar_set_open_flags(
      GTK_PLACES_SIDEBAR(places),
      GTK_PLACES_OPEN_NORMAL
    );

    gtk_box_pack_start(
      GTK_BOX(box),
      places,
      TRUE,
      TRUE,
      0
    );

    return box;
}

GtkWidget* tab_new_text(void){
    GtkWidget *box;
    GtkWidget *scrolled_window;

    box = gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      1
    );
    scrolled_window = new_scrolled_window();
    gtk_container_add(
      GTK_CONTAINER(scrolled_window),
      new_text_view()
    );
    gtk_box_pack_start(
      GTK_BOX(box),
      scrolled_window,
      TRUE,
      TRUE,
      0
    );

    return box;
}

GtkWidget* tab_new_web(void){
    GtkWidget *box;
    GtkWidget *scrolled_window;

    box = gtk_box_new(
      GTK_ORIENTATION_HORIZONTAL,
      1
    );
    scrolled_window = new_scrolled_window();
    gtk_box_pack_start(
      GTK_BOX(box),
      scrolled_window,
      TRUE,
      TRUE,
      0
    );

    return box;
}

void tab_switch(GtkNotebook *notebook, GtkWidget *page_content, guint page, gpointer data){
    gtk_entry_set_text(
      GTK_ENTRY(entry_toolbar_path),
      gtk_notebook_get_menu_label_text(
        notebook,
        page_content
      )
    );
    gtk_window_set_title(
      GTK_WINDOW(window),
      gtk_notebook_get_tab_label_text(
        notebook,
        page_content
      )
    );
}

void tab_update_labels(int page, gchar* tab_label, gchar* menu_label){
    if(page < 0){
        page = gtk_notebook_get_current_page(notebook);
    }

    GtkLabel *tab_label_title;

    GList *tab_label_children = gtk_container_get_children(
      GTK_CONTAINER(gtk_notebook_get_tab_label(
        notebook,
        gtk_notebook_get_nth_page(
          notebook,
          page
        )
      ))
    );

    tab_label_title = g_list_nth_data(
      tab_label_children,
      1
    );

    gtk_label_set_text(
      tab_label_title,
      tab_label
    );
    gtk_notebook_set_menu_label_text(
      notebook,
      gtk_notebook_get_nth_page(
        notebook,
        page
      ),
      menu_label
    );
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
