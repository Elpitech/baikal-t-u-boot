
#ifndef __GCR_H__
#define __GCR_H__

#include "cpc.h"

/* Global Control Block. */
#define GCR_BASE_PHYS		0x1FBF8000
#define GCR_BASE		0xBFBF8000
#define GCR_CORE_LOCAL_BASE	(GCR_BASE + 0x2000)
#define GCR_CORE_OTHER_BASE	(GCR_BASE + 0x4000)
#define GCR_DEBUG_BASE		(GCR_BASE + 0x6000)

#define GCR_CONFIG		(GCR_BASE + 0x0000)	/* Global Config Register */
#define GCR_BASE_REG		(GCR_BASE + 0x0008)	/* GCR Base Register */
#define GCR_BASE_UPPER_REG	(GCR_BASE + 0x000C)	/* GCR Base Upper Register (for XPA)*/
#define GCR_CONTROL		(GCR_BASE + 0x0010)	/* Global CM2 Control Register */
#define GCR_CONTROL2		(GCR_BASE + 0x0018)	/* Global CM2 Control2 Register */
#define GCR_ACCESS		(GCR_BASE + 0x0020)	/* Global CSR Access Privilege Register */
#define GCR_REV			(GCR_BASE + 0x0030)	/* GCR Revision Register */
#define GCR_ERROR_MASK		(GCR_BASE + 0x0040)	/* Global CM2 Error Mask Register */
#define GCR_ERROR_CAUSE		(GCR_BASE + 0x0048)	/* Global CM2 Error Cause Register */
#define GCR_ERROR_ADDR		(GCR_BASE + 0x0050)	/* Global CM2 Error Address Register */
#define GCR_ERROR_UPPER_ADDR	(GCR_BASE + 0x0054)	/* Global CM2 Error Upper Address Register (for XPA)*/
#define GCR_ERROR_MULT		(GCR_BASE + 0x0058)	/* Global CM2 Error Multiple Register */
#define GCR_CUSTOM_BASE		(GCR_BASE + 0x0060)	/* GCR Custom Base Register */
#define GCR_CUSTOM_UPPER_BASE	(GCR_BASE + 0x0064)	/* GCR Custom Upper Base Register (for XPA)*/
#define GCR_CUSTOM_STATUS	(GCR_BASE + 0x0068)	/* GCR Custom Status Register */
#define GCR_L2_ONLY_SYNC_BASE	(GCR_BASE + 0x0070)	/* Global L2 only Sync Register */
#define GCR_L2_ONLY_UPPER_SYNC_BASE	(GCR_BASE + 0x0074)	/* Global L2 only Upper Sync Register (for XPA)*/
#define GCR_GIC_BASE_REG	(GCR_BASE + 0x0080)	/* Global Interrupt Controller Base Address Register */
#define GCR_GIC_UPPER_BASE_REG	(GCR_BASE + 0x0084)	/* Global Interrupt Controller Upper Base Address Register (for XPA) */
#define GCR_CPC_BASE_REG	(GCR_BASE + 0x0088)	/* Cluster Power Controller Base Address Register */
#define GCR_CPC_UPPER_BASE_REG	(GCR_BASE + 0x008C)	/* Cluster Power Controller Upper Base Address Register (for XPA)*/
#define GCR_REG0_BASE		(GCR_BASE + 0x0090)	/* CM2 Region0 Base Address Register */
#define GCR_REG0_UPPER_BASE	(GCR_BASE + 0x0094)	/* CM2 Region0 Upper Base Address Register (for XPA)*/
#define GCR_REG0_MASK		(GCR_BASE + 0x0098)	/* CM2 Region0 Address Mask Register */
#define GCR_REG0_UPPER_MASK	(GCR_BASE + 0x009C)	/* CM2 Region0 Upper Address Mask Register (for XPA)*/
#define GCR_REG1_BASE		(GCR_BASE + 0x00A0)	/* CM2 Region1 Base Address Register */
#define GCR_REG1_UPPER_BASE	(GCR_BASE + 0x00A4)	/* CM2 Region1 Upper Base Address Register (for XPA)*/
#define GCR_REG1_MASK		(GCR_BASE + 0x00A8)	/* CM2 Region1 Address Mask Register */
#define GCR_REG1_UPPER_MASK	(GCR_BASE + 0x00AC)	/* CM2 Region1 Upper Address Mask Register (for XPA)*/
#define GCR_REG2_BASE		(GCR_BASE + 0x00B0)	/* CM2 Region2 Base Address Register */
#define GCR_REG2_UPPER_BASE	(GCR_BASE + 0x00B4)	/* CM2 Region2 Upper Base Address Register (for XPA)*/
#define GCR_REG2_MASK		(GCR_BASE + 0x00B8)	/* CM2 Region2 Address Mask Register */
#define GCR_REG2_UPPER_MASK	(GCR_BASE + 0x00BC)	/* CM2 Region2 Upper Address Mask Register (for XPA)*/
#define GCR_REG3_BASE		(GCR_BASE + 0x00C0)	/* CM2 Region3 Base Address Register */
#define GCR_REG3_UPPER_BASE	(GCR_BASE + 0x00C4)	/* CM2 Region3 Upper Base Address Register (for XPA)*/
#define GCR_REG3_MASK		(GCR_BASE + 0x00C8)	/* CM2 Region3 Address Mask Register */
#define GCR_REG3_UPPER_MASK	(GCR_BASE + 0x00CC)	/* CM2 Region3 Upper Address Mask Register (for XPA)*/
#define GCR_GIC_STATUS		(GCR_BASE + 0x00D0)	/* Global Interrupt Controller Status Register */
#define GCR_CACHE_REV		(GCR_BASE + 0x00E0)	/* Cache Revision Register */
#define GCR_CPC_STATUS		(GCR_BASE + 0x00F0)	/* Cluster Power Controller Status Register */
#define GCR_IOC_BASE		(GCR_BASE + 0x0100)	/* Address Base for IOMMU registers contained within the IOCUs. */
#define GCR_IOC_UPPER_BASE	(GCR_BASE + 0x0104)	/* Upper Address Base for IOMMU registers contained within the IOCUs. (for XPA)*/
#define GCR_IOMMU_STATUS	(GCR_BASE + 0x0108)	/* Existence of IOMMU inside IOCU. */
#define GCR_REG0_ATTR_BASE	(GCR_BASE + 0x0190)	/* CM Attribute-Only Region0 Base Address Register */
#define GCR_REG0_UPPER_ATTR_BASE	(GCR_BASE + 0x0194)	/* CM Attribute-Only Region0 Upper Base Address Register (for XPA)*/
#define GCR_REG0_ATTR_MASK	(GCR_BASE + 0x0198)	/* CM Attribute-Only Region0 Address Mask Register */
#define GCR_REG0_UPPER_ATTR_MASK	(GCR_BASE + 0x019C)	/* CM Attribute-Only Upper Region0 Address Mask Register (for XPA)*/
#define GCR_REG1_ATTR_BASE	(GCR_BASE + 0x01A0)	/* CM Attribute-Only Region1 Base Address Register */
#define GCR_REG1_UPPER_ATTR_BASE	(GCR_BASE + 0x01A4)	/* CM Attribute-Only Region1 Upper Base Address Register (for XPA)*/
#define GCR_REG1_ATTR_MASK	(GCR_BASE + 0x01A8)	/* CM Attribute-Only Region1 Address Mask Register */
#define GCR_REG1_UPPER_ATTR_MASK	(GCR_BASE + 0x01AC)	/* CM Attribute-Only Upper Region1 Address Mask Register (for XPA)*/
#define GCR_IOCU1_REV		(GCR_BASE + 0x0200)	/* IOCU Revision Register */
#define GCR_REG2_ATTR_BASE	(GCR_BASE + 0x0210)	/* CM Attribute-Only Region2 Base Address Register */
#define GCR_REG2_UPPER_ATTR_BASE	(GCR_BASE + 0x0214)	/* CM Attribute-Only Region2 Upper Base Address Register (for XPA)*/
#define GCR_REG2_ATTR_MASK	(GCR_BASE + 0x0218)	/* CM Attribute-Only Region2 Address Mask Register */
#define GCR_REG2_UPPER_ATTR_MASK	(GCR_BASE + 0x021C)	/* CM Attribute-Only Upper Region2 Address Mask Register (for XPA)*/
#define GCR_REG3_ATTR_BASE	(GCR_BASE + 0x0220)	/* CM Attribute-Only Region3 Base Address Register */
#define GCR_REG3_UPPER_ATTR_BASE	(GCR_BASE + 0x0224)	/* CM Attribute-Only Region3 Upper Base Address Register (for XPA)*/
#define GCR_REG3_ATTR_MASK	(GCR_BASE + 0x0228)	/* CM Attribute-Only Region3 Address Mask Register */
#define GCR_REG3_UPPER_ATTR_MASK	(GCR_BASE + 0x022C)	/* CM Attribute-Only Upper Region3 Address Mask Register (for XPA)*/

