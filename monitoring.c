

#include <unistd.h>
// g++ monitoring.c -o monitoring.out -I/usr/include/  -lmraa
#include "mraa/aio.h"
#include "mraa/gpio.h"
#include "math.h"

int
main()
{
    mraa_aio_context adc_a[2];
	mraa_gpio_context  gpio_in;
    uint16_t adc_value = 0;
	int i;
	int B=3975;//константа для работы с температурой
	int D2=0;
	float tmp, light,temperature;
    //инициализация входов
	//стандартная работа с библиотекой для работы с аналоговыми входами	
    for(i=0;i<2;i++)
	{
		adc_a[i] = mraa_aio_init(i);
		if (adc_a[i] == NULL) return 1;
    }
	gpio_in = mraa_gpio_init(2);
	if (gpio_in == NULL) return 1;
	mraa_gpio_dir(gpio_in, MRAA_GPIO_IN);
	//конец инициализации 

    while(D2==0)//Если не нажата кнопка, то выполняются следующие действия
	{   for(i=0;i<2;i++)//цикл по входам
		{   
			adc_value = mraa_aio_read(adc_a[i]);//читается значение с аналогово входа с номером i
			if(i==0)//если i равно 0, то это датчик освещенности
			{																																
				 light = (float)adc_value; // значение освещенности в Люксах
				fprintf(stdout, "ADC A[%d]=%f\n", i,  light);
			}
			else
				// перевод значения температуры в градусы Цельсия
			{
				tmp = (float)(1023-adc_value)*10000 / adc_value;
				temperature = 1/ (log(tmp/10000) / B+1 /298.15) - 273.15;
				fprintf(stdout, "ADC A[%d]=%8f\n", i, temperature );
			}
			D2=mraa_gpio_read(gpio_in);
		 sleep(1);
		}
    }
	//Завершение работы со входами
	for(i=0;i<2;i++)
		mraa_aio_close(adc_a[i]);
	mraa_gpio_close(gpio_in);

    
    return MRAA_SUCCESS;
}
//! [Interesting]
