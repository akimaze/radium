/* Copyright 2014-2016 Kjetil S. Matheussen

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

#include <QWidget>

#if USE_QT5
#include <QWindow>
#include <QScreen>
#endif

#include <math.h>
#include <stdio.h>

#include <vlVG/VectorGraphics.hpp>
#include <vlGraphics/Rendering.hpp>
#if USE_QT5
#include <vlQt5/Qt5ThreadedWidget.hpp>
#else
#include <vlQt4/Qt4ThreadedWidget.hpp>
#endif
#include <vlGraphics/SceneManagerActorTree.hpp>
#include <vlVG/SceneManagerVectorGraphics.hpp>

#include <QGLWidget>
#include <QTextEdit>
#include <QMessageBox>
#include <QApplication>
#include <QAbstractButton>
#include <QMainWindow>
#include <QGLFormat>
#include <QDebug>

#include "../common/nsmtracker.h"
#include "../common/playerclass.h"
#include "../common/list_proc.h"
#include "../common/realline_calc_proc.h"
#include "../common/time_proc.h"
#include "../common/settings_proc.h"
#include "../common/OS_Semaphores.h"
#include "../common/OS_Player_proc.h"
#include "../common/Semaphores.hpp"
#include "../common/Mutex.hpp"

#define GE_DRAW_VL
#include "GfxElements.h"
#include "Timing.hpp"
#include "Render_proc.h"

#include "Widget_proc.h"

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/*
=================================================================
==9461==ERROR: AddressSanitizer: new-delete-type-mismatch on 0x614001109440 in thread T0 (radium):
  object passed to delete has wrong type:
  size of the allocated type:   400 bytes;
  size of the deallocated type: 16 bytes.
    #0 0x7ffff7126812 in operator delete(void*, unsigned long) ../../.././libsanitizer/asan/asan_new_delete.cc:108
    #1 0x7ffff1107b0e in QOpenGLVersionFunctionsStorage::~QOpenGLVersionFunctionsStorage() opengl/qopenglversionfunctions.cpp:81
    #2 0x7ffff0e2eda4 in QOpenGLContextPrivate::~QOpenGLContextPrivate() kernel/qopenglcontext_p.h:219
    #3 0x7ffff0e2eda4 in QOpenGLContextPrivate::~QOpenGLContextPrivate() kernel/qopenglcontext_p.h:222
    #4 0x7ffff04e674b in QScopedPointerDeleter<QObjectData>::cleanup(QObjectData*) ../../include/QtCore/../../src/corelib/tools/qscopedpointer.h:60
    #5 0x7ffff04e674b in QScopedPointer<QObjectData, QScopedPointerDeleter<QObjectData> >::~QScopedPointer() ../../include/QtCore/../../src/corelib/tools/qscopedpointer.h:107
    #6 0x7ffff04e674b in QObject::~QObject() kernel/qobject.cpp:900
    #7 0x7ffff0e2e798 in QOpenGLContext::~QOpenGLContext() kernel/qopenglcontext.cpp:696
    #8 0x7ffff18cbeeb in QOpenGLWidgetPrivate::reset() kernel/qopenglwidget.cpp:678
    #9 0x7ffff18cc762 in QOpenGLWidgetPrivate::~QOpenGLWidgetPrivate() kernel/qopenglwidget.cpp:576
    #10 0x7ffff18cc762 in QOpenGLWidgetPrivate::~QOpenGLWidgetPrivate() kernel/qopenglwidget.cpp:577
    #11 0x7ffff04e674b in QScopedPointerDeleter<QObjectData>::cleanup(QObjectData*) ../../include/QtCore/../../src/corelib/tools/qscopedpointer.h:60
    #12 0x7ffff04e674b in QScopedPointer<QObjectData, QScopedPointerDeleter<QObjectData> >::~QScopedPointer() ../../include/QtCore/../../src/corelib/tools/qscopedpointer.h:107
    #13 0x7ffff04e674b in QObject::~QObject() kernel/qobject.cpp:900
    #14 0x7ffff18a91fb in QWidget::~QWidget() kernel/qwidget.cpp:1548
    #15 0x7ffff6fe3ca9 in QtWebEngineCore::RenderWidgetHostViewQtDelegateWidget::~RenderWidgetHostViewQtDelegateWidget() .moc/../render_widget_host_view_qt_delegate_widget.h:58
    #16 0x7ffff6fe3ca9 in QtWebEngineCore::RenderWidgetHostViewQtDelegateWidget::~RenderWidgetHostViewQtDelegateWidget() .moc/../render_widget_host_view_qt_delegate_widget.h:58
    #17 0x7ffff23e605e in std::default_delete<QtWebEngineCore::RenderWidgetHostViewQtDelegate>::operator()(QtWebEngineCore::RenderWidgetHostViewQtDelegate*) const /home/kjetil/site_gcc/include/c++/5.3.0/bits/unique_ptr.h:76
    #18 0x7ffff23e605e in base::internal::scoped_ptr_impl<QtWebEngineCore::RenderWidgetHostViewQtDelegate, std::default_delete<QtWebEngineCore::RenderWidgetHostViewQtDelegate> >::reset(QtWebEngineCore::RenderWidgetHostViewQtDelegate*) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/memory/scoped_ptr.h:177
    #19 0x7ffff23e605e in base::internal::scoped_ptr_impl<QtWebEngineCore::RenderWidgetHostViewQtDelegate, std::default_delete<QtWebEngineCore::RenderWidgetHostViewQtDelegate> >::~scoped_ptr_impl() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/memory/scoped_ptr.h:166
    #20 0x7ffff23e605e in scoped_ptr<QtWebEngineCore::RenderWidgetHostViewQtDelegate, std::default_delete<QtWebEngineCore::RenderWidgetHostViewQtDelegate> >::~scoped_ptr() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/memory/scoped_ptr.h:240
    #21 0x7ffff23e605e in QtWebEngineCore::RenderWidgetHostViewQt::~RenderWidgetHostViewQt() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/core/render_widget_host_view_qt.cpp:250
    #22 0x7ffff23e61f8 in QtWebEngineCore::RenderWidgetHostViewQt::~RenderWidgetHostViewQt() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/core/render_widget_host_view_qt.cpp:255
    #23 0x7ffff3051080 in content::RenderWidgetHostImpl::Destroy(bool) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/renderer_host/render_widget_host_impl.cc:1460
    #24 0x7ffff3041e7e in content::RenderViewHostImpl::ShutdownAndDestroy() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/renderer_host/render_view_host_impl.cc:958
    #25 0x7ffff31aa364 in content::FrameTree::ReleaseRenderViewHostRef(content::RenderViewHostImpl*) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/frame_host/frame_tree.cc:382
    #26 0x7ffff2fb6d9c in content::RenderFrameHostImpl::~RenderFrameHostImpl() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/frame_host/render_frame_host_impl.cc:294
    #27 0x7ffff2fb72b8 in content::RenderFrameHostImpl::~RenderFrameHostImpl() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/frame_host/render_frame_host_impl.cc:295
    #28 0x7ffff2fbb952 in linked_ptr<content::RenderFrameHostImpl>::depart() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/memory/linked_ptr.h:144
    #29 0x7ffff2fbb952 in linked_ptr<content::RenderFrameHostImpl>::~linked_ptr() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/memory/linked_ptr.h:82
    #30 0x7ffff2fbb952 in std::_List_node<linked_ptr<content::RenderFrameHostImpl> >::~_List_node() /home/kjetil/site_gcc/include/c++/5.3.0/bits/stl_list.h:106
    #31 0x7ffff2fbb952 in void __gnu_cxx::new_allocator<std::_List_node<linked_ptr<content::RenderFrameHostImpl> > >::destroy<std::_List_node<linked_ptr<content::RenderFrameHostImpl> > >(std::_List_node<linked_ptr<content::RenderFrameHostImpl> >*) /home/kjetil/site_gcc/include/c++/5.3.0/ext/new_allocator.h:124
    #32 0x7ffff2fbb952 in std::__cxx11::list<linked_ptr<content::RenderFrameHostImpl>, std::allocator<linked_ptr<content::RenderFrameHostImpl> > >::_M_erase(std::_List_iterator<linked_ptr<content::RenderFrameHostImpl> >) /home/kjetil/site_gcc/include/c++/5.3.0/bits/stl_list.h:1777
    #33 0x7ffff2fbb952 in std::__cxx11::list<linked_ptr<content::RenderFrameHostImpl>, std::allocator<linked_ptr<content::RenderFrameHostImpl> > >::erase(std::_List_const_iterator<linked_ptr<content::RenderFrameHostImpl> >) /home/kjetil/site_gcc/include/c++/5.3.0/bits/list.tcc:156
    #34 0x7ffff2fbb952 in content::RenderFrameHostManager::DeleteFromPendingList(content::RenderFrameHostImpl*) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/frame_host/render_frame_host_manager.cc:797
    #35 0x7ffff2fb0521 in content::RenderFrameHostImpl::OnSwappedOut() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/frame_host/render_frame_host_impl.cc:1295
    #36 0x7ffff2fba62a in bool IPC::Message::Dispatch<content::RenderFrameHostImpl, content::RenderFrameHostImpl, void>(IPC::Message const*, content::RenderFrameHostImpl*, content::RenderFrameHostImpl*, void*, void (content::RenderFrameHostImpl::*)()) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/ipc/ipc_message.h:158
    #37 0x7ffff2fba62a in content::RenderFrameHostImpl::OnMessageReceived(IPC::Message const&) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/frame_host/render_frame_host_impl.cc:522
    #38 0x7ffff3038b05 in content::RenderProcessHostImpl::OnMessageReceived(IPC::Message const&) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/content/browser/renderer_host/render_process_host_impl.cc:1723
    #39 0x7ffff39bdc7f in IPC::ChannelProxy::Context::OnDispatchMessage(IPC::Message const&) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/ipc/ipc_channel_proxy.cc:293
    #40 0x7ffff3d78e88 in base::Callback<void ()>::Run() const /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/callback.h:394
    #41 0x7ffff3d78e88 in base::debug::TaskAnnotator::RunTask(char const*, base::PendingTask const&) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/debug/task_annotator.cc:51
    #42 0x7ffff3d25788 in base::MessageLoop::RunTask(base::PendingTask const&) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/message_loop/message_loop.cc:486
    #43 0x7ffff3d26388 in base::MessageLoop::DeferOrRunPendingTask(base::PendingTask const&) /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/message_loop/message_loop.cc:495
    #44 0x7ffff3d2684c in base::MessageLoop::DoWork() /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/3rdparty/chromium/base/message_loop/message_loop.cc:607
    #45 0x7ffff23c0b84 in handleScheduledWork /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/core/content_browser_client_qt.cpp:193
    #46 0x7ffff23c0b84 in customEvent /newhd/fedora19stuff/qt-everywhere-opensource-src-5.7.0/qtwebengine/src/core/content_browser_client_qt.cpp:175
    #47 0x7ffff04dfbda in QObject::event(QEvent*) kernel/qobject.cpp:1285
    #48 0x7ffff186989b in QApplicationPrivate::notify_helper(QObject*, QEvent*) kernel/qapplication.cpp:3799
    #49 0x7ffff1870cb5 in QApplication::notify(QObject*, QEvent*) kernel/qapplication.cpp:3556
    #50 0x7ffff04b5c97 in QCoreApplication::notifyInternal2(QObject*, QEvent*) kernel/qcoreapplication.cpp:988
    #51 0x7ffff04b828a in QCoreApplication::sendEvent(QObject*, QEvent*) ../../include/QtCore/../../src/corelib/kernel/qcoreapplication.h:231
    #52 0x7ffff04b828a in QCoreApplicationPrivate::sendPostedEvents(QObject*, int, QThreadData*) kernel/qcoreapplication.cpp:1649
    #53 0x7ffff0506182 in postEventSourceDispatch kernel/qeventdispatcher_glib.cpp:276
    #54 0x3b4b047824 in g_main_context_dispatch (/lib64/libglib-2.0.so.0+0x3b4b047824)
    #55 0x3b4b047b57  (/lib64/libglib-2.0.so.0+0x3b4b047b57)
    #56 0x3b4b047c13 in g_main_context_iteration (/lib64/libglib-2.0.so.0+0x3b4b047c13)
    #57 0x7ffff050658e in QEventDispatcherGlib::processEvents(QFlags<QEventLoop::ProcessEventsFlag>) kernel/qeventdispatcher_glib.cpp:423
    #58 0x7ffff04b4209 in QEventLoop::exec(QFlags<QEventLoop::ProcessEventsFlag>) kernel/qeventloop.cpp:210
    #59 0x7ffff04bc23c in QCoreApplication::exec() kernel/qcoreapplication.cpp:1261
    #60 0x6f18d9 in radium_main Qt/Qt_Main.cpp:1287
    #61 0x820fae in init_radium api/api_common.c:63
    #62 0x8205c6 in _wrap_init_radium api/radium_wrap.c:577
    #63 0x3b5d4dd0e0 in PyEval_EvalFrameEx (/lib64/libpython2.7.so.1.0+0x3b5d4dd0e0)
    #64 0x3b5d4ddb1e in PyEval_EvalCodeEx (/lib64/libpython2.7.so.1.0+0x3b5d4ddb1e)
    #65 0x3b5d4ddbf1 in PyEval_EvalCode (/lib64/libpython2.7.so.1.0+0x3b5d4ddbf1)
    #66 0x3b5d4f6b99  (/lib64/libpython2.7.so.1.0+0x3b5d4f6b99)
    #67 0x3b5d4f7991 in PyRun_FileExFlags (/lib64/libpython2.7.so.1.0+0x3b5d4f7991)
    #68 0x3b5d4d4b75  (/lib64/libpython2.7.so.1.0+0x3b5d4d4b75)
    #69 0x3b5d4dd0e0 in PyEval_EvalFrameEx (/lib64/libpython2.7.so.1.0+0x3b5d4dd0e0)
    #70 0x3b5d4ddb1e in PyEval_EvalCodeEx (/lib64/libpython2.7.so.1.0+0x3b5d4ddb1e)
    #71 0x3b5d4ddbf1 in PyEval_EvalCode (/lib64/libpython2.7.so.1.0+0x3b5d4ddbf1)
    #72 0x3b5d4f6b99  (/lib64/libpython2.7.so.1.0+0x3b5d4f6b99)
    #73 0x3b5d4f7767 in PyRun_StringFlags (/lib64/libpython2.7.so.1.0+0x3b5d4f7767)
    #74 0x3b5d4f827a in PyRun_SimpleStringFlags (/lib64/libpython2.7.so.1.0+0x3b5d4f827a)
    #75 0x6f28c2 in main Qt/Qt_Main.cpp:1618
    #76 0x3b49421734 in __libc_start_main (/lib64/libc.so.6+0x3b49421734)
    #77 0x4e7860  (/home/kjetil/radium/bin/radium+0x4e7860)

0x614001109440 is located 0 bytes inside of 400-byte region [0x614001109440,0x6140011095d0)
allocated by thread T0 (radium) here:
    #0 0x7ffff7125d8a in operator new(unsigned long) ../../.././libsanitizer/asan/asan_new_delete.cc:60
    #1 0x7ffff1107d11 in QOpenGLVersionFunctionsStorage::backend(QOpenGLContext*, QOpenGLVersionFunctionsBackend::Version) opengl/qopenglversionfunctions.cpp:108

SUMMARY: AddressSanitizer: new-delete-type-mismatch ../../.././libsanitizer/asan/asan_new_delete.cc:108 operator delete(void*, unsigned long)
==9461==HINT: if you don't care about these warnings you may set ASAN_OPTIONS=new_delete_type_mismatch=0
==9461==ABORTING

*/

