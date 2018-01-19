#include "c_wrapper.hpp"
#include "MobileRT/Utils.hpp"
#include <cmath>
#include <iostream>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

static unsigned *bitmap {};
static unsigned char *buffer {};//RGBA

int main(int argc, char **argv) noexcept {
	const int threads {atoi (argv[1])};
	const int shader {atoi (argv[2])};
	const int scene {atoi (argv[3])};
  const int samplesPixel {atoi (argv[4])};
	const int samplesLight {atoi (argv[5])};
  const int width_ {::MobileRT::roundDownToMultipleOf (atoi (argv[6]), static_cast<int>(::std::sqrt (::MobileRT::NumberOfBlocks)))};
  const int height_ {::MobileRT::roundDownToMultipleOf (atoi (argv[7]), static_cast<int>(::std::sqrt (::MobileRT::NumberOfBlocks)))};
	const int accelerator {atoi (argv[8])};
  int repeats {1};
  bitmap = new unsigned[static_cast<unsigned>(width_) * static_cast<unsigned>(height_)];
  buffer = new unsigned char[static_cast<unsigned>(width_) * static_cast<unsigned>(height_) * 4u];

  RayTrace (bitmap, width_, height_, 0, threads, shader, scene, samplesPixel, samplesLight, repeats, accelerator);
  

  for (int i (0), j (0); i < width_ * height_ * 4; i += 4, j += 1) {
    const unsigned color {bitmap[j]};
    buffer[i + 0] = static_cast<unsigned char> ((color & 0x000000FF) >> 0);
    buffer[i + 1] = static_cast<unsigned char> ((color & 0x0000FF00) >> 8);
    buffer[i + 2] = static_cast<unsigned char> ((color & 0x00FF0000) >> 16);
    buffer[i + 3] = static_cast<unsigned char> ((color & 0xFF000000) >> 24);
  }
  gtk_init (&argc, &argv);
  GtkWidget *window {gtk_window_new (GTK_WINDOW_TOPLEVEL)};
  GdkPixbuf *pixbuff {
    gdk_pixbuf_new_from_data (buffer, GDK_COLORSPACE_RGB, TRUE, 8,
                              static_cast<int> (width_),
                              static_cast<int> (height_),
                              static_cast<int> (width_ * 4), nullptr, nullptr)};
  GtkWidget *image {gtk_image_new_from_pixbuf (pixbuff)};
  gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (
    [] () -> void {
      delete[] bitmap;
      delete[] buffer;
      gtk_main_quit ();
    }
  ), nullptr);
  auto *check_escape (static_cast<bool (*) (
    GtkWidget *gtkWidget, GdkEventKey *event, gpointer)>(
                        [] (GtkWidget *gtkWidget, GdkEventKey *event, gpointer) {
                          if (event -> keyval == GDK_KEY_Escape) {
                            gtk_widget_destroy (gtkWidget);
                            gtk_main_quit ();
                            return true;
                          }
                          return false;
                        })
  );
  gtk_signal_connect (GTK_OBJECT (window), "key_press_event", GTK_SIGNAL_FUNC (check_escape),
                      nullptr);
  gtk_container_add (GTK_CONTAINER (window), image);
  gtk_widget_show_all (window);
  gtk_main ();
  g_object_unref (G_OBJECT (pixbuff));
  return argc;
}
