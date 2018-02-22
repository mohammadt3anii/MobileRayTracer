#include "c_wrapper.h"
#include "MobileRT/Utils.hpp"
#include <cmath>
#include <gdk/gdkkeysyms.h>
#include <gsl/gsl>
#include <gtk/gtk.h>
#include <iostream>

int main(int argc, char **argv) noexcept {
    if (argc < 15) {
        ::std::cerr << "Not enough arguments provided" << '\n';
        return 1;
    }
    const ::gsl::multi_span<char *> args{argv, argc};

    const int threads{static_cast<int> (strtol(args[1], nullptr, 0))};
    const int shader{static_cast<int> (strtol(args[2], nullptr, 0))};
    const int scene{static_cast<int> (strtol(args[3], nullptr, 0))};
    const int samplesPixel{static_cast<int> (strtol(args[4], nullptr, 0))};
    const int samplesLight{static_cast<int> (strtol(args[5], nullptr, 0))};

    const int width_{
            ::MobileRT::roundDownToMultipleOf(static_cast<int> (strtol(args[6], nullptr, 0)),
                                              static_cast<int>(::std::sqrt(
                                                      ::MobileRT::NumberOfBlocks)))};

    const int height_{
            ::MobileRT::roundDownToMultipleOf(static_cast<int> (strtol(args[7], nullptr, 0)),
                                              static_cast<int>(::std::sqrt(
                                                      ::MobileRT::NumberOfBlocks)))};

    const int accelerator{static_cast<int> (strtol(args[8], nullptr, 0))};

    const int repeats{static_cast<int> (strtol(args[9], nullptr, 0))};
    const char *const pathObj{args[10]};
    const char *const pathMtl{args[11]};

    ::std::istringstream ssPrintStdOut(args[12]);
    ::std::istringstream ssAsync(args[13]);
    ::std::istringstream ssShowImage(args[14]);
    bool printStdOut{true};
    bool async{true};
    bool showImage{true};

    bool tPrint{true};
    bool tAsync{true};
    bool tShowImage{true};
    ssPrintStdOut >> ::std::boolalpha;
    ssPrintStdOut >> printStdOut;
    ssAsync >> ::std::boolalpha >> async;
    ssShowImage >> ::std::boolalpha >> showImage;
    if (!tAsync) {
        ::std::cerr << "Incorrect argument provided.\n";
        return 1;
    }
    if (!tPrint) {
        ::std::cerr << "Incorrect argument provided.\n";
        return 1;
    }
    if (!tShowImage) {
        ::std::cerr << "Incorrect argument provided.\n";
        return 1;
    }

    const unsigned size{static_cast<unsigned>(width_) * static_cast<unsigned>(height_)};
    ::std::unique_ptr<unsigned char[]> buffer{::std::make_unique<unsigned char[]>(size * 4u)};
    ::std::vector<unsigned> bitmap(size);
    bitmap.reserve(size);

    /*float avg {0};
    avg = ::MobileRT::incrementalAVG(1.0f);
    avg = ::MobileRT::incrementalAVG(2.0f);
    avg = ::MobileRT::incrementalAVG(3.0f);
    avg = ::MobileRT::incrementalAVG(4.0f);
    LOG("AVG = ", avg);*/

    RayTrace(bitmap.data(), width_, height_, threads, shader, scene, samplesPixel, samplesLight,
             repeats, accelerator, printStdOut, async, pathObj, pathMtl);

    if (!showImage) {
        return 0;
    }


    for (size_t i(0), j(0); i < static_cast<size_t>(size) * 4; i += 4, j += 1) {
        const unsigned color{bitmap[j]};
        buffer[i + 0] = static_cast<unsigned char> ((color & 0x000000FF) >> 0);
        buffer[i + 1] = static_cast<unsigned char> ((color & 0x0000FF00) >> 8);
        buffer[i + 2] = static_cast<unsigned char> ((color & 0x00FF0000) >> 16);
        buffer[i + 3] = static_cast<unsigned char> ((color & 0xFF000000) >> 24);
    }
    gtk_init(&argc, &argv);
    GtkWidget *window{gtk_window_new(GTK_WINDOW_TOPLEVEL)};
    GdkPixbuf *pixbuff{
            gdk_pixbuf_new_from_data(buffer.get(), GDK_COLORSPACE_RGB, TRUE, 8,
                                     static_cast<int> (width_),
                                     static_cast<int> (height_),
                                     static_cast<int> (width_ * 4), nullptr, nullptr)};
    GtkWidget *image{gtk_image_new_from_pixbuf(pixbuff)};
    gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(
            []() noexcept -> void {
                gtk_main_quit();
            }
    ), nullptr);
    auto *check_escape(static_cast<bool (*)(
            GtkWidget *gtkWidget, GdkEventKey *event, gpointer)>(
                               [](GtkWidget *gtkWidget, GdkEventKey *event,
                                  gpointer) noexcept -> bool {
                                   if (event->keyval == GDK_KEY_Escape) {
                                       gtk_widget_destroy(gtkWidget);
                                       gtk_main_quit();
                                       return true;
                                   }
                                   return false;
                               })
    );
    gtk_signal_connect(GTK_OBJECT(window), "key_press_event", GTK_SIGNAL_FUNC(check_escape),
                       nullptr);
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);
    gtk_main();
    g_object_unref(G_OBJECT(pixbuff));
    return argc;
}