#if FOR_LINUX
static int set_pthread_priority(pthread_t pthread,int policy,int priority,const char *message,const char *name){
  struct sched_param par={};
  par.sched_priority=priority;

  //if((sched_setscheduler(pid,policy,&par)!=0)){
  if ((pthread_setschedparam(pthread, policy, &par)) != 0) {
    fprintf(stderr,message,policy==SCHED_FIFO?"SCHED_FIFO":policy==SCHED_RR?"SCHED_RR":policy==SCHED_OTHER?"SCHED_OTHER":"SCHED_UNKNOWN",priority,getpid(),name,strerror(errno));
    //abort();
    return 0;
  }
  return 1;
}

static void set_realtime(int type, int priority){
  //bound_thread_to_cpu(0);
  set_pthread_priority(pthread_self(),type,priority,"Unable to set %s/%d for %d (\"%s\"). (%s)\n", "a gc thread");
}
#endif

static DEFINE_ATOMIC(bool, g_has_updated_at_least_once) = false;

DEFINE_ATOMIC(char *, GE_vendor_string) = NULL;
DEFINE_ATOMIC(char *, GE_renderer_string) = NULL;
DEFINE_ATOMIC(char *, GE_version_string) =NULL;
DEFINE_ATOMIC(uint32_t, GE_opengl_version_flags) = 0;


