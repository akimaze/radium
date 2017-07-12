/* Copyright 2012 Kjetil S. Matheussen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

#if USE_QT_REQTYPE


#include <unistd.h>

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>
#include <QSplitter>
#include <QDesktopWidget>
#include <QPointer>

#include "../common/nsmtracker.h"
#include "../common/visual_proc.h"
#include "../common/gfx_op_queue_proc.h"
#include "../common/gfx_proc.h"

#include "FocusSniffers.h"

#include "EditorWidget.h"

#include "Qt_MainWindow_proc.h"
#include "../GTK/GTK_visual_proc.h"

#include "../common/OS_system_proc.h"

#include "../common/OS_visual_input.h"
#include "../OpenGL/Widget_proc.h"


#ifdef TEST_MAIN

void X11_ResetKeysUpDowns(void){}
void *talloc_atomic(size_t size){return malloc(size);}
void *talloc(size_t size){return calloc(1,size);}
QApplication *qapplication;

#else //  TEST_MAIN

extern struct Root *root;
//extern MyApplication *qapplication;

#endif //  TEST_MAIN

static const int x_margin = 25;
static const int y_margin = 10;

namespace{

struct MyQFrame : public QFrame{
  MyQFrame(QWidget *parent)
    : QFrame(parent)
  {}

  QPointer<QWidget> _widget_to_get_focus_when_shown;
  
  void showEvent(QShowEvent* e) override {
    this->activateWindow();
    if (_widget_to_get_focus_when_shown != NULL){
      GL_lock();{
        _widget_to_get_focus_when_shown->setFocus();
      }GL_unlock();
    }
    QFrame::showEvent(e);
  }
};
  
struct MyReqType{
  QPointer<MyQFrame> frame;
  QString label_text;
  QString default_value;
  int y;
  bool widgets_disabled;
};

}

static void init_reqtype(MyReqType *reqtype){
  R_ASSERT(reqtype->frame==NULL);
  
  GL_lock(); {
    GL_pause_gl_thread_a_short_while();

    reqtype->frame = new MyQFrame(get_current_parent(true));
    //reqtype->frame->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    //reqtype->frame->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    set_window_flags(reqtype->frame, true);
    
    if (reqtype->frame->layout() != NULL){
      reqtype->frame->layout()->setSpacing(10);
      reqtype->frame->layout()->setContentsMargins(20,20,20,20);
    }
    reqtype->frame->setContentsMargins(20,20,20,20);
                         
    //reqtype->frame->resize(5,10);
    //reqtype->frame->show();
    reqtype->y = y_margin;
    reqtype->widgets_disabled = false;
  
  }GL_unlock();
}

// tvisual might be NULL (tvisual is not used, it should be replaced by "int64_t parentguinum")
ReqType GFX_OpenReq(struct Tracker_Windows *tvisual,int width,int height,const char *title){
  obtain_keyboard_focus_counting(); // disable X11 keyboard sniffer
  
  MyReqType *reqtype = new MyReqType();

  init_reqtype(reqtype);

  if (strcmp(title,"")){
    GFX_WriteString(reqtype, title);
    GFX_WriteString(reqtype, "\n");
  }
    
  return reqtype;
}

// tvisual might be NULL  (tvisual is not used, it should be replaced by "int64_t parentguinum")
void GFX_CloseReq(struct Tracker_Windows *tvisual,ReqType das_reqtype){
  //EditorWidget *editor = static_cast<EditorWidget*>(tvisual->os_visual.widget);
  MyReqType *reqtype = static_cast<MyReqType*>(das_reqtype);

  delete reqtype->frame;

  release_keyboard_focus_counting();

  OS_SYSTEM_ResetKeysUpDowns(); // Since we disabled X11 events, the X11 event sniffer didn't notice that we changed focus.

  if(reqtype->widgets_disabled==true){
    //Qt_EnableAllWidgets();
    GFX_enable_mouse_keyboard();
  }

#if USE_GTK_VISUAL
  GTK_SetFocus();
#endif

  delete reqtype;
}

void GFX_WriteString(ReqType das_reqtype,const char *text){
  MyReqType *reqtype = static_cast<MyReqType*>(das_reqtype);

  reqtype->label_text += text;
}

void GFX_SetString(ReqType das_reqtype,const char *text){
  MyReqType *reqtype = static_cast<MyReqType*>(das_reqtype);

  reqtype->default_value = text;
}

namespace{
  class MyQLineEdit : public FocusSnifferQLineEdit {
  public:
    MyQLineEdit(QFrame *parent)
      : FocusSnifferQLineEdit(parent)
      , gotit(false)
    {
      setContextMenuPolicy(Qt::NoContextMenu); // Only way I've found to avoid it from popping up on windows.
    }
        
    void keyPressEvent ( QKeyEvent * event ){
      printf("oh yeah baby %d, scancode: %x\n",event->key(),event->nativeScanCode()-8);
      //event->ignore();
      if(event->key()>0){
        QLineEdit::keyPressEvent(event);
      
        if(event->key()==Qt::Key_Return || event->key()==Qt::Key_Escape)
          gotit = true;

#if USE_GTK_VISUAL
        if(event->key()==Qt::Key_Return)
          GTK_MainQuit();
#endif
      }
    }
    bool gotit;
  };
}


static void legalize_pos(MyReqType *reqtype){
  QWidget *w = reqtype->frame;
  
  if (w->parent()!=NULL)
    return;

  w->adjustSize();
  w->updateGeometry();

  int frame_width = R_MAX(100, w->width());
  
  int width = QApplication::desktop()->screenGeometry().width();
  int legal_pos = width - frame_width;
  //printf("legal_pos: %d. width: %d, x: %d\n",legal_pos, width, w->x());

  if (w->x() > legal_pos){
    w->move(legal_pos, w->y());
  }
}
      
      

void GFX_ReadString(ReqType das_reqtype,char *buffer,int bufferlength){

  MyReqType *reqtype = static_cast<MyReqType*>(das_reqtype);

  int x = x_margin;

  QPointer<MyQLineEdit> edit = new MyQLineEdit(reqtype->frame);
  edit->insert(reqtype->default_value);
  edit->adjustSize();
  edit->updateGeometry();

  float spacing = 1.5;
  
  QStringList lines = reqtype->label_text.split('\n', QString::KeepEmptyParts);
  
  if (lines.size() > 0 ){
    for(int i = 0 ; i < lines.size() ; i++){
      QString line = lines[i];
      printf("line: -%s-\n", line.toUtf8().constData());

      QLabel *label;
      
      GL_lock(); {
        label = new QLabel(line,reqtype->frame);
        label->move(x_margin,reqtype->y + 3);
        label->show();
        label->adjustSize();
        legalize_pos(reqtype);
      }GL_unlock();

      x = x_margin + label->width() + 5;

      if(lines.size() > 1 && i==lines.size()-2)
        reqtype->y += edit->height() * spacing; // i.e. i == second last line.
      else if (i < lines.size() - 1)
        reqtype->y += label->height() * spacing;
    
    }
  } else {
    
  }

  edit->move(x, reqtype->y);

  reqtype->frame->_widget_to_get_focus_when_shown = edit;
  
  reqtype->frame->setMinimumWidth(R_MAX(reqtype->frame->width(), x + 5 + 5 + x_margin + R_MAX(20,edit->width()+10)));
  reqtype->frame->setMinimumHeight(reqtype->y+R_MAX(20,edit->height()+10));
  reqtype->frame->adjustSize();
  reqtype->frame->updateGeometry();
  
  if (reqtype->frame->isVisible()==false){
    
    const bool show_at_mouse_position = false;
    
    if (show_at_mouse_position){
      auto pos = QCursor::pos();
      pos += QPoint(-reqtype->frame->width()/2, -reqtype->frame->height() - 10);
      reqtype->frame->move(pos);
    } else {
      moveWindowToCentre(reqtype->frame);
    }
    
    reqtype->frame->show();
    
  } else {

    GL_lock();{
      edit->show();
      edit->setFocus();
    }GL_unlock();
    
  }
  
  legalize_pos(reqtype);
  
  if(reqtype->widgets_disabled==false){
    //Qt_DisableAllWidgets(reqtype->frame);
    GFX_disable_mouse_keyboard();
    reqtype->widgets_disabled=true;
  }

#if USE_GTK_VISUAL
  GTK_MainLoop();
#endif

#if USE_QT_VISUAL
  QString text = edit->text();
  int edit_height = edit->height();
    
  {
    g_and_its_not_safe_to_paint = false;
    
    radium::ScopedExec scoped_exec;
    
    while(edit->gotit==false){
      // GL_lock is needed when using intel gfx driver to avoid crash caused by opening two opengl contexts simultaneously from two threads.
      GL_lock();{
        QApplication::processEvents();
      }GL_unlock();


      // Frame is deleted if the parent is deleted. Shouldn't happen though, but just in case.
      //
      if (reqtype->frame==NULL){
#if !defined(RELEASE)
        fprintf(stderr, "  REQTYPE->FRAM == NULL\n");
        getchar();
#endif
        init_reqtype(reqtype); // recreate the frame.
        break;
      }
      
      //GTK_HandleEvents();
      QString new_text = edit->text();
      
      if(text!=new_text){
        text = new_text;
        printf("text: \"%s\"\n",text.toUtf8().constData());
      }
      
      if(reqtype->frame->isVisible()==false)
        break;
      
      msleep(10);
    }

    g_and_its_not_safe_to_paint = true;
  }
  
#endif

  if (edit != NULL)
    edit->setEnabled(false);
  
  reqtype->label_text = "";
  
  //if (lines.size()==0)
  reqtype->y = reqtype->y + edit_height + 10;

  snprintf(buffer,bufferlength-1,"%s",text.toUtf8().constData());
  printf("Got: \"%s\"\n",buffer);
}


#ifdef TEST_MAIN
/*
  g++ -g -DTEST_MAIN -DUSE_QT_VISUAL -DDEBUG -I../Qt Qt_ReqType.cpp -Wall `pkg-config --libs --cflags QtGui` && ./a.out
  gdb ./a.out
*/

int main(int argc, char **argv){
  qapplication=new QApplication(argc,argv);

  ReqType reqtype = GFX_OpenReq(NULL,30,5,"gakkgakk");

  {
    GFX_WriteString(reqtype,"hello1? ");

    char temp[500];
    GFX_ReadString(reqtype,temp,500);

    printf("Got \"%s\"\n",temp);
  }

  {
    GFX_WriteString(reqtype,"line above");
    GFX_WriteString(reqtype,"\n");
    GFX_WriteString(reqtype,"> ");
    //GFX_WriteString(reqtype,"hello2?\n Answer: ");

    char temp[500];
    GFX_ReadString(reqtype,temp,500);

    printf("Got \"%s\"\n",temp);
  }

  GFX_CloseReq(NULL,reqtype);


  return 0;
}
#endif

#endif // USE_QT_REQTYPE