/* Core-Local Control Block. */
#define GCR_CL_COHERENCE	(GCR_CORE_LOCAL_BASE + 0x0008)	/* Core Local Coherence Control Register */
#define GCR_CL_CONFIG		(GCR_CORE_LOCAL_BASE + 0x0010)	/* Core Local Config Register */
#define GCR_CL_OTHER		(GCR_CORE_LOCAL_BASE + 0x0018)	/* Core Other Addressing Register */
#define GCR_CL_RESET_BASE	(GCR_CORE_LOCAL_BASE + 0x0020)	/* Core Local Reset Exception Base Register */
#define GCR_CL_ID		(GCR_CORE_LOCAL_BASE + 0x0028)	/* Core Local Identification Register */
#define GCR_CL_RESET_EXT_BASE	(GCR_CORE_LOCAL_BASE + 0x0030)	/* Core Local Reset Exception Extended Base */
#define GCR_CL_TCID_0_PRIORITY	(GCR_CORE_LOCAL_BASE + 0x0040)	/* Core Local TCID_0_PRIORITY Register */

/* Core-Other Control Block. */
#define GCR_CO_COHERENCE	(GCR_CORE_OTHER_BASE + 0x0008)	/* Core Local Coherence Control Register */
#define GCR_CO_CONFIG		(GCR_CORE_OTHER_BASE + 0x0010)	/* Core Local Config Register */
#define GCR_CO_OTHER		(GCR_CORE_OTHER_BASE + 0x0018)	/* Core Other Addressing Register */
#define GCR_CO_RESET_BASE	(GCR_CORE_OTHER_BASE + 0x0020)	/* Core Local Reset Exception Base Register */
#define GCR_CO_ID		(GCR_CORE_OTHER_BASE + 0x0028)	/* Core Local Identification Register */
#define GCR_CO_RESET_EXT_BASE	(GCR_CORE_OTHER_BASE + 0x0030)	/* Core Local Reset Exception Extended Base */
#define GCR_CO_TCID_0_PRIORITY	(GCR_CORE_OTHER_BASE + 0x0040)	/* Core Local TCID_0_PRIORITY Register */