static bool g_safe_mode = false;

#if USE_QT5
static const bool USE_GL_LOCK = false; // Seems like this is safe... (the lock is used to avoid simultaneous access to openglcontext, but in qt5 we specifically tell qt that we use opengl from another thread, so I guess/hope qt is not accessing the openglcontext in qt5 when we do that. (this was also the main reason for switching to qt5, since the gl_lock doesn't always work perfectly in qt4.))
#else
static const bool USE_GL_LOCK = true;
#endif

static radium::Mutex mutex;
static radium::Mutex draw_mutex(true); // recursive mutex

static __thread int g_gl_lock_visits = 0; // simulate a recursive mutex this way instead of using the QThread::Recursive option since QWaitCondition doesn't work with recursive mutexes. We need recursive mutexes since calls to qsometing->exec() (which are often executed inside the gl lock) can process qt events, which again can call some function which calls gl_lock. I don't know why qt processes qt events inside exec() though. That definitely seems like the wrong desing, or maybe it's even a bug in qt. If there is some way of turning this peculiar behavior off, I would like to know about it. I don't feel that this behaviro is safe.


void GL_lock(void){

  g_gl_lock_visits++;  
  if (g_gl_lock_visits>1)
    return;

  if (USE_GL_LOCK)
    mutex.lock();

#if 0
  if (g_safe_mode)
    GL_draw_lock(); //something may deadlock
#endif
}

void GL_unlock(void){
  R_ASSERT_RETURN_IF_FALSE(g_gl_lock_visits>0);

  g_gl_lock_visits--;
  if (g_gl_lock_visits>0)
    return;

  return;

#if 0
  if (g_safe_mode)
    GL_draw_unlock();
#endif

  if (USE_GL_LOCK)
    mutex.unlock();
}

bool GL_maybeLock(void){
  
  if (g_gl_lock_visits>1)
    return false;

  GL_lock();
  return true;
}

void GL_draw_lock(void){
  draw_mutex.lock();
}

void GL_draw_unlock(void){
  draw_mutex.unlock();
}


static bool g_gl_widget_started = false;

static bool g_should_do_modal_windows = false;

bool GL_should_do_modal_windows(void){
  return g_should_do_modal_windows;
}

static DEFINE_ATOMIC(bool, g_is_currently_pausing) = false;
static radium::Semaphore g_order_pause_gl_thread;
static radium::CondWait g_ack_pause_gl_thread;

static radium::Semaphore g_order_make_current;
static radium::CondWait g_ack_make_current;

static DEFINE_ATOMIC(int, g_curr_realline);

// TS (called from both main thread and opengl thread)
void GE_set_curr_realline(int curr_realline){
  ATOMIC_SET(g_curr_realline, curr_realline);
}

// OpenGL thread
static float GE_scroll_pos(SharedVariables *sv, double realline){
  double extra = sv->top_realline - sv->curr_realline;
  return
    (   (realline+extra) * sv->fontheight  );
}



extern PlayerClass *pc;
extern struct Root *root;

extern int scrolls_per_second;
extern int default_scrolls_per_second;

TimeEstimator time_estimator;


// OpenGL thread
static double get_realline_stime(SharedVariables *sv, int realline){  
  if(realline==sv->num_reallines)
    return sv->block_duration;
  else
    return Place2STime_from_times(sv->times, &sv->realline_places[realline]);
}


// OpenGL thread
static bool need_to_reset_timing(SharedVariables *sv, double stime, int last_used_i_realline, const struct Blocks *last_used_block, double last_used_stime, double blocktime){
  if (stime < 0){
    fprintf(stderr,"Error: stime: %f, pc->blocktime: %f",stime,blocktime);
    #if 0
      #if !defined(RELEASE)
        abort();
      #endif
    #endif
    return true;
  }

  if (last_used_block != sv->block)    
    return true;
  
  if(last_used_i_realline>=sv->num_reallines) // First check that i_realline is within the range of the block. (block might have changed number of lines)
    return true;
    
  // TODO: Make the "last_stime < stime"-check configurable.
  if (stime < last_used_stime)
    return true;
  
  if(stime < get_realline_stime(sv, last_used_i_realline)) // Time is now before the line we were at when we left last time. Start searching from 0 again. (Not sure if is correct. It might be last_used_i_realline+1 instead)
    return true;

  return false;
}


// OpenGL thread
static double find_current_realline_while_playing(SharedVariables *sv, double blocktime){

  double time_in_ms = blocktime * 1000.0 / (double)pc->pfreq; // I'm not entirely sure reading pc->start_time_f instead of pc->start_time is unproblematic.
  double stime      = time_estimator.get(time_in_ms, sv->reltempo) * (double)pc->pfreq / 1000.0; // Could this value be slightly off because we just changed block, and because of that we skipped a few calles to time_estimator.get ? (it shouldn't matter though, timing is resetted when that happens. 'time_in_ms' should always be valid)

  //stime      = time_in_ms* (double)pc->pfreq / 1000.0;

  static double last_stime = stime;
    
    
  //Strictly speaking, we need to atomically get current block + pc->blocktime. But it is uncertain how important this is is.
  // Maybe store blocktime in the block itself?
  
  static int i_realline = 0; // Note that this one is static. The reason is that we are usually placed on the same realline as last time, so we remember last position and start searching from there.
  static const struct Blocks *block = NULL; // Remember block used last time. If we are not on the same block now, we can't use the i_realline value used last time.

  R_ASSERT(i_realline>=0);
  
  //                                    Common situation. We are usually on the same line as the last visit,
  if (i_realline > 0) i_realline--; //  but we need to go one step back to reload prev_line_stime.
  //                                    (storing last used 'stime1' and/or 'stime2' would be an optimization which would make my head hurt and make no difference in cpu usage)

  if (need_to_reset_timing(sv, stime, i_realline, block, last_stime, blocktime)) {
    i_realline = 0;
    block = sv->block;
    time_estimator.set_time(time_in_ms);
    stime = time_in_ms * (double)pc->pfreq / 1000.0; // Convert the current block time into number of frames.
  }

  last_stime = stime;
  
  double stime2 = get_realline_stime(sv, i_realline);
  
  while(true){

    double stime1 = stime2;
    for(;;){ // This for loop is here to handle a very special situation where we play so fast that stime1==stime2. In normal songs, this should not happen.
      stime2 = get_realline_stime(sv, i_realline+1);

#if 0
      if (stime1==stime2){ // Could probably happen if playing really fast... Not sure. (yes, it happens if playing really fast)
#if !defined(RELEASE)
        /abort();
#endif
        return i_realline;
      }
#endif
      
      if (i_realline==sv->num_reallines)
        return sv->num_reallines;
      if (stime1==stime2)
        i_realline++;
      else
        break;
    }
      
    if (stime >= stime1 && stime <= stime2){
      return scale_double(stime,
                          stime1, stime2,
                          i_realline, i_realline+1
                          );
    }

    i_realline++;

    if (i_realline==sv->num_reallines)
      break;
  }

  return sv->num_reallines;
}




