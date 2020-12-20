#pragma once

#include <functional>

#include <gc.h>


namespace radium{

  struct SuperTallocWithDestructor{
    
    std::function<void(void)> _super_finalizer;

    virtual ~SuperTallocWithDestructor(){
    }
  };
  
  template<typename T> 
  struct TallocWithDestructor : SuperTallocWithDestructor{

    T *_ret;
    std::function<void(T*)> _finalizer;
    
    TallocWithDestructor(T *ret, std::function<void(T*)> finalizer)
      : _ret(ret)
      , _finalizer(finalizer)
    {
      _super_finalizer = [this](){
        _finalizer(_ret);
      };
    }
  };  
}


extern void talloc_with_destructor_gc_finalizer(void *actual_mem_start, void *user_data);
extern void gc_able_gc_finalizer(void *actual_mem_start, void *user_data);

template<typename T> 
static inline T *talloc_with_finalizer(std::function<void(T*)> finalizer){
  T *ret = (T*)talloc(sizeof(T));
  
  radium::TallocWithDestructor<T> *gakkgakk = new radium::TallocWithDestructor<T>(ret, finalizer);
  
  GC_register_finalizer(ret, talloc_with_destructor_gc_finalizer, gakkgakk, NULL, NULL);
  
  return ret;
}

//extern int g_num_gcable;

namespace radium{
  
  struct GC_able{

    bool _destructor_has_run = false;
    
    // Destructor must actually be virtual for destructors in sub classes to be called (stupid c++).
    virtual ~GC_able(){
      /* printf("~GC_able called\n"); */
      R_ASSERT(_destructor_has_run==false);
      _destructor_has_run=true;
    }
    
    void * operator new(size_t size) {
      //printf("     CUSTOM NEW %d\n", g_num_gcable++);

      void *ret = talloc(size);
      
      GC_register_finalizer(ret, gc_able_gc_finalizer, NULL, NULL, NULL);
      
      return ret;
    }

#if !defined(RELEASE)
    bool _allowed_to_call_destructor = false;
#endif
    
    // Should never be deleted explicitly though.
    void operator delete(void *p) {
      radium::GC_able *gc_able = static_cast<radium::GC_able*>(p);

      R_ASSERT_RETURN_IF_FALSE(gc_able==dynamic_cast<radium::GC_able*>(gc_able));

      R_ASSERT_NON_RELEASE(gc_able->_destructor_has_run==false);

#if !defined(RELEASE)  
      R_ASSERT_RETURN_IF_FALSE(gc_able->_allowed_to_call_destructor);
      gc_able->_allowed_to_call_destructor = false;
      
      //printf("     CUSTOM DELETE: %d\n", g_num_gcable--);
      //abort();
#endif
    }
  };
  

}
