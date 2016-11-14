#include "MobileRT/All.h"
#include <iostream>
#include <gtk/gtk.h>
#include <omp.h>

using namespace MobileRT;

std::vector<int> testNRVO(int value, size_t size, const std::vector<int> **localVec)
{
   std::vector<int> vec(size, value);

   *localVec = &vec;

   /* Do something here.. */

   return vec;
}

void testNRVO()
{
   const std::vector<int> *localVec (nullptr);

   std::vector<int> vec (testNRVO(0, 10, &localVec));

   if (&vec == localVec)
      std::cout << "NRVO was applied" << std::endl;
   else
      std::cout << "NRVO was not applied" << std::endl;
}

void destroy( GtkWidget*, gpointer)
{
    gtk_main_quit();
}

static const int w (932);
static const int h (932);
static unsigned int canvas[h*w];

int main(int argc, char** argv)
{
    const int scene (atoi(argv[1]));
    const int shader (atoi(argv[2]));
    const int threads (atoi(argv[3]));
    
    double start (omp_get_wtime ());
    Renderer renderer(w, h, scene, shader);
    renderer.render(canvas, threads);
    double end (omp_get_wtime () - start);
    std::cout << "\nTime in secs::" << end << std::endl;

    //RGBA
    unsigned char buffer[h*w*4];
    
    for(unsigned int i (0), j (0); i < w * h * 4; i += 4, j+=1)
    {
        unsigned int color (canvas[j]);
        buffer[i + 0] = (color & 0x000000FF) >> 0;
        buffer[i+1] = (color & 0x0000FF00) >> 8;
        buffer[i+2] = (color & 0x00FF0000) >> 16;
        buffer[i+3] = (color & 0xFF000000) >> 24;
    }
    //return 0;

    gtk_init (&argc, &argv);
    GtkWidget* window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GdkPixbuf* pixbuff = gdk_pixbuf_new_from_data (buffer, GDK_COLORSPACE_RGB, TRUE, 8, w, h, w * 4, NULL, NULL); 
    GtkWidget* image = gtk_image_new_from_pixbuf (pixbuff);
    gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (destroy), NULL);
    gtk_container_add (GTK_CONTAINER (window), image);
    gtk_widget_show_all (window);
    //gtk_main ();

    return 0;
}