#if 0
static void UpdateReallineByLines(struct Tracker_Windows *window, struct WBlocks *wblock, double d_till_curr_realline){
  static STime last_time = 0;
  static struct WBlocks *last_wblock = NULL;

  int till_curr_realline = (int)d_till_curr_realline;

  if(scrolls_per_second==-1)
    scrolls_per_second = SETTINGS_read_int("scrolls_per_second", default_scrolls_per_second);

  bool do_scrolling = false;
  
  if(wblock != last_wblock)
    do_scrolling = true;
  
  else if (last_time > pc->therealtime)
    do_scrolling = true;
  
  else if(till_curr_realline < wblock->curr_realline)
    do_scrolling = true;
  
  else if(till_curr_realline > wblock->curr_realline){
    STime from_time = (STime) ((double)Place2STime(wblock->block, &wblock->reallines[wblock->curr_realline]->l.p) / wblock->block->reltempo);
    STime to_time   = (STime) ((double)Place2STime(wblock->block, &wblock->reallines[till_curr_realline]->l.p) / wblock->block->reltempo);
    
    STime time = to_time - from_time;
    
    STime time_necessary_to_scroll = pc->pfreq / scrolls_per_second;
    
    if(time>=time_necessary_to_scroll)
      do_scrolling = true;
  }
  
  if(do_scrolling==true) {
    ScrollEditorToRealLine(window,wblock,till_curr_realline);
    last_time = pc->therealtime;
    last_wblock = wblock;
  }
}
#endif


// Main thread
static Tracker_Windows *get_window(void){
  return root->song->tracker_windows;
}

// Main thread
static EditorWidget *get_editorwidget(void){
  return (EditorWidget *)get_window()->os_visual.widget;
}


volatile float g_scroll_pos = 0.0f;


// Main thread
static QMouseEvent translate_qmouseevent(const QMouseEvent *qmouseevent){
  const QPoint p = qmouseevent->pos();

  return QMouseEvent(qmouseevent->type(),
                     QPoint(p.x(), p.y() + root->song->tracker_windows->wblock->t.y1),
                     qmouseevent->globalPos(),
                     qmouseevent->button(),
                     qmouseevent->buttons(),
                     Qt::NoModifier
                     );
}

class MyQtThreadedWidget
#if USE_QT5
  : public vlQt5::Qt5ThreadedWidget
#else
  : public vlQt4::Qt4ThreadedWidget