/* Global Debug Control Block */
#define GCR_DB_TCBCONTROLB	(GCR_DEBUG_BASE + 0x0008)	/* PDTrace TCBControlB Register */
#define GCR_DB_TCBCONTROLD	(GCR_DEBUG_BASE + 0x0010)	/* CM2 PDTrace TCBControlD Register */
#define GCR_DB_TCBCONTROLE	(GCR_DEBUG_BASE + 0x0020)	/* PDTrace TCBControlE Register */
#define GCR_DB_TCBConfig	(GCR_DEBUG_BASE + 0x0028)	/* PDTrace TCB Config Register */
#define GCR_DB_TCBSYS		(GCR_DEBUG_BASE + 0x0040)	/* PDTrace TCBSYS Register */
#define GCR_DB_PC_CTL		(GCR_DEBUG_BASE + 0x0100)	/* CM2 Performance Counter Control Register */
#define GCR_DB_TCBRDP		(GCR_DEBUG_BASE + 0x0108)	/* PDTrace Trace Word Read Pointer Register */
#define GCR_DB_TCBWRP		(GCR_DEBUG_BASE + 0x0110)	/* PDTrace Trace Word Write Pointer Register */
#define GCR_DB_TCBSTP		(GCR_DEBUG_BASE + 0x0118)	/* PDTrace Trace Word Start Pointer Register */
#define GCR_DB_PC_OV		(GCR_DEBUG_BASE + 0x0120)	/* CM2 Performance Counter Overflow Status Register */
#define GCR_DB_PC_EVENT		(GCR_DEBUG_BASE + 0x0130)	/* CM2 Performance Counter Event Select Register */
#define GCR_DB_PC_CYCLE		(GCR_DEBUG_BASE + 0x0180)	/* CM2 Performance Cycle Counter Register */
#define GCR_DB_PC_QUAL0		(GCR_DEBUG_BASE + 0x0190)	/* CM2 Performance Counter 0 Qualifier Register */
#define GCR_DB_PC_CNT0		(GCR_DEBUG_BASE + 0x0198)	/* CM2 Performance Counter 0 Register */
#define GCR_DB_PC_QUAL1		(GCR_DEBUG_BASE + 0x01A0)	/* CM2 Performance Counter 1 Qualifier Register */
#define GCR_DB_PC_CNT1		(GCR_DEBUG_BASE + 0x01A8)	/* CM2 Performance Counter 1 Register */
#define GCR_DB_TCBTW_LO		(GCR_DEBUG_BASE + 0x0200)	/* PDTrace Trace Word Lo Register */
#define GCR_DB_TCBTW_HI		(GCR_DEBUG_BASE + 0x0208)	/* PDTrace Trace Word Hi Register */

