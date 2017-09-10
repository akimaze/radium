
#ifndef _RADIUM_AUDIO_SMOOTHDELAY_HPP
#define _RADIUM_AUDIO_SMOOTHDELAY_HPP

static inline int find_next_power2(int i){
  int ret = 2;
  while (ret < i)
    ret *= 2;
  
  return ret;
}



namespace radium{

#define FAUSTFLOAT float

// Based on code generated by faust. (smooth delay by Yann Orlarey)
//
//
struct SmoothDelay {

  int buffer_size;  
  float *fVec0;
  float fRec0[2];
  float fRec1[2];
  int iRec2[2];
  int iRec3[2];
  int iHslider0;
  int IOTA;

  bool can_pipe_instead;
  
  SmoothDelay(int max_size)
    : buffer_size(find_next_power2(max_size))
    , can_pipe_instead(true)
  {
    iHslider0 = 0;
    for (int i0 = 0; (i0 < 2); i0 = (i0 + 1)) {
      fRec0[i0] = 0.0f;
      
    }
    for (int i1 = 0; (i1 < 2); i1 = (i1 + 1)) {
      fRec1[i1] = 0.0f;
      
    }
    for (int i2 = 0; (i2 < 2); i2 = (i2 + 1)) {
      iRec2[i2] = 0;
      
    }
    for (int i3 = 0; (i3 < 2); i3 = (i3 + 1)) {
      iRec3[i3] = 0;
      
    }
    IOTA = 0;

    fVec0 = (float*)malloc(sizeof(float)*buffer_size);
    for (int i4 = 0; (i4 < buffer_size); i4 = (i4 + 1)) {
      fVec0[i4] = 0.0f;
    }
  }

  ~SmoothDelay(){
    free(fVec0);
    fVec0 = NULL;
  }
  
  void setSize(int size){
    size = R_MAX(0, R_MIN(buffer_size-1, size));
    if (size != iHslider0) {
      iHslider0 = size;
      if (can_pipe_instead == true && iHslider0 > 0)
        can_pipe_instead = false;
    }
  }

  int getSize(void) const {
    return safe_int_read(&iHslider0);
  }

#define R_COUNT RADIUM_BLOCKSIZE
#define INTERP (1.0/4096.0)
#define PROCESS(ANDING)                                                 \
  for (int i = 0; (i < R_COUNT); i = (i + 1)) {                           \
    float fSel1;                                                        \
    if ((fRec0[1] != 0.0f) != 0) {                                      \
      fSel1 = (((fRec1[1] > 0.0f) & (fRec1[1] < 1.0f))?fRec0[1]:0.0f);  \
                                                                        \
    } else {                                                            \
      float fSel0;                                                      \
      if (((fRec1[1] == 0.0f) & (iSlow0 != iRec2[1])) != 0) {           \
        fSel0 = INTERP;                                          \
                                                                        \
      } else {                                                          \
        fSel0 = (((fRec1[1] == 1.0f) & (iSlow0 != iRec3[1]))?-INTERP:0.0f); \
                                                                        \
      }                                                                 \
      fSel1 = fSel0;                                                    \
    }                                                                   \
    fRec0[0] = fSel1;                                                   \
    fRec1[0] = R_MAX(0.0f, R_MIN(1.0f, (fRec1[1] + fSel1)));            \
    iRec2[0] = (((fRec1[1] >= 1.0f) & (iRec3[1] != iSlow0))?iSlow0:iRec2[1]); \
    iRec3[0] = (((fRec1[1] <= 0.0f) & (iRec2[1] != iSlow0))?iSlow0:iRec3[1]); \
    float fTemp0 = float(input0[i]);                                    \
    fVec0[(IOTA & ANDING)] = fTemp0;                                    \
    output0[i] = FAUSTFLOAT((((1.0f - fRec1[0]) * fVec0[((IOTA - (iRec2[0] & ANDING)) & ANDING)]) + (fRec1[0] * fVec0[((IOTA - (iRec3[0] & ANDING)) & ANDING)]))); \
    fRec0[1] = fRec0[0];                                                \
    fRec1[1] = fRec1[0];                                                \
    iRec2[1] = iRec2[0];                                                \
    iRec3[1] = iRec3[0];                                                \
    IOTA = (IOTA + 1);                                                  \
                                                                        \
  }
  
  
  bool RT_process(int count, const FAUSTFLOAT* input0, FAUSTFLOAT* output0) {
#if !defined(RELEASE)
    R_ASSERT_RETURN_IF_FALSE2(count==R_COUNT, false);
#endif
    
    const int iSlow0 = iHslider0;

    const int anding = buffer_size-1;

    if (can_pipe_instead){

      // Must do this, even if there has been no delay earlier. If we don't do this, there will be clicks when turning on the delay.
      for(int i = 0 ; i < R_COUNT ; i++){
        fVec0[(IOTA & anding)] = input0[i];
        IOTA++;
      }

      IOTA = IOTA & anding; // To avoid ubsan hit (integer overflow). Since the CPU usage of this line is insignificant, We also do this in RELEASE mode to avoid a difference between DEBUG and RELEASE.
      
      //   if (input0 != output0)
      //  memcpy(output0, input0, sizeof(float)*R_COUNT);
      
      return false;
    }

#define M(N) } else if (buffer_size==N) { PROCESS((N-1))

    if (false) {
      return false;
    M(2);
    M(4);    
    M(8);
    M(16);
    M(32);
    M(64);
    M(128);
    M(256);
    M(512);
    M(1024);
    M(2048);
    M(4096);
    M(8192);
    M(16384);
    M(32768);
    M(65536);
    M(131072);
    M(262144);
    M(524288);
    } else
      PROCESS(anding);

    return true;
  }

};


}

#endif