#endif
  , public vl::UIEventListener
{
    
public:

  int current_width;
  int current_height;
  
  int new_width;
  int new_height;
    
  vl::ref<vl::Rendering> _rendering;
  vl::ref<vl::Transform> _scroll_transform;
  vl::ref<vl::Transform> _linenumbers_transform;
  vl::ref<vl::Transform> _scrollbar_transform;
  vl::ref<vl::Transform> _playcursor_transform;
  vl::ref<vl::SceneManagerActorTree> _sceneManager;

  vl::ref<vl::VectorGraphics> vg;
  vl::ref<vl::SceneManagerVectorGraphics> vgscene;

  PaintingData *painting_data;

  DEFINE_ATOMIC(bool, is_training_vblank_estimator);
  DEFINE_ATOMIC(double, override_vblank_value);
  bool has_overridden_vblank_value;

  float last_scroll_pos;
  double last_current_realline_while_playing;
  int last_curr_realline;
  
  bool sleep_when_not_painting;

  DEFINE_ATOMIC(bool, _main_window_is_exposed);

  // Main thread
  MyQtThreadedWidget(vl::OpenGLContextFormat vlFormat, QWidget *parent=0)
#if USE_QT5
    : Qt5ThreadedWidget(vlFormat, parent)
#else
    : Qt4ThreadedWidget(vlFormat, parent)
#endif
    , current_width(-1)
    , current_height(-1)
    , new_width(500)
    , new_height(500)
    , painting_data(NULL)
    , has_overridden_vblank_value(false)
    , last_scroll_pos(-1.0f)
    , last_current_realline_while_playing(-1.0f)
    , last_curr_realline(-1)
    , sleep_when_not_painting(true) //SETTINGS_read_bool("opengl_sleep_when_not_painting", false))
  {
    ATOMIC_SET(_main_window_is_exposed, false);
    ATOMIC_DOUBLE_SET(override_vblank_value, -1.0);
    ATOMIC_SET(is_training_vblank_estimator, false);
    setMouseTracking(true);
    //setAttribute(Qt::WA_PaintOnScreen);
  }

  // Main thread
  ~MyQtThreadedWidget(){
    fprintf(stderr,"Exit myqtthreaderwidget\n");
  }

  // OpenGL thread
  virtual void init_vl(vl::OpenGLContext *glContext) {

    printf("init_vl\n");

    _rendering = new vl::Rendering;
    _rendering->renderer()->setFramebuffer(glContext->framebuffer() );
    //_rendering->camera()->viewport()->setClearColor( vl::green );

    _scroll_transform = new vl::Transform;
    _rendering->transform()->addChild(_scroll_transform.get());

    _linenumbers_transform = new vl::Transform;
    _rendering->transform()->addChild(_linenumbers_transform.get());

    _scrollbar_transform = new vl::Transform;
    _rendering->transform()->addChild(_scrollbar_transform.get());

    _playcursor_transform = new vl::Transform;
    _rendering->transform()->addChild(_playcursor_transform.get());


    // installs a SceneManagerActorTree as the default scene manager
    
    _sceneManager = new vl::SceneManagerActorTree;
    _rendering->sceneManagers()->push_back(_sceneManager.get());

#if 0
    vl::mat4 mat = vl::mat4::getTranslation(0,0,0);
    mat = vl::mat4::getTranslation(0,0,0) * mat;
    _rendering->camera()->setViewMatrix( mat );
#endif

    glContext->initGLContext();
    glContext->addEventListener(this);


    //printf("FLAGS: %d\n",QGLFormat::openGLVersionFlags());
    //gets(NULL);

#if FOR_LINUX
    if(0)set_realtime(SCHED_FIFO,1); // TODO: Add priority inheritance to all locks. Setting the OpenGL thread to a higher priority might not make a difference because of priority inversion.
#endif
  }

  /** Event generated when the bound OpenGLContext bocomes initialized or when the event listener is bound to an initialized OpenGLContext. */
  // OpenGL thread
  virtual void initEvent() {
    printf("initEvent\n");
    
    _rendering->sceneManagers()->clear();
    
    vg = new vl::VectorGraphics;
    
    vgscene = new vl::SceneManagerVectorGraphics;
    vgscene->vectorGraphicObjects()->push_back(vg.get());
    _rendering->sceneManagers()->push_back(vgscene.get());
  }

private:
  // OpenGL thread
  bool canDraw(){
    if (vg.get()==NULL)
      return false;
    else
      return true;
  }
public:

  // Main thread
  virtual void mouseReleaseEvent( QMouseEvent *qmouseevent){
    QMouseEvent event = translate_qmouseevent(qmouseevent);
    get_editorwidget()->mouseReleaseEvent(&event);
    //GL_create(get_window(), get_window()->wblock);
  }

  // Main thread
  virtual void mousePressEvent( QMouseEvent *qmouseevent){
    QMouseEvent event = translate_qmouseevent(qmouseevent);
    get_editorwidget()->mousePressEvent(&event);
    //GL_create(get_window(), get_window()->wblock);
  }

  // Main thread
  virtual void mouseMoveEvent( QMouseEvent *qmouseevent){
    QMouseEvent event = translate_qmouseevent(qmouseevent);
    get_editorwidget()->mouseMoveEvent(&event);
    //GL_create(get_window(), get_window()->wblock);
  }

  virtual void wheelEvent(QWheelEvent *qwheelevent){
    //QMouseEvent event = translate_qmouseevent(qmouseevent);
    get_editorwidget()->wheelEvent(qwheelevent);
    //GL_create(get_window(), get_window()->wblock);
  }
    
  virtual void leaveEvent(QEvent * event){
    fprintf(stderr, "LEAVING\n");
    setNormalMousePointer(-1);
  }
  
  /** Event generated right before the bound OpenGLContext is destroyed. */
  virtual void destroyEvent() {
    fprintf(stderr,"destroyEvent\n");
  }

  virtual void paintEvent( QPaintEvent *e ){
    //fprintf(stderr,"GLWindow paintEvent\n");
    GL_create(get_window(), get_window()->wblock);
  }

private:

  // OpenGL thread
  bool draw(bool force_repaint = false){
    static bool needs_repaint2 = false; // TODO: Clean up this.
    
    bool needs_repaint;

    int player_id = ATOMIC_GET(pc->play_id);
    bool is_playing = ATOMIC_GET(pc->player_state)==PLAYER_STATE_PLAYING;
    
    painting_data = GE_get_painting_data(painting_data, &needs_repaint);
    //printf("needs_repaint: %d, painting_data: %p\n",(int)needs_repaint,painting_data);
    
    if (painting_data==NULL){
      return false;
    }
    
    
    needs_repaint2 = needs_repaint2 || needs_repaint;
    
    SharedVariables *sv = GE_get_shared_variables(painting_data);
    
    //if (needs_repaint)
    //  printf("   Needs repaint %d\n",sv->block->l.num);
    
    double blocktime = 0.0;
    
    if (is_playing){
      
      const struct Blocks *block = (const struct Blocks*)atomic_pointer_read((void**)&pc->block);
#if 1    
      if (sv->block!=block) { // Check that our blocktime belongs to the block that is rendered.
        #if 1
          //printf("Waiting...\n");
          _rendering->render();
          return true;
        #else
          return false; // Returning false uses more CPU on Intel gfx, and generally seems to may cause jumpy graphics, but here we are just waiting for the block to be rendered.
        #endif
      }
#endif
      blocktime = ATOMIC_DOUBLE_GET(block->player_time);

      R_ASSERT_NON_RELEASE(blocktime==-100 || blocktime>=0.0);

      //printf("blocktime: %f\n",blocktime);
      
      if (is_playing){
        if (blocktime < -10.0) {  // I.e. we just switched block, but the blocktime has not been calculated yet.
          //return false;
          _rendering->render();
          return true;      
        }
      }

    }
    
    
    if (force_repaint || needs_repaint || current_height != new_height || current_width != new_width) {

      if (current_height != new_height || current_width != new_width){

        printf("Changing height\n");
        
        //_rendering->sceneManagers()->clear();
        _rendering->camera()->viewport()->setWidth(new_width);
        _rendering->camera()->viewport()->setHeight(new_height);
        _rendering->camera()->setProjectionOrtho(-0.5f);
   
        GE_set_height(new_height);
        //create_block(_rendering->camera()->viewport()->width(), _rendering->camera()->viewport()->height());
        
        //initEvent();
        
        current_height = new_height;
        current_width = new_width;

      } else {
        vg->clear();
        //printf("   Clearing\n");
      }
      
      //GE_set_curr_realline(sv->curr_realline);

      GE_draw_vl(painting_data, _rendering->camera()->viewport(), vg, _scroll_transform, _linenumbers_transform, _scrollbar_transform, _playcursor_transform);

      needs_repaint2 = false;
    }

    double current_realline_while_playing;
    
    if (is_playing) {
      
      
      current_realline_while_playing = find_current_realline_while_playing(sv, blocktime);
      
    } else {
      
      current_realline_while_playing = 0.0;
      
    }
    
    R_ASSERT_NON_RELEASE(current_realline_while_playing >= 0);
    
    double till_realline;
    if (ATOMIC_GET_RELAXED(root->play_cursor_onoff))
      till_realline = ATOMIC_GET(g_curr_realline);
    else if (is_playing)
      till_realline = current_realline_while_playing;
    else
      till_realline = ATOMIC_GET(g_curr_realline);

    
    float scroll_pos = GE_scroll_pos(sv, till_realline);

    
    if (player_id != ATOMIC_GET(pc->play_id)) // In the very weird and unlikely case that the player has stopped and started since the top of this function (the computer is really struggling), we return false
      return false;

    if (!is_playing && scroll_pos == last_scroll_pos && !needs_repaint)
      return false; // Use less cpu.

    //printf("scrolling\n");

    // scroll
    {
      vl::mat4 mat = vl::mat4::getRotation(0.0f, 0, 0, 1);
      mat.translate(0,scroll_pos,0);
      _scroll_transform->setLocalAndWorldMatrix(mat);
    }
    
    // linenumbers
    {
      vl::mat4 mat = vl::mat4::getRotation(0.0f, 0, 0, 1);
      mat.translate(0,scroll_pos,0);
      _linenumbers_transform->setLocalAndWorldMatrix(mat);
    }
    
    // scrollbar
    {
      vl::mat4 mat = vl::mat4::getRotation(0.0f, 0, 0, 1);
      float scrollbarpos = scale(till_realline,
                                 0, sv->num_reallines - (is_playing?0:1),
                                 -2, -(sv->scrollbar_height - sv->scrollbar_scroller_height - 1)
                                 );
      //printf("bar_length: %f, till_realline: %f. scrollpos: %f, pos: %f, max: %d\n",bar_length,till_realline, scrollpos, pos, window->leftslider.x2);
      mat.translate(0,scrollbarpos,0);
      _scrollbar_transform->setLocalAndWorldMatrix(mat);
    }
    
    // playcursor (i.e. the red horizontal line which is moving down when playing and the "PC" button is pressed)
    {
      vl::mat4 mat = vl::mat4::getRotation(0.0f, 0, 0, 1);

      float playcursorpos = scroll_pos - GE_scroll_pos(sv, current_realline_while_playing);

      mat.translate(0,playcursorpos,0);
      _playcursor_transform->setLocalAndWorldMatrix(mat);
    }

    GE_update_triangle_gradient_shaders(painting_data, scroll_pos);
    
    _rendering->render();
    safe_volatile_float_write(&g_scroll_pos, scroll_pos);
    
    last_scroll_pos = scroll_pos;
    last_current_realline_while_playing = current_realline_while_playing;
    last_curr_realline = ATOMIC_GET(g_curr_realline);

    return true;
  }

  // OpenGL thread
  void swap(void){
    if (USE_GL_LOCK)
      radium::ScopedMutex lock(&mutex);

    // Swap to the newly rendered buffer
    if ( openglContext()->hasDoubleBuffer())
      openglContext()->swapBuffers();
  }

public:

  /** Event generated when the bound OpenGLContext does not have any other message to process 
      and OpenGLContext::continuousUpdate() is set to \p true or somebody calls OpenGLContext::update(). */
  // OpenGL thread
  virtual void updateEvent() {

    bool handle_current = true; // I don't know if there's any point setting this to true.

    if (handle_current)
      QGLWidget::makeCurrent();
      
    if (g_order_pause_gl_thread.tryWait()) {
      g_ack_pause_gl_thread.notify_one();
      OS_WaitAtLeast(200);
    }
    
    if (g_order_make_current.tryWait()) {
      if (!handle_current)
        QGLWidget::makeCurrent();
      g_ack_make_current.notify_one();
    }

    ATOMIC_SET(g_is_currently_pausing, false);
    
    if (ATOMIC_GET(GE_version_string)==NULL) {
      ATOMIC_SET(GE_vendor_string, V_strdup((const char*)glGetString(GL_VENDOR)));
      ATOMIC_SET(GE_renderer_string, V_strdup((const char*)glGetString(GL_RENDERER)));
      ATOMIC_SET(GE_version_string, V_strdup((const char*)glGetString(GL_VERSION)));
      printf("vendor: %s, renderer: %s, version: %s \n",(const char*)ATOMIC_GET(GE_vendor_string),(const char*)ATOMIC_GET(GE_renderer_string),(const char*)ATOMIC_GET(GE_version_string));

      ATOMIC_SET(GE_opengl_version_flags, QGLFormat::openGLVersionFlags());
      //abort();
    }


    ATOMIC_SET(g_has_updated_at_least_once, true);

#if USE_QT5
    if (ATOMIC_GET(_main_window_is_exposed)==false){
      OS_WaitAtLeast(200);
      if (handle_current)
        QGLWidget::doneCurrent();
      return;
    }
#endif

    
    double overridden_vblank_value = ATOMIC_DOUBLE_GET(override_vblank_value);
    
    if (has_overridden_vblank_value==false && overridden_vblank_value > 0.0) {

      time_estimator.set_vblank(overridden_vblank_value);
      ATOMIC_SET(is_training_vblank_estimator, false);
      has_overridden_vblank_value = true;

    } else {

      if(ATOMIC_GET(is_training_vblank_estimator))
        ATOMIC_SET(is_training_vblank_estimator, time_estimator.train());

    }
    

    // This is the only place the opengl thread waits. When swap()/usleep() returns, updateEvent is called again immediately.

    {
      if (ATOMIC_GET(is_training_vblank_estimator)==true) {

        swap();

      } else {

        bool must_swap;

        {
          radium::ScopedMutex lock(&draw_mutex);

          if (canDraw()) {
            must_swap = draw();
          } else {
            must_swap = true;
          }
        }

        if (must_swap)
          swap();
        
        else if (g_safe_mode || !sleep_when_not_painting) // probably doesn't make any sense setting sleep_when_not_painting to false. Besides, setting it to false may cause 100% CPU usage (intel gfx) or very long calls to GL_lock() (nvidia gfx).
          swap();

        else
          //usleep(20); // Don't want to buzy-loop
          usleep(1000 * time_estimator.get_vblank());
                 
        if (g_safe_mode)
          GL_unlock();
      }
    }

    ATOMIC_SET(g_has_updated_at_least_once, true);

    if (handle_current)
      QGLWidget::doneCurrent();
  }

  // Main thread
  void set_vblank(double value){
    ATOMIC_DOUBLE_SET(override_vblank_value, value);
  }

  // Necessary to avoid error with clang++.
  virtual void resizeEvent(QResizeEvent *qresizeevent) {
#if USE_QT5
    vlQt5::Qt5ThreadedWidget::resizeEvent(qresizeevent);
#else
    vlQt4::Qt4ThreadedWidget::resizeEvent(qresizeevent);
#endif
  }

  /** Event generated when the bound OpenGLContext is resized. */
  // OpenGL thread
  virtual void resizeEvent(int w, int h) {
    printf("resisizing %d %d\n",w,h);

    new_width = w;
    new_height = h;


#if 1

    /*
    _rendering->sceneManagers()->clear();

    _rendering->camera()->viewport()->setWidth(w);
    _rendering->camera()->viewport()->setHeight(h);
    _rendering->camera()->setProjectionOrtho(-0.5f);
    */
    
    initEvent();
    
    ////create_block(_rendering->camera()->viewport()->width(), _rendering->camera()->viewport()->height());
#endif

    GE_set_height(h);

    //for(int i = 0; i < 100 ; i ++){
    GFX_ScheduleEditorRedraw();
    //draw(true);
    updateEvent();
    //usleep(5000);
    //    }
  }
  // The rest of the methods in this class are virtual methods required by the vl::UIEventListener class. Not used.

  /** Event generated whenever setEnabled() is called. */
  virtual void enableEvent(bool enabled){
    printf("enableEvent %d\n",(int)enabled);
  }

  /** Event generated whenever a listener is bound to an OpenGLContext context. */
  virtual void addedListenerEvent(vl::OpenGLContext*) {
    printf("addedListenerEvent\n");
  }

  /** Event generated whenever a listener is unbound from an OpenGLContext context. */
  virtual void removedListenerEvent(vl::OpenGLContext*) {
    printf("removedListenerEvent\n");
  }
  
  /** Event generated when the mouse moves. */
  virtual void mouseMoveEvent(int x, int y) {
    printf("mouseMove %d %d\n",x,y);
    //_rendering->sceneManagers()->clear();
    //create_block();
    //initEvent();
  }
  
  /** Event generated when one of the mouse buttons is released. */
  virtual void mouseUpEvent(vl::EMouseButton button, int x, int y) {
    printf("mouseMove %d %d\n",x,y);
    //_rendering->sceneManagers()->clear();
    //create_block(_rendering->camera()->viewport()->width(), _rendering->camera()->viewport()->height());
    //initEvent();
  }
  
  /** Event generated when one of the mouse buttons is pressed. */
  virtual void mouseDownEvent(vl::EMouseButton button, int x, int y) {
    printf("mouseMove %d %d\n",x,y);
    //_rendering->sceneManagers()->clear();
    //create_block();
    //initEvent(); 
  }
  
  /** Event generated when the mouse wheel rotated. */
  virtual void mouseWheelEvent(int n) {
    printf("mouseWheel %d\n",n);
  }

  // Necessary to avoid error with clang++.
  virtual void keyPressEvent(QKeyEvent *event){
#if USE_QT5
    vlQt5::Qt5ThreadedWidget::keyPressEvent(event);
#else
    vlQt4::Qt4ThreadedWidget::keyPressEvent(event);
#endif
  }

  /** Event generated when a key is pressed. */
  virtual void keyPressEvent(unsigned short unicode_ch, vl::EKey key) {
    printf("key pressed\n");

    //_rendering->sceneManagers()->clear();
    //create_block();
    //initEvent();
  }

  // Necessary to avoid error with clang++.
  virtual void keyReleaseEvent(QKeyEvent *event){
#if USE_QT5
    vlQt5::Qt5ThreadedWidget::keyReleaseEvent(event);
#else
    vlQt4::Qt4ThreadedWidget::keyReleaseEvent(event);
#endif
  }


  /** Event generated when a key is released. */
  virtual void keyReleaseEvent(unsigned short unicode_ch, vl::EKey key) {
  }
  
  /** Event generated when one or more files are dropped on the bound OpenGLContext's area. */
  virtual void fileDroppedEvent(const std::vector<vl::String>& files) {
  }
  
  /** Event generated when the bound OpenGLContext is shown or hidden. */
  virtual void visibilityEvent(bool visible) {
    printf("visibilityEvent %d\n",(int)visible);
  }
};


