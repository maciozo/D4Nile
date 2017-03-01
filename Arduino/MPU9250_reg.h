#ifndef MPU9250_REG_H_INCLUDED
#define MPU9250_REG_H_INCLUDED

#define MPU9250_I2C_CLOCK_SPEED                  400000UL // I2C is 400KHz max
#define MPU9250_WRITE                    		 0x68 // This address is used by MPU9250 when ADC0 pin is logic low
#define MPU9250_READ                  			 0x69 // This address is used by MPU9250 when ADC0 pin is logic high

// Note, this is the reset value for all registers except 
// - Register 107 (0x01) Power Management 1
// - Register 117 (0x71) WHO_AM_I
#define REG_RESET                        0x00

// From section 7.5 SPI Interface
// SPI read and write operations are completed in 16 or more clock cycles (two or more bytes). The
// first byte contains the SPI A ddress, and the following byte(s) contain(s) the SPI data. The first
// bit of the first byte contains the Read/Write bit and indicates the Read (1) or Write (0) operation.
// The following 7 bits contain the Register Address. In cases of multiple-byte Read/Writes, data is
// two or more bytes...
#define READ_MASK                        0x80

// Self Test, Gyro
#define SELF_TEST_X_GYRO                 0x00
#define SELF_TEST_Y_GYRO                 0x01
#define SELF_TEST_Z_GYRO                 0x02



// Self Test, Accelerometer
#define SELF_TEST_X_ACCEL                0x0d
#define SELF_TEST_Y_ACCEL                0x0e
#define SELF_TEST_Z_ACCEL                0x0f

 // Gyro Offset
#define XG_OFFSET_H                      0x13
#define XG_OFFSET_L                      0x14
#define YG_OFFSET_H                      0x15
#define YG_OFFSET_L                      0x16
#define ZG_OFFSET_H                      0x17
#define ZG_OFFSET_L                      0x18


#define SMPLRT_DIV                       0x19

// Config
#define CONFIG                           0x1a
#define GYRO_CONFIG                      0x1b
#define ACCEL_CONFIG                     0x1c
#define ACCEL_CONFIG_2                   0x1d
#define LP_ACCEL_ODR                     0x1e

#define WOM_THR                          0x1f

#define FIFO_EN                          0x23

// I2C
#define I2C_MST_CTRL                     0x24
#define I2C_SLV0_ADDR                    0x25
#define I2C_SLV0_REG                     0x26
#define I2C_SLV0_CTRL                    0x27

#define I2C_SLV1_ADDR                    0x28
#define I2C_SLV1_REG                     0x29
#define I2C_SLV1_CTRL                    0x2a

#define I2C_SLV2_ADDR                    0x2b
#define I2C_SLV2_REG                     0x2c
#define I2C_SLV2_CTRL                    0x2d

#define I2C_SLV3_ADDR                    0x2e
#define I2C_SLV3_REG                     0x2f
#define I2C_SLV3_CTRL                    0x30

#define I2C_SLV4_ADDR                    0x31
#define I2C_SLV4_REG                     0x32
#define I2C_SLV4_DO                      0x33
#define I2C_SLV4_CTRL                    0x34
#define I2C_SLV4_DI                      0x35

#define I2C_MST_STATUS                   0x36

#define INT_PIN_CFG                      0x37
#define INT_ENABLE                       0x38

#define DMP_INT_STATUS                   0x39 // Check DMP Interrupt, see 0x6d

#define INT_STATUS                       0x3a

// Accel XOUT
#define ACCEL_XOUT_H                     0x3b
#define ACCEL_XOUT_L                     0x3c
#define ACCEL_YOUT_H                     0x3d
#define ACCEL_YOUT_L                     0x3e
#define ACCEL_ZOUT_H                     0x3f
#define ACCEL_ZOUT_L                     0x40

// Temp.
#define TEMP_OUT_H                       0x41
#define TEMP_OUT_L                       0x42

// Gyro.
#define GYRO_XOUT_H                      0x43
#define GYRO_XOUT_L                      0x44
#define GYRO_YOUT_H                      0x45
#define GYRO_YOUT_L                      0x46
#define GYRO_ZOUT_H                      0x47
#define GYRO_ZOUT_L                      0x48

// Ext. Sensor data
#define EXT_SENS_DATA_00                 0x49
#define EXT_SENS_DATA_01                 0x4a
#define EXT_SENS_DATA_02                 0x4b
#define EXT_SENS_DATA_03                 0x4c
#define EXT_SENS_DATA_04                 0x4d
#define EXT_SENS_DATA_05                 0x4e
#define EXT_SENS_DATA_06                 0x4f
#define EXT_SENS_DATA_07                 0x50
#define EXT_SENS_DATA_08                 0x51
#define EXT_SENS_DATA_09                 0x52
#define EXT_SENS_DATA_10                 0x53
#define EXT_SENS_DATA_11                 0x54
#define EXT_SENS_DATA_12                 0x55
#define EXT_SENS_DATA_13                 0x56
#define EXT_SENS_DATA_14                 0x57
#define EXT_SENS_DATA_15                 0x58
#define EXT_SENS_DATA_16                 0x59
#define EXT_SENS_DATA_17                 0x5a
#define EXT_SENS_DATA_18                 0x5b
#define EXT_SENS_DATA_19                 0x5c
#define EXT_SENS_DATA_20                 0x5d
#define EXT_SENS_DATA_21                 0x5e
#define EXT_SENS_DATA_22                 0x5f
#define EXT_SENS_DATA_23                 0x60

// I2C slave
#define I2C_SLV0_DO                      0x63
#define I2C_SLV1_DO                      0x64
#define I2C_SLV2_DO                      0x65
#define I2C_SLV3_DO                      0x66

#define I2C_MST_DELAY_CTRL               0x67


// Signal path
#define SIGNAL_PATH_RESET                0x68

// Motion detect
#define MOT_DETECT_CTRL                  0x69

// User
#define USER_CTRL                        0x6a // Bit 7 enable DMP, bit 3 reset DMP. See 0x6d

// Power management
#define PWR_MGMT_1                       0x6b
#define PWR_MGMT_2                       0x6c

// ...Looked for notes on DMP features, but Invensense docs were lacking.
// Found kriswiner's Arduino sketch for Basic AHRS, and found values/notes for
// Digital Motion Processing registers.
//
// See https://github.com/kriswiner/MPU-9250/blob/master/MPU9250BasicAHRS.ino
#define DMP_BANK                         0x6d
#define DMP_RW_PNT                       0x6e
#define DMP_REG                          0x6f
#define DMP_REG_1                        0x70
#define DMP_REG_2                        0x71

// FIFO Count
#define FIFO_COUNTH                      0x72	
#define FIFO_COUNTL                      0x73
#define FIFO_R_W                         0x74
#define WHO_AM_I                         0x75 //should return something else???

// Accel. offset
#define XA_OFFSET_H                      0x77
#define XA_OFFSET_L                      0x78
#define YA_OFFSET_H                      0x7a
#define YA_OFFSET_L                      0x7b
#define ZA_OFFSET_H                      0x7d
#define ZA_OFFSET_L                      0x7e

#endif