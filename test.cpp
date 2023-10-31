#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(){
    Display *display = XOpenDisplay(nullptr);
    if(display == nullptr){
        std::cerr << "Can not open display" <<std::endl;
        return -1;
    }

    int screen = DefaultScreen(display);

    Window window = XCreateSimpleWindow(display,
        RootWindow(display , screen),
        0,0,400,300,1 , 
        BlackPixel(display,screen),
        WhitePixel(display,screen));
    
    std::cout << "create window success" << std::endl;

    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display , window , &wmDeleteWindow , 1);
    
    XSelectInput(display , window , 
        ExposureMask 
        | KeyPressMask 
        | LeaveWindowMask 
        | PointerMotionMask 
        | PointerMotionHintMask
        | ClientMessage
        | StructureNotifyMask);

    XMapWindow(display , window);

    XStoreName(display, window , "Hello XLIB");

    std::cout << "create XStoreName success" << std::endl;

    XFontStruct *fontInfo = XLoadQueryFont(display , "6x13");
    if(fontInfo == nullptr){
        std::cerr << "fontInfo is null quit!" <<std::endl;
        return -2;
    }
    GC gc = XCreateGC(display , window , 0 , nullptr);

    XSetFont(display , gc , fontInfo->fid);

    XSetForeground(display , gc , WhitePixel(display , screen));

    XEvent event;

    while (true) {
        XNextEvent(display, &event);

        std::cout << "event type:" << event.type << std::endl;
        if (event.type == Expose) {
            XDrawString(display, window, gc, 10, 30, "Hello window", 12);
        } else if (event.type == KeyPress || event.type == ClientMessage) {
            break;
        }else if(event.type == MotionNotify){
            std::cout << "x : " << event.xmotion.x 
                << "  y : " << event.xmotion.y << std::endl;
        }

    }//end while

    XUnloadFont(display, fontInfo->fid);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}