static vl::ref<MyQtThreadedWidget> widget;

static bool use_estimated_vblank_questionmark(){
  return SETTINGS_read_bool("use_estimated_vblank", false);
}

static void store_use_estimated_vblank(bool value){
  return SETTINGS_write_bool("use_estimated_vblank", value);
}

static bool have_earlier_estimated_value(){
  return SETTINGS_read_double("vblank", -1.0) > 0.0;
}

bool GL_notify_that_main_window_is_exposed(void){
  static bool gotit=false;

#if USE_QT5
  if (gotit==false){
    QWindow *window = widget->windowHandle();
    if (window != NULL){
      if (window->isExposed()) {
        ATOMIC_SET(widget->_main_window_is_exposed, true);

        QWindow *qwindow = widget->windowHandle();
        if (qwindow==NULL){
          abort();
        }
        QScreen *qscreen = qwindow->screen();
        if (qscreen==NULL){
          abort();
        }
        widget->set_vblank(1000.0 / qscreen->refreshRate());
        //GFX_Message(NULL, "refresh rate: %d\n", qscreen->refreshRate());

        gotit = true;
      }
    }
  }else{
        QWindow *qwindow = widget->windowHandle();
        if (qwindow==NULL){
          abort();
        }
        QScreen *qscreen = qwindow->screen();
        if (qscreen==NULL){
          abort();
        }
        widget->set_vblank(1000.0 /  qscreen->refreshRate());
        //printf("refresh rate: %f\n", qscreen->refreshRate());
  }

#else
  ATOMIC_SET(widget->_main_window_is_exposed, true);
  gotit = true;
#endif
  
  return gotit;
}
            
