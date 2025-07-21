/*****************************************************************************
 * @Author                : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @CreatedDate           : 2025-07-15 09:18:15                              *
 * @LastEditors           : Oberson-Antoine<antoine.oberson@yahoo.fr>        *
 * @LastEditDate          : 2025-07-15 13:13:45                              *
 * @FilePath              : Trotten_Electric_Table/main/MCP23017_p.h         *
 ****************************************************************************/


#pragma once

#define MCP23017_ADDRESS 0x20

typedef enum{
    MCP23017_REG_IODIRA   = 0x00, /*!< DIRECTION REGISTER A */
    MCP23017_REG_IODIRB   = 0x01, /*!< DIRECTION REGISTER B */
    MCP23017_REG_IPOLA    = 0x02, /*!< INPUT POLARITY REGISTER A */
    MCP23017_REG_IPOLB    = 0x03, /*!< INPUT POLARITY REGISTER B */
    MCP23017_REG_GPINTENA = 0x04, /*!< INTERRUPT-ON-CHANGE CONTROL REGISTER A */
    MCP23017_REG_GPINTENB = 0x05, /*!< INTERRUPT-ON-CHANGE CONTROL REGISTER B */
    MCP23017_REG_DEFVALA  = 0x06, /*!< DEFAULT COMPARE VALUE A */
    MCP23017_REG_DEFVALB  = 0x07, /*!< DEFAULT COMPARE VALUE B */
    MCP23017_REG_INTCONA  = 0x08, /*!< INTERRUPT-ON-CHANGE CONTROL REGISTER A */
    MCP23017_REG_INTCONB  = 0x09, /*!< INTERRUPT-ON-CHANGE CONTROL REGISTER B */
    MCP23017_REG_IOCONA   = 0x0A, /*!< I/O EXPANDER CONFIGURATION REGISTER A */
    MCP23017_REG_IOCONB   = 0x0B, /*!< I/O EXPANDER CONFIGURATION REGISTER B */
    MCP23017_REG_GPPUA    = 0x0C, /*!< PULL-UP RESISTOR REGISTER A */
    MCP23017_REG_GPPUB    = 0x0D, /*!< PULL-UP RESISTOR REGISTER B */
    MCP23017_REG_INTFA    = 0x0E, /*!< INTERRUPT FLAG REGISTER A */
    MCP23017_REG_INTFB    = 0x0F, /*!< INTERRUPT FLAG REGISTER B */
    MCP23017_REG_INTCAPA  = 0x10, /*!< INTERRUPT CAPTURED VALUE FOR PORT REGISTER A */
    MCP23017_REG_INTCAPB  = 0x11, /*!< INTERRUPT CAPTURED VALUE FOR PORT REGISTER B */
    MCP23017_REG_GPIOA    = 0x12, /*!< GENERAL PURPOSE I/O PORT REGISTER A */
    MCP23017_REG_GPIOB    = 0x13, /*!< GENERAL PURPOSE I/O PORT REGISTER B */
    MCP23017_REG_OLATA    = 0x14, /*!< OUTPUT LATCH REGISTER 0 A */
    MCP23017_REG_OLATB    = 0x15, /*!< OUTPUT LATCH REGISTER 0 B */
} mcp23017_reg_t;


//Init and configures the MCP23017
void MCP23017_init(void);