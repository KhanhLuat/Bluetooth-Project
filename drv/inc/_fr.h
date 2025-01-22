/*
 MB91570 Series I/O register declaration file V01L02
 ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICONDUCTOR LIMITED 2011
 LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICONDUCTOR LIMITED

 This file is based on "_fr.h"(V01L02) provided by Fujitsu Semiconductor Limited,
 and is merged with the difference between "_fr.h"(V01L02) and "_fr.h"(V02L02) 
 in order to support 144 pin 578/579 CPU type without changing I/O resister declaration(mb91570.h)
 from V01L02 to V02L02. 
*/
#ifndef __FR_H__
#define __FR_H__

#if defined(__CPU_MB91F575__) || \
    defined(__CPU_MB91F575S__) || \
    defined(__CPU_MB91F577__) || \
    defined(__CPU_MB91F577S__) || \
    defined(__CPU_MB91F575_FR80__) || \
    defined(__CPU_MB91F575S_FR80__) || \
    defined(__CPU_MB91F577_FR80__) || \
    defined(__CPU_MB91F577S_FR80__) || \
    defined(__CPU_MB91F578C__) || \
    defined(__CPU_MB91F578CS__) || \
    defined(__CPU_MB91F578CH__) || \
    defined(__CPU_MB91F578CHS__) || \
    defined(__CPU_MB91F579C__) || \
    defined(__CPU_MB91F579CS__) || \
    defined(__CPU_MB91F579CH__) || \
    defined(__CPU_MB91F579CHS__) || \
    defined(__CPU_MB91F578C_FR80__) || \
    defined(__CPU_MB91F578CS_FR80__) || \
    defined(__CPU_MB91F578CH_FR80__) || \
    defined(__CPU_MB91F578CHS_FR80__) || \
    defined(__CPU_MB91F579C_FR80__) || \
    defined(__CPU_MB91F579CS_FR80__) || \
    defined(__CPU_MB91F579CH_FR80__) || \
    defined(__CPU_MB91F579CHS_FR80__)

#ifdef __FASM__
#include "mb91570_a.inc"
#else
#include "mb91570.h"
#endif
#else
#error "The I/O register file of the specified CPU option does not exist"
#endif

#endif /* __FR_H__ */