void GL_pause_gl_thread_a_short_while(void){

  if (!USE_GL_LOCK)
    return;
  
  if (GL_get_pause_rendering_on_off()==false)
    return;
  
  if (g_gl_widget_started == false) // deadlock without this check.
    return;

  if (ATOMIC_GET(g_is_currently_pausing))
    return;
    
  R_ASSERT_RETURN_IF_FALSE(g_gl_lock_visits>=1);
  if (g_gl_lock_visits>=2) // If this happens we are probably called from inside a widget/dialog->exec() call. Better not wake up the gl thread.
    return;

  ATOMIC_SET(g_is_currently_pausing, true);
  
  g_order_pause_gl_thread.signal();

  if (g_ack_pause_gl_thread.wait(&mutex, 4000)==false){  // Have a timeout in case the opengl thread is stuck
#ifdef RELEASE
    printf("warning: g_ack_pause_gl_thread timed out\n");
#else
    GFX_Message(NULL, "Debug mode warning: g_ack_pause_gl_thread timed out\n");
#endif
  }
}


void GL_EnsureMakeCurrentIsCalled(void){
  //printf("  Ensure make current\n");
  
  if (!USE_GL_LOCK)
    return;

  R_ASSERT_RETURN_IF_FALSE(g_gl_lock_visits>=1); 
  if (g_gl_lock_visits>=2) // If this happens we are probably called from inside a widget/dialog->exec() call. Better not wake up the gl thread.
    return;

  g_order_make_current.signal();

  if (g_ack_make_current.wait(&mutex, 4000)==false){  // Have a timeout in case the opengl thread is stuck
#ifdef RELEASE
    printf("warning: g_ack_make_current timed out\n");
#else
    GFX_Message(NULL, "warning: g_ack_make_current timed out\n");
#endif
  }
}

double GL_get_estimated_vblank(){
  return 1000.0 / SETTINGS_read_double("vblank", 60.0);
}

static void store_estimated_value(double period){
  printf("***************** Storing %f\n",1000.0 / period);
  SETTINGS_write_double("vblank", 1000.0 / period);
}

void GL_erase_estimated_vblank(void){
  SETTINGS_write_double("vblank", -1.0);
  store_use_estimated_vblank(false);
  GFX_Message(NULL, "Stored vblank value erased. Restart Radium to estimate a new vblank value.");
}

void GL_set_vsync(bool onoff){
  SETTINGS_write_bool("vsync", onoff);
}

bool GL_get_vsync(void){
  return SETTINGS_read_bool("vsync", true);
}

void GL_set_multisample(int size){
  SETTINGS_write_int("multisample", size);
}

int GL_get_multisample(void){
  return R_BOUNDARIES(1, SETTINGS_read_int("multisample", 4), 32);
}

void GL_set_safe_mode(bool onoff){
  printf("setting safe mode to %d\n",onoff);
  SETTINGS_write_bool("safe_mode", onoff);
}

bool GL_get_safe_mode(void){
  return SETTINGS_read_bool("safe_mode", false);
}

static bool g_pause_rendering_on_off = false;

static void init_g_pause_rendering_on_off(void){
  g_pause_rendering_on_off = SETTINGS_read_bool("pause_rendering", false);
}

void GL_set_pause_rendering_on_off(bool onoff){
  SETTINGS_write_bool("pause_rendering", onoff);
  g_pause_rendering_on_off = onoff;
}

bool GL_get_pause_rendering_on_off(void){
  return g_pause_rendering_on_off;
}

#if defined(FOR_MACOSX)
extern "C" void cocoa_set_best_resolution(void *view);
#endif

static bool is_opengl_certainly_too_old_questionmark(void){
  if (! ((QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_2_0) == QGLFormat::OpenGL_Version_2_0))
    return true;
  else
    return false;
}

static bool is_opengl_version_recent_enough_questionmark(void){
  if ((QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_3_0) == QGLFormat::OpenGL_Version_3_0)
    return true;
  else
    return false;
}

static void setup_widget(QWidget *parent){
  vl::VisualizationLibrary::init();

  vl::OpenGLContextFormat vlFormat;
  vlFormat.setDoubleBuffer(true);
  //vlFormat.setDoubleBuffer(false);
  vlFormat.setRGBABits( 8,8,8,8 );
  vlFormat.setDepthBufferBits(24);
  vlFormat.setFullscreen(false);
  vlFormat.setMultisampleSamples(GL_get_multisample()); // multisampling 32 seems to make text more blurry. 16 sometimes makes program crawl in full screen (not 32 though).
  //vlFormat.setMultisampleSamples(8); // multisampling 32 seems to make text more blurry. 16 sometimes makes program crawl in full screen (not 32 though).
  //vlFormat.setMultisampleSamples(32); // multisampling 32 seems to make text more blurry. 16 sometimes makes program crawl in full screen (not 32 though).
  vlFormat.setMultisample(GL_get_multisample()>1);
  //vlFormat.setMultisample(false);
  vlFormat.setVSync(GL_get_vsync());
  //vlFormat.setVSync(false);

  widget = new MyQtThreadedWidget(vlFormat, parent);
  widget->resize(1000,1000);
  widget->show();

  widget->setAutomaticDelete(false);  // dont want auto-desctruction at program exit.
}

