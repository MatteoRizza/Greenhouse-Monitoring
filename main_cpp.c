#include "mbed.h"
#include "mbed-trace/mbed_trace.h"
#include "mbedtls/debug.h"

#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"

#include "core/cloud.h"

#define AWS_IOT_UPDATE_FREQUENCY    10

DigitalOut led(LED1);

// Connect services 
int connect(){
    int result = 0;

    int cloud_status = cloud_connect();
    if(cloud_status == 1){
        result = 1;    
    }
    
    return result;
}

// Main
int main(){
    mbed_trace_init();
    
    printf("IoT device starting\r\n");
    
    // Define sensor vars 
    float temperature = 0;
    float humidity = 0;
    
    // Initalize sensors 
    BSP_TSENSOR_Init();
    BSP_HSENSOR_Init();
    
    // Initialize cloud (connect wifi) 
    cloud_init();
    
    // Connect to network 
    int connected = connect();
    while(1) {
        if(connected == 1){
            //It works
            led = 1;
            
            // Get current sensor data
            temperature = BSP_TSENSOR_ReadTemp();
            humidity = BSP_HSENSOR_ReadHumidity();
            
            // Send message to cloud
            cloud_send(temperature,humidity);
            
            // wait 
            wait(AWS_IOT_UPDATE_FREQUENCY);
        }else{
            // It doens't work  
            wait(2);  
            led=0;
        }
    }
}