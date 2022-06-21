/*
 File:		arm_cpuid.c
 License: 	MIT

 Copyright (c) 2020 André van Schoubroeck

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "arm_cpuid.h"

char* cpuid() {
	static char buff[64] = "Unknown";
	// ARM v7m manual,page 702, section B4.1.2.
	// 0xE000ED00 // CPUID
	SCB->CPUID;
	cpuid_t *cpuid = (cpuid_t*) (&SCB->CPUID);
	//
	if ((cpuid->PartNo & (0b11 << 10)) == (0b11 << 10)) {
		// Cortex family
		if ((cpuid->PartNo & (0b10 << 4)) == (0b10 << 4)) {
			// Cortex M
			sprintf(buff, "Cortex-M%d r%dp%d", cpuid->PartNo & 0xF,
					cpuid->Variant, cpuid->Revision);
		}
	}
	return buff;
	// But the interesting part comes at 0xE000ED40 // ID_PFR0 (Processor Feature Register) and following
	// We should be able to access it through
	SCB->PFR[0];
	SCB->PFR[1];
	/*
	 __IM  uint32_t PFR[2U];         		 //< Offset: 0x040 (R/ )  Processor Feature Register
	 __IM  uint32_t DFR;                    //< Offset: 0x048 (R/ )  Debug Feature Register
	 __IM  uint32_t ADR;                    //< Offset: 0x04C (R/ )  Auxiliary Feature Register
	 __IM  uint32_t MMFR[4U];               //< Offset: 0x050 (R/ )  Memory Model Feature Register
	 __IM  uint32_t ISAR[5U];               //< Offset: 0x060 (R/ )  Instruction Set Attributes Register
	 */

}

romtable_pid_t extract_romtable_pid(romtable_id_t *rid) {
	return (romtable_pid_t ) { .pid = rid->pid0 << 0 | rid->pid1 << 8
					| rid->pid2 << 16 | rid->pid3 << 24
					| (uint64_t) rid->pid4 << 32 | (uint64_t) rid->pid5 << 40
					| (uint64_t) rid->pid6 << 48 | (uint64_t) rid->pid7 << 56 } ;
		}