/* Reset values */
#define                  GCR_CONFIG_VAL		0x00040101
#define                GCR_BASE_REG_VAL		0x1fbf8050
#define          GCR_BASE_UPPER_REG_VAL		0x00000000
#define                 GCR_CONTROL_VAL		0x00000001
#define                GCR_CONTROL2_VAL		0x00040006
#define                  GCR_ACCESS_VAL		0x000000ff
#define                     GCR_REV_VAL		0x00000701
#define              GCR_ERROR_MASK_VAL		0x000a002a
#define             GCR_ERROR_CAUSE_VAL		0x00000000
#define              GCR_ERROR_ADDR_VAL		0x00000000
#define        GCR_ERROR_UPPER_ADDR_VAL		0x00000000
#define              GCR_ERROR_MULT_VAL		0x00000000
#define             GCR_CUSTOM_BASE_VAL		0x00000000
#define       GCR_CUSTOM_UPPER_BASE_VAL		0x00000000
#define           GCR_CUSTOM_STATUS_VAL		0x00000000
#define       GCR_L2_ONLY_SYNC_BASE_VAL		0x00000000
#define GCR_L2_ONLY_UPPER_SYNC_BASE_VAL		0x00000000
#define            GCR_GIC_BASE_REG_VAL		0x1bdc0001
#define      GCR_GIC_UPPER_BASE_REG_VAL		0x00000000
#define            GCR_CPC_BASE_REG_VAL		0x1bde0001
#define      GCR_CPC_UPPER_BASE_REG_VAL		0x00000000
#define               GCR_REG0_BASE_VAL		0x00000000
#define		GCR_REG0_UPPER_BASE_VAL		0x00000000
#define               GCR_REG0_MASK_VAL		0x00000000
#define         GCR_REG0_UPPER_MASK_VAL		0x00000000
#define               GCR_REG1_BASE_VAL		0x00000000
#define         GCR_REG1_UPPER_BASE_VAL		0x00000000
#define               GCR_REG1_MASK_VAL		0x00000000
#define         GCR_REG1_UPPER_MASK_VAL		0x00000000
#define               GCR_REG2_BASE_VAL		0x00000000
#define         GCR_REG2_UPPER_BASE_VAL		0x00000000
#define               GCR_REG2_MASK_VAL		0x00000000
#define         GCR_REG2_UPPER_MASK_VAL		0x00000000
#define               GCR_REG3_BASE_VAL		0x00000000
#define         GCR_REG3_UPPER_BASE_VAL		0x00000000
#define               GCR_REG3_MASK_VAL		0x00000000
#define         GCR_REG3_UPPER_MASK_VAL		0x00000000
#define              GCR_GIC_STATUS_VAL		0x00000001
#define               GCR_CACHE_REV_VAL		0x00000a00
#define              GCR_CPC_STATUS_VAL		0x00000001
#define                GCR_IOC_BASE_VAL		0x00000000
#define          GCR_IOC_UPPER_BASE_VAL		0x00000000
#define            GCR_IOMMU_STATUS_VAL		0x00000000
#define          GCR_REG0_ATTR_BASE_VAL		0x00000000
#define    GCR_REG0_UPPER_ATTR_BASE_VAL		0x00000000
#define          GCR_REG0_ATTR_MASK_VAL		0x00000000
#define    GCR_REG0_UPPER_ATTR_MASK_VAL		0x00000000
#define          GCR_REG1_ATTR_BASE_VAL		0x00000000
#define    GCR_REG1_UPPER_ATTR_BASE_VAL		0x00000000
#define          GCR_REG1_ATTR_MASK_VAL		0x00000000
#define    GCR_REG1_UPPER_ATTR_MASK_VAL		0x00000000
#define               GCR_IOCU1_REV_VAL		0x00000000
#define          GCR_REG2_ATTR_BASE_VAL		0x00000000
#define    GCR_REG2_UPPER_ATTR_BASE_VAL		0x00000000
#define          GCR_REG2_ATTR_MASK_VAL		0x00000000
#define    GCR_REG2_UPPER_ATTR_MASK_VAL		0x00000000
#define          GCR_REG3_ATTR_BASE_VAL		0x00000000
#define    GCR_REG3_UPPER_ATTR_BASE_VAL		0x00000000
#define          GCR_REG3_ATTR_MASK_VAL		0x00000000
#define    GCR_REG3_UPPER_ATTR_MASK_VAL		0x00000000
#define            GCR_CL_COHERENCE_VAL		0x00000000
#define               GCR_CL_CONFIG_VAL		0x00000000
#define                GCR_CL_OTHER_VAL		0x00000000
#define           GCR_CL_RESET_BASE_VAL		0xbfc00000
#define                   GCR_CL_ID_VAL		0x00000000
#define       GCR_CL_RESET_EXT_BASE_VAL		0x40000001
#define      GCR_CL_TCID_0_PRIORITY_VAL		0x00000000
#define            GCR_CO_COHERENCE_VAL 	0x00000000
#define               GCR_CO_CONFIG_VAL 	0x00000000
#define                GCR_CO_OTHER_VAL 	0x00000000
#define           GCR_CO_RESET_BASE_VAL 	0xbfc00000
#define       GCR_CO_RESET_EXT_BASE_VAL 	0x40000001
#define      GCR_CO_TCID_0_PRIORITY_VAL 	0x00000000
#define          GCR_DB_TCBCONTROLB_VAL		0x08000402
#define          GCR_DB_TCBCONTROLD_VAL		0x00000f00
#define          GCR_DB_TCBCONTROLE_VAL		0x00000100
#define            GCR_DB_TCBConfig_VAL		0x000e3c33
#define               GCR_DB_TCBSYS_VAL		0x80000000
#define               GCR_DB_PC_CTL_VAL		0x00000002
#define               GCR_DB_TCBRDP_VAL		0x00000000
#define               GCR_DB_TCBWRP_VAL		0x00000000
#define               GCR_DB_TCBSTP_VAL		0x00000000
#define                GCR_DB_PC_OV_VAL		0x00000000
#define             GCR_DB_PC_EVENT_VAL		0x00000000
#define             GCR_DB_PC_CYCLE_VAL		0x00000000
#define             GCR_DB_PC_QUAL0_VAL		0x00000000
#define              GCR_DB_PC_CNT0_VAL		0x00000000
#define             GCR_DB_PC_QUAL1_VAL		0x00000000
#define              GCR_DB_PC_CNT1_VAL		0x00000000
#define             GCR_DB_TCBTW_LO_VAL		0x00000000
#define             GCR_DB_TCBTW_HI_VAL		0x00000000


