/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-08-07 11:54:45                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-08-07 11:54:45                              *
 * @FilePath              : Trotten_Electric_Table/main/INA260.h             *
 ****************************************************************************/

#pragma once

#define INA_ADDRESS 0x40


//==========================
// INA260 Register Addresses
// ==========================
#define INA260_REG_CONFIG            0x00  // Configuration Register (R/W)
#define INA260_REG_CURRENT           0x01  // Current Register (R)
#define INA260_REG_BUS_VOLTAGE       0x02  // Bus Voltage Register (R)
#define INA260_REG_POWER             0x03  // Power Register (R)
#define INA260_REG_MASK_ENABLE       0x06  // Mask/Enable Register (R/W)
#define INA260_REG_ALERT_LIMIT       0x07  // Alert Limit Register (R/W)
#define INA260_REG_MANUFACTURER_ID   0xFE  // Manufacturer ID Register (R)
#define INA260_REG_DIE_ID            0xFF  // Die ID Register (R)


 // Bit 15 - Reset Bit
#define INA_RST                        (1 << 15)

// Bits 14:12 - Reserved (read-only, default 110)

// Bits 11:9 - Averaging Mode
#define INA_AVG_MASK                  (0x7 << 9)
#define INA_AVG_1                     (0x0 << 9)
#define INA_AVG_4                     (0x1 << 9)
#define INA_AVG_16                    (0x2 << 9)
#define INA_AVG_64                    (0x3 << 9)
#define INA_AVG_128                   (0x4 << 9)
#define INA_AVG_256                   (0x5 << 9)
#define INA_AVG_512                   (0x6 << 9)
#define INA_AVG_1024                  (0x7 << 9)

// Bits 8:6 - VBUS Conversion Time
#define INA_VBUSCT_MASK              (0x7 << 6)
#define INA_VBUSCT_140US             (0x0 << 6)
#define INA_VBUSCT_204US             (0x1 << 6)
#define INA_VBUSCT_332US             (0x2 << 6)
#define INA_VBUSCT_588US             (0x3 << 6)
#define INA_VBUSCT_1_1MS             (0x4 << 6)
#define INA_VBUSCT_2_116MS           (0x5 << 6)
#define INA_VBUSCT_4_156MS           (0x6 << 6)
#define INA_VBUSCT_8_244MS           (0x7 << 6)

// Bits 5:3 - ISHCT Conversion Time (Shunt Current)
#define INA_ISHCT_MASK               (0x7 << 3)
#define INA_ISHCT_140US              (0x0 << 3)
#define INA_ISHCT_204US              (0x1 << 3)
#define INA_ISHCT_332US              (0x2 << 3)
#define INA_ISHCT_588US              (0x3 << 3)
#define INA_ISHCT_1_1MS              (0x4 << 3)
#define INA_ISHCT_2_116MS            (0x5 << 3)
#define INA_ISHCT_4_156MS            (0x6 << 3)
#define INA_ISHCT_8_244MS            (0x7 << 3)

// Bits 2:0 - Operating Mode
#define INA_MODE_MASK                0x7
#define INA_MODE_POWER_DOWN          0x0
#define INA_MODE_SHUNT_TRIGGERED     0x1
#define INA_MODE_BUS_TRIGGERED       0x2
#define INA_MODE_SHUNT_BUS_TRIGGERED 0x3
#define INA_MODE_SHUNT_CONTINUOUS    0x5
#define INA_MODE_VOL_CONTINUOUS      0x6
#define INA_MODE_SHUNT_BUS_CONTINUOUS 0x7

//Init and configures the INA260        
void INA260_init(void);

/**
 * Reads the INA260 buffer
 * @return uint8 value 
 */
uint16_t INA260_read(uint8_t reg);