QWidget *GL_create_widget(QWidget *parent){

#if defined(FOR_MACOSX)
  // doesn't work.
  //cocoa_set_best_resolution(NULL);//(void*)widget->winId());
#endif

  g_safe_mode = GL_get_safe_mode();
  init_g_pause_rendering_on_off();
    
  if (QGLFormat::hasOpenGL()==false) {
    GFX_Message(NULL,"OpenGL not found");
    exit(-1);
    return NULL;
  }

  if (is_opengl_certainly_too_old_questionmark()){
    GFX_Message(NULL,
                "Your version of OpenGL is too old. Radium can not run.\n"
                "\n"
                "This is usually caused by lacking a specific graphics card driver, so that the fallback software OpenGL driver is used instead.\n"
                "\n"
                "To solve this problem, you might want to try updating your graphics card driver."
                );
    exit(-1);
    return NULL;
  }
  
  if (!is_opengl_version_recent_enough_questionmark()){
    vector_t v = {};
    VECTOR_push_back(&v,"Try to run anywyay"); // (but please don't send a bug report if Radium crashes)");
    VECTOR_push_back(&v,"Quit");

    int ret = GFX_Message(&v,
                          "Your version of OpenGL is too old.\n"
                          "\n"
                          "This is usually caused by lacking a specific graphics card driver, so that the fallback software OpenGL driver is used instead.\n"
                          "\n"
                          "To solve this problem, you might want to try updating your graphics card driver."
                          );
    if (ret==1)
      return NULL;

    usleep(10*1000);
    //QThread::currentThread()->wait(1000*10);
  }

  setup_widget(parent);
  widget->set_vblank(GL_get_estimated_vblank());
  
  
  while(ATOMIC_GET(GE_vendor_string)==NULL || ATOMIC_GET(GE_renderer_string)==NULL || ATOMIC_GET(GE_version_string)==NULL)
    usleep(5*1000);
  
  {
    QString s_vendor((const char*)ATOMIC_GET(GE_vendor_string));
    QString s_renderer((const char*)ATOMIC_GET(GE_renderer_string));
    QString s_version((const char*)ATOMIC_GET(GE_version_string));
    qDebug() << "___ GE_version_string: " << s_version;
    printf("vendor: %s, renderer: %s, version: %s \n",(const char*)ATOMIC_GET(GE_vendor_string),(const char*)ATOMIC_GET(GE_renderer_string),(const char*)ATOMIC_GET(GE_version_string));
    //getchar();

    
        

    bool show_mesa_warning = true;
    
#ifdef FOR_LINUX
    if (!s_version.contains("nvidia", Qt::CaseInsensitive) && SETTINGS_read_bool("show_not_nvidia_warning", true)) {
      GL_set_pause_rendering_on_off(true);
        
      GFX_Message(NULL,
                  "It seems like you are not using an NVIDIA card or the nvidia gfx driver."
                  "<p>"
                  "In order to avoid seemingly random crashes, the option \"Briefly pause graphics rendering when opening windows\" has been enabled "
                  "under the \"Windows\" tab in the \"Preferences\" window."
                  "<p>"
                  "In case you are using a new GFX driver, you might want to uncheck this option."
                  "<p>"
                  "You will not see this warning again."
                  );
      
      SETTINGS_write_bool("show_not_nvidia_warning", false);
    }
    
    if (s_vendor.contains("ATI"))
      if (SETTINGS_read_bool("show_catalyst_gfx_message_during_startup", true)) {
        vector_t v = {};
        VECTOR_push_back(&v,"Ok");
        VECTOR_push_back(&v,"Don't show this message again");
        
        int result = GFX_Message(&v,
                                 "AMD Catalyst OpenGL driver on Linux detected."
                                 "<p>"
                                 "For best performance, vsync should be turned off. You do this by going to the \"Edit\" menu and select \"Preferences\"."
                                 "<p>"
                                 "In addition, \"Tear Free Desktop\" should be turned on in the catalyst configuration program (run the \"amdcccle\" program)."
                                 "<p>"
                                 "If you notice choppy graphics, it might help to overclock the graphics card: <A href=\"http://www.overclock.net/t/517861/how-to-overclocking-ati-cards-in-linux\">Instructions for overclocking can be found here</A>. If Radium crashes when you show or hide windows, it might help to upgrade driver to the latest version, or (better) use the Gallium AMD OpenGL driver instead, or (even better) get an Nvidia card."
                                 );
        if (result==1)
          SETTINGS_write_bool("show_catalyst_gfx_message_during_startup", false);

        //g_should_do_modal_windows = true;
        g_should_do_modal_windows = false;
      }

    
    if (s_renderer.contains("Gallium") && s_renderer.contains("AMD")) {
      if (SETTINGS_read_bool("show_gallium_gfx_message_during_startup", true)) {
        vector_t v = {};
        VECTOR_push_back(&v,"Ok");
        VECTOR_push_back(&v,"Don't show this message again");
        
        int result = GFX_Message(&v,
                                 "Gallium AMD OpenGL driver detected."
                                 "<p>"
                                 "For best performance with this driver, vsync should be turned off. You do this by going to the \"Edit\" menu and select \"Preferences\"."
                                 "<p>"
                                 "If you notice choppy graphics, it might help to install the binary driver instead. However for newer versions of MESA, the performance of this driver seems just as good. It could be worth trying though, but the binary driver might be less stable.</A>."
                                 "<p>"
                                 "However, although the Gallium driver seems less unstable than the Catalyst driver, Radium still crashes sometimes using this driver (tested with the Gallium 0.4). The best option on Linux is to get an Nvidia card, and use the binary driver for that card. Newest Intel GFX drivers also seems stable."
                                 );
        if (result==1)
          SETTINGS_write_bool("show_gallium_gfx_message_during_startup", false);

      }
      
      show_mesa_warning = false;
    }
    
    if (s_vendor.contains("nouveau", Qt::CaseInsensitive)) {
      GFX_Message(NULL,
                  "Warning!"
                  "<p>"
                  "Nouveau OpenGL driver detected."
                  "<p>"
                  "The Nouveau driver currently performes worse than the Nvidia driver."
                  "<p>"
                  "The Nvidia driver can be installed to get faster / smoother graphics. It can be downloaded here: <a href=\"http://www.nvidia.com/object/unix.html\">http://www.nvidia.com/object/unix.html</a>"
                  );
      show_mesa_warning = false;
    }

    if (s_vendor.contains("Intel")) {
      if (SETTINGS_read_bool("show_intel_gfx_message2_during_startup", true)) {
        vector_t v = {};
        VECTOR_push_back(&v,"Ok");
        VECTOR_push_back(&v,"Don't show this message again");

        int result = GFX_Message(&v,
                                 "<strong>Intel OpenGL driver detected.</strong>"
                                 "<p>"
                                 "<ol>"
                                 "<li>For best performance, the Intel Xorg driver should be configured like this:"
                                 "<p>"
                                 "<pre>"
                                 "Section \"Device\"\n"
                                 "\n"
                                 "   Identifier  \"Intel Graphics\"\n"
                                 "   Driver      \"intel\"\n"
                                 "\n"
                                 "   Option \"AccelMethod\" \"sna\"\n"
                                 "   Option \"TearFree\" \"true\"\n"
                                 "\n"
                                 "EndSection\n"
                                 "</pre>"
                                 "<p>"
                                 "Your driver is likely to already be configured like this. But in "
                                 "case you see tearing or the scrolling is not silky smooth, you might want to check "
                                 "your X configuration. You might also want to download the latest version "
                                 "of the driver, which can be found here: <a href=\"https://01.org/linuxgraphics/\">https://01.org/linuxgraphics/</a>"
                                 "<p>"
                                 "<p>"
                                 "<li>In addition, vsync in Radium should be turned off. You do this by going to the \"Edit\" menu and select \"Preferences\"."
                                 "</ol>"
                                 );

        if (result==1)
          SETTINGS_write_bool("show_intel_gfx_message2_during_startup", false);
      }

      //g_should_do_modal_windows = true;
      g_should_do_modal_windows = false;
      show_mesa_warning = false;
    }
#endif

    
    if (s_version.contains("mesa", Qt::CaseInsensitive) && show_mesa_warning==true)
      GFX_Message(NULL,
                  "Warning!\n"
                  "MESA OpenGL driver detected.\n"
                  "\n"
                  "MESA OpenGL driver renders graphics in software, which is likely to be significantly slower than to render in hardware.\n"
                  "\n"
                  "In addition, the graphics tends to not look as good."
                  );

  }  

  while(ATOMIC_GET(g_has_updated_at_least_once)==false)
    usleep(5*1000);

  g_gl_widget_started = true;
  
  return widget.get();
}


void GL_stop_widget(QWidget *widget){
  MyQtThreadedWidget *mywidget = static_cast<MyQtThreadedWidget*>(widget);

  mywidget->stop();
  //delete mywidget;
}


static void show_message_box(QMessageBox *box){
  //box->setWindowFlags(Qt::WindowStaysOnTopHint);//Qt::WindowStaysOnTopHint|Qt::SplashScreen|Qt::Window | Qt::FramelessWindowHint|Qt::Popup);
  box->setWindowFlags(Qt::WindowStaysOnTopHint); //Qt::Popup);
  
    
  box->setText("Please wait, estimating vblank refresh rate. This takes 3 - 10 seconds");
  box->setInformativeText("!!! Don't move the mouse or press any key !!!");

  if(have_earlier_estimated_value()){

    QString message = QString("Don't estimate again. Use last estimated value instead (")+QString::number(1000.0/GL_get_estimated_vblank())+" Hz).";
#if 1
    box->addButton(message,QMessageBox::ApplyRole);
#else
    QAbstractButton *msgBox_useStoredValue = (QAbstractButton*)box->addButton(message,QMessageBox::ApplyRole);
    printf((char*)msgBox_useStoredValue);
#endif

    //safeShow(box);
    box->show();

  } else {

    //safeShow(box);//->show();
    box->show();

    box->button(QMessageBox::Ok)->hide();

  }
  
  qApp->processEvents();
}


void GL_maybe_estimate_vblank(QWidget *qwidget){
  return;

  static bool been_here = false;
  
  MyQtThreadedWidget *widget = static_cast<MyQtThreadedWidget*>(qwidget);

  if (been_here==true)
    return;
  else
    been_here = true;
  
  if (use_estimated_vblank_questionmark() == true) {
    widget->set_vblank(GL_get_estimated_vblank());
    return;
  }
  

  QMessageBox box;

  bool have_earlier = have_earlier_estimated_value();

  show_message_box(&box);

  ATOMIC_SET(widget->is_training_vblank_estimator, true);
      
  while(ATOMIC_GET(widget->is_training_vblank_estimator)==true) {
    if(box.clickedButton()!=NULL){
      widget->set_vblank(GL_get_estimated_vblank());
      store_use_estimated_vblank(true);
      break;
    }
    if (have_earlier)
      qApp->processEvents();
    else
      qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    
    qApp->flush();
    
    usleep(5*1000);
  }
  
  if (box.clickedButton()==NULL)
    store_estimated_value(time_estimator.get_vblank());
  

  printf("\n\n\n Closing box\n\n\n");
  box.close();

}

