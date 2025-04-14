#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static WebKitWebView *web_view;
static GtkWidget    *url_bar;

// Check for http:// or https:// prefix
gboolean is_valid_url(const char *input) {
    return g_str_has_prefix(input, "http://") || g_str_has_prefix(input, "https://");
}

// Build initial URI from argv[]
char *build_initial_uri(int argc, char *argv[]) {
    if (argc == 1) {
        // no args: default to google.com
        return g_strdup("https://www.google.com");
    }

    // join argv[1..] with spaces
    GString *joined = g_string_new(NULL);
    for (int i = 1; i < argc; i++) {
        if (i > 1) g_string_append_c(joined, ' ');
        g_string_append(joined, argv[i]);
    }

    const char *input = joined->str;
    char *uri;
    if (is_valid_url(input)) {
        uri = g_strdup(input);
    } else {
        // URL-encode spaces to '+'
        char *escaped = g_uri_escape_string(input, " ", FALSE);
        uri = g_strdup_printf("https://www.google.com/search?q=%s", escaped);
        g_free(escaped);
    }
    g_string_free(joined, TRUE);
    return uri;
}

// When you press Enter in URL bar
void on_url_activate(GtkEntry *entry, gpointer user_data) {
    const char *input = gtk_entry_get_text(entry);
    char *uri;
    if (is_valid_url(input)) {
        uri = g_strdup(input);
    } else {
        char *escaped = g_uri_escape_string(input, " ", FALSE);
        uri = g_strdup_printf("https://www.google.com/search?q=%s", escaped);
        g_free(escaped);
    }
    webkit_web_view_load_uri(web_view, uri);
    g_free(uri);
}

// Sync URL bar when page loads
void on_load_changed(WebKitWebView *view, WebKitLoadEvent event, gpointer data) {
    if (event == WEBKIT_LOAD_COMMITTED) {
        const char *uri = webkit_web_view_get_uri(view);
        gtk_entry_set_text(GTK_ENTRY(url_bar), uri);
    }
}

// Keyboard shortcuts
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_l) {
        gtk_widget_grab_focus(url_bar);
        return TRUE;
    }
    if ((event->state & GDK_MOD1_MASK) && event->keyval == GDK_KEY_Left) {
        if (webkit_web_view_can_go_back(web_view))
            webkit_web_view_go_back(web_view);
        return TRUE;
    }
    if ((event->state & GDK_MOD1_MASK) && event->keyval == GDK_KEY_Right) {
        if (webkit_web_view_can_go_forward(web_view))
            webkit_web_view_go_forward(web_view);
        return TRUE;
    }
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_r) {
        webkit_web_view_reload(web_view);
        return TRUE;
    }
    return FALSE;
}

// Toolbar callbacks
void on_back_clicked(GtkButton *btn, gpointer user_data) {
    if (webkit_web_view_can_go_back(web_view))
        webkit_web_view_go_back(web_view);
}
void on_forward_clicked(GtkButton *btn, gpointer user_data) {
    if (webkit_web_view_can_go_forward(web_view))
        webkit_web_view_go_forward(web_view);
}
void on_reload_clicked(GtkButton *btn, gpointer user_data) {
    webkit_web_view_reload(web_view);
}
void on_info_clicked(GtkButton *btn, gpointer user_data) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_CLOSE,
        "Shortcuts:\n"
        "• Ctrl+L → focus URL bar\n"
        "• Ctrl+R → reload page\n"
        "• Alt+← / Alt+→ → back / forward\n\n"
        "Type a term and hit Enter → Google search\n"
        "Type full URL → direct navigation"
    );
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Build the URI from args
    char *initial_uri = build_initial_uri(argc, argv);

    // Window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    // Layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Toolbar
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 4);

    GtkWidget *btn_back    = gtk_button_new_with_label("<<");
    GtkWidget *btn_forward = gtk_button_new_with_label(">>");
    GtkWidget *btn_reload  = gtk_button_new_with_label("⟳");
    GtkWidget *btn_info    = gtk_button_new_with_label("i");
    url_bar                = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(hbox), btn_back,    FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_forward, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_reload,  FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_info,    FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), url_bar,     TRUE,  TRUE,  0);

    g_signal_connect(btn_back,    "clicked", G_CALLBACK(on_back_clicked),    NULL);
    g_signal_connect(btn_forward, "clicked", G_CALLBACK(on_forward_clicked), NULL);
    g_signal_connect(btn_reload,  "clicked", G_CALLBACK(on_reload_clicked),  NULL);
    g_signal_connect(btn_info,    "clicked", G_CALLBACK(on_info_clicked),    window);

    // WebView
    web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(web_view), TRUE, TRUE, 0);

    g_signal_connect(web_view, "load-changed", G_CALLBACK(on_load_changed), NULL);
    g_signal_connect(url_bar,   "activate",     G_CALLBACK(on_url_activate), NULL);

    // Load initial URI
    webkit_web_view_load_uri(web_view, initial_uri);
    g_free(initial_uri);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