/* GCR_CONFIG */
#define GCR_CONFIG_PCORES_POS	0
#define GCR_CONFIG_PCORES_BITS	8
#define GCR_CONFIG_PCORES_MASK	0xFF

/* GCR_BASE_REG */
#define GCR_BASE_REG_CCA_POS	5	/* Cache Coherence Attribute */
#define GCR_BASE_REG_CCA_BITS	3

#define GCR_BASE_CCA_WT		0x0	/* Write Through */
#define GCR_BASE_CCA_UC		0x2	/* Uncached */
#define GCR_BASE_CCA_WB		0x3	/* Writeback, cacheable, noncoherent */
#define GCR_BASE_CCA_CWBE	0x4	/* Mapped to WB */
#define GCR_BASE_CCA_CWB	0x5	/* Mapped to WB */
#define GCR_BASE_CCA_UCA	0x7	/* Uncached Accelerated */

#define GCR_BASE_REG_CCAEN_POS	4	/* CCA enable */
#define GCR_BASE_REG_CCAEN_BITS	1
#define GCR_BASE_REG_CCAEN	(1 << GCR_BASE_REG_CCAEN_POS)

#define GCR_BASE_REG_CM2_TARGET_POS	0	/* Determines the target device */
#define GCR_BASE_REG_CM2_TARGET_BITS	2

