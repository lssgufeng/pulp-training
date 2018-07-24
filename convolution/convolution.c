// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include "pulp.h"
#include "convolution.h"

void check_Conv3x3_Scalar();
void check_Conv3x3_Vector();
void check_Conv5x5_Scalar();
void check_Conv5x5_Vector();

RT_L2_DATA Pixel Out[IMG_DIM];
RT_L2_DATA Pixel In[IMG_DIM];
RT_L2_DATA Pixel Kernel[FILT_DIM];

int main()
{
  
  int errors = 0;
  
#ifdef DOTP
  check_Conv5x5_Vector();
#else
  check_Conv5x5_Scalar();
#endif
  
  return 0;
}

void check_Conv5x5_Scalar() {
  
  // start benchmark
  Filtc Kernel5x5_Scalar[FILT_DIM];
  Pixel In[IMG_DIM];


  printf("2D Convolution WINDOW=%d, DATA_WIDTH=%d\n",FILT_WIN,DATA_WIDTH);
  InitKernel(Kernel5x5_Scalar,FILT_WIN);
  InitData(In, IMG_DIM);
  InitZero(Out, IMG_DIM);
  
  reset_timer();
  start_timer();
  
  Conv5x5_Scalar(In, Out, IMG_ROW, IMG_COL, Kernel5x5_Scalar);
  
  stop_timer();
  
  printf("Number of cycles: %d\n ",get_time());
  
  checkresult(Out, Gold_Out_Img, IMG_DIM);
  
}

void check_Conv5x5_Vector() {

  // start benchmark
  Filtc Kernel5x5_Vector[FILT_DIM];
  Pixel In[IMG_DIM];
  
  printf("2D Convolution WINDOW=%d, DATA_WIDTH=%d\n",FILT_WIN,DATA_WIDTH);
  InitKernel(Kernel5x5_Vector,FILT_WIN);
  InitData(In, IMG_DIM);
  InitZero(Out, IMG_DIM);
  
  reset_timer();
  start_timer();
  
  Conv5x5_Vector(In, Out, IMG_ROW, IMG_COL, Kernel5x5_Vector);
    
  stop_timer();
  printf("Number of cycles: %d\n ",get_time());
  
  checkresult(Out, Gold_Out_Img, IMG_DIM);

}

// load kernel
void __attribute__ ((noinline)) InitKernel(Filtc * __restrict__ Kernel, int size)
{
  int i;
  int n = size*size;
  for (i=0; i < n; i++) {
      Kernel[i] = Filter_Kern[i];
  }
}

// load input img
void __attribute__ ((noinline)) InitData(Pixel * __restrict__ Img, int size)
{
  int i;

  for (i=0; i < size; i++) 
      Img[i] = In_Img[i];

}

// load initialize out to 0
void __attribute__ ((noinline)) InitZero(Pixel * __restrict__ Img, int size)
{
  int i;

  for (i=0; i < size; i++) 
      Img[i] = 0;

}

#define CONV2D_DEBUG
int  __attribute__ ((noinline)) checkresult(Pixel * __restrict__ Out, Pixel * __restrict__ OutGold, int N)
{
  int i;
  int err = 0;

  for (i = 0; i<N; i++) {
    if (Out[i]!=OutGold[i]) {
#ifdef CONV2D_DEBUG
      printf("At index %d: Actual value: %x: Expected: %x\n", i, Out[i],  OutGold[i]);
#endif
      err++;
    }
  }
  return err;
}
