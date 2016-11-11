#include "MobileRT/All.hpp"
#include <iostream>
#include <gtk/gtk.h>
#include <ctime>
//#include <pthread.h>

using namespace MobileRT;

/* Another callback */
void destroy( GtkWidget*, gpointer)
{
    gtk_main_quit();
}

static const int w = 932;
static const int h = 932;
static unsigned int canvas[h*w];
/*
void *thread_work(void* args)
{
    Renderer *ta = (Renderer*) args;
    ta->render(canvas, w);
    return NULL;
}
*/
int main(int argc, char** argv)
{
    clock_t start = clock();
    const int scene = atoi(argv[1]);
    const int shader = atoi(argv[2]);
	Renderer renderer(w, h, w, scene, shader);
    renderer.render(canvas, w);
/*
    pthread_t *thread_handles = (pthread_t*) malloc(1*sizeof(pthread_t));
    pthread_create(&thread_handles[0], (pthread_attr_t*) NULL, thread_work, (void*) &renderer);
*/
    clock_t end = clock() - start;
    double elapsed_secs = double(end) / CLOCKS_PER_SEC;
    std::cout << "\nTime in secs::" << elapsed_secs << std::endl;

    //RGBA
    unsigned char buffer[h*w*4];
    
    for(unsigned int i = 0, j = 0; i < w * h * 4; i += 4, j+=1)
    {
        unsigned int color = canvas[j];
        buffer[i  ] = color & 0xFF;
        buffer[i+1] = (color & 0x0000FF00) >> 8;
        buffer[i+2] = (color & 0x00FF0000) >> 16;
        buffer[i+3] = (color & 0xFF000000) >> 24;
    }

    //exit (0);
    //GTK
    gtk_init (&argc, &argv);
    GtkWidget* window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GdkPixbuf* pixbuff = gdk_pixbuf_new_from_data (buffer, GDK_COLORSPACE_RGB, TRUE, 8, w, h, w * 4, NULL, NULL); 
    GtkWidget* image = gtk_image_new_from_pixbuf (pixbuff);
    gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
    gtk_container_add (GTK_CONTAINER (window), image);
    gtk_widget_show_all (window);
    gtk_main ();

    return 0;
}