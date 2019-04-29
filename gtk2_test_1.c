//~ Compilation command:
//~ gcc $(pkg-config --cflags --libs gtk+-2.0) gtk2_test_1.c -o gtk2_test_1

#include <gtk/gtk.h>

#define WIN_W 512
#define WIN_H 512
#define BUF_SIZE 32
#define LINE_SPACING 16
#define HANDLE_SPACING 4

//~ static gboolean continue_timer = FALSE;
//~ static gboolean start_timer = FALSE;
//~ static GdkPixmap *pixmap = NULL;
gint x_pos = 0;
//~ gint y_pos = 0;

gint buf[BUF_SIZE];

void end_program(GtkWidget *wid, gpointer ptr) {
    gtk_main_quit();
}

static gboolean update(gpointer data) {
    GtkWidget *widget = data;
    
    //~ gdouble time_stamp = g_timer_elapsed(data, NULL);
    //~ printf("%3.4f | callback triggered\n", time_stamp);
    //~ printf("callback triggered %d\n", time_1);
    //~ return continue_timer;
    
    //~ gdk_draw_line(pixmap, widget->style->black_gc, 
                        //~ 0, y_pos,
                        //~ x_pos, y_pos);
    x_pos++;
    if (x_pos > 512){
        x_pos = 0;
        //~ y_pos++;
    }
    buf[0] = x_pos;
    buf[15] = x_pos;
    buf[31] = x_pos;
    
    gtk_widget_queue_draw_area(widget,
        0, 0,
        widget->allocation.width, widget->allocation.height);
    
    return TRUE;
}

//~ static gboolean configure_event(GtkWidget *widget, GdkEventConfigure *event) {
    //~ if (pixmap)
        //~ g_object_unref (pixmap);

    //~ pixmap = gdk_pixmap_new (widget->window,
               //~ widget->allocation.width,
               //~ widget->allocation.height,
               //~ -1);
    //~ gdk_draw_rectangle (pixmap,
              //~ widget->style->white_gc,
              //~ TRUE,
              //~ 0, 0,
              //~ widget->allocation.width,
              //~ widget->allocation.height);

    //~ return TRUE;
//~ }

static gboolean expose_event(GtkWidget *widget, GdkEventExpose *event) {
    
    
    //~ gdk_draw_drawable(widget->window,
        //~ widget->style->fg_gc[gtk_widget_get_state(widget)],
        //~ pixmap,
        //~ event->area.x, event->area.y,
        //~ event->area.x, event->area.y,
        //~ event->area.width, event->area.height);
        
    cairo_t *cr;
    cairo_pattern_t *pat;
    int i;
    
    cr = gdk_cairo_create(gtk_widget_get_window(widget));
    pat = cairo_pattern_create_linear(0.0, 0.0,  0.0, 512.0);
    cairo_pattern_add_color_stop_rgba(pat, 0, 0.2, 0.2, 0.3, 1.0);
    cairo_pattern_add_color_stop_rgba(pat, 1, 0.0, 0.0, 0.0, 1.0);
    
    //~ cairo_set_source_rgb(cr, 0.8, 0.8, 1.0);
    cairo_set_source(cr, pat);
    cairo_rectangle(cr, 0.0, 0.0, 512.0, 512.0);
    cairo_fill(cr);
    
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, 512 - x_pos, 512);
    //~ cairo_line_to(cr, x_pos, 0);
    cairo_curve_to(cr, 512 - x_pos, 412, x_pos, 100, x_pos, 0);
    cairo_stroke(cr);
    
    //~ cairo_move_to(cr, 0, WIN_H - buf[i]);
    for (i = 0; i < BUF_SIZE; i++) {
        if (i < 1) {
            cairo_move_to(cr, (LINE_SPACING / 2), WIN_H - buf[i]);
        } else {
            cairo_curve_to(cr,
                           LINE_SPACING * (i - 1) + HANDLE_SPACING + (LINE_SPACING / 2), WIN_H - buf[i - 1],
                           LINE_SPACING * i - HANDLE_SPACING + (LINE_SPACING / 2), WIN_H - buf[i],
                           LINE_SPACING * i + (LINE_SPACING / 2), WIN_H - buf[i]
                           );
        }
        
    }
    cairo_stroke(cr);
    
    cairo_pattern_destroy(pat);
    cairo_destroy(cr);

    return FALSE;
}

int main(int argc, char *argv[]) {
    int i;
    for (i = 0; i < BUF_SIZE; i++) {
        buf[i] = 0;
    }
    //~ buf[15] = 100;
    
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *lbl = gtk_label_new("GTK test");
    //~ GtkWidget *image = gtk_image_new_from_file("/usr/share/icons/gnome/24x24/emblems/emblem-urgent.png");
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 512, 512);
    
    g_signal_connect(drawing_area, "expose_event", G_CALLBACK(expose_event), NULL);
    //~ g_signal_connect(drawing_area, "configure_event", G_CALLBACK(configure_event), NULL);
    
    GtkWidget *btn = gtk_button_new_with_label("Close window");
    //~ GTimer *timer1 = g_timer_new();
    
    g_signal_connect(btn, "clicked", G_CALLBACK(end_program), NULL);
    g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);
    
    GtkWidget *box = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), lbl, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn, TRUE, TRUE, 0);
    //~ gtk_box_pack_start(GTK_BOX(box), image, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), drawing_area, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(win), box);
    
    g_timeout_add(1000/60.0, update, drawing_area);
    //~ continue_timer = TRUE;
    //~ start_timer = TRUE;
    
    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
