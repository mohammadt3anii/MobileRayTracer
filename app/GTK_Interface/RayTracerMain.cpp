#include "GTK_Interface/c_wrapper.h"
#include "MobileRT/Utils.hpp"
#include <cmath>
#include <gdk/gdkkeysyms.h>
#include <gsl/gsl>
#include <gtk/gtk.h>
#include <iostream>

::std::int32_t main(::std::int32_t argc, char **argv) noexcept {
    assert(argc == 15);
    const ::gsl::multi_span<char *> args{argv, argc};

    const ::std::int32_t threads{static_cast<::std::int32_t> (strtol(args[1], nullptr, 0))};
    const ::std::int32_t shader{static_cast<::std::int32_t> (strtol(args[2], nullptr, 0))};
    const ::std::int32_t scene{static_cast<::std::int32_t> (strtol(args[3], nullptr, 0))};
    const ::std::int32_t samplesPixel{static_cast<::std::int32_t> (strtol(args[4], nullptr, 0))};
    const ::std::int32_t samplesLight{static_cast<::std::int32_t> (strtol(args[5], nullptr, 0))};

    const ::std::int32_t width_{
            ::MobileRT::roundDownToMultipleOf(static_cast<::std::int32_t> (strtol(args[6], nullptr, 0)),
                                              static_cast<::std::int32_t>(::std::sqrt(
                                                      ::MobileRT::NumberOfBlocks)))};

    const ::std::int32_t height_{
            ::MobileRT::roundDownToMultipleOf(static_cast<::std::int32_t> (strtol(args[7], nullptr, 0)),
                                              static_cast<::std::int32_t>(::std::sqrt(
                                                      ::MobileRT::NumberOfBlocks)))};

    const ::std::int32_t accelerator{static_cast<::std::int32_t> (strtol(args[8], nullptr, 0))};

    const ::std::int32_t repeats{static_cast<::std::int32_t> (strtol(args[9], nullptr, 0))};
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
    assert(tAsync && tPrint && tShowImage);

    const ::std::uint32_t size{static_cast<::std::uint32_t>(width_) * static_cast<::std::uint32_t>(height_)};
    ::std::unique_ptr<uint8_t[]> buffer{::std::make_unique<uint8_t[]>(size * 4u)};
    ::std::vector<::std::uint32_t> bitmap(size);
    bitmap.reserve(size);

    RayTrace(bitmap.data(), width_, height_, threads, shader, scene, samplesPixel, samplesLight,
             repeats, accelerator, printStdOut, async, pathObj, pathMtl);

    if (!showImage) {
        return 0;
    }


    for (size_t i(0), j(0); i < static_cast<size_t>(size) * 4; i += 4, j += 1) {
        const ::std::uint32_t color{bitmap[j]};
        buffer[i + 0] = static_cast<uint8_t> ((color & 0x000000FF) >> 0);
        buffer[i + 1] = static_cast<uint8_t> ((color & 0x0000FF00) >> 8);
        buffer[i + 2] = static_cast<uint8_t> ((color & 0x00FF0000) >> 16);
        buffer[i + 3] = static_cast<uint8_t> ((color & 0xFF000000) >> 24);
    }
    gtk_init(&argc, &argv);
    GtkWidget *window{gtk_window_new(GTK_WINDOW_TOPLEVEL)};
    GdkPixbuf *pixbuff{
            gdk_pixbuf_new_from_data(buffer.get(), GDK_COLORSPACE_RGB, TRUE, 8,
                                     static_cast<::std::int32_t> (width_),
                                     static_cast<::std::int32_t> (height_),
                                     static_cast<::std::int32_t> (width_ * 4), nullptr, nullptr)};
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
