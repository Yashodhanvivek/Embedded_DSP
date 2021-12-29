// Calculate DFT and its Inverse DFT on STM32 for ECG signal


#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "arm_math.h"                   // ARM::CMSIS:DSP

#define	SIG_LENGTH		200
#define IMP_RSP_LENGTH		29
#define	ECG_LENGTH				640
extern void SystemClock_Config(void);
extern float32_t inputSignal_f32_1kHz_15kHz[SIG_LENGTH];
extern   float32_t _640_points_ecg_[640];

float32_t REX[SIG_LENGTH/2 ];
float32_t IMX[SIG_LENGTH/2];

float32_t ecg_REX[ECG_LENGTH/2];
float32_t ecg_IMX[ECG_LENGTH/2];

float32_t idft_output_sig[ECG_LENGTH];


// prototypes
void plot_input_signal(void);
void plot_both_signal(void);
void plot_ecg_signal(void);
void plot_ecg_rex_signal(void);
void get_ecg_dft_output_mag(void);
void plot_ecg_idft_signal(void);
 void plot_original_sig_and_idft_sig(void);

float32_t inputSample;
float32_t rexSample;
float32_t imxSample;
float32_t ecgSample;
float32_t ecg_rexSample;
float32_t idft_output_ecgSample;

void plot_imp_response(void);
void plot_output_signal(void);
void plot_all(void);
void calc_sig_dft(float32_t *sig_src_arr, float32_t *sig_dest_rex_arr, float32_t *sig_dest_imx_arr, uint32_t sig_length);
void calc_sig_idft(float32_t *idft_out_arr,float32_t *sig_src_rex_arr,float32_t *sig_src_imx_arr,uint32_t idft_length);

void plot_rex_signal(void);
void get_dft_output_mag(void);

uint32_t freq;

int main()
{
	HAL_Init();
	SystemClock_Config();
	freq = HAL_RCC_GetHCLKFreq();


         calc_sig_dft(& _640_points_ecg_[0], &ecg_REX[0], &ecg_IMX[0], ECG_LENGTH);
	 calc_sig_idft(&idft_output_sig[0],&ecg_REX[0],&ecg_IMX[0],ECG_LENGTH);

  plot_original_sig_and_idft_sig();


	while(1)
	{
	}
}

void calc_sig_idft(float32_t *idft_out_arr,float32_t *sig_src_rex_arr,float32_t *sig_src_imx_arr,uint32_t idft_length)
{
	int i,j,k;
	for(k=0;k<idft_length/2;k++)
	{
		sig_src_rex_arr[k] =  sig_src_rex_arr[k]/(idft_length/2);
	  sig_src_imx_arr[k]  = -sig_src_imx_arr[k]/(idft_length/2);
	}
	
	  sig_src_rex_arr[0] =  sig_src_rex_arr[0]/(idft_length/2);
	  sig_src_imx_arr[0]  = -sig_src_imx_arr[0]/(idft_length/2);
	
	 for(i=0;i<idft_length;i++)
	{
		idft_out_arr[i] =0;
	}
	
	 for(k=0;k<idft_length/2;k++)
	{
		 for(i=0;i<idft_length;i++)
		 {
					
			    idft_out_arr[i] =  idft_out_arr[i] + sig_src_rex_arr[k]*cos(2*PI*k*i/idft_length);
			 	  idft_out_arr[i] =  idft_out_arr[i] + sig_src_imx_arr[k]*sin(2*PI*k*i/idft_length);

		 }
	}
}


 void calc_running_sum(float32_t *sig_src_arr,float32_t *sig_dest_arr,uint32_t sig_length)
 {
	 int i;
	 sig_src_arr[0] = sig_dest_arr[0];
	 for(i=0;i<sig_length;i++)
	 {
		 sig_dest_arr[i]= sig_dest_arr[i-1]+sig_src_arr[i];
	 }
 }
 
  void calc_first_difference(float32_t *sig_src_arr,float32_t *sig_dest_arr, uint32_t sig_length)
{
		sig_dest_arr[0] =0;
		int  i;
		for(i=0;i<sig_length;i++)
		{
			 sig_dest_arr[i] = sig_src_arr[i] - sig_src_arr[i-1];
		}
}





void calc_sig_dft(float32_t *sig_src_arr, float32_t *sig_dest_rex_arr, float32_t *sig_dest_imx_arr, uint32_t sig_length)
{
	int i,k,j;
	for(j=0;j<(sig_length/2);j++)
	{
		 sig_dest_rex_arr[j] =0;
		  sig_dest_imx_arr[j] =0;
	}
	
	 for(k=0;k<(sig_length/2);k++)
	{
		for(i=0;i<sig_length;i++)
		{
					     sig_dest_rex_arr[k] =  sig_dest_rex_arr[k] + sig_src_arr[i]*cos(2*PI*k*i/sig_length);
							 sig_dest_imx_arr[k] =  sig_dest_imx_arr[k] - sig_src_arr[i]*sin(2*PI*k*i/sig_length);


		}
	}
}

void get_dft_output_mag(void)
{
	int k;
	for(k=0;k<(SIG_LENGTH/2);k++){
	 REX[k] = fabs(REX[k]);
	}
}

void get_ecg_dft_output_mag(void)
{
	int k;
	for(k=0;k<(ECG_LENGTH/2);k++){
	 ecg_REX[k] = fabs(ecg_REX[k]);
	}
}
void plot_ecg_signal(void)
{
	int i,j;
	  for(i=0;i<640;i++)
	 {
		 ecgSample  = _640_points_ecg_[i];
		 for(j=0;j<3000;j++){}
			 if(i==639)i=0;
	 }
}

void plot_ecg_idft_signal(void)
{
	int i,j;
	  for(i=0;i<ECG_LENGTH;i++)
	 {
		 idft_output_ecgSample  = idft_output_sig[i];
		 for(j=0;j<6000;j++){}
			 if(i==ECG_LENGTH-1)i=0;
	 }
}
 void plot_original_sig_and_idft_sig(void)
 {
	 int i,j;
	  for(i=0;i<ECG_LENGTH;i++)
	 {
		 idft_output_ecgSample  = idft_output_sig[i];
		 ecgSample  = _640_points_ecg_[i];

		 for(j=0;j<6000;j++){}
			 if(i==ECG_LENGTH-1)i=0;
	 }
 }
void plot_ecg_rex_signal(void)
{
	int i,j;
	  for(i=0;i<(ECG_LENGTH/2);i++)
	{
		 ecg_rexSample = ecg_REX[i];
		for(j=0;j<3000;j++){}
	}
}
void plot_rex_signal(void)
{
	int i,j;
	 for(i=0;i<(SIG_LENGTH/2);i++)
	{
		rexSample = REX[i];
	  for(j=0;j<3000;j++){}
	}
}

void SysTick_Handler(void)
{
	 HAL_IncTick();
	 HAL_SYSTICK_IRQHandler();
	
}
