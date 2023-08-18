/**
 * @file   xdma.h
 * @brief  Main definitions used in XDMA Linux drivers.
 * 
 */

#ifndef _XDMA_H_
//-----------------------------------------------------------------------------
// glbl
#define XDMA_DEV_NAME           "my_dma"          // noted in "/dev/XDMA_DEV_NAME"
#define XDMA_MEM_SIZE           (4*(2048*2048))  
//-----------------------------------------------------------------------------
// driver INFo codes
//  -> [7:0]
#define XDMA_INFO_INIT          (0x00)
#define XDMA_INFO_INIT_OK       (0x01)
//  -> [15:8]
#define XDMA_INFO_OPEN_DEV_OK   (0x01 << 8)
#define XDMA_INFO_OPEN_DEV_ERR  (0x0F << 8)
#define XDMA_INFO_OPEN_DMA_OK   (0x10 << 8)
#define XDMA_INFO_OPEN_DMA_ERR  (0xF0 << 8)
//-----------------------------------------------------------------------------
// ioctl :
#define XDMA_DEVICE_TYPE        'x'
#define XDMA_MAKE_IOCTL(t,c)    _IO((t), (c))
//  -> command code
#define XDMA_ADDRESS            (0)
#define XDMA_SIZE         	(1)
#define XDMA_COMMAND	        (2)

//  -> requests code
#define IOCTL_XDMA_ADDRESS      XDMA_MAKE_IOCTL(XDMA_DEVICE_TYPE, XDMA_ADDRESS)
#define IOCTL_XDMA_SIZE   	    XDMA_MAKE_IOCTL(XDMA_DEVICE_TYPE, XDMA_SIZE)
#define IOCTL_XDMA_COMMAND   	XDMA_MAKE_IOCTL(XDMA_DEVICE_TYPE, XDMA_COMMAND)

//-----------------------------------------------------------------------------
#endif // _XDMA_H_