#define GCR_BASE_CM2_TARGET_MEM		0
#define GCR_BASE_CM2_TARGET_IOCU0	2
#define GCR_BASE_CM2_TARGET_IOCU1	3


/* GCR_BASE_REG */
#define GCR_BASE_REG_MASK	0xffff8000	/* Bits [31:15] */

/* GCR_GIC_BASE_REG */
#define GIC_BASE_REG_ADDR_MASK	0xfffe0000	/* Bits [31:17] */
#define GIC_BASE_REG_ADDR_EN	0x1

/* GCR_CPC_BASE_REG , Cluster Power Controller */
#define CPC_BASE_REG_ADDR_MASK	0xffff8000	/* Bits [31:15] */
#define CPC_BASE_REG_ADDR_EN	0x1

/* GCR_CL_OTHER */
#define GCR_CL_OTHER_CORENUM_SHIFT	16	/* Core number of the register set to be accessed. */

/* GCR_CL_COHERENCE */
#define COH_DOMAIN_EN_DFLT		0x3	/* Coherent domain includes 2 cores only. */
#define COH_DOMAIN_MAX_CORE_NUM		1	/* Two cores : 0 and 1. */

#define READ_GCR_REG(r)       READ_MEMORY_REG(r)
#define WRITE_GCR_REG(r, v)   WRITE_MEMORY_REG(r, v)

/*
 * TEST_GCR_REG_VAL macro returns 0 if the reg register, masked by
 * reg_mask is to equal to its reg_val default value.
 */
#define TEST_GCR_REG_VAL TEST_MEM_REG_ID_VAL

/* 
 * TEST_GCR_REG_ID macro returns 0 if the reg register is to equal to
 * its reg_val default value.
 */
#define TEST_GCR_REG_ID TEST_MEM_REG_ID

/* 
 * TEST_GCR_READ macro returns 0 if the reg register can be read.
 */
#define TEST_GCR_READ   TEST_READ_MEM_REG


#endif /* __GCR_H__ */
