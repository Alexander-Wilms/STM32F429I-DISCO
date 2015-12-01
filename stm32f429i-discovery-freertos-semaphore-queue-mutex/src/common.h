/*
 * common.h
 *
 *  Created on: Nov 2, 2015
 *      Author: schaefer
 */

#ifndef COMMON_H_
#define COMMON_H_

#ifdef DEBUG
#define ASSERT(x) if((x)==0) asm("bkpt 0")
#else
#define ASSERT(x)
#endif

#define COMMON __attribute__ ((section ("common_data")))
#define CONSTEXPR_ROM constexpr __attribute__ ((section (".rodata")))
#define ROM const __attribute__ ((section (".rodata")))
#define CCM_DATA __attribute__ ((section (".ccm_data")))

extern uint32_t __common_data_start__[];
extern uint32_t __common_data_end__[];
#define COMMON_BLOCK __common_data_start__
#define COMMON_SIZE  256 // cross-check with _Common_Data_Region_Size within sections.ld file !

#endif /* COMMON_H_ */
