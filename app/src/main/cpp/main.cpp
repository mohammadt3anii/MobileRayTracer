#include <iostream>
#include "MobileRT/All.h"

#include <gtk/gtk.h>

#include <ctime>

using namespace MobileRT;

/* Another callback */
void destroy( GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char** argv)
{
    const int w = 932;
    const int h = 932;
    int scene = atoi(argv[1]);
    int shader = atoi(argv[2]);
	Renderer renderer(w, h, w, scene, shader);
    uint32_t canvas[h*w];

    clock_t start = clock();
    renderer.render(canvas, w, h);
    clock_t end = clock() - start;
    double elapsed_secs = double(end) / CLOCKS_PER_SEC;
    std::cout << "\nTime in secs::" << elapsed_secs << std::endl;


    //RGBA
    unsigned char buffer[h*w*4];
    uint32_t i = 0;
    uint32_t j = 0;
    for(; i < w*h*4; i+=4, j+=1)
    {
        uint32_t color = canvas[j];
        buffer[i  ] = color & 0xFF;
        buffer[i+1] = (color & 0x0000FF00) >> 8;
        buffer[i+2] = (color & 0x00FF0000) >> 16;
        buffer[i+3] = (color & 0xFF000000) >> 24;
    }

    //GTK
    gtk_init (&argc, &argv);
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GdkPixbuf* pixbuff = gdk_pixbuf_new_from_data(buffer,GDK_COLORSPACE_RGB,TRUE,8,w,h,w*4, NULL,NULL);
    GtkWidget* image = gtk_image_new_from_pixbuf(pixbuff);

    gtk_signal_connect(GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);

    gtk_container_add(GTK_CONTAINER (window), image);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}